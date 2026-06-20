import cv2 as cv
import numpy as np

#边界框排序函数：输入为一个四元组（x,y,w,h)，先按列排序，再按行排序
def sort_boundaries(boundaries):
    boundaries.sort(key=lambda b: b[1])
    result_boundaries = []
    i=0
    while i < len(boundaries):
        row_y=boundaries[i][1]
        row_h=boundaries[i][3]
        row_boundaries=[]
        while i<len(boundaries) and abs(boundaries[i][1]-row_y) < row_h/2:#y坐标相近的表示在同一行
            row_boundaries.append(boundaries[i])
            i+=1

        row_boundaries.sort(key=lambda b: b[0])
        for b in row_boundaries:
            result_boundaries.append(b)

    return result_boundaries

#载入图像
im=cv.imread('letter.jpg')
if im is None:
    print('no image')

im_gray=cv.cvtColor(im,cv.COLOR_BGR2GRAY)
im_blur=cv.GaussianBlur(im_gray,(5,5),0)#高斯模糊
letter={}#创建字符字典
thresh = cv.adaptiveThreshold(im_blur, 255,
                              cv.ADAPTIVE_THRESH_GAUSSIAN_C,
                              cv.THRESH_BINARY_INV, 11, 2)
contours,hierarchy = cv.findContours(thresh, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)#轮廓识别
#为轮廓绘制矩形并存入数组中
boundaries=[]
for cnt in contours:
    if cv.contourArea(cnt)<100:
        continue
    x,y,w,h = cv.boundingRect(cnt)
    boundaries.append([x,y,w,h])

#排序轮廓和字母顺序对应
boundaries=sort_boundaries(boundaries)
for index,b in enumerate(boundaries):
    if index<=25:
        ch=chr(ord('A')+index)
    else:
        ch=chr(ord('0')+index-26)
    letter[ch]=im[b[1]:b[1]+b[3],b[0]:b[0]+b[2]]

#创建空图
my_im= np.zeros((1146,1588,3), np.uint8)
my_im[:]=[255,255,255]
#目标字符串
my_id="23122798"
my_name="LIZEXUN"
#字符间隔
interval=200
for index,ch in enumerate(my_id):
    rows,cols,channels = letter[ch].shape
    my_im[0:rows, index*interval:index*interval+cols] = letter[ch]

for index,ch in enumerate(my_name):
    rows,cols,channels = letter[ch].shape
    my_im[300:rows+300, index*interval:index*interval+cols] = letter[ch]

#窗口关闭逻辑，esc退出
while True:
    cv.imshow(f'',my_im)
    key=cv.waitKey(0)
    if key==27:
        break
cv.destroyAllWindows()