#huffman_compress
import numpy as np
import cv2 as cv
import heapq
import pickle


class HuffmanNode:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq


def build_huffman_tree(frequencies):
    heap = [HuffmanNode(char, freq) for char, freq in frequencies.items()]
    heapq.heapify(heap)

    while len(heap) > 1:
        node1 = heapq.heappop(heap)
        node2 = heapq.heappop(heap)
        merged = HuffmanNode(None, node1.freq + node2.freq)
        merged.left = node1
        merged.right = node2
        heapq.heappush(heap, merged)
    return heap[0]


def generate_codes(node, current_code, codes):
    if node is None:
        return
    if node.char is not None:
        codes[node.char] = current_code
    generate_codes(node.left, current_code + "0", codes)
    generate_codes(node.right, current_code + "1", codes)


def compress_image(image_path, output_path):
    img = cv.imread(image_path, cv.IMREAD_GRAYSCALE)
    rows, cols = img.shape
    pixels = img.flatten()

    freq_dict = {}
    for p in pixels:
        freq_dict[p] = freq_dict.get(p, 0) + 1

    root = build_huffman_tree(freq_dict)
    codes = {}
    generate_codes(root, "", codes)

    encoded_str = "".join([codes[p] for p in pixels])

    padding = 8 - len(encoded_str) % 8
    encoded_str += "0" * padding

    byte_array = bytearray()
    for i in range(0, len(encoded_str), 8):
        byte_array.append(int(encoded_str[i:i + 8], 2))

    data_to_save = {
        'shape': (rows, cols),
        'codes': codes,
        'padding': padding,
        'data': byte_array
    }
    with open(output_path, 'wb') as f:
        pickle.dump(data_to_save, f)

    print(f"压缩完成！原图大小: {img.size} bytes, 压缩后数据大小: {len(byte_array)} bytes")
    return codes


def decompress_image(input_path, output_path):
    with open(input_path, 'rb') as f:
        loaded_data = pickle.load(f)

    shape = loaded_data['shape']
    codes = loaded_data['codes']
    padding = loaded_data['padding']
    byte_data = loaded_data['data']

    bit_str = "".join(f"{b:08b}" for b in byte_data)
    if padding > 0:
        bit_str = bit_str[:-padding]

    reverse_codes = {v: k for k, v in codes.items()}
    decoded_pixels = []
    temp_code = ""
    for bit in bit_str:
        temp_code += bit
        if temp_code in reverse_codes:
            decoded_pixels.append(reverse_codes[temp_code])
            temp_code = ""

    decoded_img = np.array(decoded_pixels, dtype=np.uint8).reshape(shape)
    cv.imwrite(output_path, decoded_img)
    return decoded_img

if __name__ == '__main__':
    compress_image("web.bmp","web_compressed.bin")
    decompress_image("web_compressed.bin","web_restored.bmp")