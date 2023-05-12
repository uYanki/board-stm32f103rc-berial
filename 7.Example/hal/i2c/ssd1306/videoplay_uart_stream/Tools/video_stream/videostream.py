import serial
from time import sleep

# 运行前先解压文件!!!


def get_bmp_data(filepath):
    f = open(filepath)
    bmp_data = []
    for line in f:
        val = line.strip().split(',')
        if len(val) == 0:
            continue
        bmp_data.append([int(x) for x in val])
    f.close()
    return bmp_data


def main():
    # 设置端口号、波特率、超时时间
    com = serial.Serial('com30', 500000, timeout=10)
    if not com.isOpen():
        raise "fail to open COM"

    bmp_data = get_bmp_data('video_bad_apple.txt')
    # bmp_data = get_bmp_data('video_digimon_adventure.txt')

    print(len(bmp_data))
    for frame in bmp_data[0:]:
        com.write(bytes(frame))
        sleep(0.02)


main()
