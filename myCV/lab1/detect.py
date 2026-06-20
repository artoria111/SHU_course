import cv2 as cv
import numpy as np

# 1. 读取图片
im = cv.imread('letter.jpg')

# 2. [修正] 先检查图片是否存在，防止 crash
if im is None:
    print('错误：找不到图片')
    exit()

im_gray=cv.cvtColor(im,cv.COLOR_BGR2GRAY)
im_blur=cv.GaussianBlur(im_gray,(5,5),0)#高斯模糊
letter={}#创建字符字典
thresh = cv.adaptiveThreshold(im_blur, 255,
                              cv.ADAPTIVE_THRESH_GAUSSIAN_C,
                              cv.THRESH_BINARY_INV, 11, 2)
contours,hierarchy = cv.findContours(thresh, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)#轮廓识别

for cnt in contours:
    x, y, w, h = cv.boundingRect(cnt)
    cv.rectangle(im, (x, y), (x + w, y + h), (0, 255, 0), 2)

while True:
    cv.imshow(f'',im)
    key=cv.waitKey(0)
    if key==27:
        break
cv.destroyAllWindows()