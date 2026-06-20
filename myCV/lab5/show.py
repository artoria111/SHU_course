import cv2 as cv
import numpy as np

#show.py——测试解压后的图片能否在OpenCV中正常显示
im = cv.imread("web_restored.bmp")
while True:
    cv.imshow("image", im)
    key = cv.waitKey(0)
    if key == 27:
        break
cv.destroyAllWindows()