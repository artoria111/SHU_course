# -*- coding: utf-8 -*-
"""
智能招聘匹配系统 — 基于NLP的职位推荐助手
===========================================
题目：4）XXX问答系统 → 智能招聘问答系统

NLP技术栈（5+）：
1. 命名实体识别（技能名、职位名提取）
2. 语义搜索（SentenceTransformer 技能→职位匹配）
3. 规则信息抽取（用户画像：经验/学历/薪资）
4. 文本匹配（求职者画像 vs 职位要求 加权打分）
5. 依存句法分析（spaCy）
6. 关键词提取与消歧
"""
import os
os.environ["HF_ENDPOINT"] = "https://hf-mirror.com"

from flask import Flask, request, jsonify, render_template
import json
from sentence_transformers import SentenceTransformer, util
import spacy
import time
import re
import random

app = Flask(__name__,
            template_folder='../数据/templates')

# ===== 加载模型 =====
print("加载 SentenceTransformer...")
sentence_model = SentenceTransformer('paraphrase-multilingual-MiniLM-L12-v2')
print("SentenceTransformer 加载完成")

print("加载 spaCy 中文模型...")
nlp = spacy.load("zh_core_web_sm")
print("spaCy 加载完成")

# ===== 技能关键词库（用于NER实体识别）=====
SKILL_KEYWORDS = {
    # 编程语言
    "Java", "Python", "C++", "C#", "Go", "Rust", "Kotlin", "Swift",
    "JavaScript", "TypeScript", "PHP", "Ruby", "Scala", "Matlab", "R语言",
    "Shell", "SQL", "HTML", "CSS",
    # 框架 & 库
    "Spring Boot", "Spring Cloud", "MyBatis", "Hibernate", "Django",
    "Flask", "FastAPI", "React", "Vue.js", "Angular", "Node.js",
    "Express.js", "jQuery", "Bootstrap", "TensorFlow", "PyTorch",
    "Keras", "Pandas", "NumPy", "Scikit-learn", "OpenCV",
    # 数据库
    "MySQL", "PostgreSQL", "Oracle", "SQL Server", "MongoDB", "Redis",
    "Elasticsearch", "HBase", "Cassandra", "Neo4j", "Hive",
    # 云计算 & DevOps
    "Docker", "Kubernetes", "K8s", "Jenkins", "Ansible", "Terraform",
    "AWS", "Azure", "阿里云", "腾讯云", "CI/CD", "Git", "GitLab",
    "Prometheus", "Grafana", "ELK",
    # 大数据
    "Hadoop", "Spark", "Flink", "Kafka", "RabbitMQ", "Storm",
    "数据仓库", "ETL", "数据湖",
    # 移动开发
    "Android", "iOS", "React Native", "Flutter", "小程序开发",
    "SwiftUI", "Jetpack",
    # 软技能 & 工具
    "项目管理", "需求分析", "系统设计", "架构设计", "技术管理",
    "团队管理", "敏捷开发", "Scrum", "数据分析", "用户研究",
    "原型设计", "Axure", "Figma", "Sketch", "Photoshop",
    "Illustrator", "Jira", "Confluence", "Excel",
    # 安全
    "渗透测试", "安全审计", "加密算法", "WAF", "IDS", "IPS",
    # 嵌入式 & 硬件
    "嵌入式开发", "ARM", "Linux驱动", "RTOS", "单片机", "FPGA",
    "I2C", "SPI", "UART", "PCB设计",
    # 其他技术
    "微服务", "分布式系统", "高并发", "性能优化", "设计模式",
    "RESTful API", "GraphQL", "gRPC", "WebSocket",
    "NLP", "计算机视觉", "推荐系统", "强化学习", "深度学习",
    "机器学习", "数据挖掘", "大语言模型", "Transformer",
    "Unity", "Unreal Engine", "游戏引擎", "3D渲染",
}

# ===== 技能同义词映射（缩写→标准名）=====
SKILL_SYNONYMS = {
    "js": "JavaScript", "JS": "JavaScript",
    "ts": "TypeScript", "TS": "TypeScript",
    "k8s": "Kubernetes", "K8s": "Kubernetes", "k8S": "Kubernetes",
    "vue": "Vue.js", "Vue": "Vue.js",
    "reactjs": "React", "react": "React",
    "spring": "Spring Boot",
    "tf": "TensorFlow",
    "torch": "PyTorch",
    "sklearn": "Scikit-learn",
    "cv": "计算机视觉",
    "nlp": "NLP",
    "llm": "大语言模型",
    "ci/cd": "CI/CD", "cicd": "CI/CD",
    "db": "MySQL",
    "aws": "AWS", "azure": "Azure",
    "gcp": "云计算",
    "ml": "机器学习",
    "dl": "深度学习",
    "oop": "设计模式",
    "api": "RESTful API",
    "mysql": "MySQL", "postgres": "PostgreSQL", "mongo": "MongoDB",
    "redis": "Redis", "es": "Elasticsearch",
    "git": "Git", "docker": "Docker", "linux": "Linux",
}

