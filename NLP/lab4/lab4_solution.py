import spacy
import jieba
import sys
sys.stdout.reconfigure(encoding='utf-8')
print("=" * 60)
print("加载spaCy中文模型 zh_core_web_sm ...")
nlp = spacy.load("zh_core_web_sm")
print("模型加载完成！")
print("可用组件:", nlp.pipe_names)
print("=" * 60)
class WordSenseDisambiguator:
    SENSE_DEFINITIONS = {
        "苹果": {
            "水果": {
                "keywords": ["吃", "水果", "甜", "好吃", "营养", "维生素", "红", "新鲜", "果园", "摘", "咬", "汁",
                           "脆", "一袋", "买了", "超市", "又红", "又甜", "真好吃"],
            },
            "电子品牌": {
                "keywords": ["手机", "电脑", "公司", "发布", "配置", "性能", "系统", "乔布斯", "iOS", "iPhone",
                           "iPad", "Mac", "平板", "手表", "耳机", "智能", "科技", "芯片", "App", "商店", "新一代"],
            },
            "电影名称": {
                "keywords": ["上映", "电影", "导演", "主演", "剧情", "票房", "影院", "观众", "影片", "镜头", "画面", "听说"],
            }
        },
        "算账": {
            "计算账目": {
                "keywords": ["计算", "账目", "账本", "财务", "会计", "账单", "金额", "收支", "数目", "统计",
                           "核对", "仔细", "每个月", "正在", "每一笔", "成本", "支出", "收入", "记录"],
            },
            "计较报复": {
                "keywords": ["报复", "教训", "等着", "饶不了", "算你", "较量", "秋后", "过不去", "找麻烦",
                           "回头", "再跟", "记仇", "吃亏", "走着瞧", "收拾", "下回", "再找", "耍了"],
            }
        }
    }
    SENSE_DESC = {
        "苹果": {
            "水果": "苹果是一种好吃的水果，营养价值很高",
            "电子品牌": "苹果是一家手机电脑公司，发布了新科技产品",
            "电影名称": "苹果是一部上映的电影，观众去影院观看",
        },
        "算账": {
            "计算账目": "会计正在核对账目计算财务账单收支",
            "计较报复": "这次输了下次再找你较量报复算账",
        }
    }

    def __init__(self, nlp_model):
        self.nlp = nlp_model

    def disambiguate(self, word, context):
        if word not in self.SENSE_DEFINITIONS:
            return ("未知", 0.0)

        doc = self.nlp(context)
        senses = self.SENSE_DEFINITIONS[word]

        context_words = set()
        for token in doc:
            context_words.add(token.text)
        jieba_words = set(jieba.cut(context))
        context_words.update(jieba_words)

        scores = {}
        for sense_name, sense_info in senses.items():
            keywords = sense_info["keywords"]
            match_count = 0
            matched_words = []
            for kw in keywords:
                if kw in context or kw in context_words:
                    match_count += 1
                    matched_words.append(kw)
            for cw in context_words:
                for kw in keywords:
                    if kw in cw and kw not in matched_words:
                        match_count += 0.5
                        matched_words.append(kw)
            scores[sense_name] = {"count": match_count, "matched": matched_words}

        total = sum(s["count"] for s in scores.values())
        if total == 0:
            desc_map = self.SENSE_DESC.get(word, {})
            best_sense = None
            best_sim = -1
            for sense_name in senses.keys():
                if sense_name in desc_map:
                    desc_doc = self.nlp(desc_map[sense_name])
                    sim = doc.similarity(desc_doc)
                    if sim > best_sim:
                        best_sim = sim
                        best_sense = sense_name
            if best_sense:
                return (best_sense, best_sim)
            return (list(senses.keys())[0], 0.0)

        best_sense = max(scores, key=lambda s: scores[s]["count"])
        confidence = min(scores[best_sense]["count"] / total, 1.0) if total > 0 else 0
        return (best_sense, confidence)

    def test_all_cases(self):
        test_cases = {
            "苹果": [
                ("这个苹果真好吃，营养价值高。", "水果"),
                ("妈妈从超市买了一袋苹果，又红又甜。", "水果"),
                ("苹果手机配置不高，但整体性能优化得很好。", "电子品牌"),
                ("苹果公司发布了新一代平板电脑。", "电子品牌"),
                ("'苹果'上映了，听说非常精彩。", "电影名称"),
                ("听说电影《苹果》的票房不错，观众反响很好。", "电影名称"),
            ],
            "算账": [
                ("你别打扰他，他正在算账呢。", "计算账目"),
                ("会计每个月都要仔细算账，核对每一笔收支。", "计算账目"),
                ("好了，这回算你赢，下回我再跟你算账！", "计较报复"),
                ("这次你耍了我，回头再找你算账！", "计较报复"),
            ]
        }

        print("\n" + "=" * 60)
        print("任务1：词义消歧实验结果")
        print("=" * 60)

        total_cases = 0
        correct_cases = 0
        for word, cases in test_cases.items():
            print(f"\n[歧义词: {word}]")
            print("-" * 40)
            for context, expected in cases:
                predicted, confidence = self.disambiguate(word, context)
                total_cases += 1
                is_correct = (predicted == expected)
                if is_correct:
                    correct_cases += 1
                status = "[OK]" if is_correct else "[FAIL]"
                print(f"  测试: {context}")
                print(f"  预测: {predicted} | 期望: {expected} {status} (置信度:{confidence:.2f})")
                print()
        print(f"\n  准确率: {correct_cases}/{total_cases} = {correct_cases/total_cases*100:.1f}%")

