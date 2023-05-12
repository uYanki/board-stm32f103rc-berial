# BadApple-LCD1602

## Usage 用法

Use your favorite C compiler to compile `pic-encoder.c`, then run: 

编译 `pic-encoder.c` 文件，然后使用 `ffmepg` 进行视频转换

```
ffmpeg -i "*FileIn*" -vf colorchannelmixer=.299:.587:.114:0:.299:.587:.114:0:.299:.587:.114,scale=23:17:flags=lanczos -pix_fmt gray -f rawvideo - | pic-encoder
```
(replace `*FileIn*` with your video file) to generate binary file (`out.bin`).

输入的视频文件名 `FileIn`，输出的字节文件名 `out.bin` （在 `pic-encoder.c` 中进行更改）

Then, copy the binary file to a SD Card, connect SD card to an Arduino, download the sketch and enjoy!

## Lyrics? 歌词

Lyrics generator is working in progress, please wait a moment...

---

## Example

```
ffmpeg -i "bad_apple.mp4" -r 3 -vf colorchannelmixer=.299:.587:.114:0:.299:.587:.114:0:.299:.587:.114,scale=23:17:flags=lanczos -pix_fmt gray -f rawvideo - | pic-encoder
```

* `pic-encoder`：调用 pic-encoder.exe （没有 `pic-encoder.exe` 就得先编译 `pic-encoder.h` ）
* `-r 3`：每秒抽 3 帧（防止单片机内存被撑爆）
* `-t 10`：截取 10 秒

生成的 `.bin` 文件需使用 `bin2bytes.py` 转为 `.c` 文件。

