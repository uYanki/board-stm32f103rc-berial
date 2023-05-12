close all
y1=filter_test(:,1);
y2=filter_test(:,2);

subplot(211)
plot(y1,'r'),grid,title('原始数据');
subplot(212)
plot(y2,'r'),grid,title('滤波后数据'); 作者：zeshoufx https://www.bilibili.com/read/cv19597688?spm_id_from=333.999.0.0 出处：bilibili