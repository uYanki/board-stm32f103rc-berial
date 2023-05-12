# 取色阶

import cv2
import numpy as np

kernel = np.ones((5, 5), np.uint8)

# 3×3的10灰度级矩阵，0为黑色，1为白色
m1 = 255 * np.array(((0, 0, 0), (0, 0, 0), (0, 0, 0)), np.uint8)
m2 = 255 * np.array(((0, 1, 0), (0, 0, 0), (0, 0, 0)), np.uint8)
m3 = 255 * np.array(((0, 1, 0), (0, 0, 0), (0, 0, 1)), np.uint8)
m4 = 255 * np.array(((1, 1, 0), (0, 0, 0), (0, 0, 1)), np.uint8)
m5 = 255 * np.array(((1, 1, 0), (0, 0, 0), (1, 0, 1)), np.uint8)
m6 = 255 * np.array(((1, 1, 1), (0, 0, 0), (1, 0, 1)), np.uint8)
m7 = 255 * np.array(((1, 1, 1), (0, 0, 1), (1, 0, 1)), np.uint8)
m8 = 255 * np.array(((1, 1, 1), (0, 0, 1), (1, 1, 1)), np.uint8)
m9 = 255 * np.array(((1, 1, 1), (1, 0, 1), (1, 1, 1)), np.uint8)
m10 = 255 * np.array(((1, 1, 1), (1, 1, 1), (1, 1, 1)), np.uint8)
# 10阶灰度序列
grayScale = [m1, m2, m3, m4, m5, m6, m7, m8, m9, m10]

m1x2 = 255 * np.array(((0, 0), (0, 0)), np.uint8)
m2x2 = 255 * np.array(((0, 1), (0, 0)), np.uint8)
m3x2 = 255 * np.array(((0, 1), (1, 0)), np.uint8)
m4x2 = 255 * np.array(((0, 1), (1, 1)), np.uint8)
m5x2 = 255 * np.array(((1, 1), (1, 1)), np.uint8)

grayScalex2 = [m1x2, m2x2, m3x2, m4x2, m5x2]


# 图像灰度抖动
def Halftone(img_Gray):
    img_Halftoning = np.zeros([32 * 2, 64 * 2], np.uint8)  # 初始化输出图像
    img_ten_m = img_Gray / 51  # 从0~255映射到0~10
    img_ten = np.rint(img_ten_m)
    img_ten_int = img_ten.astype(np.uint8)
    # print(img_ten_int)

    for y in range(32):  # 逐行扫描
        for x in range(64):
            graylevel = img_ten_int[y][x] - 1  # 获取对应灰阶
            y_half = y * 2  # 映射抖动目标坐标
            x_half = x * 2
            for y_h in range(2):
                for x_h in range(2):
                    # img_Halftoning[y_half + y_h][x_half + x_h] = m10[y_h][x_h]
                    gray = grayScale[graylevel]
                    img_Halftoning[y_half + y_h][x_half + x_h] = gray[y_h][x_h]
    # img_Halftoning = cv2.threshold(img_Halftoning, 127, 255, cv2.THRESH_BINARY)  # 二值化单色图
    return img_Halftoning


# 将多张图片聚合到一个窗口
def stackImages(scale, imgArray):
    rows = len(imgArray)
    cols = len(imgArray[0])
    rowsAvailable = isinstance(imgArray[0], list)
    width = imgArray[0][0].shape[1]
    height = imgArray[0][0].shape[0]
    if rowsAvailable:
        for x in range(0, rows):
            for y in range(0, cols):
                if imgArray[x][y].shape[:2] == imgArray[0][0].shape[:2]:
                    imgArray[x][y] = cv2.resize(imgArray[x][y], (0, 0), None, scale, scale)
                else:
                    imgArray[x][y] = cv2.resize(imgArray[x][y], (imgArray[0][0].shape[1], imgArray[0][0].shape[0]),
                                                None, scale, scale)
                if len(imgArray[x][y].shape) == 2:
                    imgArray[x][y] = cv2.cvtColor(imgArray[x][y], cv2.COLOR_GRAY2BGR)
        imageBlank = np.zeros((height, width, 3), np.uint8)
        hor = [imageBlank] * rows
        hor_con = [imageBlank] * rows
        for x in range(0, rows):
            hor[x] = np.hstack(imgArray[x])
        ver = np.vstack(hor)
    else:
        for x in range(0, rows):
            if imgArray[x].shape[:2] == imgArray[0].shape[:2]:
                imgArray[x] = cv2.resize(imgArray[x], (0, 0), None, scale, scale)
            else:
                imgArray[x] = cv2.resize(imgArray[x], (imgArray[0].shape[1], imgArray[0].shape[0]), None, scale, scale)
            if len(imgArray[x].shape) == 2:
                imgArray[x] = cv2.cvtColor(imgArray[x], cv2.COLOR_GRAY2BGR)
        hor = np.hstack(imgArray)
        ver = hor
    return ver


