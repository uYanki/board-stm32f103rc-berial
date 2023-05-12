
import cv2
import serial
import time

# file = "./videos/bad_apple.mp4"
file = "./videos/apple.mp4"
# file = "./videos/test.mp4"
port = "COM30"
baudrate = 500000

#################################

uart = serial.Serial(port, baudrate, timeout=10)
cap = cv2.VideoCapture(file)

fps = cap.get(cv2.CAP_PROP_FPS)
print(f"FPS:{fps}")

cur_frame = 0
max_frame = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

data = [0 for i in range(1024)]

while True:

    retval, img = cap.read()

    cur_frame += 1
    if cur_frame == max_frame:
        cur_frame = 0
        cap.set(cv2.CAP_PROP_POS_FRAMES, 0)

    img = cv2.resize(img, (128, 64))
    img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    ret, img = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

    for i in range(0, 64, 8):
        for j in range(128):
            val = 0
            for k in range(8):
                if img[i+k][j]:
                    val |= 1 << k
            data[(i << 4) + j] = val
    uart.flush()
    uart.write(data)

    cv2.imshow(file, img)
    if cv2.waitKey(1) == 27:  # ESC
        cap.release()
        cv2.destroyAllWindows()
        break

    time.sleep(0.01)
