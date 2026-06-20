import spacy
import jieba
import re
print("=" * 60)
print("加载spaCy中文模型 zh_core_web_sm ...")
nlp = spacy.load("zh_core_web_sm")
print("模型加载完成！可用组件:", nlp.pipe_names)
print("=" * 60)
CN_SURNAMES = set("""
赵钱孙李周吴郑王冯陈褚卫蒋沈韩杨朱秦尤许何吕施张
孔曹严华金魏陶姜戚谢邹喻柏水窦章云苏潘葛奚范彭郎
鲁韦昌马苗凤花方俞任袁柳酆鲍史唐费廉岑薛雷贺倪汤
滕殷罗毕郝邬安常乐于时傅皮下齐康伍余元卜顾孟平黄
和穆萧尹姚邵湛汪祁毛禹狄米贝明臧计伏成戴谈宋茅庞
熊纪舒屈项祝董梁杜阮蓝闵席季麻强贾路娄危江童颜郭
梅盛林刁钟徐邱骆高夏蔡田樊胡凌霍虞万支柯昝管卢莫
经房裘缪干解应宗丁宣贲邓郁单杭洪包诸左石崔吉钮龚
程嵇邢滑裴陆荣翁荀羊於惠甄麴家封芮羿储靳汲邴糜松
井段富巫乌焦巴弓牧隗山谷车侯宓蓬全郗班仰秋仲伊宫
宁仇栾暴甘钭厉戎祖武符刘景詹束龙叶幸司韶郜黎蓟薄
印宿白怀蒲邰从鄂索咸籍赖卓蔺屠蒙池乔阴鬱胥能苍双
闻莘党翟谭贡劳逄姬申扶堵冉宰郦雍卻璩桑桂濮牛寿通
边扈燕冀郏浦尚农温别庄晏柴瞿阎充慕连茹习宦艾鱼容
向古易慎戈廖庾终暨居衡步都耿满弘匡国文寇广禄阙东
欧殳沃利蔚越夔隆师巩厍聂晁勾敖融冷訾辛阚那简饶空
曾毋沙乜养鞠须丰巢关蒯相查后荆红游竺权逯盖益桓公
万俟司马上官欧阳夏侯诸葛闻人东方赫连皇甫尉迟公羊
澹台公冶宗政濮阳淳于单于太叔申屠公孙仲孙轩辕令狐
钟离宇文长孙慕容鲜于闾丘司徒司空丌官司寇仉督子车
颛孙端木巫马公西漆雕乐正壤驷公良拓跋夹谷宰父谷梁
段干百里东郭南门呼延归海羊舌微生岳帅缑亢况后有琴
梁丘左丘东门西门商牟佘佴伯赏南宫墨哈谯笪年爱阳佟
""".replace("\n", ""))

