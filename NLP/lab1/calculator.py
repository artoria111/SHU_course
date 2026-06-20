import math
import re
from collections import Counter


def calculate_entropy(freq_dict, total_tokens):
    entropy = 0.0
    for count in freq_dict.values():
        probability = count / total_tokens
        entropy -= probability * math.log2(probability)
    return entropy


def print_frequency_sums(sorted_counts, thresholds):
    for n in thresholds:
        top_n_sum = sum(count for item, count in sorted_counts[:n])
        print(f"前 {n:<4} 个高频项的频数总和: {top_n_sum}")


def process_chinese(text):
    print(f"\n================ 【中文语料统计】 ================")
    chinese_characters = re.findall(r'[\u4e00-\u9fa5]', text)
    total_chars = len(chinese_characters)
    print(f"语料总有效汉字数: {total_chars}")

    if total_chars == 0:
        print("未检测到汉字。\n")
        return

    counter = Counter(chinese_characters)
    sorted_counts = counter.most_common()

    print("\n--- 1. 前100个高频汉字 ---")
    for i in range(15):
        if i<14:
            print(sorted_counts[i*7:i*7+7])
        else:
            print(sorted_counts[98:100])


    print("\n--- 2. 字频总和统计 ---")
    thresholds = [1, 20, 100, 600, 2000, 3000]
    print_frequency_sums(sorted_counts, thresholds)

    entropy = calculate_entropy(counter, total_chars)
    print(f"\n--- 3. 信息熵计算 ---")
    print(f"该中文语料的信息熵为: {entropy:.4f} 比特/字\n")


def process_english(text):
    print(f"\n================ 【英文语料统计】 ================")
    english_words = re.findall(r'[a-z]+', text.lower())
    total_words = len(english_words)
    print(f"语料总有效单词数: {total_words}")

    if total_words == 0:
        print("未检测到英文单词。\n")
        return

    counter = Counter(english_words)
    sorted_counts = counter.most_common()
    print("\n--- 1. 前100个高频单词 ---")
    for i in range(15):
        if i<14:
            print(sorted_counts[i*7:i*7+7])
        else:
            print(sorted_counts[98:100])
    print("\n--- 2. 词频总和统计 ---")
    thresholds = [1, 20, 100, 600, 2000, 3000]
    print_frequency_sums(sorted_counts, thresholds)
    entropy = calculate_entropy(counter, total_words)
    print(f"\n--- 3. 信息熵计算 ---")
    print(f"该英文语料的信息熵为: {entropy:.4f} 比特/词\n")


def read_corpus_file(file_path):
    encodings_to_try = ['utf-8', 'gb18030', 'gbk', 'latin-1']
    for enc in encodings_to_try:
        try:
            with open(file_path, 'r', encoding=enc) as f:
                content = f.read()
                print(f"成功使用 '{enc}' 编码读取文件: {file_path}")
                return content
        except UnicodeDecodeError:
            continue
        except Exception as e:
            print(f"读取文件 {file_path} 时发生系统错误: {e}")
            return ""


def main():

    cn_corpus = read_corpus_file("狂人日记.txt")
    process_chinese(cn_corpus)

    en_corpus = read_corpus_file("爱丽丝梦游仙境en.txt")
    process_english(en_corpus)



if __name__ == "__main__":
    main()