﻿https://www.baidu.com/link?url=79Us-dUOAWOkiiztSYQQ42nlCAoUR4bhGZLHf5M3MaCoEPtFpkh9IEr3C1OCBLaQn9RWUQBfnpm0eSzf9O8CAytfFzbfK8w-uw5V4J35szG&wd=&eqid=955c106e0004be3300000006627c9cf4
https://ost.51cto.com/posts/1811

!! adc 是不能用的

注:ADC1,ADC2,ADC3 不能接一起,会短路的 (画原理图的时候悬空即可)

i2c模式下:CS 要接高电平(3v3就行),否则不识别.
	SDO 接低电平时,地址为0x18,接高电平时,地址为0x19 (悬空默认0x19)
