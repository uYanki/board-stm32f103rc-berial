
ia=clarke_test(:,1);
ib=clarke_test(:,2);
ia1=clarke_test(:,3);
ib1=clarke_test(:,4);

subplot(411)
plot(ia,'red'),grid,title('A相电流值');
subplot(412)
plot(ib,'red'),grid,title('B相电流值');

subplot(413)
plot(ia1,'red'),grid,title('阿法轴电流值');
subplot(414)
plot(ib1,'red'),grid,title('贝塔轴电流值'); 作者：zeshoufx https://www.bilibili.com/read/cv19590539?spm_id_from=333.999.0.0 出处：bilibili