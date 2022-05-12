#### 新建项目

* 选择芯片 `AT89C52`

![image-20220419164310541](start.assets/new1.png)

* 设置编译时生成HEX文件

![image-20220419164502417](start.assets/new2.png)

* 编写基本代码

![image-20220419164756455](start.assets/new3.png)

#### VS Code

![image-20220419170506316](start.assets/vscode.png)

```json
"includePath": [
    "${workspaceFolder}/**",
    "C:/Keil_v5/C51/INC",
    "C:/Keil_v5/C51/INC/**"
],
"defines": [
    "_DEBUG",
    "UNICODE",
    "_UNICODE",
	"sbit=char",
	"code=const",
	"xdata=",
	"data=",
	"interrupt=",
	"bit=u8",
    "sfr=char"
]
```

注：勿启用 Keil Assistant 插件，该插件会去除自己配置的 includePath，导致找不到头文件

![image-20220419171107829](start.assets/vscode_plugin.png)
