https://oshwhub.com/liuzewen/axp173-ea3036-dian-yuan-guan-li-xin-pian-yan-zheng

http://www.x-powers.com/index.php/Info/product_detail/article_id/27



佳芯微电子是非定制电压版本（上电默认输出 DCDC1、LCD1、LDO2、LDO3 默认输出3.3V，DCDC2、LDO4 默认输出1.2V）



电流走向：VBUS/VBAT/ACIN -> IPSOUT -> LDOn/DCDCn

充电时温度升高（手摸不会感觉发烫）

接VBUS是自动开机，接VBAT是不会自动开机的

长按PWRON关机(4s以上，可配置)，再短按几秒开机(4s以下)

发生短路会关机

关机状态下，LDO1仍然有效



VBUSEN：低电平VBUS参与供电，高电平VBUS不参与供电。（测试方法：接个锂电池，改变VBUSEN的电平，看串口输出的锂电池电流方向。低时VBUS供电，高时锂电池供电。）