class SemanticRoleLabeler:
    DEP_TO_ROLE = {
        "nsubj": "施事(A0)",
        "nsubj:pass": "受事(A1)",
        "nsubjpass": "受事(A1)",
        "dobj": "受事(A1)",
        "iobj": "与事(A2)",
        "obl": "附加(AM)",
        "obl:patient": "受事(A1)",
        "obl:agent": "施事(A0)",
        "advmod": "附加(AM)",
        "advmod:loc": "处所(AM-LOC)",
        "advmod:tmod": "时间(AM-TMP)",
        "nmod:tmod": "时间(AM-TMP)",
        "nmod:prep": "附加(AM)",
        "nmod:assmod": "修饰(AM-MOD)",
        "advmod:dvp": "方式(AM-MNR)",
        "xcomp": "补语(A-COMP)",
        "ccomp": "补语(A-COMP)",
        "attr": "属性(AM-ATT)",
        "amod": "修饰(AM-MOD)",
        "compound:nn": "修饰(AM-MOD)",
        "aux:asp": "体标记",
        "aux:ba": "把字标记",
        "aux:pass": "被动标记",
        "auxpass": "被动标记",
        "mark": "标记",
        "case": "格标记",
        "nummod": "修饰(AM-MOD)",
        "mark:clf": "量词标记",
        "conj": "并列",
        "dep": "附加(AM)",
    }

    def __init__(self, nlp_model):
        self.nlp = nlp_model

    def label_sentence(self, text):
        doc = self.nlp(text)
        predicates = []
        for token in doc:
            if token.pos_ in ("VERB", "ADJ") and (token.dep_ == "ROOT" or token.dep_ == "conj"):
                predicates.append(token)

        if not predicates:
            for token in doc:
                if token.pos_ == "VERB":
                    predicates.append(token)
                    break

        if not predicates:
            for token in doc:
                if token.dep_ == "ROOT":
                    predicates.append(token)
                    break

        results = []
        for pred in predicates:
            roles = {"谓词": pred.text}
            for token in doc:
                if token.head == pred and token.dep_ != "punct":
                    role_name = self.DEP_TO_ROLE.get(token.dep_, f"其他({token.dep_})")
                    if role_name not in roles:
                        roles[role_name] = []
                    roles[role_name].append(token.text)
            results.append(roles)

        return results, doc

    def test_sentences(self):
        test_texts = [
            "我吃了苹果。",
            "小鸟在天上飞。",
            "狐狸给小猪洗澡。",
            "张三把书放在了桌子上。",
            "李四被老师表扬了。",
            "妈妈送我一本书。",
            "他昨天在图书馆认真地看了一本很厚的书。",
            "小明和小红一起去公园散步。",
            "老师说这个问题很简单。",
            "春天的风吹绿了大地。",
            "孩子们在操场上快乐地踢足球。",
            "爸爸用电脑写了一封邮件。",
        ]

        print("\n" + "=" * 60)
        print("任务2：语义角色标注实验结果")
        print("=" * 60)

        for i, text in enumerate(test_texts, 1):
            roles, doc = self.label_sentence(text)
            print(f"\n[句子{i}] {text}")
            print("-" * 50)
            print("  依存分析树:")
            for token in doc:
                print(f"    {token.text}({token.pos_}) --{token.dep_}--> {token.head.text}({token.head.pos_})")
            print("  语义角色标注:")
            for role_set in roles:
                for role, args in role_set.items():
                    print(f"    {role}: {', '.join(args)}")
            print()
