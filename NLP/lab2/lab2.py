import jieba
import os

class ForwardMaximumMatching:
    def __init__(self, vocab_set):
        self.vocab = vocab_set
        self.max_word_length = max([len(word) for word in vocab_set]) if vocab_set else 0

    def cut(self, text):
        result = []
        index = 0
        text_length = len(text)

        while index < text_length:
            matched = False
            for length in range(self.max_word_length, 0, -1):
                if index + length <= text_length:
                    word = text[index: index + length]
                    if word in self.vocab:
                        result.append(word)
                        index += length
                        matched = True
                        break

            if not matched:
                result.append(text[index])
                index += 1

        return result

def calculate_accuracy(predicted_words, gold_words):
    def get_word_intervals(words):
        intervals = []
        start = 0
        for w in words:
            intervals.append((start, start + len(w)))
            start += len(w)
        return set(intervals)

    pred_intervals = get_word_intervals(predicted_words)
    gold_intervals = get_word_intervals(gold_words)

    correct_count = len(pred_intervals.intersection(gold_intervals))

    if len(pred_intervals) == 0:
        return 0.0
    return correct_count / len(pred_intervals)

def load_dictionary(dict_path):
    vocab_set = set()
    print(f"正在加载词典文件: {dict_path} ...")
    try:
        with open(dict_path, 'r', encoding='utf-8') as f:
            for line in f:
                parts = line.strip().split()
                if parts:
                    word = parts[0]  # 取每行的第一部分（即词语本身）
                    vocab_set.add(word)
        print(f"词典加载完成，共载入 {len(vocab_set)} 个词汇。\n")
    except Exception as e:
        print(f"词典加载失败，请检查路径和编码: {e}")
    return vocab_set

def main():
    dict_file = "dict.txt"
    test_file = "test.txt"
    gold_file = "gold.txt"
    output_file = "result.txt"

    if not os.path.exists(test_file) or not os.path.exists(gold_file):
        print("错误：找不到 test.txt 或 gold.txt，请先创建它们！")
        return

    my_vocab = load_dictionary(dict_file)
    if not my_vocab:
        return

    fmm_segmenter = ForwardMaximumMatching(my_vocab)

    print("================ 开始读取语料并评测 =================\n")
    total_fmm_acc = 0
    total_jieba_acc = 0
    valid_sentences_count = 0
    with open(test_file, 'r', encoding='utf-8') as f_test, \
            open(gold_file, 'r', encoding='utf-8') as f_gold,\
            open(output_file, 'w', encoding='utf-8') as f_out:

        for i, (raw_line, gold_line) in enumerate(zip(f_test, f_gold), 1):
            sentence = raw_line.strip()
            gold_str = gold_line.strip()

            if not sentence or not gold_str:
                continue

            gold_standard = gold_str.split()
            fmm_result = fmm_segmenter.cut(sentence)
            fmm_acc = calculate_accuracy(fmm_result, gold_standard)
            total_fmm_acc += fmm_acc

            jieba_result = list(jieba.cut(sentence))
            jieba_acc = calculate_accuracy(jieba_result, gold_standard)
            total_jieba_acc += jieba_acc

            valid_sentences_count += 1
            f_out.write(f"--- 句子 {i} ---\n")
            f_out.write(f"原句: {sentence}\n")
            f_out.write(f"FMM结果: {' / '.join(fmm_result)}  (准确率: {fmm_acc * 100:.2f}%)\n")
            f_out.write(f"Jieba结果: {' / '.join(jieba_result)}  (准确率: {jieba_acc * 100:.2f}%)\n")
            f_out.write("\n")
            if i <= 5:
                print(f"--- 句子 {i} ---")
                print(f"[待 分 词]: {sentence}")
                print(f"[标准答案]: {' / '.join(gold_standard)}")
                print(f"[FMM 结果]: {' / '.join(fmm_result)}  | 准确率: {fmm_acc * 100:.2f}%")
                print(f"[Jieba结果]: {' / '.join(jieba_result)}  | 准确率: {jieba_acc * 100:.2f}%\n")

    if valid_sentences_count > 0:
        avg_fmm = total_fmm_acc / valid_sentences_count
        avg_jieba = total_jieba_acc / valid_sentences_count
        print("================ 测试集最终评测结果 =================\n")
        print(f"有效测试句子数: {valid_sentences_count}")
        print(f"自写 FMM 算法平均正确率:   {avg_fmm * 100:.2f}%")
        print(f"Jieba 分词工具平均正确率:  {avg_jieba * 100:.2f}%")
    else:
        print("没有读取到有效的句子，请检查你的 txt 文件内容。")


if __name__ == "__main__":
    main()