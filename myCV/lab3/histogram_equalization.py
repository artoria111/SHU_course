import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
#histogram_equalization.py
def histogram_equalization(img):
    f=np.zeros(256)
    rows,cols=img.shape
    for i in range(rows):
        for j in range(cols):
            f[img[i,j]]=f[img[i,j]]+1

    p=f/(rows*cols)
    cdf=np.cumsum(p)
    cdf=cdf*255.0+0.5
    lut=np.array(cdf,dtype=np.uint8)
    out_img=lut[img]

    return out_img



if __name__ == '__main__':
    im = cv.imread("img/hill.jpg",0)
    if im is None:
        print("Cannot open image")

    im_hist_equal=histogram_equalization(im)
    im_hist_equal_cv=cv.equalizeHist(im_hist_equal)
    plt.figure(figsize=(12, 10))
    plt.subplot(2, 2, 1)
    plt.hist(im.flatten(), 256, range=(0, 256), color='r')
    plt.title("Origin hist",fontsize=20)
    plt.xlim([0, 256])

    plt.subplot(2, 2, 2)
    plt.hist(im_hist_equal.flatten(), 256, range=(0, 256), color='g')
    plt.title("My hist",fontsize=20)
    plt.xlim([0, 256])

    plt.subplot(2, 2, 3)
    plt.hist(im_hist_equal_cv.flatten(), 256, range=(0, 256), color='b')
    plt.title("CV hist",fontsize=20)
    plt.xlim([0, 256])

    plt.show()

    while True:
        cv.imshow('im_hist_equal', im_hist_equal)
        cv.imshow('im_hist_equal_cv', im_hist_equal_cv)
        cv.imshow('im_origin', im)
        key = cv.waitKey(0)
        if key == 27:
            break
    cv.destroyAllWindows()
