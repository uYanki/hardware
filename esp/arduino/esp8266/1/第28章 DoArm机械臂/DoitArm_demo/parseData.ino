
#include "project.h"

#include "DoitArm.h"
DoitArm arm;

static char strFloat[32];
char* printFloat(float a)
{
  dtostrf(a, 4, 3, strFloat);
  return strFloat;
}
void printHelp(void)
{
  char buff[1024];
  sprintf(buff, "\r\n----------DoARM demo V1.0----------\r\n");
  strcat(buff, "'h' : print this help\r\n");
  strcat(buff, "'z' : goto zero position\r\n");
  strcat(buff, "'p' : print arm status\r\n");
  strcat(buff, "'d' : set/get current speed, set: d:1,1  1~10  get: d:2\r\n");
  strcat(buff, "'c' : movetoCart, c:x,y,z\r\n");
  strcat(buff, "'t' : movetoTool, t:x,y,z\r\n");
  strcat(buff, "'s' : sigle axis move, s:1,10\r\n");
  strcat(buff, "      1: [-90,30]\r\n");
  strcat(buff, "      2: [-30,60]\r\n");
  strcat(buff, "      3: [-40,35]\r\n");
  strcat(buff, "      4: [-90,90]\r\n");
  strcat(buff, "      5: [-50,90]\r\n");
  strcat(buff, "      6: [-80,80]\r\n");
  strcat(buff, "'w' : goto waitPosition, eg:w:0~9\r\n");
  strcat(buff, "'g' : set gripper angle eg:g:,30\r\n");
  strcat(buff, "----------------------------------------------------\r\n");
  print_log(buff);
}
void doPrintStatus(void)
{
  char buff[1024];
  float a[6];
  arm.getCurrentPosition(a, 6);
  float b[6];
  arm.getTargetPosition(b, 6);
  uint8_t v = arm.getCurrentSpeed();
  sprintf(buff, " Status:%d(0:SERVO_OFF 1:STAND_BY 2:MOVING)\r\n Speed:%d\r\n",
          arm.getArmStatus(), v);
  strcat(buff, "-------Arm Status-------\r\n");
  strcat(buff, " Current pos:(");
  strcat(buff, printFloat(a[0])); strcat(buff, ",");
  strcat(buff, printFloat(a[1])); strcat(buff, ",");
  strcat(buff, printFloat(a[2])); strcat(buff, ",");
  strcat(buff, printFloat(a[3])); strcat(buff, ",");
  strcat(buff, printFloat(a[4])); strcat(buff, ",");
  strcat(buff, printFloat(a[5])); strcat(buff, ")\r\n");
  strcat(buff, " Target  pos:(");
  strcat(buff, printFloat(b[0])); strcat(buff, ",");
  strcat(buff, printFloat(b[1])); strcat(buff, ",");
  strcat(buff, printFloat(b[2])); strcat(buff, ",");
  strcat(buff, printFloat(b[3])); strcat(buff, ",");
  strcat(buff, printFloat(b[4])); strcat(buff, ",");
  strcat(buff, printFloat(b[5])); strcat(buff, ")\r\n");

  DoitArm::stMat mat;
  arm.getToolCurrentPosture(&mat);
  char m[12];
  strcat(buff, " Current Posture:\r\n ");
  dtostrf(mat.nx, 4, 3, m); strcat(buff, m);
  dtostrf(mat.ox, 4, 3, m); strcat(buff, "\t"); strcat(buff, m);
  dtostrf(mat.ax, 4, 3, m); strcat(buff, "\t"); strcat(buff, m);
  dtostrf(mat.px, 4, 3, m); strcat(buff, "\t"); strcat(buff, m);
  dtostrf(mat.ny, 4, 3, m); strcat(buff, "\r\n "); strcat(buff, m);
  dtostrf(mat.oy, 4, 3, m); strcat(buff, "\t"); strcat(buff, m);
  dtostrf(mat.ay, 4, 3, m); strcat(buff, "\t"); strcat(buff, m);
  dtostrf(mat.py, 4, 3, m); strcat(buff, "\t"); strcat(buff, m);
  dtostrf(mat.nz, 4, 3, m); strcat(buff, "\r\n "); strcat(buff, m);
  dtostrf(mat.oz, 4, 3, m); strcat(buff, "\t"); strcat(buff, m);
  dtostrf(mat.az, 4, 3, m); strcat(buff, "\t"); strcat(buff, m);
  dtostrf(mat.pz, 4, 3, m); strcat(buff, "\t"); strcat(buff, m); strcat(buff, "\r\n 0\t0\t0\t1\t\r\n");

  strcat(buff, "------------------------\r\n");
  print_log(buff);
}
void doSigleAxisMove(char *p)
{
  if (strlen(p) < 5 ||
      *p != 's' ||
      !(*(p + 2) >= '1' && *(p + 2) <= '6'))
  {
    print_log("Invalid parameters!\r\n");
    return;
  }
  for (int i = 0; i < strlen(p + 4); i++)
  {
    if (!((*(p + 4 + i) >= '0' && *(p + 4 + i) <= '9') ||
          (*(p + 4 + i) == '-' && i == 0) ||
          (*(p + 4 + i) == '.')))
    {
      print_log("Invalid parameters!\r\n");
      return;
    }
  }
  int axisNum = *(p + 2) - '0';
  float angle = atof(p + 4);// 0 ~180  -> SERVO_LIMIT_MIN~ SERVO_LIMIT_MAX
  if (angle < -90 || angle > 90)
  {
    print_log("angle should between:-90~90\r\n");
    return;
  }
  char t[64];
  sprintf(t, "SigleAxisMove axis:%d,angle:%s\t\r\n", axisNum, printFloat(angle));
  print_log(t);
  //arm.moveAxis(axisNum, angle);
  arm.setTargetPosition(axisNum, angle);
  arm.beginMove();
}
void doGripper(char *p)
{ //g:-20
  if (strlen(p) < 3 ||
      *p != 'g' )
  {
    print_log("Invalid parameters!\r\n");
    return;
  }

  for (int i = 0; i < strlen(p + 2); i++)
  {
    if (!((*(p + 2 + i) >= '0' && *(p + 2 + i) <= '9') ||
          (*(p + 2 + i) == '-' && i == 0) ||
          (*(p + 2 + i) == '.')))
    {
      print_log("Invalid parameters!\r\n");
      return;
    }
  }
  float angle = atof(p + 2);
  if (angle < -90 || angle > 90)
  {
    print_log("angle should between:-90~90\r\n");
    return;
  }
  char t[64];
  sprintf(t, "Gripper angle:%s\t\r\n", printFloat(angle));
  print_log(t);
  arm.gripper(angle);
}

