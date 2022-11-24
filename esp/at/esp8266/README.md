https://zhuanlan.zhihu.com/p/55626114

https://blog.csdn.net/Xiaomo_haa/article/details/108514149



基础指令

| CMD    | INFO         | 换行符 |
| ------ | ------------ | ------ |
| AT     | 测试状态     | √      |
| AT+RST | 重启模块     | √      |
| AT+GMR | 查看版本信息 | √      |



| CMD                        | INFO   | 换行符 |
| -------------------------- | ------ | ------ |
| AT+CWJAP                   | 查询AP | √      |
| AT+CWJAP="ssid","password" | 连接AP | √      |
| AT+CWQAP                   | 断开AP | √      |

AT+CWJAP="HUAWEI-Y6AZGD","13631520360"

关闭开机自动进入透传模式:AT+SAVETRANSLINK=0 

#### 连接 TCP 服务器

| CMD                                   | INFO          | 换行符 |
| ------------------------------------- | ------------- | ------ |
| AT+CWMODE=1                           | 设置为STA模式 | √      |
| AT+CWJAP="ssid","password"            | 连接AP        | √      |
| AT+CIPMUX=0                           | 单连接模式    | √      |
| AT+CIPSTART="TCP","192.168.3.20",8080 | 建立TCP连接   | √      |
| AT+CIPMODE=1                          | 透传模式      | √      |
| AT+CIPSEND                            | 进入数据模式  | √      |
| +++                                   | 退出透传模式  | ×      |

##### 透传模式下 - POST 请求（JSON）

```上传数据
POST / HTTP/1.1
Content-Type:application/json
Content-length:11

{"temp":10}
```
