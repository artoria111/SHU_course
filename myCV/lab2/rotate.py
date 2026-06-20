#rotate.py
import cv2 as cv
import numpy as np
import math as m

im=cv.imread("test1_origin.jpg")
if im is None:
    print("Cannot open image")
def my_rotate(im,theta):
    theta=m.radians(theta)
    rows,cols,ch=im.shape
    im_rotated=np.zeros(im.shape,np.uint8)
    center_x=cols//2
    center_y=rows//2
    for u in range(rows):
        for v in range(cols):
            x=v-center_x
            y=u-center_y
            new_x=x*m.cos(theta)-y*m.sin(theta)
            new_y=x*m.sin(theta)+y*m.cos(theta)
            x1 = int(new_x + center_x)
            y1 = int(new_y + center_y)
            x2 = min(x1+1,cols-1)
            y2 = min(y1+1,rows-1)
            if 0<=x1<cols and 0<=y1<rows:
                lam_x= new_x+center_x-x1
                lam_y= new_y+center_y-y1
                r1=(1-lam_x)*im[y1,x1]+lam_x*im[y1,x2]
                r2=(1-lam_x)*im[y2,x1]+lam_x*im[y2,x2]
                p=(1-lam_y)*r1+lam_y*r2
                im_rotated[u][v]=p

    return im_rotated

im_rotated=my_rotate(im,45)
while True:
    cv.imshow(f'',im_rotated)
    key=cv.waitKey(0)
    if key==27:
        break
cv.destroyAllWindows()