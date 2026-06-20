import cv2
import numpy as np

#threshold.py
def basic_global_threshold(img, delta_t=0.1):
    t = np.mean(img)
    while True:
        g1 = img[img > t]
        g2 = img[img <= t]
        if len(g1) == 0 or len(g2) == 0:
            break
        m1 = np.mean(g1)
        m2 = np.mean(g2)
        t_new = 0.5 * (m1 + m2)
        if abs(t - t_new) < delta_t:
            t = t_new
            break
        t = t_new
    binary_img = (img > t).astype(np.uint8) * 255
    return binary_img, t


def otsu_threshold(img):
    h, w = img.shape
    hist = cv2.calcHist([img], [0], None, [256], [0, 256]).flatten()
    p = hist / (h * w)
    p1 = np.cumsum(p)
    m = np.cumsum(np.arange(256) * p)
    m_g = m[-1]
    denominator = p1 * (1 - p1)
    valid_idx = np.where(denominator > 0)[0]
    sigma_b_squared = np.zeros(256)
    sigma_b_squared[valid_idx] = ((m_g * p1[valid_idx] - m[valid_idx]) ** 2) / denominator[valid_idx]
    max_val = np.max(sigma_b_squared)
    indices = np.where(sigma_b_squared == max_val)[0]
    best_k = np.mean(indices)
    binary_img = (img > best_k).astype(np.uint8) * 255
    return binary_img, best_k

def process_and_compare(file_path):
    img = cv2.imread(file_path, cv2.IMREAD_GRAYSCALE)
    if img is None:
        print(f"无法读取文件: {file_path}")
        return
    res_basic, t_basic = basic_global_threshold(img)
    res_otsu, t_otsu = otsu_threshold(img)
    print(f"\n图像: {file_path}")
    print(f"基本全局阈值: {t_basic:.2f}")
    print(f"Otsu最优阈值: {t_otsu:.2f}")
    top_row = np.hstack((img, res_basic))
    combined = np.hstack((img, res_basic, res_otsu))
    cv2.imshow(f"Original | Basic | Otsu - {file_path}", combined)
    cv2.waitKey(0)


if __name__ == "__main__":
    for name in ['rice.tiff', 'finger.tif', 'poly.tif']:
        process_and_compare(name)
    cv2.destroyAllWindows()