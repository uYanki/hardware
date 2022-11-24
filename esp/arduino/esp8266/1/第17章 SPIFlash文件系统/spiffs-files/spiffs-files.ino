/***************************
	SPIFFS.open(const char* filename,const char*mode);
 	@Parameters filename 文件名
 	@Parameters mode
 	 |---------|---------------|------------------------|-----------------|---------------------|
 	 |mode     |模式字符串意义  |解释	                  	|如果文件已经存在	  |     如果文件不存在	|
     |---------|---------------|------------------------|-----------------|---------------------|
	 |  "r"	   |读          	   |打开一个文件进行读取    	|从开始读	      |未能打开            	|
	 |	"w"	   |写          	   |创建一个文件编写	      	|破坏内容	      |创建新的            	|
	 |	"a"	   |附加	       	   |追加到一个文件	      	|写结束	          |创建新的            	|
	 |	"r+"   |扩展阅读   	   |打开一个文件进行读/写   	|从开始读	      |错误                	|
	 |	"w+"   |编写扩展   	   |创建一个文件读/写	      	|破坏内容	      |创建新的            	|
	 |	"a+"   |附加扩展   	   |打开一个文件进行读/写结束	|写结束	          |创建新的            	|
     |------------------------------------------------------------------------------------------|
 **************************/
#include "FS.h"

File myFile;

void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SPIFFS card...");

  if (!SPIFFS.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
// 检测文件是否存在，没有则建立一个
  if (SPIFFS.exists("example.txt")) {
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");
  }

  //同时只能打开一个文件，打开一个需要关闭另一个
  Serial.println("Creating example.txt...");
  myFile = SPIFFS.open("example.txt", "w");
  myFile.close();

  // 检测文件是否存在
  if (SPIFFS.exists("example.txt")) {
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");
  }

  // 删除文件
  Serial.println("Removing example.txt...");
  SPIFFS.remove("example.txt");

  if (SPIFFS.exists("example.txt")) {
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");
  }
}

void loop()
{
  // nothing happens after setup finishes.
}



