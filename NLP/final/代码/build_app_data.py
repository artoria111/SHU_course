# -*- coding: utf-8 -*-
"""
数据转换：爬虫原始数据 → App使用的JSON格式
============================================
输入: scraped_positions.json (爬虫产出)
输出: position_features_info.json + position_detail_info.json
"""
import json, re, random

# ===== 职位详情模板（根据职位类型生成合理的JD） =====
JD_TEMPLATES = {
    "开发": {
        "summary": ["学历要求：{education}", "经验要求：{experience}", "薪资范围：{salary}"],
        "detail": [
            {"title": "岗位职责",
             "content": "1. 负责公司核心产品的设计、开发与维护；2. 参与系统架构设计和技术方案评审；3. 编写高质量代码，完成单元测试与集成测试；4. 参与代码审查，保障代码质量和系统稳定性；5. 与产品、测试团队紧密协作，按时交付高质量产品。"},
            {"title": "任职要求",
             "content": "1. {experience}以上开发经验，计算机相关专业{education}及以上学历；2. 精通相关技术栈，有扎实的编程基础和良好的编码习惯；3. 熟悉常用设计模式、数据结构和算法；4. 具备良好的沟通协作能力和问题解决能力；5. 有大型项目经验或开源贡献者优先。"},
            {"title": "福利待遇",
             "content": "1. 五险一金+补充商业保险；2. {salary}薪资+年终奖金+项目奖金；3. 弹性工作制，周末双休；4. 免费三餐/餐补、下午茶、健身房；5. 技术培训基金，支持参加技术大会。"},
        ]
    },
    "管理": {
        "summary": ["学历要求：{education}", "经验要求：{experience}", "薪资范围：{salary}"],
        "detail": [
            {"title": "岗位职责",
             "content": "1. 负责团队管理和项目推进，制定技术发展规划；2. 主导技术方案评审和关键技术决策；3. 培养团队成员，建立高效协作的团队文化；4. 与业务部门紧密协作，确保技术方案满足业务需求；5. 推动技术创新和流程优化，提升团队效率。"},
            {"title": "任职要求",
             "content": "1. {experience}以上相关经验，其中3年以上团队管理经验；2. {education}及以上学历；3. 出色的领导力、沟通能力和战略思维；4. 有成功带领团队交付复杂项目的经验；5. 行业资源丰富者优先。"},
            {"title": "福利待遇",
             "content": "1. 七险一金+企业年金；2. {salary}薪资+年终分红+期权；3. 弹性工作+独立办公环境；4. 高端体检+全家医疗保险；5. EMBA/管理培训补贴。"},
        ]
    },
    "默认": {
        "summary": ["学历要求：{education}", "经验要求：{experience}", "薪资范围：{salary}"],
        "detail": [
            {"title": "岗位职责",
             "content": "1. 负责本职工作的规划与执行；2. 与团队协作完成项目目标；3. 持续学习和提升专业技能；4. 参与部门内部的技术分享和知识沉淀；5. 完成上级安排的其他工作任务。"},
            {"title": "任职要求",
             "content": "1. {experience}以上相关工作经验；2. {education}及以上学历，相关专业优先；3. 具备良好的职业素养和团队合作精神；4. 较强的学习能力和执行力；5. 有行业相关经验者优先。"},
            {"title": "福利待遇",
             "content": "1. 五险一金，法定节假日；2. {salary}薪资+绩效奖金；3. 定期团建+年度旅游；4. 完善的培训和晋升体系；5. 舒适的办公环境。"},
        ]
    }
}


def select_template(job_name):
    """根据职位名选择合适的JD模板"""
    if any(kw in job_name for kw in ['经理', '总监', '主管', '主任', '总裁']):
        return "管理"
    if any(kw in job_name for kw in ['工程', '开发', '程序员', '研发', '技术']):
        return "开发"
    return "默认"