uint8_t flag_continueMove = false;
uint8_t lastStep = 0;
void doWaitPosition(char* p)
{
  if (strlen(p) < 3 )
  {
    print_log("Invalid parameters!  w:1 or w:2 w:3\r\n");
    return;
  }

  if (*(p + 2) == '0')
  {
    float pos[6] = {0.000, -30.000, 0.000, 0.000, 0.000, 0.000};
    arm.setWaitPosition(pos, 6);
    arm.gotoWaitPosition();
    lastStep = 0 ;
    print_log("gotoWaitPosition ok!\r\n");
  }
  else if (*(p + 2) == '1')
  {
    float a[6] = {0.000, 60.000, 0.000, 0.000, 0.000, 0.000};
    arm.setWaitPosition(a, 6);
    arm.gotoWaitPosition();
    lastStep = 1 ;
    print_log("gotoWaitPosition ok!\r\n");
  }
  else if (*(p + 2) == '2')
  {
    float a[6] = {0.000, 30.000, -30.000, 0.000, 0.000, 0.000};
    arm.setWaitPosition(a, 6);
    arm.gotoWaitPosition();
    lastStep = 2 ;
    print_log("gotoWaitPosition ok!\r\n");
  }
  else if (*(p + 2) == '3')
  {
    float a[6] = {0.000, 30.000, 30.000, 0.000, 0.000, 0.000};
    arm.setWaitPosition(a, 6);
    arm.gotoWaitPosition();
    lastStep = 3 ;
    print_log("gotoWaitPosition ok!\r\n");
  }
  else if (*(p + 2) == '4')
  {
    float a[6] = { -20.000, 5.000, -10.000, 50.000, 30.000, 30.000};
    arm.setWaitPosition(a, 6);
    arm.gotoWaitPosition();
    lastStep = 4 ;
    print_log("gotoWaitPosition ok!\r\n");
  }
  else if (*(p + 2) == '5')
  {
    float a[6] = {20.000, 5.000, -10.000, -50.000, 30.000, 30.000};
    arm.setWaitPosition(a, 6);
    arm.gotoWaitPosition();
    lastStep = 5;
    print_log("gotoWaitPosition ok!\r\n");
  }
  else if (*(p + 2) == '6')
  {
    float a[6] = {0.000, 0.000, 0.000, 0.000, 30.000, 0.000};
    arm.setWaitPosition(a, 6);
    arm.gotoWaitPosition();
    arm.gripper(40);
    lastStep = 6;
    print_log("gotoWaitPosition ok!\r\n");
  }
  else if (*(p + 2) == '7')
  {
    float a[6] = {0.000, 60.000, 35.000, 0.000, 50.000, 0.000};
    arm.setWaitPosition(a, 6);
    arm.gotoWaitPosition();
    arm.gripper(40);
    lastStep = 7;
    print_log("gotoWaitPosition ok!\r\n");
  }
  else if (*(p + 2) == '8')
  {
    float a[6] = {0.000, -10.000, -40.000, 0.000, 30.000, 0.000};
    arm.setWaitPosition(a, 6);
    arm.gotoWaitPosition();
    arm.gripper(20);
    lastStep = 8;
    print_log("gotoWaitPosition ok!\r\n");
  }
  else if (*(p + 2) == '9')
  {
    //continue move
    flag_continueMove = true;
    float a[6] = {0.000, -10.000, 0.000, 0.000, 0.000, 0.000};
    arm.setWaitPosition(a, 6);
    arm.gotoWaitPosition();
    arm.gripper(30);
    lastStep = 9;
  }
  else
  {
    print_log("Invalid parameters! w:0~9\r\n");
  }
}

