import cv2

MAX_VALUE = 4000
MIN_VALUE = 0

IS_VIDEO = False


def img2xy(img):
    # image size
    width = img.shape[1]
    heigth = img.shape[0]
    cv2.imshow('vid', img)
    if IS_VIDEO == False:
        print('size=%dX%d' % (width, heigth))

    cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img = cv2.Canny(img, 100, 200)

    if IS_VIDEO == False:
        cv2.imshow('img', img)
        cv2.waitKey(0)

    # 二进制点阵
    points = [[0 for x in range(width)] for y in range(heigth)]
    for y in range(heigth):
        for x in range(width):
            points[y][x] = 1 if img[y][x] == 255 else 0

    # 值映射
    output_list = []
    for y in range(heigth):
        for x in range(width):
            if points[y][x] == 1:
                output_list.append([int(x*MAX_VALUE/width), 4000-int(y*MAX_VALUE/heigth)])

    i = 0
    for data in output_list:
        i += 2
        print(data[0], end=', ')  # x
        print(data[1], end=', ')  # y
        if i % 10 == False:
            print()  # newline

    if IS_VIDEO == False:
        print()
        print('length = %d' % i)
        cv2.waitKey(0)


if not IS_VIDEO:
    # image
    img = cv2.imread('img.png')
    if img is None:
        print('fail to open image')
        exit()
    img2xy(img)
else:
    # video
    video = cv2.VideoCapture('bad apple.mp4')
    if not video.isOpened():
        print('fail to open video')
    else:
        while True:
            ret, frame = video.read()
            if frame is not None:
                img2xy(frame)
            else:
                print('[end]')
                break