def generate_detail(job):
    """根据职位数据生成详细信息"""
    template = JD_TEMPLATES[select_template(job['name'])]

    exp = job.get('experience', '3年') or '3年'
    edu = job.get('education', '本科') or '本科'
    salary = job.get('salary', '面议') or '面议'

    detail = {"name": job['name']}

    # Summary
    detail["summary"] = [s.format(education=edu, experience=exp, salary=salary)
                         for s in template["summary"]]
    # 添加公司信息
    if job.get('company'):
        detail["summary"].append(f"公司：{job['company']}")
    if job.get('industry'):
        detail["summary"].append(f"行业：{job['industry']}")
    if job.get('company_size'):
        detail["summary"].append(f"规模：{job['company_size']}")

    # Detail
    detail["detail"] = []
    for section in template["detail"]:
        content = section["content"].format(education=edu, experience=exp, salary=salary)
        detail["detail"].append({"title": section["title"], "content": content})

    # 面试流程（通用）
    detail["detail"].append({
        "title": "面试流程",
        "content": "1. 简历筛选（1-3个工作日）；2. 技术/业务面试；3. 部门负责人面试；4. HR面试；5. 发放offer。"
    })

    return detail


def build_features(job):
    """构建职位特征数据"""
    salary = job.get('salary', '面议') or '面议'
    exp = job.get('experience', '经验不限') or '经验不限'
    edu = job.get('education', '学历不限') or '学历不限'

    requirements = f"{edu}及以上，{exp}"
    if job.get('city'):
        requirements += f"，工作地点：{job['city']}"

    return {
        "name": job['name'],
        "requirements": requirements,
        "salary_range": salary,
        "skills": job.get('skills', ["沟通能力", "团队协作"]),
    }


def main():
    # 1. 加载爬虫数据
    with open("../数据/json/scraped_positions.json", "r", encoding="utf-8") as f:
        scraped = json.load(f)
    print(f"爬虫数据: {len(scraped)} 条")

    # 2. 加载手工补充的IT职位数据（如有）
    try:
        with open("../数据/json/position_features_info.json", "r", encoding="utf-8") as f:
            existing_features = json.load(f)
        print(f"已有手工数据: {len(existing_features)} 条")
    except:
        existing_features = []

    # 3. 为爬虫数据补充技能
    for job in scraped:
        if not job.get('skills') or len(job['skills']) <= 2:
            job['skills'] = ["沟通能力", "团队协作", "问题解决", "专业能力", "学习能力"]

    # 4. 合并数据（爬虫数据 + 手工IT数据补充多样性）
    all_features = {}
    all_details = {}

    # 先加入手工IT数据
    for item in existing_features:
        key = item['name']
        all_features[key] = item
        # 为手工数据生成detail
        if key not in all_details:
            detail = generate_detail({
                'name': key,
                'experience': '3年',
                'education': '本科',
                'salary': item.get('salary_range', '面议'),
            })
            all_details[key] = detail

    # 再加入爬虫数据（覆盖同名的）
    for job in scraped:
        name = job['name']
        all_features[name] = build_features(job)
        all_details[name] = generate_detail(job)

    features_list = list(all_features.values())
    details_list = list(all_details.values())

    print(f"合并后: {len(features_list)} 个职位特征, {len(details_list)} 个职位详情")

    # 5. 保存
    with open("../数据/json/position_features_info.json", "w", encoding="utf-8") as f:
        json.dump(features_list, f, ensure_ascii=False, indent=2)
    print("已保存: position_features_info.json")

    with open("../数据/json/position_detail_info.json", "w", encoding="utf-8") as f:
        json.dump(details_list, f, ensure_ascii=False, indent=2)
    print("已保存: position_detail_info.json")

    # 6. 统计
    print(f"\n=== 数据统计 ===")
    print(f"总职位数: {len(features_list)}")
    cities = set()
    for j in scraped:
        if j.get('city'):
            cities.add(j['city'])
    print(f"覆盖城市: {cities}")
    skills_all = set()
    for f in features_list:
        skills_all.update(f.get('skills', []))
    print(f"技能关键词: {len(skills_all)} 个")
    print(f"技能样本: {list(skills_all)[:20]}")


if __name__ == "__main__":
    main()
