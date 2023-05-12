close all
clc
fs=6400;
N=128;
n=0:N-1;
y=1.6+sin(100*pi*n/fs)+0.3*sin(300*pi*n/fs)+0.5*sin(700*pi*n/fs);
y1=data';
y2=abs(fft(y))/64;
y2(1)=y2(1)/2;
y3=y2(1:64);
subplot(311)
plot(n,y),grid,title('原始数据');
subplot(312)
stem((0:63)*50,y1,'fill'),xlabel('频率/Hz'),ylabel('幅值'),grid,title('gd32傅里叶变换结果'),axis([0 600 0 2]);
subplot(313)
stem((0:63)*50,y3,'fill'),xlabel('频率/Hz'),ylabel('幅值'),grid,title('MATLAB傅里叶变换结果'),axis([0 600 0 2]); 