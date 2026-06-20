# -*- coding: utf-8 -*-
"""
爬虫：从猎聘网(liepin.com)爬取真实职位数据
============================================
策略：
1. 从搜索列表页提取职位链接
2. 访问每个详情页，页面中"相关推荐"区域包含40+个job-card结构化数据
3. 解析每个卡片的：职位名、城市、薪资、经验、学历、公司、行业、规模
4. 从详情页发现更多链接，迭代爬取
"""
import requests, re, json, time, random
from bs4 import BeautifulSoup

HEADERS = {
    "User-Agent": "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
}

# 技能推断词典
SKILL_INFERENCE = {
    "java": ["Java", "Spring Boot", "Spring Cloud", "MyBatis", "MySQL", "Redis", "Docker", "微服务", "Linux", "Git"],
    "python": ["Python", "Django", "Flask", "FastAPI", "MySQL", "Redis", "Docker", "Linux", "Pandas", "NumPy"],
    "前端": ["JavaScript", "TypeScript", "React", "Vue.js", "HTML5", "CSS3", "Webpack", "Node.js", "小程序", "Git"],
    "web": ["JavaScript", "React", "Vue.js", "HTML5", "CSS3", "Node.js", "Git", "RESTful API"],
    "算法": ["Python", "PyTorch", "TensorFlow", "机器学习", "深度学习", "NLP", "推荐系统", "数据挖掘", "C++", "Transformer"],
    "产品": ["需求分析", "原型设计", "Axure", "Figma", "数据分析", "用户研究", "PRD撰写", "竞品分析", "项目管理", "敏捷开发"],
    "测试": ["自动化测试", "Selenium", "JMeter", "Python", "SQL", "测试用例设计", "接口测试", "性能测试", "Jenkins"],
    "数据分析": ["Python", "SQL", "Excel", "Tableau", "Pandas", "NumPy", "数据可视化", "统计学", "PowerBI"],
    "运维": ["Linux", "Docker", "Kubernetes", "Jenkins", "Ansible", "AWS", "Prometheus", "Shell", "Python", "CI/CD"],
    "devops": ["Linux", "Docker", "Kubernetes", "Jenkins", "Terraform", "AWS", "CI/CD", "Python", "Shell", "Prometheus"],
    "android": ["Java", "Kotlin", "Android SDK", "Jetpack", "MVVM", "Retrofit", "性能优化", "OkHttp", "Git"],
    "ios": ["Swift", "Objective-C", "UIKit", "SwiftUI", "Combine", "Xcode", "Core Data", "Git"],
    "c++": ["C++", "STL", "QT", "多线程", "网络编程", "Linux", "设计模式", "数据结构", "算法"],
    "go": ["Go", "gRPC", "Protobuf", "Docker", "Kubernetes", "Redis", "Kafka", "MySQL", "微服务", "Linux"],
    "架构师": ["系统架构设计", "微服务", "分布式系统", "高并发", "性能调优", "Java", "Go", "K8s", "Kafka", "MySQL"],
    "技术总监": ["系统架构", "技术管理", "团队建设", "项目管理", "战略规划", "Java", "微服务", "云计算", "大数据"],
    "cto": ["系统架构", "技术管理", "团队建设", "项目管理", "战略规划", "微服务", "云计算", "大数据"],
    "网络安全": ["渗透测试", "安全审计", "WAF", "IDS/IPS", "Python", "Linux", "网络协议", "加密算法", "漏洞扫描", "等级保护"],
    "安全": ["渗透测试", "安全审计", "WAF", "Python", "Linux", "网络协议", "加密算法", "漏洞扫描"],
    "全栈": ["Java", "Python", "JavaScript", "React", "Vue.js", "Spring Boot", "MySQL", "Redis", "Docker", "DevOps"],
    "ui": ["Figma", "Sketch", "Photoshop", "Illustrator", "用户研究", "交互设计", "视觉设计", "设计规范", "动效设计"],
    "设计": ["Figma", "Sketch", "Photoshop", "Illustrator", "用户研究", "交互设计", "视觉设计", "设计规范"],
    "项目经理": ["项目管理", "敏捷管理", "Scrum", "风险管理", "成本控制", "沟通协调", "Jira", "进度管理", "PMP"],
    "云计算": ["AWS", "Azure", "Docker", "Kubernetes", "Terraform", "Linux", "Python", "CI/CD", "微服务架构"],
    "云": ["AWS", "Azure", "阿里云", "Docker", "Kubernetes", "Terraform", "Linux", "CI/CD"],
    "大数据": ["Hadoop", "Spark", "Flink", "Kafka", "Hive", "Scala", "Python", "SQL", "数据仓库", "ETL"],
    "数据工程": ["Hadoop", "Spark", "Flink", "Kafka", "Hive", "Scala", "Python", "SQL", "数据仓库", "ETL"],
    "嵌入式": ["C语言", "ARM", "Linux驱动", "RTOS", "单片机", "I2C", "SPI", "UART", "电子电路", "嵌入式开发"],
    "游戏": ["Unity", "Unreal Engine", "C#", "C++", "3D渲染", "游戏引擎", "Lua", "OpenGL"],
    "运营": ["用户运营", "内容运营", "数据分析", "活动策划", "Excel", "SQL", "社群运营", "增长黑客"],
    "市场": ["品牌策划", "数字营销", "SEO/SEM", "内容营销", "数据分析", "市场调研", "广告投放", "社交媒体运营"],
    "销售": ["客户开发", "商务谈判", "CRM", "销售管理", "市场分析", "合同管理"],
    "数据库": ["MySQL", "Oracle", "SQL Server", "数据库优化", "备份恢复", "高可用架构", "Redis", "MongoDB", "Linux"],
    "dba": ["MySQL", "Oracle", "SQL Server", "数据库优化", "备份恢复", "高可用架构", "Redis", "MongoDB", "Linux"],
    "人工智能": ["Python", "PyTorch", "大语言模型", "深度学习", "NLP", "计算机视觉", "Transformer", "模型优化"],
    "ai": ["Python", "PyTorch", "大语言模型", "深度学习", "NLP", "计算机视觉", "Transformer", "模型优化"],
    "区块链": ["Solidity", "Go", "Rust", "智能合约", "以太坊", "共识算法", "加密算法", "DeFi"],
    "硬件": ["Verilog", "VHDL", "FPGA", "PCB设计", "电子电路", "嵌入式开发", "ARM"],
    "电子": ["Verilog", "VHDL", "FPGA", "PCB设计", "电子电路", "嵌入式开发", "ARM"],
    "机械": ["SolidWorks", "AutoCAD", "Pro/E", "有限元分析", "机械设计", "UG/NX"],
    "生物": ["PCR", "细胞培养", "分子生物学", "蛋白质纯化", "基因编辑"],
    "化学": ["HPLC", "GC-MS", "有机合成", "分析化学", "制剂研发"],
    "医药": ["临床试验", "GCP", "药物研发", "药理学", "注册申报"],
}

