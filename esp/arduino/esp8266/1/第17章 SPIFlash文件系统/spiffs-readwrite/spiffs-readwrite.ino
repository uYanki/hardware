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
  //创建文件test.txt
  myFile = SPIFFS.open("/test.txt", "w");
  if (myFile) {
    //在文件test.txt中写入testing 1, 2, 3.
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    //关闭文件
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening test.txt");
  }
  //打开读取文件test.txt
  myFile = SPIFFS.open("/test.txt","r");
  if (myFile) {
    Serial.println("test.txt:");
    while (myFile.available()) {
      //读取文件并输出
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}

void loop()
{
  // nothing happens after setup
}


