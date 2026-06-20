import cv2 as cv
import numpy as np
#laplace_sharpening
def laplace_sharpening(img):
    kernel=np.array([[1,1,1],
                     [1,-8,1],
                     [1,1,1]])
    height, width= img.shape
    pad=3//2
    img_pad=np.pad(img,((pad,pad),(pad,pad)),'reflect')
    img_pad=img_pad.astype(np.int64)
    out_img=np.zeros((height,width),dtype=np.int64)
    for i in range(height):
        for j in range(width):
            window=img_pad[i:i+3,j:j+3]
            cur_mat=np.multiply(window,kernel)
            out_img[i,j]=img[i][j]-np.sum(cur_mat)

    out_img=np.clip(out_img,0,255)
    return out_img.astype(np.uint8)


if __name__ == '__main__':
    im = cv.imread("img/blurry_moon.tif",0)
    if im is None:
        print("Cannot open image")

    im_laplace_sharpen=laplace_sharpening(im)
    while True:
        cv.imshow('im_origin', im)
        cv.imshow('im_laplace_sharpen', im_laplace_sharpen)
        key = cv.waitKey(0)
        if key == 27:
            break
    cv.destroyAllWindows()