def normalize_skills(skills):
    """将缩写/别名标准化为正式技能名"""
    normalized = set()
    for s in skills:
        normalized.add(SKILL_SYNONYMS.get(s, s))
    return list(normalized)

# ===== 意图识别 =====
INTENT_PATTERNS = {
    "求职咨询": [
        r"(找|求职|应聘|跳槽|换工作|内推|推荐.*工作|帮我.*(匹配|推荐|看看|找))",
        r"(什么.*(职位|工作|岗位).*(适合|推荐))",
        r"(我想.*(找|做|干|当|入行|转行))",
        r"(有没有.*(合适|推荐).*工作)",
    ],
    "技能评估": [
        r"(我会|我擅长|我精通|我熟悉|我学过|我做过).*",
        r".*\d+年.*(经验|开发)",
        r"(掌握了|熟练).*",
    ],
    "薪资咨询": [
        r"(薪资|工资|待遇|薪水|月薪|年薪|多少钱|多少K|多少k).*",
        r".*(值多少钱|能拿多少|薪资.*水平)",
    ],
    "职位查询": [
        r"(什么是|介绍一下|了解).*(职位|岗位|工程师|经理)",
        r".*(要求|需要.*什么.*技能).*",
    ],
    "闲聊": [
        r"(你好|嗨|hello|hi|谢谢|感谢|再见|拜拜)",
        r"(今天.*天气|你.*(怎么样|是什么|干啥))",
    ],
}

def classify_intent(text):
    """基于关键词+正则的意图分类"""
    scores = {}
    for intent, patterns in INTENT_PATTERNS.items():
        score = 0
        for pat in patterns:
            if re.search(pat, text):
                score += 1
        if score > 0:
            scores[intent] = score
    if not scores:
        return "求职咨询"  # 默认
    return max(scores, key=scores.get)

POSITION_KEYWORDS = {
    "Java开发工程师", "前端开发工程师", "Python开发工程师",
    "数据分析师", "算法工程师", "产品经理", "DevOps工程师",
    "测试工程师", "全栈开发工程师", "技术总监", "架构师",
    "项目经理", "运营经理", "Android开发工程师", "iOS开发工程师",
    "网络安全工程师", "嵌入式开发工程师", "Go开发工程师",
    "游戏开发工程师", "云计算工程师", "数据库管理员",
    "UI设计师", "UX设计师", "C++开发工程师", "区块链开发工程师",
    "人工智能研究员", "市场营销经理", "售前技术支持",
    "高级Java工程师", "高级前端工程师", "数据工程师",
}

# ===== 内存存储 =====
in_memory_db = {
    "chat_sessions": {},
    "chat_messages": {},
    "user_profiles": {},
    "skill_entities": {},
}

def get_timestamp():
    return time.strftime("%Y-%m-%d %H:%M:%S")

# ===== 加载职位数据 =====
print("加载职位特征数据...")
with open("../数据/json/position_features_info.json", "r", encoding="utf-8") as f:
    position_info_list = json.load(f)
print(f"职位特征数据: {len(position_info_list)} 个职位")

print("加载职位详情数据...")
with open("../数据/json/position_detail_info.json", "r", encoding="utf-8") as f:
    position_detail_data = json.load(f)
print(f"职位详情数据: {len(position_detail_data)} 个职位")

# 构建数据索引
position_detail_map = {item["name"]: item for item in position_detail_data}
position_names = [item["name"] for item in position_info_list]
position_info_map = {item["name"]: item for item in position_info_list}

# 预计算职位向量
print("计算职位向量...")
position_embeddings = sentence_model.encode(position_names, convert_to_tensor=True)
print("职位向量计算完成")

# 构建技能→职位反向索引
skill_to_positions = {}
for pos in position_info_list:
    for skill in pos.get("skills", []):
        if skill not in skill_to_positions:
            skill_to_positions[skill] = []
        skill_to_positions[skill].append(pos["name"])

