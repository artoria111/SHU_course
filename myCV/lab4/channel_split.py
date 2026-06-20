import cv2 as cv
import numpy as np
#channel_split.py

def channel_split(img):
    red=img[:,:,0]
    green=img[:,:,1]
    blue=img[:,:,2]
    return red,green,blue


if __name__ == '__main__':
    im = cv.imread("araras.jpg")
    if im is None:
        print("Cannot open image")

    red_channel,green_channel,blue_channel = channel_split(im)

    while True:
        cv.imshow('im_origin', im)
        cv.imshow('red_channel', red_channel)
        cv.imshow('green_channel', green_channel)
        cv.imshow('blue_channel', blue_channel)
        key = cv.waitKey(0)
        if key == 27:
            break
    cv.destroyAllWindows()