class NamedEntityRecognizer:
    LOC_SUFFIXES = ["省", "市", "县", "区", "镇", "村", "路", "街", "巷",
                   "山", "河", "湖", "海", "江", "岛", "湾", "港"]

    ORG_SUFFIXES = ["公司", "集团", "大学", "学院", "中学", "小学", "医院",
                   "银行", "研究所", "中心", "局", "部", "处", "厅", "院",
                   "社", "行", "厂", "店", "委员会", "协会"]

    ORG_COMPOUNDS = ["有限", "股份", "技术", "科技", "发展", "实业", "控股",
                    "责任", "工程", "信息", "国际", "管理", "咨询", "服务"]

    def __init__(self, nlp_model):
        self.nlp = nlp_model

    def recognize(self, text):
        doc = self.nlp(text)
        entities = {}
        used_spans = set()
        for ent in doc.ents:
            if ent.label_ in ("PERSON", "GPE", "ORG", "LOC"):
                etype = ent.label_
                if etype == "LOC":
                    etype = "GPE"
                key = (ent.text, etype)
                entities[key] = entities.get(key, 0) + 1
                for i in range(ent.start, ent.end):
                    used_spans.add(i)
        NAME_BLACKLIST = {"教育", "高校", "上海", "北京", "深圳", "杭州", "南京", "广州",
                         "成都", "武汉", "公司", "有限", "技术", "工程", "信息", "科技",
                         "人民", "中华", "中国", "美国", "日本", "韩国"}
        for token in doc:
            if token.i in used_spans:
                continue
            if token.ent_type_ in ("GPE", "ORG", "LOC"):
                continue
            if token.text in NAME_BLACKLIST:
                continue
            if token.pos_ in ("PROPN", "NOUN") and 2 <= len(token.text) <= 3:
                if token.text[0] in CN_SURNAMES:
                    entities[(token.text, "PERSON")] = entities.get((token.text, "PERSON"), 0) + 1
                    for i in range(token.i, token.i + 1):
                        used_spans.add(i)


        for token in doc:
            if token.pos_ == "PROPN" and 2 <= len(token.text) <= 3:
                if token.text not in NAME_BLACKLIST:
                    if token.text[0] in CN_SURNAMES and token.ent_type_ in ("GPE", "LOC"):
                        entities[(token.text, "PERSON")] = entities.get((token.text, "PERSON"), 0) + 1


        for token in doc:
            if token.i in used_spans:
                continue
            if token.pos_ in ("PROPN", "NOUN") and len(token.text) >= 2:
                for suffix in self.ORG_SUFFIXES:
                    if token.text.endswith(suffix) and len(token.text) > len(suffix):
                        is_loc = False
                        for ls in self.LOC_SUFFIXES:
                            if token.text.endswith(ls):
                                is_loc = True
                                break
                        if not is_loc:
                            entities[(token.text, "ORG")] = entities.get((token.text, "ORG"), 0) + 1
                            break

        return list(entities.keys())

    def evaluate(self, test_cases):
        total_gold = 0
        total_pred = 0
        total_correct = 0

        print("\n" + "=" * 60)
        print("任务1：命名实体识别实验结果")
        print("=" * 60)

        for i, (text, gold_entities) in enumerate(test_cases, 1):
            predictions = self.recognize(text)
            pred_set = set(predictions)
            gold_set = set(gold_entities)

            correct = pred_set & gold_set
            missed = gold_set - pred_set
            extra = pred_set - gold_set

            total_gold += len(gold_set)
            total_pred += len(pred_set)
            total_correct += len(correct)

            print(f"\n[测试{i}] {text}")
            print(f"  金标: {gold_set}")
            print(f"  预测: {pred_set}")
            if correct:
                print(f"  正确: {correct}")
            if missed:
                print(f"  漏识: {missed}")
            if extra:
                print(f"  误识: {extra}")

        precision = total_correct / total_pred if total_pred > 0 else 0
        recall = total_correct / total_gold if total_gold > 0 else 0
        f1 = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0

        print(f"\n{'-' * 40}")
        print(f"总体评估:")
        print(f"  金标实体总数: {total_gold}")
        print(f"  预测实体总数: {total_pred}")
        print(f"  正确识别数: {total_correct}")
        print(f"  准确率(Precision): {precision:.2%}")
        print(f"  召回率(Recall): {recall:.2%}")
        print(f"  F1值: {f1:.2%}")

        return precision, recall, f1


def build_ner_test_cases():
    return [
        ("张三和李四去了北京出差。", [
            ("张三", "PERSON"), ("李四", "PERSON"), ("北京", "GPE")
        ]),
        ("小明在清华大学读书。", [
            ("小明", "PERSON"), ("清华大学", "ORG")
        ]),
        ("上海是一座国际化大都市。", [
            ("上海", "GPE")
        ]),
        ("马云创办了阿里巴巴公司。", [
            ("马云", "PERSON"), ("阿里巴巴公司", "ORG")
        ]),
        ("王伟在中国银行工作，他住在南京。", [
            ("王伟", "PERSON"), ("中国银行", "ORG"), ("南京", "GPE")
        ]),
        ("华为技术有限公司发布了最新的5G芯片。", [
            ("华为技术有限公司", "ORG")
        ]),
        ("李华毕业于北京大学后去了深圳的腾讯公司。", [
            ("李华", "PERSON"), ("北京大学", "ORG"), ("深圳", "GPE"), ("腾讯公司", "ORG")
        ]),
        ("教育部发布了新的教育政策。", [
            ("教育部", "ORG")
        ]),
        ("乔布斯在苹果公司发布了iPhone。", [
            ("乔布斯", "PERSON"), ("苹果公司", "ORG")
        ]),
        ("杭州的浙江大学是一所著名高校。", [
            ("杭州", "GPE"), ("浙江大学", "ORG")
        ]),
        ("习近平主席在北京人民大会堂会见了美国代表团。", [
            ("习近平", "PERSON"), ("北京", "GPE"), ("人民大会堂", "ORG")
        ]),
        ("张经理在上海市浦东新区工作。", [
            ("张经理", "PERSON"), ("上海市", "GPE"), ("浦东新区", "GPE")
        ]),
    ]