void doSetCurrentSpeed(char* p)
{ //set: d:1,1  1~10  get: d:2
  if (strlen(p) < 3 )
  {
    print_log("Invalid parameters!  d:1,1 or d:2\r\n");
    return;
  }
  if (*(p + 2) == '1' && (strlen(p) == 5 || strlen(p) == 6))
  {
    int a = atoi(p + 4);
    if (arm.setCurrentSpeed(a) == true)
    {
      char t[64];
      sprintf(t, "Set current speed to : %d ok\r\n", a);
      print_log(t);
    }
    else
      print_log("Set current speed failed\r\n");
  }
  else
  {
    uint8_t v = arm.getCurrentSpeed();
    char t[64];
    sprintf(t, "Current speed is : %d\r\n", v);
    print_log(t);
  }
}
void moveFinished_cb(void)
{
  print_log("moveFinished_cb\r\n");
  doPrintStatus();
  print_log(" >");
  if(flag_continueMove)
  {
    switch(lastStep)
    {
      case 9: doWaitPosition("w:0");break;
      case 0: doWaitPosition("w:1");break;
      case 1: doWaitPosition("w:2");break;
      case 2: doWaitPosition("w:3");break;
      case 3: doWaitPosition("w:4");break;
      case 4: doWaitPosition("w:5");break;
      case 5: doWaitPosition("w:6");break;
      case 6: doWaitPosition("w:7");break;
      case 7: doWaitPosition("w:8");flag_continueMove=false;break;
      default:break;
    }
  }
}
/*
  初始化
*/
void initParseData()
{
  /*s:1, -90 30
    s:2,-30 60 反的
    s:3, -40 35
    s:4, -90 90
    s:5, -50 90
    s:6, -80 80
  */
  Serial.println("[initParseData]");
  printHelp();

  //各轴舵机中位偏移
  float bias[6] = {65, 5, -8, 5, 0, 0};
  arm.setServoBias(bias, 6);
  //各轴舵机移动范围
  arm.setServoRange(-90, 30, -30, 60, -40, 35, -90, 90, -50, 90, -80, 80);
  //各轴舵机在驱动板上的位置
  arm.setChannel(11, 9, 13, 4, 1, 3);
  arm.attachServo();
  arm.setCurrentSpeed(2);
  arm.setCaliChannel(15);//设置标定舵机channel为15号，在attachServo之后使用
  //设置抓手通道号
  arm.setGripperChannel(14);
  //设置零点
  arm.gotoZeroPosition();

  //int a[6] = {0,0,0,0,0};
  //arm.setWaitPosition(a, 6);
  //arm.gotoWaitPosition();
  arm.registMoveFinishCB(&moveFinished_cb);
}
void split(char *src, const char *separator, char **dest, int *num)
{
  char *pNext;
  int count = 0;
  if (src == NULL || strlen(src) == 0) return;
  if (separator == NULL || strlen(separator) == 0) return;
  pNext = strtok(src, separator);
  while (pNext != NULL)
  {
    *dest++ = pNext; ++count;
    pNext = strtok(NULL, separator);
  }
  *num = count;
}
void doMoveToCart(char *p)
{
  //movetoCart, c:x,y,z
  char *numStr[128];
  int num = 0;
  split(p + 2, ",", numStr, &num);
  if (num != 3)
  {
    print_log("Invalid parameters!  c:10,20,30\r\n");
    return;
  }
  float x = atof(numStr[0]);
  float y = atof(numStr[1]);
  float z = atof(numStr[2]);
  if (!arm.movetoCart(x, y, z))
  {
    print_log("Can not move!\r\n");
  }
}
void doMoveToTool(char *p)
{
  //doMoveToTool, t:x,y,z
  char *numStr[128];
  int num = 0;
  split(p + 2, ",", numStr, &num);
  if (num != 3)
  {
    print_log("Invalid parameters!  t:10,20,30\r\n");
    return;
  }
  float x = atof(numStr[0]);
  float y = atof(numStr[1]);
  float z = atof(numStr[2]);
  if (!arm.movetoTool(x, y, z))
  {
    print_log("Can not move!\r\n");
  }
}
void doGotoZeroPosition(void)
{
  arm.gotoZeroPosition();
  print_log("goto ZeroPosition\r\n");
}
/*
  UDP接收包处理
*/
void parseUDPPackage(char *p)
{
  if (p[strlen(p) - 2] == '\r' || p[strlen(p) - 2] == '\n') p[strlen(p) - 2] = 0x00;
  switch (*p)
  {
    case 'h': printHelp(); break;
    case 'z': doGotoZeroPosition(); break;
    case 'p': doPrintStatus(); break;
    case 'c': doMoveToCart(p); break;
    case 't': doMoveToTool(p); break;
    case 'd': doSetCurrentSpeed(p); break;
    case 's': doSigleAxisMove(p); break;
    case 'w': doWaitPosition(p); break;
    case 'g': doGripper(p); break;
    default:
      if (!(*p == '\r' || *p == '\n' || (*p == 0xFF && *(p + 1) == 0xFD))) {
        print_log("[Invalid CMD]\r\n"); break;
      }
  }
  print_log("> ");
}
/*
  TCP接收包处理
*/
void parseTCPPackage(char *p)
{
  print_log("[TCP parseData:]");
  print_log(p);
}
/*
  串口接收包处理
*/
void parseUartPackage(char *p)
{
  print_log("[Uart parseData]");
  print_log(p);
  parseUDPPackage(p);
}

//static u32 pretick = 0;
void doArmTick(void)
{
  arm.handleServo();
  //急停
  //arm.emeryStop();

  /*if (millis() - pretick > 1000)
    {
    pretick = millis();
    print_log(".");
    }*/
}

