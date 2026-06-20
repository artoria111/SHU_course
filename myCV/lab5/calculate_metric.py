import numpy as np
import os
import cv2 as cv

#calculate_metric
def calculate_metrics(original_img, decompressed_img, original_path, compressed_path):
    orig = original_img.astype(np.float64)
    decomp = decompressed_img.astype(np.float64)
    mse = np.mean((orig - decomp) ** 2)
    rmse = np.sqrt(mse)
    size_orig = os.path.getsize(original_path)
    size_comp = os.path.getsize(compressed_path)
    cr = size_orig / size_comp
    print("-" * 30)
    print(f"评价结果 [{os.path.basename(original_path)}]:")
    print(f"  - 均方根误差 (RMSE): {rmse:.6f}")
    print(f"  - 原始大小: {size_orig} Bytes")
    print(f"  - 压缩大小: {size_comp} Bytes")
    print(f"  - 压缩比 (CR): {cr:.4f}:1")
    print("-" * 30)

    return rmse, cr

if __name__ == '__main__':
    orig_img=cv.imread('bridge.bmp')
    recon_img=cv.imread('bridge_restored.bmp')
    rmse, cr = calculate_metrics(orig_img, recon_img, 'bridge.bmp', 'bridge.bmp.pred.bin')