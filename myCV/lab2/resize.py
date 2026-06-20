#resize.py
import cv2 as cv
import numpy as np

def resize_of_bilinter(im,scale):
    rows, cols, ch = im.shape
    new_rows = rows * scale
    new_cols = cols * scale
    im_resized = np.zeros((new_rows, new_cols, ch), np.uint8)
    for u in range(new_rows):
        for v in range(new_cols):
            new_x = v / scale
            new_y = u / scale
            x1 = int(new_x)
            y1 = int(new_y)
            x2 = min(x1 + 1, cols - 1)
            y2 = min(y1 + 1, rows - 1)
            if 0 <= x1 < cols and 0 <= y1 < rows:
                lam_x = new_x - x1
                lam_y = new_y - y1
                r1 = (1 - lam_x) * im[y1, x1] + lam_x * im[y1, x2]
                r2 = (1 - lam_x) * im[y2, x1] + lam_x * im[y2, x2]
                p = (1 - lam_y) * r1 + lam_y * r2
                im_resized[u][v] = p

    return im_resized

def resize_of_nearest(im,scale):
    rows, cols, ch = im.shape
    new_rows = rows * scale
    new_cols = cols * scale
    im_resized = np.zeros((new_rows, new_cols, ch), np.uint8)
    for u in range(new_rows):
        for v in range(new_cols):
            new_x = v / scale
            new_y = u / scale
            x = int(new_x)
            y = int(new_y)
            if 0 <= x < cols and 0 <= y < rows:
                im_resized[u][v] = im[y][x]

    return im_resized

if __name__ == '__main__':
    im=cv.imread("test2.jpg")
    im_o=cv.imread("test2_origin.jpg")
    if im is None or im_o is None:
        print("Cannot open image")

    im_resized_of_bilinear = resize_of_bilinter(im,2)
    im_resized_of_nearest = resize_of_nearest(im,2)
    im_opencv_resized=cv.resize(im,None,fx=2, fy=2, interpolation = cv.INTER_CUBIC)
    while True:
        cv.imshow('my_resize_of_bilinear',im_resized_of_bilinear)
        cv.imshow('my_resize_of_nearest',im_resized_of_nearest)
        cv.imshow('opencv_resized',im_opencv_resized)
        cv.imshow('origin',im_o)
        key=cv.waitKey(0)
        if key==27:
            break
    cv.destroyAllWindows()