import spacy
from spacy.tokens import Doc
import stanza
import json
import os

MY_GOLD_HEADS = {
    0: [2, 8, 8, 5, 8, 5, 8, 9, 15, 15, 13, 13, 15, 13, 0],
    1: [6, 6, 4, 6, 4, 0],
    2: [9, 7, 5, 5, 7, 5, 9, 9, 0, 9, 9, 9, 12, 9, 18, 18, 18, 14],
    3: [2, 4, 2, 6, 6, 0, 6],
    4: [2, 4, 4, 0, 4, 8, 6, 10, 8, 4],
    5: [0, 3, 1, 3, 4, 4],
    6: [13, 13, 13, 7, 6, 7, 12, 7, 10, 7, 7, 13, 0],
    7: [7, 7, 7, 7, 7, 7, 0],
    8: [6, 6, 4, 2, 2, 7, 0, 9, 7],
    9: [2, 7, 2, 5, 7, 7, 0, 9, 7, 9],
    10: [4, 3, 4, 0, 4, 7, 4],
    11: [3, 3, 5, 5, 0, 8, 8, 5],
    12: [2, 4, 2, 5, 0, 5, 5, 10, 8, 5, 12, 10],
    13: [3, 1, 5, 5, 0, 7, 5, 7, 10, 12, 12, 8],
    14: [2, 4, 4, 0, 6, 8, 8, 4],
    15: [5, 1, 5, 5, 0],
    16: [2, 3, 5, 5, 0, 5, 8, 10, 10, 5],
    17: [9, 9, 9, 5, 6, 9, 6, 9, 0],
    18: [3, 3, 0, 3, 8, 5, 8, 3],
    19: [2, 0, 9, 5, 9, 5, 9, 9, 2],
    20: [7, 7, 7, 7, 4, 7, 0],
    21: [2, 0, 9, 9, 8, 8, 8, 4, 2],
    22: [4, 4, 4, 0, 4, 7, 4, 9, 7],
    23: [6, 6, 5, 5, 6, 0, 8, 6]
}

class WhitespaceTokenizer:
    def __init__(self, vocab):
        self.vocab = vocab

    def __call__(self, text):
        words = text.split(' ')
        return Doc(self.vocab, words=words)


def setup_nlp_tools():
    print("正在加载 NLP 模型...")
    nlp_spacy = spacy.load("zh_core_web_sm")
    nlp_spacy.tokenizer = WhitespaceTokenizer(nlp_spacy.vocab)

    nlp_stanza = stanza.Pipeline('zh',
                                 processors='tokenize,pos,lemma,depparse',
                                 tokenize_pretokenized=True,
                                 use_gpu=True,download_method=None)
    return nlp_spacy, nlp_stanza


def load_gold_sentences(filepath="gold.txt"):
    word_lists = []
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            for line in f:
                line = line.strip()
                if line:
                    word_lists.append(line.split(' '))
        return word_lists
    except FileNotFoundError:
        print(f"未找到 {filepath}，请确保文件存在。")
        return []

def run_parsing_and_save(nlp_spacy, nlp_stanza, word_lists):
    results_spacy = []
    results_stanza = []

    print("\n================ 开始进行强制预分词的依存句法分析 ================")
    for i, words in enumerate(word_lists):
        raw_text = "".join(words)
        text_with_spaces = " ".join(words)

        doc_spacy = nlp_spacy(text_with_spaces)
        spacy_sentence_res = {"id": i, "text": raw_text, "tokens": []}
        for token in doc_spacy:
            head_idx = token.head.i + 1 if token.dep_ != "ROOT" else 0
            spacy_sentence_res["tokens"].append({
                "id": token.i + 1, "word": token.text, "pos": token.pos_,
                "head": head_idx, "deprel": token.dep_
            })
        results_spacy.append(spacy_sentence_res)

        doc_stanza = nlp_stanza([words])
        stanza_sentence_res = {"id": i, "text": raw_text, "tokens": []}
        for word in doc_stanza.sentences[0].words:
            stanza_sentence_res["tokens"].append({
                "id": word.id, "word": word.text, "pos": word.upos,
                "head": word.head, "deprel": word.deprel
            })
        results_stanza.append(stanza_sentence_res)

    with open("parsing_result_spacy.json", "w", encoding="utf-8") as f:
        json.dump(results_spacy, f, ensure_ascii=False, indent=4)
    with open("parsing_result_stanza.json", "w", encoding="utf-8") as f:
        json.dump(results_stanza, f, ensure_ascii=False, indent=4)

    return results_spacy, results_stanza