# 热门城市
CITIES = ["北京", "上海", "深圳", "杭州", "广州", "成都", "武汉", "南京", "苏州", "西安", "长沙", "重庆", "东莞", "厦门", "天津"]


def parse_job_card(card):
    """解析 job-card-pc-container div"""
    text = card.get_text(separator=' ', strip=True)
    parts = [p.strip() for p in text.split() if p.strip()]

    job = {
        "name": parts[0] if parts else "",
        "city": "",
        "salary": "",
        "experience": "",
        "education": "",
        "company": "",
        "industry": "",
        "company_size": "",
    }

    for i, p in enumerate(parts):
        # 城市
        if not job["city"]:
            for city in CITIES:
                if city in p and len(p) <= 8:
                    job["city"] = city if city in p else p[:6]
                    break
        # 薪资
        if not job["salary"] and re.match(r'\d+[-~]\d+[kK万]', p):
            job["salary"] = p
        # 经验
        if not job["experience"] and re.match(r'(\d+年|经验不限|\d+[-~]\d+年)', p):
            job["experience"] = p
        # 学历
        if not job["education"] and re.match(r'(学历不限|大专|本科|硕士|博士|中专)', p):
            job["education"] = p
        # 公司
        if not job["company"] and ('公司' in p or '集团' in p or '有限' in p) and len(p) > 4:
            job["company"] = p
        # 规模
        if not job["company_size"] and re.match(r'\d+[-~]\d+人|少于\d+人|\d+人以[上内]|未融资|已上市|[A-D]轮|天使轮', p):
            job["company_size"] = p

    return job if job["name"] else None


def infer_skills(job_name):
    """根据职位名称推断技能"""
    name_lower = job_name.lower()
    for kw, skills in SKILL_INFERENCE.items():
        if kw in name_lower:
            return skills
    # 泛匹配
    if '工程' in job_name and '师' in job_name:
        return ["编程能力", "问题解决", "团队协作", "沟通能力", "Linux", "Git"]
    return ["沟通能力", "团队协作", "问题解决", "学习能力"]


