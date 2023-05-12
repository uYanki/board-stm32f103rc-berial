/**
  ******************************************************************************
  * @file    bsp_calendar.c
  * @author  移植自linux万年历
  * @version V1.0
  * @date    2013-xx-xx
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

#include "./rtc/bsp_calendar.h"

const uint8_t year_code[597]=
{
	0x04,0xAe,0x53, //1901 0
	0x0A,0x57,0x48, //1902 3
	0x55,0x26,0xBd, //1903 6
	0x0d,0x26,0x50, //1904 9
	0x0d,0x95,0x44, //1905 12
	0x46,0xAA,0xB9, //1906 15
	0x05,0x6A,0x4d, //1907 18
	0x09,0xAd,0x42, //1908 21
	0x24,0xAe,0xB6, //1909
	0x04,0xAe,0x4A, //1910
	0x6A,0x4d,0xBe, //1911
	0x0A,0x4d,0x52, //1912
	0x0d,0x25,0x46, //1913
	0x5d,0x52,0xBA, //1914
	0x0B,0x54,0x4e, //1915
	0x0d,0x6A,0x43, //1916
	0x29,0x6d,0x37, //1917
	0x09,0x5B,0x4B, //1918
	0x74,0x9B,0xC1, //1919
	0x04,0x97,0x54, //1920
	0x0A,0x4B,0x48, //1921
	0x5B,0x25,0xBC, //1922
	0x06,0xA5,0x50, //1923
	0x06,0xd4,0x45, //1924
	0x4A,0xdA,0xB8, //1925
	0x02,0xB6,0x4d, //1926
	0x09,0x57,0x42, //1927
	0x24,0x97,0xB7, //1928
	0x04,0x97,0x4A, //1929
	0x66,0x4B,0x3e, //1930
	0x0d,0x4A,0x51, //1931
	0x0e,0xA5,0x46, //1932
	0x56,0xd4,0xBA, //1933
	0x05,0xAd,0x4e, //1934
	0x02,0xB6,0x44, //1935
	0x39,0x37,0x38, //1936
	0x09,0x2e,0x4B, //1937
	0x7C,0x96,0xBf, //1938
	0x0C,0x95,0x53, //1939
	0x0d,0x4A,0x48, //1940
	0x6d,0xA5,0x3B, //1941
	0x0B,0x55,0x4f, //1942
	0x05,0x6A,0x45, //1943
	0x4A,0xAd,0xB9, //1944
	0x02,0x5d,0x4d, //1945
	0x09,0x2d,0x42, //1946
	0x2C,0x95,0xB6, //1947
	0x0A,0x95,0x4A, //1948
	0x7B,0x4A,0xBd, //1949
	0x06,0xCA,0x51, //1950
	0x0B,0x55,0x46, //1951
	0x55,0x5A,0xBB, //1952
	0x04,0xdA,0x4e, //1953
	0x0A,0x5B,0x43, //1954
	0x35,0x2B,0xB8, //1955
	0x05,0x2B,0x4C, //1956
	0x8A,0x95,0x3f, //1957
	0x0e,0x95,0x52, //1958
	0x06,0xAA,0x48, //1959
	0x7A,0xd5,0x3C, //1960
	0x0A,0xB5,0x4f, //1961
	0x04,0xB6,0x45, //1962
	0x4A,0x57,0x39, //1963
	0x0A,0x57,0x4d, //1964
	0x05,0x26,0x42, //1965
	0x3e,0x93,0x35, //1966
	0x0d,0x95,0x49, //1967
	0x75,0xAA,0xBe, //1968
	0x05,0x6A,0x51, //1969
	0x09,0x6d,0x46, //1970
	0x54,0xAe,0xBB, //1971
	0x04,0xAd,0x4f, //1972
	0x0A,0x4d,0x43, //1973
	0x4d,0x26,0xB7, //1974
	0x0d,0x25,0x4B, //1975
	0x8d,0x52,0xBf, //1976
	0x0B,0x54,0x52, //1977
	0x0B,0x6A,0x47, //1978
	0x69,0x6d,0x3C, //1979
	0x09,0x5B,0x50, //1980
	0x04,0x9B,0x45, //1981
	0x4A,0x4B,0xB9, //1982
	0x0A,0x4B,0x4d, //1983
	0xAB,0x25,0xC2, //1984
	0x06,0xA5,0x54, //1985
	0x06,0xd4,0x49, //1986
	0x6A,0xdA,0x3d, //1987
	0x0A,0xB6,0x51, //1988
	0x09,0x37,0x46, //1989
	0x54,0x97,0xBB, //1990
	0x04,0x97,0x4f, //1991
	0x06,0x4B,0x44, //1992
	0x36,0xA5,0x37, //1993
	0x0e,0xA5,0x4A, //1994
	0x86,0xB2,0xBf, //1995
	0x05,0xAC,0x53, //1996
	0x0A,0xB6,0x47, //1997
	0x59,0x36,0xBC, //1998
	0x09,0x2e,0x50, //1999 294
	0x0C,0x96,0x45, //2000 297
	0x4d,0x4A,0xB8, //2001 300
	0x0d,0x4A,0x4C, //2002
	0x0d,0xA5,0x41, //2003
	0x25,0xAA,0xB6, //2004
	0x05,0x6A,0x49, //2005
	0x7A,0xAd,0xBd, //2006
	0x02,0x5d,0x52, //2007
	0x09,0x2d,0x47, //2008
	0x5C,0x95,0xBA, //2009
	0x0A,0x95,0x4e, //2010
	0x0B,0x4A,0x43, //2011
	0x4B,0x55,0x37, //2012
	0x0A,0xd5,0x4A, //2013
	0x95,0x5A,0xBf, //2014
	0x04,0xBA,0x53, //2015
	0x0A,0x5B,0x48, //2016
	0x65,0x2B,0xBC, //2017
	0x05,0x2B,0x50, //2018
	0x0A,0x93,0x45, //2019
	0x47,0x4A,0xB9, //2020
	0x06,0xAA,0x4C, //2021
	0x0A,0xd5,0x41, //2022
	0x24,0xdA,0xB6, //2023
	0x04,0xB6,0x4A, //2024
	0x69,0x57,0x3d, //2025
	0x0A,0x4e,0x51, //2026
	0x0d,0x26,0x46, //2027
	0x5e,0x93,0x3A, //2028
	0x0d,0x53,0x4d, //2029
	0x05,0xAA,0x43, //2030
	0x36,0xB5,0x37, //2031
	0x09,0x6d,0x4B, //2032
	0xB4,0xAe,0xBf, //2033
	0x04,0xAd,0x53, //2034
	0x0A,0x4d,0x48, //2035
	0x6d,0x25,0xBC, //2036
	0x0d,0x25,0x4f, //2037
	0x0d,0x52,0x44, //2038
	0x5d,0xAA,0x38, //2039
	0x0B,0x5A,0x4C, //2040
	0x05,0x6d,0x41, //2041
	0x24,0xAd,0xB6, //2042
	0x04,0x9B,0x4A, //2043
	0x7A,0x4B,0xBe, //2044
	0x0A,0x4B,0x51, //2045
	0x0A,0xA5,0x46, //2046
	0x5B,0x52,0xBA, //2047
	0x06,0xd2,0x4e, //2048
	0x0A,0xdA,0x42, //2049
	0x35,0x5B,0x37, //2050
	0x09,0x37,0x4B, //2051
	0x84,0x97,0xC1, //2052
	0x04,0x97,0x53, //2053
	0x06,0x4B,0x48, //2054
	0x66,0xA5,0x3C, //2055
	0x0e,0xA5,0x4f, //2056
	0x06,0xB2,0x44, //2057
	0x4A,0xB6,0x38, //2058
	0x0A,0xAe,0x4C, //2059
	0x09,0x2e,0x42, //2060
	0x3C,0x97,0x35, //2061
	0x0C,0x96,0x49, //2062
	0x7d,0x4A,0xBd, //2063
	0x0d,0x4A,0x51, //2064
	0x0d,0xA5,0x45, //2065
	0x55,0xAA,0xBA, //2066
	0x05,0x6A,0x4e, //2067
	0x0A,0x6d,0x43, //2068
	0x45,0x2e,0xB7, //2069
	0x05,0x2d,0x4B, //2070
	0x8A,0x95,0xBf, //2071
	0x0A,0x95,0x53, //2072
	0x0B,0x4A,0x47, //2073
	0x6B,0x55,0x3B, //2074
	0x0A,0xd5,0x4f, //2075
	0x05,0x5A,0x45, //2076
	0x4A,0x5d,0x38, //2077
	0x0A,0x5B,0x4C, //2078
	0x05,0x2B,0x42, //2079
	0x3A,0x93,0xB6, //2080
	0x06,0x93,0x49, //2081
	0x77,0x29,0xBd, //2082
	0x06,0xAA,0x51, //2083
	0x0A,0xd5,0x46, //2084
	0x54,0xdA,0xBA, //2085
	0x04,0xB6,0x4e, //2086
	0x0A,0x57,0x43, //2087
	0x45,0x27,0x38, //2088
	0x0d,0x26,0x4A, //2089
	0x8e,0x93,0x3e, //2090
	0x0d,0x52,0x52, //2091
	0x0d,0xAA,0x47, //2092
	0x66,0xB5,0x3B, //2093
	0x05,0x6d,0x4f, //2094
	0x04,0xAe,0x45, //2095
	0x4A,0x4e,0xB9, //2096
	0x0A,0x4d,0x4C, //2097
	0x0d,0x15,0x41, //2098
	0x2d,0x92,0xB5  //2099
};

///////////////////////////////////////////////////////////////////////////////////////////////////////// 
//         以下为24节气计算相关程序			   
// 
//    每年24节气标志表   
//    有兴趣的朋友可按照上面给的原理添加其它年份的表格
//    不是很清楚的朋友可给我发EMAIL		   
/////////////////////////////////////////////////////////////////////////////////////////////////////////
const uint8_t YearMonthBit[]=
{
	0x4E,0xA6,0x99,		//2000
	0x9C,0xA2,0x98,		//2001
	0x80,0x00,0x18,		//2002
	0x00,0x10,0x24,		//2003
	0x4E,0xA6,0x99,		//2004
	0x9C,0xA2,0x98,		//2005
	0x80,0x82,0x18,		//2006
	0x00,0x10,0x24,		//2007
	0x4E,0xA6,0xD9,		//2008
	0x9E,0xA2,0x98,		//2009

	0x80,0x82,0x18,		//2010
	0x00,0x10,0x04,		//2011
	0x4E,0xE6,0xD9,		//2012
	0x9E,0xA6,0xA8,		//2013
	0x80,0x82,0x18,		//2014
	0x00,0x10,0x00,		//2015
	0x0F,0xE6,0xD9,		//2016
	0xBE,0xA6,0x98,		//2017
	0x88,0x82,0x18,		//2018
	0x80,0x00,0x00,		//2019

	0x0F,0xEF,0xD9,		//2020
	0xBE,0xA6,0x99,		//2021
	0x8C,0x82,0x98,		//2022
	0x80,0x00,0x00,		//2023
	0x0F,0xEF,0xDB,		//2024
	0xBE,0xA6,0x99,		//2025
	0x9C,0xA2,0x98,		//2026
	0x80,0x00,0x18,		//2027
	0x0F,0xEF,0xDB,		//2028
	0xBE,0xA6,0x99,		//2029

	0x9C,0xA2,0x98,		//2030
	0x80,0x00,0x18,		//2031
	0x0F,0xEF,0xDB,		//2032
	0xBE,0xA2,0x99,		//2033
	0x8C,0xA0,0x98,		//2034
	0x80,0x82,0x18,		//2035
	0x0B,0xEF,0xDB,		//2036
	0xBE,0xA6,0x99,		//2037
	0x8C,0xA2,0x98,		//2038
	0x80,0x82,0x18,		//2039

	0x0F,0xEF,0xDB,		//2040
	0xBE,0xE6,0xD9,		//2041 
	0x9E,0xA2,0x98,		//2042
	0x80,0x82,0x18,		//2043
	0x0F,0xEF,0xFB,		//2044
	0xBF,0xE6,0xD9,		//2045
	0x9E,0xA6,0x98,		//2046
	0x80,0x82,0x18,		//2047
	0x0F,0xFF,0xFF,		//2048
	0xFC,0xEF,0xD9,		//2049
	0xBE,0xA6,0x18 		//2050
};

const uint8_t days[24]=
{
	6,20,4,19,6,21,         //一月到三月  的节气基本日期
	5,20,6,21,6,21,         //四月到六月  的节气基本日期
	7,23,8,23,8,23,         //七月到九月  的节气基本日期
	8,24,8,22,7,22,         //十月到十二月的节气基本日期
};

//以公历日期先后排序
const int8_t *JieQiStr[24]=   
{
 // 名称        角度    公历日期     周期 //
	"小寒",     //285     1月 6日
	"大寒",     //300     1月20日    29.5天
	"立春",     //315     2月 4日
	"雨水",     //330     2月19日    29.8天
	"惊蛰",     //345     3月 6日
	"春分",     //  0     3月21日    30.2天
	"清明",     // 15     4月 5日
	"谷雨",     // 30     4月20日    30.7天
	"立夏",     // 45     5月 6日
	"夏满",     // 60     5月21日    31.2天
	"芒种",     // 75     6月 6日
	"夏至",     // 90     6月21日    31.4天
	"小暑",     //105     7月 7日
	"大暑",     //120     7月23日    31.4天
	"立秋",     //135     8月 8日
	"处暑",     //150     8月23日    31.1天
	"白露",     //165     9月 8日
	"秋分",     //180     9月23日    30.7天
	"寒露",     //195    10月 8日
	"霜降",     //210    10月24日    30.1天
	"立冬",     //225    11月 8日
	"小雪",     //240    11月22日    29.7天
	"大雪",     //255    12月 7日
	"冬至"      //270    12月22日    29.5天
};

//下部分数据是农历部分要使用的
//月份数据表
uint8_t  const day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};
unsigned short const day_code2[3]={0x111,0x130,0x14e};
uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表
uint8_t const *sky[10]=  {"甲","乙","丙","丁","戊","己","庚","辛","壬","癸",};//天干
uint8_t const *earth[12]={"子","丑","寅","卯","辰","巳","午","未","申","酉","戌","亥",};//地支
uint8_t const *monthcode[12]={"一","二","三","四","五","六","七","八","九","十","冬","腊",};//农历月份
uint8_t const *nongliday[4]={"初","十","廿","三",};//农历日期  

///////////////////////////////////////////////////////////////////////
//支持从1900年到2099年的农历查询
//支持从2000年到2050年的节气查询
//子函数,用于读取数据表中农历月的大月或小月,如果该月为大返回1,为小返回0
uint8_t GetMoonDay(uint8_t month_p,unsigned short table_addr)
{
	switch (month_p)
	{
		case 1:
			if((year_code[table_addr]&0x08)==0)	return(0);
			else 								return(1); 
		case 2:
			if((year_code[table_addr]&0x04)==0)	return(0);
			else 								return(1);
		case 3:
			if((year_code[table_addr]&0x02)==0)	return(0);
			else 								return(1);
		case 4:
			if((year_code[table_addr]&0x01)==0)	return(0);
			else 								return(1);
		case 5:
			if((year_code[table_addr+1]&0x80)==0)	return(0);
			else 									return(1);
		case 6:
			if((year_code[table_addr+1]&0x40)==0)	return(0);
			else 									return(1);
		case 7:
			if((year_code[table_addr+1]&0x20)==0)	return(0);
			else 									return(1);
		case 8:
			if((year_code[table_addr+1]&0x10)==0)	return(0);
			else 									return(1);
		case 9:
			if((year_code[table_addr+1]&0x08)==0)	return(0);
			else 									return(1);
		case 10:
			if((year_code[table_addr+1]&0x04)==0)	return(0);
			else 									return(1);
		case 11:
			if((year_code[table_addr+1]&0x02)==0)	return(0);
			else 									return(1);
		case 12:
			if((year_code[table_addr+1]&0x01)==0)	return(0);
			else 									return(1);
		case 13:
			if((year_code[table_addr+2]&0x80)==0)	return(0);
			else 									return(1);
	}
	return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// 
// 函数名称:GetChinaCalendar
//功能描述:公农历转换(只允许1901-2099年)
// 输　入:  year        公历年
//          month       公历月
//          day         公历日
//          p           储存农历日期地址
// 输　出:  1           成功
//          0           失败																			 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t GetChinaCalendar(uint16_t  year,uint8_t month,uint8_t day,uint8_t *p)
{ 
	uint8_t temp1,temp2,temp3,month_p,yearH,yearL;	
	uint8_t flag_y;
	unsigned short temp4,table_addr;

	yearH=year/100;	yearL=year%100;//年份的高低两个字节 
	if((yearH!=19)&&(yearH!=20))return(0);//日期不在19xx ~ 20xx 范围内,则退出
	
	// 定位数据表地址  
	if(yearH==20)	table_addr=(yearL+100-1)*3;
	else  			table_addr=(yearL-1)*3;

	// 取当年春节所在的公历月份  
	temp1=year_code[table_addr+2]&0x60;	
	temp1>>=5;

	// 取当年春节所在的公历日  
	temp2=year_code[table_addr+2]&31; 

	// 计算当年春年离当年元旦的天数,春节只会在公历1月或2月  
	if(temp1==1) 	temp3=temp2-1; 
	else 			temp3=temp2+31-1; 

	// 计算公历日离当年元旦的天数  
	if (month<10) 	temp4=day_code1[month-1]+day-1;
	else  			temp4=day_code2[month-10]+day-1;
	// 如果公历月大于2月并且该年的2月为闰月,天数加1  
	if ((month>2)&&(yearL%4==0)) 	temp4++;

	// 判断公历日在春节前还是春节后  
	if (temp4>=temp3)
	{ 						
		temp4-=temp3;
		month=1;
		month_p=1;
							
		flag_y=0;
		if(GetMoonDay(month_p,table_addr)==0)	temp1=29; //小月29天
		else 									temp1=30; //大小30天
		// 从数据表中取该年的闰月月份,如为0则该年无闰月  
		temp2=year_code[table_addr]/16; 	
		while(temp4>=temp1)
		{
			temp4-=temp1;
			month_p++;
			if(month==temp2)
			{
				flag_y=~flag_y;
				if(flag_y==0)month++;
			}
			else month++;
			if(GetMoonDay(month_p,table_addr)==0)	temp1=29;
			else 									temp1=30;
		}
		day=temp4+1;
	}
	// 公历日在春节前使用下面代码进行运算  
	else
	{ 						
		temp3-=temp4;
		if (yearL==0)
		{
			yearL=100-1;
			yearH=19;
		}
		else yearL--;
		table_addr-=3;
		month=12;
		temp2=year_code[table_addr]/16; 	
		if (temp2==0)	month_p=12; 
		else 			month_p=13; 

		flag_y=0;
		if(GetMoonDay(month_p,table_addr)==0)	temp1=29; 
		else 									temp1=30; 
		while(temp3>temp1)
		{
			temp3-=temp1;
			month_p--;
			if(flag_y==0)		month--;
			if(month==temp2)	flag_y=~flag_y;
			if(GetMoonDay(month_p,table_addr)==0)	temp1=29;
			else 									temp1=30;
		}
		day=temp1-temp3+1;
	}

	*p++=yearH;
	*p++=yearL;
	*p++=month;
	*p=day;	
	return(1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称:GetSkyEarth
// 功能描述:输入公历日期得到一个甲子年(只允许1901-2099年)
// 输　入:  year        公历年
//          p           储存星期地址
// 输　出:  无																							   
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetSkyEarth(uint16_t year,uint8_t *p)
{
	uint8_t x;
	
	if(year>=1984)
	{
		year=year-1984;
		x=year%60;				
	}
	else
	{
		year=1984-year;
		x=60-year%60;
	}
	*p=x;
}
//将指定字符source复制no个给target
void StrCopy(uint8_t *target,uint8_t const *source,uint8_t no)
{
	uint16_t i;	 
	for(i=0;i<no;i++)
	{
		*target++=*source++;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称:GetChinaCalendarStr
// 功能描述:输入公历日期得到农历字符串	
//          如:GetChinaCalendarStr(2007,02,06,str) 返回str="丙戌年腊月十九"
// 输　入:  year        公历年
//          month       公历月
//          day         公历日
//          str         储存农历日期字符串地址   15Byte
// 输　出:  无																							  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetChinaCalendarStr(uint16_t year,uint8_t month,uint8_t day,uint8_t *str)
{
	uint8_t NLyear[4];
	uint8_t SEyear;
	
	StrCopy(&str[0],(u8 *)"甲子年正月初一",15);
	if(GetChinaCalendar(year,month,day,(u8 *)NLyear)==0)	return;
	GetSkyEarth(NLyear[0]*100+NLyear[1],&SEyear);
	StrCopy(&str[0],(u8 *)  sky[SEyear%10],2);	//  甲
	StrCopy(&str[2],(u8 *)earth[SEyear%12],2);	//  子	
	
	if(NLyear[2]==1)	StrCopy(&str[6],(u8 *)"正",2);
	else				StrCopy(&str[6],(u8 *)monthcode[NLyear[2]-1],2);		
	
	if(NLyear[3]>10) 	StrCopy(&str[10],(u8 *)nongliday[NLyear[3]/10],2);	
	else				StrCopy(&str[10],(u8 *)"初",2);
	StrCopy(&str[12],(u8 *)monthcode[(NLyear[3]-1)%10],2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称:GetJieQi
// 功能描述:输入公历日期得到本月24节气日期 day<15返回上半月节气,反之返回下半月	
//          如:GetJieQiStr(2007,02,08,str) 返回str[0]=4
// 输　入:  year        公历年
//          month       公历月
//          day         公历日
//          str         储存对应本月节气日期地址   1Byte
// 输　出:  1           成功
//          0           失败																			  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 GetJieQi(u16 year,u8 month,u8 day,u8 *JQdate)
{
	u8 bak1,value,JQ;

	if((year<2000)||(year>2050))     return 0;//节气表的范围限制
	if((month==0) ||(month>12))      return 0;
	JQ = (month-1) *2 ;		                        //获得节气顺序标号(0～23
	if(day >= 15) JQ++; 	                        //判断是否是上半月

	bak1=YearMonthBit[(year-2000)*3+JQ/8];          //获得节气日期相对值所在字节  
	value =((bak1<<(JQ%8))&0x80);                   //获得节气日期相对值状态

	*JQdate=days[JQ];								//得到基本节气日
	if( value != 0 )
	{
		//判断年份,以决定节气相对值1代表1,还是－1。
		if( (JQ== 1||JQ== 11||JQ== 18||JQ== 21)&&year< 2044)  (*JQdate)++;
		else                                                  (*JQdate)--;
	}
	return 1;
}
static u8 const MonthDayMax[]={31,28,31,30,31,30,31,31,30,31,30,31,};
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称:GetJieQiStr
// 功能描述:输入公历日期得到24节气字符串	
//          如:GetJieQiStr(2007,02,08,str) 返回str="离雨水还有11天"
// 输　入:  year        公历年
//          month       公历月
//          day         公历日
//          str         储存24节气字符串地址   15Byte
// 输　出:  1           成功
//          0           失败																			  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 GetJieQiStr(u16 year,u8 month,u8 day,u8 *str)
{
	u8 JQdate,JQ,MaxDay;

	if(GetJieQi(year,month,day,&JQdate)==0)	return 0;

	JQ = (month-1) *2 ;                             //获得节气顺序标号(0～23
	if(day >= 15) JQ++;                             //判断是否是上半月

	if(day==JQdate)                                 //今天正是一个节气日
	{
		StrCopy(str,(u8 *)JieQiStr[JQ],5);
		return 1;
	}
	                                                //今天不是一个节气日
	StrCopy(str,(u8 *)"离立冬还有??天",15);
	if(day<JQdate)                                  //如果今天日期小于本月的节气日期
	{
		StrCopy(&str[2],(u8 *)JieQiStr[JQ],4);
		day=JQdate-day;
	} 
	else                                            //如果今天日期大于本月的节气日期
	{
             if((JQ+1) >23)  return 0;
		StrCopy(&str[2],(u8 *)JieQiStr[JQ+1],4);
		if(day < 15)
		{
			GetJieQi(year,month,15,&JQdate);
			day=JQdate-day;
		}
		else                                        //翻月
		{
			MaxDay=MonthDayMax[month-1];
			if(month==2)                            //润月问题
			{
				if((year%4==0)&&((year%100!=0)||(year%400==0))) MaxDay++;
			}
			if(++month==13)	month=1;
			GetJieQi(year,month,1,&JQdate);
			day=MaxDay-day+JQdate;
		}
	}
	str[10]=day/10+'0';
	str[11]=day%10+'0';
	return 1;
}

