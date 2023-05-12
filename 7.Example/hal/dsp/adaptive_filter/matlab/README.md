[https://www.bilibili.com/read/cv20101814]

阶数

![img](README.assets/64901f99561150e4edbdad0109b9a58c0fd48781.png@831w_624h_progressive.webp)

20阶，迭代次数22次



![img](README.assets/6fe94f0f4b5784926f94dffe39f3a62bd1fff8bf.png@831w_624h_progressive.webp)

20阶，迭代次数100次





![img](README.assets/8c3dfdb4331c8e39b8631052c00c85b0697842ba.png@831w_624h_progressive.webp)

20阶，迭代次数300次



![img](README.assets/70d987adc730307105567a757d3640ad9f91d452.png@831w_624h_progressive.webp)

10阶，迭代次数100次



![img](README.assets/72f28fb948f51459664714890955a19e19027361.png@831w_624h_progressive.webp)

32阶，迭代次数100次



![img](README.assets/2acd4c9258cecf6f79a9585bfa818d0b7ff9c368.png@831w_624h_progressive.webp)

16阶，迭代次数128次



IIR滤波测试

IIR滤波结果



![img](README.assets/f5bc6c7bd0231b2ee37ea2e29caee70f39e2e16a.jpg@842w_630h_progressive.webp)

IIR滤波[https://www.bilibili.com/read/cv19597688]



![img](README.assets/add6a98fb81b82065fd96fe956d5f86e49882a02.jpg@842w_630h_progressive.webp)

自适应滤波（16阶，128次迭代）

结论：

可以看到IIR滤波后比较平滑，但IIR有明显的相位延迟，另外自适应滤波阶数前序列为0，且运行时间较长；自适应滤波随着迭代次数增加效果变好，随着阶数增加效果变好，运行时间也变长。