# 边沿提取
def Img2Stack(img):  # 输入图片为原图
    imgGray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # 灰度图 边缘处理对黑白效果好
    imgBlur = cv2.GaussianBlur(imgGray, (7, 7), 1.2)  # 模糊 去除噪声
    imgCanny = cv2.Canny(imgGray, 50, 50)  # 边沿
    imgDialation = cv2.dilate(imgCanny, kernel, iterations=1)  # 迭代次数 加粗线条
    imgMirro = cv2.bitwise_not(imgGray)
    ret, thresh = cv2.threshold(imgMirro, 127, 255, cv2.THRESH_BINARY)  # 二值化单色图

    imgStack = stackImages(3, ([img, imgGray],  # 打开一个窗口一次性显示6张图片 方便对比与预览 第一个数字为单张图片缩放倍数
                               [imgBlur, imgCanny],
                               [imgDialation, thresh]))
    cv2.imshow("ImageStack", imgStack)
    return thresh  # 按照自己的需求返回对应的图片 不需要的图片可以注释掉以增加帧率


# 转化为Bitmap格式
def toBitmap(img):
    screen = np.zeros((8, 128), dtype=np.uint8)  # 存储要发送出串口的数据
    for y in range(8):
        for x in range(128):
            temp = 0
            for bit in range(8):
                if img[y * 8 + bit][x] == 255:
                    temp |= 0x01 << bit
            screen[y][x] = temp

    bitmapOut = []  # 转换为十六进制数组
    i = 0
    for y in range(8):
        for x in range(128):
            bitmapOut.append(hex(screen[y][x]))
            i = i + 1
    return bitmapOut


# 位图数组格式化输出
def oledFormat_Anim(bitmapOut, frame):
    print("// ", frame)
    print("{", end='')
    for i in range(1024):
        print(bitmapOut[i], end='')
        if i != 1023:
            print(",", end='')
    print("},")


cap = cv2.VideoCapture("cxk2.mp4")  # 打开一个视频

count = 0
frame = 1

if __name__ == '__main__':
    while True:
        if count % 2 == 0:  # 抽帧（一半）

            # ------ 灰度抖动 ------ #
            ret, img = cap.read()  # 读取1帧
            img = np.array(img)  # 转化为np数组
            img = cv2.resize(img, (64, 32))  # 缩小尺寸
            # img = Img2Stack(img)  # 边沿提取及显示
            imgGray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # 转换灰度图
            imgHalftone = Halftone(imgGray)  # 灰度抖动算法
            # img = cv2.resize(img, (128, 64))
            # imgGray = cv2.resize(imgGray, (128, 64))
            # imgHalftone_mini = cv2.resize(imgHalftone, (128, 64))

            bitmapOut = toBitmap(imgHalftone)  # 转换位图格式
            oledFormat_Anim(bitmapOut, frame)  # 格式化输出位图数组
            #
            # imgStack = stackImages(2, ([img, img],  # 打开一个窗口一次性显示6张图片 方便对比与预览 第一个数字为单张图片缩放倍数
            #                            [imgGray, imgGray],
            #                            [imgHalftone_mini, imgHalftone_mini]))
            cv2.imshow('', imgHalftone)

            # ------ 边沿提取 ------ #
            # ret, img = cap.read()  # 读取1帧
            # img = np.array(img)  # 转化为np数组
            # img = cv2.resize(img, (128, 64))  # 缩小尺寸
            # img = Img2Stack(img)  # 边沿提取及显示
            # bitmapOut = toBitmap(img)  # 转换位图格式
            # oledFormat_Anim(bitmapOut, frame)  # 格式化输出位图数组

            frame = frame + 1
        count = count + 1
        keyValue = cv2.waitKey(1)
        if keyValue & 0xFF == ord('q'):  # q键退出
            break

cv2.destroyAllWindows()