# ===== NER：技能和职位提取 =====
def extract_skills_and_positions(text):
    """从用户输入中提取技能关键词和职位名"""
    doc = nlp(text)
    found_skills = set()
    found_positions = set()

    # 1. 关键词直接匹配
    for skill in SKILL_KEYWORDS:
        if skill.lower() in text.lower():
            found_skills.add(skill)

    # 1b. 检查同义词缩写：用户可能用 js→JavaScript, k8s→Kubernetes
    text_lower = text.lower()
    for alias, standard in SKILL_SYNONYMS.items():
        if alias.lower() in text_lower:
            found_skills.add(standard)

    for pos in POSITION_KEYWORDS:
        if pos in text:
            found_positions.add(pos)

    # 2. spaCy词性辅助：专有名词可能是技能
    for token in doc:
        if token.pos_ in ("PROPN", "NOUN") and len(token.text) >= 2:
            # 检查英文技能（如 Java, Python 等）
            if token.text in SKILL_KEYWORDS:
                found_skills.add(token.text)

    # 3. 同义词标准化：js→JavaScript, k8s→Kubernetes 等
    skills = normalize_skills(found_skills)

    return skills, list(found_positions)

# ===== 用户画像提取 =====
def extract_user_profile(text):
    """从文本中提取求职者画像"""
    profile = {}

    # 工作经验
    exp_patterns = [
        r"(\d{1,2})\s*年\s*(工作)?(开发)?(从业)?经验",
        r"工作\s*(\d{1,2})\s*年",
        r"(\d{1,2})\s*年\s*(从业|工作|开发)",
        r"做了?\s*(\d{1,2})\s*年",
        r"(\d{1,2})\s*年\s*(的)?",
    ]
    for pattern in exp_patterns:
        m = re.search(pattern, text)
        if m:
            exp = next((int(g) for g in m.groups() if g and g.isdigit()), None)
            if exp:
                profile["years_of_experience"] = exp
                break

    # 学历
    if re.search(r"(博士|博研|研究生)", text):
        profile["education"] = "博士"
    elif re.search(r"(硕士|研究生)", text):
        profile["education"] = "硕士"
    elif re.search(r"(本科|学士|大学)", text):
        profile["education"] = "本科"
    elif re.search(r"(大专|专科)", text):
        profile["education"] = "大专"

    # 期望薪资
    salary_patterns = [
        r"(\d{1,2})\s*[kK]\s*[-~至到]\s*(\d{1,2})\s*[kK]",
        r"薪资.*?(\d{1,2})\s*[kK]",
        r"月薪.*?(\d{1,2})\s*[kK]",
    ]
    for pattern in salary_patterns:
        m = re.search(pattern, text)
        if m:
            low = int(m.group(1))
            profile["expected_salary_min"] = low * 1000
            try:
                high = int(m.group(2))
                profile["expected_salary_max"] = high * 1000
            except (IndexError, ValueError):
                profile["expected_salary_max"] = low * 1000 + 5000
            break

    # 当前职位
    for pos in POSITION_KEYWORDS:
        if pos in text:
            profile["current_position"] = pos
            break

    # 城市/地点
    cities = ["北京", "上海", "深圳", "杭州", "广州", "成都", "武汉", "南京", "苏州", "西安"]
    for city in cities:
        if city in text:
            profile["preferred_city"] = city
            break

    return profile

def generate_profile_summary(profile):
    parts = []
    if profile.get("years_of_experience"):
        parts.append(f"{profile['years_of_experience']}年经验")
    if profile.get("education"):
        parts.append(profile["education"])
    if profile.get("expected_salary_min"):
        parts.append(f"期望{profile['expected_salary_min']//1000}K-{profile['expected_salary_max']//1000}K")
    if profile.get("current_position"):
        parts.append(f"当前职位: {profile['current_position']}")
    if profile.get("preferred_city"):
        parts.append(f"意向城市: {profile['preferred_city']}")
    if parts:
        return "💼 求职者画像：" + " | ".join(parts)
    return "💼 暂无求职者画像信息"

# ===== 职位匹配 =====
def check_requirements(user_profile, requirements_text):
    """检查用户画像是否满足职位基本要求"""
    if not requirements_text:
        return True
    # 经验检查
    exp = user_profile.get("years_of_experience")
    if exp:
        m = re.search(r"(\d+)[-~至]*(\d*)\s*年", requirements_text)
        if m:
            req_min = int(m.group(1))
            if exp < req_min:
                return False
    # 学历检查
    edu = user_profile.get("education")
    edu_levels = {"大专": 1, "本科": 2, "硕士": 3, "博士": 4}
    if edu and edu in edu_levels:
        for level_name, level_val in edu_levels.items():
            if level_name in requirements_text and edu_levels.get(edu, 0) < level_val:
                return False
    return True