def evaluate_accuracy(results_spacy, results_stanza, gold_heads):
    print("\n================ 依存句法分析正确率对比 (UAS) ================")

    if not gold_heads:
        print("提示：你尚未在代码顶部配置 MY_GOLD_HEADS 字典，无法计算准确率。")
        return

    total_spacy_correct = 0
    total_stanza_correct = 0
    total_valid_tokens = 0
    valid_sentence_count = 0

    for idx, gold_head_list in gold_heads.items():
        if idx >= len(results_spacy):
            continue

        text = results_spacy[idx]['text']
        print(f"评测句子 [{idx + 1}]: {text}")

        spacy_pred = [token['head'] for token in results_spacy[idx]['tokens']]
        stanza_pred = [token['head'] for token in results_stanza[idx]['tokens']]
        length = len(gold_head_list)

        if len(spacy_pred) != length:
            print(f"  [警告] 无法计算！你提供的真值长度({length})与本句的分词数({len(spacy_pred)})不一致。")
            continue

        spacy_correct = sum(1 for i in range(length) if spacy_pred[i] == gold_head_list[i])
        stanza_correct = sum(1 for i in range(length) if stanza_pred[i] == gold_head_list[i])

        total_spacy_correct += spacy_correct
        total_stanza_correct += stanza_correct
        total_valid_tokens += length
        valid_sentence_count += 1

        print(f"  -> spaCy 预测:  {spacy_pred}")
        print(f"  -> Stanza 预测: {stanza_pred}")
        print(f"  -> 人工真值:    {gold_head_list}")
        print(f"  ★ spaCy UAS 正确率:  {(spacy_correct / length) * 100:.2f}%")
        print(f"  ★ Stanza UAS 正确率: {(stanza_correct / length) * 100:.2f}%\n")

    if total_valid_tokens > 0:
        overall_spacy_acc = (total_spacy_correct / total_valid_tokens) * 100
        overall_stanza_acc = (total_stanza_correct / total_valid_tokens) * 100

        print("================ 总体评测结果总结 ================")
        print(f"✅ 有效评测句子数: {valid_sentence_count} 句")
        print(f"✅ 参与评测总词数: {total_valid_tokens} 个词")
        print(f"🏆 spaCy 总体平均 UAS 正确率:  {overall_spacy_acc:.2f}%")
        print(f"🏆 Stanza 总体平均 UAS 正确率: {overall_stanza_acc:.2f}%")
        print("================================================\n")

def batch_visualize_to_html(results_list, folder_name):
    print(f"正在生成 {folder_name} 的 HTML 可视化文件...")
    os.makedirs(folder_name, exist_ok=True)

    for sentence_data in results_list:
        sentence_id = sentence_data["id"] + 1
        custom_words = [{"text": "ROOT\n[0]", "tag": "ROOT"}]
        custom_arcs = []

        for tok in sentence_data["tokens"]:
            word_idx, head_idx = tok["id"], tok["head"]
            custom_words.append({"text": f"{tok['word']}\n[{word_idx}]", "tag": tok["pos"]})

            if head_idx == word_idx:
                continue

            start_node = min(head_idx, word_idx)
            end_node = max(head_idx, word_idx)
            direction = "right" if head_idx < word_idx else "left"

            custom_arcs.append({
                "start": start_node, "end": end_node,
                "label": tok["deprel"], "dir": direction
            })

        manual_data = {"words": custom_words, "arcs": custom_arcs}
        html_content = spacy.displacy.render(
            manual_data, style="dep", manual=True, page=True,
            options={"distance": 120, "word_spacing": 35, "arrow_stroke": 2, "arrow_width": 8}
        )

        if "<head>" in html_content:
            html_content = html_content.replace("<head>", "<head>\n    <meta charset=\"utf-8\">")

        file_path = os.path.join(folder_name, f"sentence_{sentence_id}.html")
        with open(file_path, "w", encoding="utf-8") as f:
            f.write(html_content)

def main():
    nlp_spacy, nlp_stanza = setup_nlp_tools()

    word_lists = load_gold_sentences("gold.txt")
    if not word_lists:
        return

    res_spacy, res_stanza = run_parsing_and_save(nlp_spacy, nlp_stanza, word_lists)

    evaluate_accuracy(res_spacy, res_stanza, MY_GOLD_HEADS)

    print("================ 启动句法树批量可视化 ================")
    batch_visualize_to_html(res_spacy, "spacy_html")
    batch_visualize_to_html(res_stanza, "stanza_html")
    print("全部网页生成完毕！请在当前目录下查看 spacy_html 和 stanza_html 文件夹。")


if __name__ == "__main__":
    main()