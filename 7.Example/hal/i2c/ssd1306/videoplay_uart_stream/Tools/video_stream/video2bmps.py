import cv2


def bit2num(pixcels):
    output_val = 0
    for i, pix in enumerate(pixcels):
        if pix > 128:  # 白色
            output_val |= 1 << i
    return output_val


def main():
    video_path = r'E:\board-stm32f103rc-berial\7.Example\HAL\08.I2C\11 ssd1306\03 videoplay_uart_stream\Tools\03 mp4 player\videos\test.mp4'  # 视频文件
    fout = open('video_bad_apple2.txt', 'w')  # 解码后保存的文件

    cap = cv2.VideoCapture(video_path)  # 打开视频
    while True:
        ret, frame = cap.read()       # 读取帧
        if not ret:
            break

        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)  # 转换为灰度的画面
        frame = cv2.resize(frame, (128, 64))  # 图像尺寸调整到128*64大小
        convert_val = []
        for row in range(0, 64, 8):   # page0 ~ page7
            for col in range(0, 128):  # seg0 ~ seg127
                cur_data = frame[row: row+8, col]  # 取出对应的8个像素点
                convert_val.append(str(bit2num(cur_data)))  # 转换成8位的数据

        fout.write("%s\n" % (','.join(convert_val)))

        # cv2.imshow("capture", frame)  #显示画面
        # if cv2.waitKey(30) & 0xff == ord('q'): #按q退出
        #    break


main()