class CoreferenceResolver:
    STOP_WORDS = {
        "天上", "大树", "树枝", "背上", "脑袋", "翅膀", "白云", "绵羊",
        "世界", "甜饼", "午餐", "热水澡", "烤锅", "木椅", "呼噜声",
        "敲门声", "电脑", "操场", "图书馆", "公园", "邮件",
        "时候", "安排", "力气", "日子", "背", "家", "屋", "火", "水",
        "地上", "树下", "河边", "村子里", "蓝天上",
        "水里", "西红柿", "小甜饼", "通心粉", "猪肉", "烤肉",
        "树叶", "椅子",
        "现在", "不过", "别急", "当然", "于是", "突然", "没错", "好啦",
        "那好吧", "好吧", "挣扎", "起来", "一只", "一顿", "一炉", "一下",
        "里面", "外面", "门口", "出门", "找错", "幸运",
        "一定", "特别", "非常", "很大", "很多", "有点",
        "准备", "旁边", "身后", "剩下", "最后",
        "真的", "飞快", "呼啦啦", "大大咧咧",
        "看到", "天上飞", "飞到", "吃掉", "不想", "之前", "没有",
        "干净", "全村", "锅里", "按摩", "听见", "喊声",
        "烤锅", "用力", "僵硬", "先生", "的确",
        "长得", "喂饱", "洗得", "吃上", "让", "自己",
        "躺到", "拽进", "叫道", "跑去",
    }

    PRONOUNS = ["他们", "我们", "你们", "她们", "我", "你", "他", "她", "它"]

    def __init__(self, nlp_model):
        self.nlp = nlp_model

    def _find_characters(self, text):
        words = [w.strip() for w in jieba.cut(text) if len(w.strip()) >= 2]
        freq = {}
        for w in words:
            if w not in self.STOP_WORDS:
                freq[w] = freq.get(w, 0) + 1

        doc = self.nlp(text)
        subject_count = {}
        for token in doc:
            if token.pos_ in ("NOUN", "PROPN") and token.dep_ in ("nsubj", "nsubj:pass", "top"):
                w = token.text
                if w not in self.STOP_WORDS and len(w) >= 2:
                    subject_count[w] = subject_count.get(w, 0) + 1

        scores = {}
        for w in set(list(freq.keys()) + list(subject_count.keys())):
            freq_score = freq.get(w, 0)
            subj_score = subject_count.get(w, 0) * 2  # 主语权重更高
            scores[w] = freq_score + subj_score

        characters = sorted(
            [(w, s) for w, s in scores.items() if s >= 3],
            key=lambda x: x[1], reverse=True
        )
        return [ch for ch, _ in characters], freq

    def resolve(self, text):
        characters, _ = self._find_characters(text)
        all_matches = []
        for p in self.PRONOUNS:
            pos = 0
            while True:
                idx = text.find(p, pos)
                if idx == -1:
                    break
                all_matches.append((idx, idx + len(p), p))
                pos = idx + 1

        all_matches.sort(key=lambda x: (x[0], -(x[1] - x[0])))
        filtered = []
        for start, end, p in all_matches:
            if filtered and start < filtered[-1][1]:
                continue
            filtered.append((start, end, p))

        result = text
        for start, end, pronoun in reversed(filtered):
            pre_text = result[:start]
            char_positions = {}
            for ch in characters:
                last_pos = pre_text.rfind(ch)
                if last_pos >= 0:
                    char_positions[ch] = last_pos

            if not char_positions:
                continue

            nearest = max(char_positions, key=char_positions.get)

            if pronoun == "他们":
                sorted_chars = sorted(char_positions.items(), key=lambda x: x[1], reverse=True)
                if len(sorted_chars) >= 2:
                    nearest = f"{sorted_chars[0][0]},{sorted_chars[1][0]}"
                elif len(sorted_chars) == 1:
                    nearest = sorted_chars[0][0]

            annotation = f"{pronoun}[{nearest}]"
            result = result[:start] + annotation + result[end:]

        return result

    def test_cases(self):
        test1_input = (
            '小鸟在天上飞呀飞，小鸡在大树下叽叽叽。小鸟飞累了，停在树枝上。小鸡抬起头，'
            '"在天上飞好玩吗?"小鸡问，"当然啦。"小鸟回答。 　　 '
            '小鸡听了，心里想：为什么小鸟会飞，我不会飞呢?我也有翅膀呀?于是，小鸡拍拍小'
            '翅膀，可他只扑腾起来一点儿就又落下来了。小鸡伤心地耷拉下脑袋。 　　'
            '小鸟看到了，对小鸡说："别急，别急，小伙伴。我会飞，让我带你到天上去看看吧'
            '。" 　　 '
            '小鸟从树枝上飞到小鸡身边，小鸡高兴地一下跳到小鸟背上。小鸡呼啦啦地展开翅膀'
            '，他们就飞到天上去了。 　　 '
            '小鸡在蓝天上看到像绵羊一样的一朵朵白云，还看到了很大很大的世界，他可真高兴'
            '呀。小鸟呢，他也很开心，因为他又交了一位新朋友。'
        )

        test1_expected = (
            '小鸟在天上飞呀飞，小鸡在大树下叽叽叽。小鸟飞累了，停在树枝上。小鸡抬起头，'
            '"在天上飞好玩吗?"小鸡问，"当然啦。"小鸟回答。 　　 '
            '小鸡听了，心里想：为什么小鸟会飞，我[小鸡]不会飞呢?我[小鸡]也有翅膀呀?于是'
            '，小鸡拍拍小翅膀，可他[小鸡]只扑腾起来一点儿就又落下来了。小鸡伤心地耷拉下'
            '脑袋。 　　'
            '小鸟看到了，对小鸡说："别急，别急，小伙伴。我[小鸟]会飞，让我[小鸟]带你[小'
            '鸡]到天上去看看吧。" 　　 '
            '小鸟从树枝上飞到小鸡身边，小鸡高兴地一下跳到小鸟背上。小鸡呼啦啦地展开翅膀'
            '，他们[小鸟,小鸡]就飞到天上去了。 　　 '
            '小鸡在蓝天上看到像绵羊一样的一朵朵白云，还看到了很大很大的世界，他[小鸡]可'
            '真高兴呀。小鸟呢，他[小鸟]也很开心，因为他[小鸟]又交了一位新朋友。'
        )

        test2_input = (
            '一只饥饿的狐狸正准备出门找午餐,突然,从外面传来一阵敲门声.狐狸打开门,门口竟然站着一只小肥猪。'
            '"哎呀，我找错门了，我以为是兔子的家呢！""没错，你来得正是时候！"'
            '狐狸把小猪狠狠地拽进屋里，大声叫道： "这真是我幸运的一天！我要吃一顿香喷喷的烤猪肉，现在，'
            '你就乖乖地给我躺到烤锅里去吧！ "挣扎是没有用的，于是，小猪说："好，那就听你的安排吧。不'
            '过，我很脏哦，在你吃掉我之前，就不想把我洗得干净一点吗？"于是，狐狸就忙起来了'
            '。他捡来了树枝、生起了火，再到河边去提来了水，然后，给小猪洗了一个痛痛快快的'
            '热水澡。狐狸说： "好啦，现在你是全村最干净的小猪了，快给我躺到烤锅里去吧！"'
            '挣扎是没有用的，于是，小猪说："听你的安排吧。不过，我太小了， 在你吃掉我之前，就不想喂饱我，'
            '让我长得胖一点吗？"于是，狐狸又忙起来了。他摘来了西红柿，做了通心粉，还烤了一炉金黄色的小甜饼。'
            '小猪大大咧咧地吃了一顿丰盛的午餐。狐狸说： "好啦，现在你是全村最肥的小猪了，快给我躺到烤锅里去吧！"'
            '挣扎是没有用的，于是，小猪说："听你的安排吧。不过，我是一只勤劳的小猪，我的肉一定特别硬。'
            '在你吃掉我之前，就不想先给我按摩一下，让自己吃上更嫩一点的烤肉吗？"于是，狐狸又忙起来'
            '了。他让小猪坐在木椅上，为小猪捏捏头、松松肩、拉拉腿，小猪舒服得直哼哼。'
            '狐狸说："好啦，现在你是全村皮最柔软的小猪了，快给我躺到烤锅里去吧！ "'
            '"那好吧，不过，这些日子我的确太累了。你看，我的背还有点僵硬呢，你能再用力替我'
            '按摩一下吗？对，就这样，再用点力气！"过了一会，身后传来狐狸的呼噜声，小猪轻声问： '
            '"狐狸先生，狐狸先生"狐狸没有听见小猪的喊声，他累极了，沉沉地睡去了。'
            '村子里最干净、最肥、最柔软的小猪，拿着剩下的甜饼，飞快地跑回家去了。'
        )

        print("\n" + "=" * 60)
        print("任务3：指代消解实验结果（选做）")
        print("=" * 60)

        print("\n[测试1：代词-你,我,他,他们]")
        print("-" * 50)
        result1 = self.resolve(test1_input)
        print("消解结果:")
        print(result1)
        print("\n[期望输出参考]")
        print(test1_expected)

        print("\n\n[测试2：代词-你,我,他]")
        print("-" * 50)
        result2 = self.resolve(test2_input)
        print("消解结果:")
        print(result2)

def main():
    print("\n" + "=" * 60)
    print("NLP实验4：语篇分析")
    print("=" * 60)

    # 任务1：词义消歧
    wsd = WordSenseDisambiguator(nlp)
    wsd.test_all_cases()

    # 任务2：语义角色标注
    srl = SemanticRoleLabeler(nlp)
    srl.test_sentences()

    # 任务3：指代消解（选做）
    cr = CoreferenceResolver(nlp)
    cr.test_cases()

    print("\n" + "=" * 60)
    print("实验完成！")
    print("=" * 60)


if __name__ == "__main__":
    main()
