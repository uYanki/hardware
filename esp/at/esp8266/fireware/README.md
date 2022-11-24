固件下载地址: 
https://docs.ai-thinker.com/esp8266/sdk

固件下载教程：
https://blog.csdn.net/LiangWF22/article/details/122390492
https://blog.csdn.net/jackcsdnfghdtrjy/article/details/104770612

flash_download_tool
- FLASH SIZE：和要写入的固件的大小相同（板子上焊接的flash芯片ROM至少要大于等于固件大小）
- UART BAUD：波特率可选择921600，这样下载固件用时比较短

esp01/esp01s：
擦除/下载固件前，先使用杜邦线将IO0接地，再使用杜邦线将EN接地，然后将连接着EN的杜邦线从地移开，再将连接着IO0的杜邦线从地移开，最后点击擦除/下载固件按钮即可

hekrv1.1：
没有引出IO0，可以先将esp01模块和hekr模块的flash芯片拆下，将装在hekr模块上的flash焊再esp01模块上，烧录固件后再焊回hekr模块上

esp8266_sdk_v0.9.1：固件源码等

