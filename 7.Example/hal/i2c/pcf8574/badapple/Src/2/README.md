# bad_apple
This project was used to play BadApple video in LCD1602 screen in stm32f103c8t6 mini board.
It includes the following two folders:

1) python
process_video.py was used to generate video_table data from BadApple.mp4.
By using opencv functions, this python program captured 5 images per seconds,
and converted these images to 24*18 binary gray image.
1 pixel just needs 1 bit data, and some pixels can't be displayed like follows.
So we only needs 40 bytes for 1 frame. 
The video's length is 03:39, total frames are 1095(5fps). The size of total video is 43800.
stm32f103c8t6 has 64K Flash, so we can put the whole video data into the memory.

            # size: 24 * 18
            # 0---4 6--10 12-16 18-22
            # |   | |   | |   | |   |
            # 7---+ +---+ +---+ +---+
            #
            # 9---+ +---+ +---+ +---+
            # |   | |   | |   | |   |
            # 16--+ +---+ +---+ +---+
process_video.py was also used to generate a series of bmp files which can be used to genrate
an animation video, so that we can check the result.

We need copy the source video BadApple.mp4 to the same folder, and create a pic sub folder,
then run this script, we can get the video_table data from stdout, 
and get a series of bmp files in pic folder. We can run the following command to generate a demo video.

> ffmpeg -r 5 -i pic/frame%d.bmp -vcodec mpeg4 -r 5 BadApple_demo.mp4

convert_subtitles.py was used to genertae subtitles_table data from katakana subtitles.

2) stm32f103
This folder is the stm32f103 program, vscode + arm gcc tool chain was used to build this program.
There are 2 play modes for this program, one play the animation with time,
the other one play the animation with katakana subtitles.
PC13 GPIO was used to switch these 2 modes.

Video:
Animation with time:
https://www.bilibili.com/video/av22051504

Animation with katakana subtitles:
https://www.bilibili.com/video/av24336066/
