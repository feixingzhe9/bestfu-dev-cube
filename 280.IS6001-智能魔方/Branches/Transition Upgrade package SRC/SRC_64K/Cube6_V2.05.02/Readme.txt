﻿********************************************************************************
							六面体版本说明
********************************************************************************
================================================================================
V2.05.02	时间：2016.03.31	硬件：IS6001 V4.03		修改：周雁
烧录文件：IS6001_V2.05.02_burn.hex
升级文件：IS6001_V2.05.02_update.dat
说    明：
        1.库升级至V10128版本；
	2、解决flash加密异常问题;
================================================================================

================================================================================
V2.05.01	时间：2016.03.23	硬件：IS6001 V4.03		修改：周雁
烧录文件：IS6001_V2.05.01_burn.hex
升级文件：IS6001_V2.05.01_update.dat
说    明：
        1.库升级至V10126版本；
================================================================================

================================================================================
V2.05.00	时间：2015.12.08	硬件：IS6001 V4.03		修改：周雁
烧录文件：IS6001_V2.05.00_burn.hex
升级文件：IS6001_V2.05.00_update.dat
说    明：
        1.库升级至V10127版本；
		2.充电时魔方不可用；
		3.增加在唤醒时，若当前面和上次的面一致时，有震动提示，但不发联动信息；
================================================================================

================================================================================
V2.01.07	时间：2015.11.06	硬件：IS6001 V4.02		修改：周雁
烧录文件：IS6001_V2.01.07_burn.hex
升级文件：IS6001_V2.01.07_update.dat
说    明：
        1.增加电量小于30%时，震动两次，提示用户充电；电量低于10%时，直接休眠；
		2.删掉充电完成震动提示;
		3.修改休眠时间至30s;
================================================================================

================================================================================
V2.01.06	时间：2015.10.30	硬件：IS6001 V4.02		修改：周雁
烧录文件：IS6001_V2.01.06_burn.hex
升级文件：IS6001_V2.01.06_update.dat
说    明：
		1.升级库文件 V1.01.23;
        2.增加首次充电震动提示;
		3.增加充电完成震动提示;
================================================================================

================================================================================
V2.01.05	时间：2014.12.02	硬件：Six_Control V1.3		修改：李偌淮
烧录文件：IS6001_V2.01.05_burn.hex
升级文件：IS6001_V2.01.05_update.dat
说    明：
	1.升级库文件 V1.00.29
       进一步防止 EEPROM 操作异常
================================================================================
================================================================================
V2.01.04	时间：2014.10.08	硬件：Six_Control V1.3		修改：李偌淮
烧录文件：IS6001_V2.01.04_burn.hex
升级文件：IS6001_V2.01.04_update.dat
说    明：
	1.升级库文件 V1.00.25 +
       支持RF433信号测试
       支持eeprom异常检测
================================================================================
================================================================================
V2.01.03
说    明：
	1.交换面值  2--5   3--4
================================================================================
================================================================================
V2.01.02
说    明：
	1.修改初始面重复触发BUG
================================================================================
================================================================================
V2.01.01
	库版本：V1.00.18
================================================================================
================================================================================
V2.01.00
	第一发布版（无修改）
================================================================================	
================================================================================
V2.00.03
	库版本更新：V1.00.14
	取消读取电量值时每次测量  改为上电首次测量的电量值
================================================================================

================================================================================
V2.00.02
	根据设计部要求调整各面值顺序
	马达震动时间由50ms增大到80ms
	
================================================================================
V2.00.01
	库版本更新：V1.00.11

================================================================================
V2.00.00
	功能：
	1.新库新编码
	2.完成基本功能
	3.添加电池电量接口
	4.属性变化自动上报

