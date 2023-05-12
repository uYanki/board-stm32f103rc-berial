import sys
import cv2

o_fps = 5
vc = cv2.VideoCapture("BadApple.mp4")
if vc.isOpened():
    i_fps = vc.get(cv2.CAP_PROP_FPS)
    num = int(i_fps/o_fps)
    f_i = 0
    f_o = 0
    first_data = True
    print("const uint8_t video_table[] = {")
    while True:
        rval, frame = vc.read()
        if not rval:
            break
        if f_i % num == 0:
            # output size: 24 * 18
            # 0---4 6--10 12-16 18-22
            # |   | |   | |   | |   |
            # 7---+ +---+ +---+ +---+
            #
            # 9---+ +---+ +---+ +---+
            # |   | |   | |   | |   |
            # 16--+ +---+ +---+ +---+
            #
            # Not display in DS1602: x=5,11,17,23 or y=8,17
            # 1 block: 5*8 bits; 1 frame: 5*8*8 bits = 40 bytes
            resizeImage = cv2.resize(frame, (24, 18))
            grayImage = cv2.cvtColor(resizeImage, cv2.COLOR_BGR2GRAY)
            ret, bImage2 = cv2.threshold(grayImage, 127, 255, cv2.THRESH_BINARY)
            # generate picture for animation
            resizeImage2 = cv2.resize(bImage2, (240, 180), interpolation=cv2.INTER_NEAREST)
            cv2.imwrite("pic\\frame%d.bmp" % f_o, resizeImage2)
            # generate image for video data
            ret, bImage = cv2.threshold(grayImage, 127, 1, cv2.THRESH_BINARY)
            for j in range(0, 2):
                sys.stdout.write("  ")
                for i in range(0, 4):
                    data = 0
                    mm = 128
                    for k in range(0, 8):
                        for l in range(0, 5):
                            data = data + bImage[k+9*j,l+6*i]*mm
                            if mm == 1:
                                mm = 128
                                if first_data:
                                    first_data = False
                                else:
                                    sys.stdout.write(",")
                                sys.stdout.write("0x%x" % data)
                                data = 0
                            else:
                                mm = mm/2
                sys.stdout.write("\n")
            f_o = f_o + 1
        f_i = f_i + 1
    print("};")
    print("Total: %d frames" % f_o)
else:
    print("Fail to open video file!")
vc.release()
