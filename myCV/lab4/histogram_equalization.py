import cv2 as cv
import numpy as np
#histogram_equalization.py

def rgb_equalization(img):
    blue,green,red=cv.split(img)
    equalized_red = cv.equalizeHist(red)
    equalized_blue = cv.equalizeHist(blue)
    equalized_green = cv.equalizeHist(green)
    equalized_img=cv.merge([equalized_red,equalized_blue,equalized_green])
    return equalized_img
def v_equalization(img):
    hsv_img = cv.cvtColor(img,cv.COLOR_BGR2HSV)
    h,s,v=cv.split(hsv_img)
    equalized_v=cv.equalizeHist(v)
    equalized_img=cv.merge([h,s,equalized_v])
    result_equalized_img=cv.cvtColor(equalized_img,cv.COLOR_HSV2BGR)
    return result_equalized_img


if __name__ == '__main__':
    im = cv.imread("sky.bmp")
    if im is None:
        print("Cannot open image")

    rgb_equalized_im = rgb_equalization(im)
    v_equalized_im = v_equalization(im)

    while True:
        cv.imshow('im_origin', im)
        cv.imshow('rgb_equalized_im', rgb_equalized_im)
        cv.imshow('v_equalized_im', v_equalized_im)
        key = cv.waitKey(0)
        if key == 27:
            break
    cv.destroyAllWindows()
