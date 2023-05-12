import serial
from matplotlib import pyplot as plt
import numpy as np


def main():
    com = serial.Serial('com30', 115200, timeout=10)
    print(com)

    retry = 0
    if com.isOpen():
        plt.ion()  # plt.ioff()
        data = np.zeros((1, 1, 3), dtype=np.uint8)
        while True:
            # 读取数据
            line = com.readline()
            line = line.decode('utf8').strip()
            r, g, b = line.split('\t')
            data[:] = (int(r), int(g), int(b))
            print(r, g, b)
            # 显示图像
            plt.clf()
            plt.imshow(data)
            plt.pause(0.01)
            # 清空缓冲区（必须清除！！，否则会因plt库的延时导致颜色显示不灵敏）
            com.flushInput()


main()
