
%用于通过设定 period和prescaler求解DAC输出的正弦信号频率

TIM_Period    = 19;                %定时周期数 
TIM_Prescaler = 0;                 %定时时钟分频
point =32;                         %单周期信号点数 
Tsysclk=1/72000000;                 %系统时钟周期
Ttim = Tsysclk * (TIM_Prescaler+1); %定时时钟周期
Tupdate = Ttim * (TIM_Period +1);   %定时器触发周期
Tsin = Tupdate * point;             %一个正弦波周期内的dac点数
fsin = 1/Tsin                       %正弦波频率