def match_positions(skills, user_profile, top_k=5):
    """基于技能和用户画像匹配最合适的职位"""
    combined_text = " ".join(skills) if skills else " "
    input_vector = sentence_model.encode(combined_text, convert_to_tensor=True)

    # 语义搜索
    semantic_results = util.semantic_search(input_vector, position_embeddings, top_k=top_k * 2)[0]

    scored = []
    for item in semantic_results:
        pos_name = position_names[item["corpus_id"]]
        pos_info = position_info_map.get(pos_name, {})
        score = item["score"]

        # 1. 技能直接匹配加分
        pos_skills = set(pos_info.get("skills", []))
        user_skills = set(skills)
        skill_overlap = len(pos_skills & user_skills)
        if skill_overlap > 0:
            score *= (1 + 0.2 * skill_overlap)

        # 2. 需求匹配加分
        requirements = pos_info.get("requirements", "")
        if check_requirements(user_profile, requirements):
            score *= 1.3

        # 3. 当前职位匹配加分
        if user_profile.get("current_position") and user_profile["current_position"] == pos_name:
            score *= 1.5

        scored.append((pos_name, score, skill_overlap))

    scored.sort(key=lambda x: x[1], reverse=True)
    return scored[:top_k]

# ===== 内存数据库操作 =====
def create_session(title="新的对话"):
    sid = len(in_memory_db["chat_sessions"]) + 1
    in_memory_db["chat_sessions"][sid] = {"id": sid, "title": title, "created_at": get_timestamp()}
    in_memory_db["chat_messages"][sid] = []
    in_memory_db["user_profiles"][sid] = {}
    in_memory_db["skill_entities"][sid] = set()
    return sid

def get_sessions():
    return list(in_memory_db["chat_sessions"].values())

def get_messages(sid):
    return in_memory_db["chat_messages"].get(sid, [])

def save_message(sid, role, content):
    msgs = in_memory_db["chat_messages"].get(sid, [])
    msgs.append({"role": role, "content": content, "created_at": get_timestamp()})
    if len(msgs) == 1 and role == "user":
        title = content[:10]
        if sid in in_memory_db["chat_sessions"]:
            in_memory_db["chat_sessions"][sid]["title"] = title

def save_skill_entity(sid, skill):
    if sid in in_memory_db["skill_entities"]:
        in_memory_db["skill_entities"][sid].add(skill)

def load_session_skills(sid):
    return list(in_memory_db["skill_entities"].get(sid, set()))

def delete_session(sid):
    in_memory_db["chat_sessions"].pop(sid, None)
    in_memory_db["chat_messages"].pop(sid, None)
    in_memory_db["user_profiles"].pop(sid, None)
    in_memory_db["skill_entities"].pop(sid, None)
    return True

def load_user_profile(sid):
    return in_memory_db["user_profiles"].get(sid, {}).copy()

def merge_and_save_profile(sid, old_profile, new_profile):
    updated = False
    merged = old_profile.copy()
    for key in ['years_of_experience', 'education', 'expected_salary_min',
                'expected_salary_max', 'current_position', 'preferred_city']:
        if new_profile.get(key) is not None and new_profile.get(key) != old_profile.get(key):
            merged[key] = new_profile[key]
            updated = True
    if updated:
        in_memory_db["user_profiles"][sid] = merged
    return updated

# ===== 提示模板 =====
INPUT_CONFIRM = [
    "💬 了解，您提到的是：{}",
    "📝 收到您的求职描述：{}",
    "🔊 我听到了：{}",
]
NO_SKILL = [
    "❗ 抱歉，没能识别出具体的技能关键词。您能再详细描述一下您的技术栈吗？",
    "😕 暂时没找到明确的技能关键词，请尝试更具体的描述，比如「我会Java和Spring Boot」。",
]
NO_MATCH = [
    "⚠️ 根据您的描述，暂时无法精确匹配合适的职位。可以再补充一些技能或经验信息吗？",
    "😔 没找到高度匹配的职位，请描述更多的技能。",
]
NO_DETAIL = [
    "❓ 抱歉，暂时没有该职位的详细信息。",
    "⚠️ 该职位的详情暂时缺失。",
]

# ===== Flask路由 =====
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/chat_sessions', methods=['GET', 'POST'])
def api_chat_sessions():
    if request.method == 'POST':
        title = request.json.get('title', '新的对话')
        sid = create_session(title)
        return jsonify({"id": sid, "title": title})
    return jsonify([{"id": s['id'], "title": s['title']} for s in get_sessions()])

@app.route('/api/chat_sessions/<int:sid>', methods=['DELETE'])
def api_delete_session(sid):
    try:
        ok = delete_session(sid)
        return (jsonify({"message": "删除成功"}), 200) if ok else (jsonify({"message": "不存在"}), 404)
    except Exception as e:
        return jsonify({"message": "删除失败", "error": str(e)}), 500

