#define HX710_SCK 2
#define HX710_DT 3
long HX710_Buffer = 0;
long Weight_Maopi = 0,Weight_Shiwu = 0;
//****************************************************
//初始化HX711
//****************************************************
void Init_Hx710(){
  pinMode(HX710_SCK, OUTPUT); 
  pinMode(HX710_DT, INPUT);
}
//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi()
{
  HX710_Buffer = HX710_Read();
  Weight_Maopi = HX710_Buffer/100;    
} 

//****************************************************
//称重
//****************************************************
unsigned int Get_Weight()
{
  HX710_Buffer = HX710_Read();
  HX710_Buffer = HX710_Buffer/100;

  Weight_Shiwu = HX710_Buffer;
  Weight_Shiwu = Weight_Shiwu - Weight_Maopi;       //获取实物的AD采样数值。
  
  Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/7.35);   
    //计算实物的实际重量
    //因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的4.30这个除数。
    //当发现测试出来的重量偏大时，增加该数值。
    //如果测试出来的重量偏小时，减小改数值。
    //该数值一般在7.16左右。因传感器不同而定。
    //+0.05是为了四舍五入百分位

  return Weight_Shiwu;
}

//****************************************************
//读取HX711
//****************************************************
unsigned long HX710_Read(void)  //增益128
{
  unsigned long count; 
  unsigned char i;
  bool Flag = 0;
  digitalWrite(HX710_DT, HIGH);
  delayMicroseconds(1);
  digitalWrite(HX710_SCK, LOW);
  delayMicroseconds(1);
    count=0; 
    while(digitalRead(HX710_DT)); 
    for(i=0;i<24;i++)
  { 
      digitalWrite(HX710_SCK, HIGH); 
    delayMicroseconds(1);
      count=count<<1; 
    digitalWrite(HX710_SCK, LOW); 
    delayMicroseconds(1);
      if(digitalRead(HX710_DT))
      count++; 
  } 
  digitalWrite(HX710_SCK, HIGH); 
  count ^= 0x800000;
  delayMicroseconds(1);
  digitalWrite(HX710_SCK, LOW); 
  delayMicroseconds(1);
  
  return(count);
}

