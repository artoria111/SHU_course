import cv2 as cv
import numpy as np
import time
#gamma.py
def gamma_direct(img,gamma):
    img_float=img.astype(np.float64)/255.0
    out_float=np.power(img_float,gamma)
    out_img=(out_float*255.0).astype(np.uint8)
    return out_img

def gamma_lut(img,gamma):
    table=[]
    for i in range(256):
        cur=i/255.0
        val=int(cur**gamma*255.0)
        table.append(val)

    table_array=np.array(table,dtype=np.uint8)
    out_img=table_array[img]

    return out_img

if __name__ == '__main__':
    im = cv.imread("img/dark.jpg")
    if im is None:
        print("Cannot open image")

    gamma=0.5
    start_time = time.time()
    im_gamma_direct=gamma_direct(im,gamma)
    print(f"不使用查找表耗时: {(time.time() - start_time) * 1000:.4f} ms")

    start_time = time.time()
    im_gamma_lut=gamma_lut(im,gamma)
    print(f"使用查找表耗时: {(time.time() - start_time) * 1000:.4f} ms")

    while True:
        cv.imshow('im_gamma_direct', im_gamma_direct)
        cv.imshow('im_gamma_lut', im_gamma_lut)
        cv.imshow('im_origin', im)
        key = cv.waitKey(0)
        if key == 27:
            break
    cv.destroyAllWindows()
