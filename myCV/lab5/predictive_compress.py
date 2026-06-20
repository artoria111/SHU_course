import numpy as np
import cv2
import heapq
import pickle
import os
#predictive_compress
class HuffmanNode:
    def __init__(self, char, freq):
        self.char, self.freq = char, freq
        self.left = self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

def get_huffman_codes(data):
    freq = {}
    for x in data: freq[x] = freq.get(x, 0) + 1
    heap = [HuffmanNode(c, f) for c, f in freq.items()]
    heapq.heapify(heap)
    while len(heap) > 1:
        n1, n2 = heapq.heappop(heap), heapq.heappop(heap)
        merged = HuffmanNode(None, n1.freq + n2.freq)
        merged.left, merged.right = n1, n2
        heapq.heappush(heap, merged)
    codes = {}

    def _gen(node, s):
        if node.char is not None: codes[node.char] = s; return
        _gen(node.left, s + "0");
        _gen(node.right, s + "1")

    if heap: _gen(heap[0], "")
    return codes

def predictive_compress(img_path, out_path):
    img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE).astype(np.int16)
    h, w = img.shape
    errors = np.zeros((h, w), dtype=np.int16)
    errors[:, 0] = img[:, 0]
    errors[:, 1:] = img[:, 1:] - img[:, :-1]
    flat_errors = errors.flatten() + 255
    codes = get_huffman_codes(flat_errors)
    bit_str = "".join([codes[x] for x in flat_errors])
    padding = (8 - len(bit_str) % 8) % 8
    bit_str += "0" * padding
    byte_arr = bytearray(int(bit_str[i:i + 8], 2) for i in range(0, len(bit_str), 8))
    with open(out_path, 'wb') as f:
        pickle.dump({'shape': (h, w), 'codes': codes, 'padding': padding, 'data': byte_arr}, f)

def predictive_decompress(in_path):
    with open(in_path, 'rb') as f:
        payload = pickle.load(f)

    h, w = payload['shape']
    rev_codes = {v: k for k, v in payload['codes'].items()}
    bit_str = "".join(f"{b:08b}" for b in payload['data'])
    if payload['padding'] > 0: bit_str = bit_str[:-payload['padding']]

    decoded_errors = []
    tmp = ""
    for bit in bit_str:
        tmp += bit
        if tmp in rev_codes:
            decoded_errors.append(rev_codes[tmp] - 255)
            tmp = ""

    errors = np.array(decoded_errors).reshape(h, w)
    recon = np.zeros((h, w), dtype=np.uint8)


    for j in range(1, w):
        recon[:, j] = (errors[:, j] + recon[:, j - 1]).astype(np.uint8)
    return recon

if __name__ == '__main__':
    for img_name in ['bridge.bmp', 'web.bmp']:
        orig = cv2.imread(img_name, 0)
        bin_file = img_name + ".pred.bin"

        predictive_compress(img_name, bin_file)
        recon = predictive_decompress(bin_file)