class EntityRelationExtractor:
    def __init__(self, nlp_model, ner):
        self.nlp = nlp_model
        self.ner = ner

    def extract_relations(self, text):
        doc = self.nlp(text)
        pred_entities = self.ner.recognize(text)
        relations = []

        if len(pred_entities) < 2:
            return relations
        entity_map = {}
        for ent_text, ent_type in pred_entities:
            for token in doc:
                if token.text == ent_text or token.text in ent_text:
                    if token.i not in entity_map:
                        entity_map[token.i] = (ent_text, ent_type)

        for token in doc:
            if token.text == "是" and token.dep_ == "cop":
                root = token.head
                subj = None
                for child in root.children:
                    if child.dep_ == "nsubj":
                        subj = child
                        break
                if subj and root:
                    subj_ent = self._find_entity(subj, entity_map, doc)
                    obj_ent = self._find_entity(root, entity_map, doc)
                    if subj_ent and not obj_ent:
                        obj_text = self._get_full_text(root, doc)
                        if obj_text:
                            relations.append((subj_ent, "is_a", obj_text))
                    elif subj_ent and obj_ent and subj_ent != obj_ent:
                        relations.append((subj_ent, "is_a", obj_ent))
            elif token.text in ("位于", "坐落", "坐落于"):
                subj = self._find_child(token, "nsubj")
                obj = self._find_child(token, "dobj")
                if not obj:
                    obj = self._find_child(token, "nmod:prep")
                if subj and obj:
                    subj_ent = self._find_entity(subj, entity_map, doc)
                    obj_ent = self._find_entity(obj, entity_map, doc)
                    if subj_ent and obj_ent:
                        relations.append((subj_ent, "located_in", obj_ent))

            elif token.text in ("创办", "创立", "创建", "成立", "建立", "组建"):
                subj = self._find_child(token, "nsubj")
                obj = self._find_child(token, "dobj")
                if subj and obj:
                    subj_ent = self._find_entity(subj, entity_map, doc)
                    obj_ent = self._find_entity(obj, entity_map, doc)
                    if subj_ent and obj_ent:
                        relations.append((subj_ent, "founded_by", obj_ent))

            elif token.dep_ == "nmod:assmod" and token.pos_ in ("NOUN", "PROPN"):
                head = token.head
                dep_ent = self._find_entity(token, entity_map, doc)
                head_ent = self._find_entity(head, entity_map, doc)
                if dep_ent and not head_ent:
                    head_text = self._get_full_text(head, doc)
                    if head_text and head_text != dep_ent:
                        relations.append((dep_ent, "has_part", head_text))
                elif dep_ent and head_ent and dep_ent != head_ent:
                    relations.append((dep_ent, "has_part", head_ent))

        if not relations:
            for token in doc:
                if token.pos_ == "VERB" and token.dep_ == "ROOT":
                    subj = self._find_child(token, "nsubj")
                    obj = self._find_child(token, "dobj")
                    if subj and obj:
                        subj_ent = self._find_entity(subj, entity_map, doc)
                        obj_ent = self._find_entity(obj, entity_map, doc)
                        if subj_ent and obj_ent and subj_ent != obj_ent:
                            rel = self._infer_relation(token.text)
                            relations.append((subj_ent, rel, obj_ent))

        seen = set()
        unique = []
        for r in relations:
            if r not in seen:
                seen.add(r)
                unique.append(r)
        return unique

    def _find_child(self, token, dep_label):
        for child in token.children:
            if child.dep_ == dep_label:
                return child
        return None

    def _find_entity(self, token, entity_map, doc):
        if token is None:
            return None
        if token.i in entity_map:
            return entity_map[token.i][0]
        for child in token.children:
            if child.dep_ in ("compound:nn", "nmod:assmod") and child.i in entity_map:
                return entity_map[child.i][0]
        for tidx, (etext, etype) in entity_map.items():
            if token.text == etext or token.text in etext or etext in token.text:
                return etext
        if token.dep_ in ("compound:nn",) and token.head.i in entity_map:
            return entity_map[token.head.i][0]
        return None

    def _get_full_text(self, token, doc):
        parts = []
        for child in sorted(token.lefts, key=lambda x: x.i):
            if child.dep_ in ("compound:nn", "amod", "nmod:assmod"):
                if not parts or child.i == parts[-1][1]:
                    parts.append((child.text, child.i + 1))
        text_parts = [p[0] for p in parts]
        text_parts.append(token.text)
        return "".join(text_parts) if text_parts else None

    def _infer_relation(self, verb):
        if verb in ("是", "为", "属于"):
            return "is_a"
        elif verb in ("位于", "坐落", "在"):
            return "located_in"
        elif verb in ("创办", "创立", "创建", "成立", "建立", "组建"):
            return "founded_by"
        elif verb in ("工作", "任职", "供职"):
            return "employed_by"
        else:
            return "related_to"

    def evaluate(self, test_cases):
        total_gold = 0
        total_correct = 0
        total_pred = 0

        print("\n" + "=" * 60)
        print("任务2：实体关系抽取实验结果")
        print("=" * 60)

        for i, (text, gold_relations) in enumerate(test_cases, 1):
            predictions = self.extract_relations(text)
            gold_set = set(gold_relations)
            pred_set = set(predictions)

            correct = pred_set & gold_set
            missed = gold_set - pred_set
            extra = pred_set - gold_set

            total_gold += len(gold_set)
            total_pred += len(pred_set)
            total_correct += len(correct)

            print(f"\n[测试{i}] {text}")
            print(f"  金标: {gold_set}")
            print(f"  预测: {pred_set if pred_set else '无'}")
            if correct:
                print(f"  正确: {correct}")
            if missed:
                print(f"  漏识: {missed}")
            if extra:
                print(f"  误识: {extra}")

        precision = total_correct / total_pred if total_pred > 0 else 0
        recall = total_correct / total_gold if total_gold > 0 else 0
        f1 = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0

        print(f"\n{'-' * 40}")
        print(f"总体评估:")
        print(f"  金标关系总数: {total_gold}")
        print(f"  预测关系总数: {total_pred}")
        print(f"  正确识别数: {total_correct}")
        print(f"  准确率(Precision): {precision:.2%}")
        print(f"  召回率(Recall): {recall:.2%}")
        print(f"  F1值: {f1:.2%}")

        return precision, recall, f1