def crawl_from_detail(url, visited, all_jobs, depth=0):
    """从详情页爬取卡片数据，并发现新链接"""
    if url in visited or depth > 1:
        return
    visited.add(url)

    try:
        r = requests.get(url, headers=HEADERS, timeout=15)
        if r.status_code != 200:
            return
        soup = BeautifulSoup(r.text, 'html.parser')

        # 1. 解析当前页面的job cards
        cards = soup.find_all('div', class_='job-card-pc-container')
        for card in cards:
            job = parse_job_card(card)
            if job and job['name'] and len(job['name']) >= 3:
                key = (job['name'], job['company'])
                if key not in all_jobs:
                    job['skills'] = infer_skills(job['name'])
                    all_jobs[key] = job

        # 2. 发现新的/zpXXXXX/链接
        if depth == 0:
            new_links = set()
            for a in soup.find_all('a', href=re.compile(r'^/zp\w{8,}/')):
                new_url = "https://www.liepin.com" + a['href']
                new_links.add(new_url)

            # 最多访问5个新链接
            for new_url in list(new_links)[:5]:
                time.sleep(random.uniform(0.5, 1.5))
                crawl_from_detail(new_url, visited, all_jobs, depth + 1)

    except Exception as e:
        print(f"    Error: {str(e)[:60]}")


def main():
    all_jobs = {}
    visited = set()

    # 种子URL：从不同搜索关键词获取（使用已知可访问的URL）
    seed_urls = [
        # Java开发相关
        "https://www.liepin.com/zptxjsyfjl8l8f/",   # 高级技术研发经理
        "https://www.liepin.com/zpfzblgcs9e1m/",     # 电子硬件工程师
        "https://www.liepin.com/zpjxsjgjjl9d10y/",   # 机械设计高级经理
        "https://www.liepin.com/zplmjxgcs3g7s/",     # 精密机械工程师
        # 再手动构造几个常见关键词的搜索页进行提取
    ]

    print("开始爬取...")
    for i, url in enumerate(seed_urls, 1):
        print(f"\n[{i}/{len(seed_urls)}] {url[-12:]}")
        crawl_from_detail(url, visited, all_jobs)
        print(f"  当前累计: {len(all_jobs)} 个职位")
        time.sleep(random.uniform(1, 2))

    jobs_list = list(all_jobs.values())
    print(f"\n总计爬取: {len(jobs_list)} 个unique职位")

    # 按职位类型分类统计
    categories = {}
    for j in jobs_list:
        name = j['name']
        for kw in ['Java', 'Python', '前端', '算法', '产品', '测试', '运维', '架构',
                    'Android', 'iOS', 'C++', 'Go', '经理', '总监', '设计', '安全',
                    '数据', '全栈', '开发', '工程']:
            if kw in name:
                categories[kw] = categories.get(kw, 0) + 1
    print(f"职位分类: {categories}")

    # 保存
    with open("../数据/json/scraped_positions.json", "w", encoding="utf-8") as f:
        json.dump(jobs_list, f, ensure_ascii=False, indent=2)
    print(f"\n已保存: scraped_positions.json ({len(jobs_list)} 条)")

    # 样本
    print("\n=== 样本 ===")
    for j in jobs_list[:10]:
        print(f"  {j['name']} | {j['city']} | {j['salary']} | {j['experience']} | {j['company'][:20]}")


if __name__ == "__main__":
    main()


# app_recruitment.py 中的核心打分算法
def calculate_match_score(user_profile, job_feature, semantic_sim):
    """
    综合语义相似度与业务规则的加权打分
    逻辑：基础分(语义) * 技能因子 * 职位因子 * 门槛因子
    """
    score = semantic_sim

    # 1. 技能重叠加成：每重合一个关键技能，得分提升20%
    user_skills = set(user_profile.get('skills', []))
    job_skills = set(job_feature.get('skills', []))
    overlap_count = len(user_skills & job_skills)
    score *= (1 + 0.2 * overlap_count)

    # 2. 职位名称精准匹配加成：50%的巨额提振
    if user_profile.get('positions'):
        for pos in user_profile['positions']:
            if pos.lower() in job_feature['title'].lower():
                score *= 1.5
                break

    # 3. 经验门槛校验：满足要求则给予30%的奖励分
    u_exp = user_profile.get('experience')
    j_exp = job_feature.get('experience_req')
    if u_exp is not None and j_exp is not None:
        if u_exp >= j_exp:
            score *= 1.3
        else:
            # 经验不足则进行惩罚
            score *= 0.7

    return round(score, 4)
