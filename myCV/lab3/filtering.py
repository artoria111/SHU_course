import cv2 as cv
import numpy as np
#filtering.py
def mean_filtering(img,kernel_size=3):
    height, width= img.shape
    pad=kernel_size//2
    img_pad=np.pad(img,((pad,pad),(pad,pad)),'reflect')
    out_img=np.zeros((height,width),dtype=np.uint8)
    for i in range(height):
        for j in range(width):
            window=img_pad[i:i+kernel_size,j:j+kernel_size]
            out_img[i,j]=int(np.mean(window))

    return out_img

def median_filtering(img,kernel_size=3):
    height, width= img.shape
    pad=kernel_size//2
    img_pad=np.pad(img,((pad,pad),(pad,pad)),'reflect')
    out_img=np.zeros((height,width),dtype=np.uint8)
    for i in range(height):
        for j in range(width):
            window=img_pad[i:i+kernel_size,j:j+kernel_size]
            out_img[i,j]=int(np.median(window))

    return out_img

if __name__ == '__main__':
    im = cv.imread("img/pcb.png",0)
    if im is None:
        print("Cannot open image")

    im_mean_filter=mean_filtering(im)
    im_median_filter=median_filtering(im)
    im_median_filter=median_filtering(im_median_filter)

    while True:
        cv.imshow('im_origin', im)
        cv.imshow('im_mean_filter', im_mean_filter)
        cv.imshow('im_median_filter', im_median_filter)
        key = cv.waitKey(0)
        if key == 27:
            break
    cv.destroyAllWindows()