def build_re_test_cases():
    return [
        ("清华大学位于北京市海淀区。", [
            ("清华大学", "located_in", "北京市海淀区")
        ]),
        ("马云创办了阿里巴巴公司。", [
            ("马云", "founded_by", "阿里巴巴公司")
        ]),
        ("北京大学的信息工程学院很有名。", [
            ("北京大学", "has_part", "信息工程学院")
        ]),
        ("华为公司的总部位于深圳。", [
            ("华为公司", "has_part", "总部"),
            ("总部", "located_in", "深圳"),
        ]),
        ("任正非创立了华为技术有限公司。", [
            ("任正非", "founded_by", "华为技术有限公司")
        ]),
        ("腾讯公司坐落于深圳。", [
            ("腾讯公司", "located_in", "深圳")
        ]),
        ("李彦宏创建了百度公司。", [
            ("李彦宏", "founded_by", "百度公司")
        ]),
    ]

def main():
    print("\n" + "=" * 60)
    print("NLP实验5：语义分析")
    print("=" * 60)
    ner = NamedEntityRecognizer(nlp)
    test_cases = build_ner_test_cases()
    ner.evaluate(test_cases)
    re_extractor = EntityRelationExtractor(nlp, ner)
    re_test_cases = build_re_test_cases()
    re_extractor.evaluate(re_test_cases)
    print("\n" + "=" * 60)
    print("实验完成！")
    print("=" * 60)


if __name__ == "__main__":
    main()