@app.route('/api/chat_messages/<int:sid>', methods=['GET'])
def api_chat_messages(sid):
    return jsonify([{"role": m['role'], "content": m['content']} for m in get_messages(sid)])

@app.route('/stream/<int:sid>', methods=['POST'])
def stream(sid):
    user_input = request.json.get('message', '').strip()
    if not user_input:
        return jsonify({"content": "❗ 请输入有效的文本。"})

    save_message(sid, "user", user_input)
    full_response = []
    print(f"[session {sid}] 输入: {user_input}")

    # 1. 用户画像
    new_profile = extract_user_profile(user_input)
    old_profile = load_user_profile(sid)
    merge_and_save_profile(sid, old_profile, new_profile)
    profile = load_user_profile(sid)

    # 2. 确认输入
    full_response.append(random.choice(INPUT_CONFIRM).format(user_input))

    # 2b. 意图识别
    intent = classify_intent(user_input)
    intent_icons = {"求职咨询": "💼", "技能评估": "📊", "薪资咨询": "💰", "职位查询": "🔎", "闲聊": "💬"}
    full_response.append(f"{intent_icons.get(intent, '📌')} 识别意图：{intent}")

    # 3. 实体识别：技能+职位
    skills, positions = extract_skills_and_positions(user_input)
    entity_msg = f"🔍 识别到的技能/职位关键词：{', '.join(skills + positions) if (skills or positions) else '（无）'}"
    full_response.append(entity_msg)

    for s in skills:
        save_skill_entity(sid, s)

    all_skills = load_session_skills(sid)
    merged_skills = list(set(all_skills))

    if set(skills) != set(merged_skills):
        full_response.append(f"🧠 会话中累计技能标签：{', '.join(merged_skills)}")

    if not skills and not merged_skills:
        full_response.append(random.choice(NO_SKILL))
        full_response.append(generate_profile_summary(profile))
        content = "\n\n".join(full_response)
        save_message(sid, "assistant", content)
        return jsonify({"content": content})

    # 4. 职位匹配
    scored = match_positions(merged_skills, profile, top_k=5)
    if not scored:
        full_response.append(random.choice(NO_MATCH))
        full_response.append(generate_profile_summary(profile))
        content = "\n\n".join(full_response)
        save_message(sid, "assistant", content)
        return jsonify({"content": content})

    full_response.append("🎯 为您匹配的职位：")
    for i, (name, score, overlap) in enumerate(scored, 1):
        pos_info = position_info_map.get(name, {})
        salary = pos_info.get("salary_range", "面议")
        match_pct = min(int(score * 100), 100)
        stars = "⭐" * min(5, max(1, overlap + 1))
        full_response.append(
            f"{i}. **{name}**  {stars}\n"
            f"   💰 薪资范围：{salary}  |  匹配度：{match_pct}%\n"
            f"   📋 要求：{pos_info.get('requirements', '详见职位描述')[:80]}..."
        )

    # 5. 职位详情（展示第一匹配职位）
    top_name = scored[0][0]
    detail = position_detail_map.get(top_name)
    if detail:
        full_response.append(f"\n📌 **{top_name}** 详细信息：")
        if detail.get("summary"):
            for line in detail["summary"][:4]:
                full_response.append(f"  • {line}")
        if detail.get("detail"):
            for section in detail["detail"][:3]:
                text = section["content"]
                if len(text) > 250:
                    text = text[:250] + "..."
                full_response.append(f"\n  🔹 **{section['title']}**\n  {text}")
    else:
        full_response.append(random.choice(NO_DETAIL))

    # 6. 画像总结
    full_response.append("\n" + generate_profile_summary(profile))

    # 引导补充
    missing = []
    if not profile.get("years_of_experience"):
        missing.append("您有几年工作经验？（如：3年Java开发经验）")
    if not profile.get("education"):
        missing.append("您的学历是？（如：本科/硕士）")
    if not profile.get("expected_salary_min"):
        missing.append("您的期望薪资？（如：期望15K-20K）")
    if missing:
        full_response.append("\n💡 补充以下信息可以获得更精准的推荐：")
        for m in missing:
            full_response.append(f"  - {m}")

    content = "\n".join(full_response)
    save_message(sid, "assistant", content)
    return jsonify({"content": content})


if __name__ == '__main__':
    print("\n" + "=" * 60)
    print("智能招聘匹配系统")
    print("访问地址: http://localhost:7002")
    print("=" * 60)
    app.run(host='0.0.0.0', port=7002, debug=False)
