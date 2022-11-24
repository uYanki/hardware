//***************************************
// BMP085 IIC���Գ���
// ʹ�õ�Ƭ��STC89C51 
// ����11.0592M
// ��ʾ��LCD1602
// ���뻷�� Keil uVision2
// �ο��꾧��վ24c04ͨ�ų���
// ʱ�䣺2011��7��1��
// QQ��531389319
//****************************************
#include  <REG51.H>	  
#include  <math.h>    //Keil library  
#include  <stdlib.h>  //Keil library  
#include  <stdio.h>   //Keil library	
#include  <INTRINS.H> //Keil library  
#define   uchar unsigned char
#define   uint unsigned int	
#define   DataPort P0    //LCD1602���ݶ˿�
sbit	  SCL=P1^0;      //IICʱ�����Ŷ���
sbit 	  SDA=P1^1;      //IIC�������Ŷ���
sbit      LCM_RS=P2^0;   //LCD1602����˿�		
sbit      LCM_RW=P2^1;   //LCD1602����˿�		
sbit      LCM_EN=P2^2;   //LCD1602����˿� 

#define	BMP085_SlaveAddress   0xee	  //����������IIC�����еĴӵ�ַ                               

#define OSS 0	// Oversampling Setting (note: code is not set up to use other OSS values)
							   
typedef unsigned char  BYTE;
typedef unsigned short WORD;
   	
uchar ge,shi,bai,qian,wan,shiwan;           //��ʾ����
int  dis_data;                              //����

short ac1;
short ac2; 
short ac3; 
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1; 
short b2;
short mb;
short mc;
short md;

void delay(unsigned int k);
void InitLcd();                            //��ʼ��lcd1602

void WriteDataLCM(uchar dataW);
void WriteCommandLCM(uchar CMD,uchar Attribc);
void DisplayOneChar(uchar X,uchar Y,uchar DData);
void conversion(long temp_data);

void  Single_Write(uchar SlaveAddress,uchar REG_Address,uchar REG_data);   //����д������
uchar Single_Read(uchar REG_Address);                                      //������ȡ�ڲ��Ĵ�������
void  Multiple_Read(uchar,uchar);                                          //�����Ķ�ȡ�ڲ��Ĵ�������
//------------------------------------
void Delay5us();
void Delay5ms();
void BMP085_Start();
void BMP085_Stop();
void BMP085_SendACK(bit ack);
bit  BMP085_RecvACK();
void BMP085_SendByte(BYTE dat);
BYTE BMP085_RecvByte();
void BMP085_ReadPage();
void BMP085_WritePage();
//-----------------------------------

//*********************************************************
void conversion(long temp_data)  
{  
    
    shiwan=temp_data/100000+0x30 ;
    temp_data=temp_data%100000;   //ȡ������ 
    wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //ȡ������
	qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //ȡ������
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //ȡ������
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //ȡ������
    ge=temp_data+0x30; 	
}

/*******************************/
void delay(unsigned int k)	
{						
unsigned int i,j;				
for(i=0;i<k;i++)
{			
for(j=0;j<121;j++)			
{;}}						
}
/*******************************/
void WaitForEnable(void)	
{					
DataPort=0xff;		
LCM_RS=0;LCM_RW=1;_nop_();
LCM_EN=1;_nop_();_nop_();
while(DataPort&0x80);	
LCM_EN=0;				
}					
/*******************************/
void WriteCommandLCM(uchar CMD,uchar Attribc)
{					
if(Attribc)WaitForEnable();	
LCM_RS=0;LCM_RW=0;_nop_();
DataPort=CMD;_nop_();	
LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}					
/*******************************/
void WriteDataLCM(uchar dataW)
{					
WaitForEnable();		
LCM_RS=1;LCM_RW=0;_nop_();
DataPort=dataW;_nop_();	
LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}		
/***********************************/
void InitLcd()				
{			
WriteCommandLCM(0x38,1);	
WriteCommandLCM(0x08,1);	
WriteCommandLCM(0x01,1);	
WriteCommandLCM(0x06,1);	
WriteCommandLCM(0x0c,1);
}			
/***********************************/
void DisplayOneChar(uchar X,uchar Y,uchar DData)
{						
Y&=1;						
X&=15;						
if(Y)X|=0x40;					
X|=0x80;			
WriteCommandLCM(X,0);		
WriteDataLCM(DData);		
}						

/**************************************
��ʱ5΢��(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺�����ע��ʱ�ӹ���ʱ��Ҫ�޸�
������1T��MCUʱ,���������ʱ����
**************************************/
void Delay5us()
{
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
}

/**************************************
��ʱ5����(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
������1T��MCUʱ,���������ʱ����
**************************************/
void Delay5ms()
{
    WORD n = 560;

    while (n--);
}

/**************************************
��ʼ�ź�
**************************************/
void BMP085_Start()
{
    SDA = 1;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 0;                    //�����½���
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
}

/**************************************
ֹͣ�ź�
**************************************/
void BMP085_Stop()
{
    SDA = 0;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 1;                    //����������
    Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void BMP085_SendACK(bit ack)
{
    SDA = ack;                  //дӦ���ź�
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
**************************************/
bit BMP085_RecvACK()
{
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    CY = SDA;                   //��Ӧ���ź�
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ

    return CY;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void BMP085_SendByte(BYTE dat)
{
    BYTE i;

    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;              //�Ƴ����ݵ����λ
        SDA = CY;               //�����ݿ�
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    BMP085_RecvACK();
}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
BYTE BMP085_RecvByte()
{
    BYTE i;
    BYTE dat = 0;

    SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        dat |= SDA;             //������               
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    return dat;
}
/*
//���ֽ�д��BMP085�ڲ�����*******************************

void Single_Write(uchar SlaveAddress,uchar REG_Address,uchar REG_data)
{
    BMP085_Start();                  //��ʼ�ź�
    BMP085_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    BMP085_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ
    BMP085_SendByte(REG_data);       //�ڲ��Ĵ�������
    BMP085_Stop();                   //����ֹͣ�ź�
}
*/
/*
//���ֽڶ�ȡBMP085�ڲ�����********************************
uchar Single_Read(uchar REG_Address)
{  uchar REG_data;
    BMP085_Start();                          //��ʼ�ź�
    BMP085_SendByte(BMP085_SlaveAddress);           //�����豸��ַ+д�ź�
    BMP085_SendByte(REG_Address);            //���ʹ洢��Ԫ��ַ	
    BMP085_Start();                          //��ʼ�ź�
    BMP085_SendByte(BMP085_SlaveAddress+1);         //�����豸��ַ+���ź�
    REG_data=BMP085_RecvByte();              //�����Ĵ�������
	BMP085_SendACK(1);   
	BMP085_Stop();                           //ֹͣ�ź�
    return REG_data; 
}
*/
//*********************************************************
//����BMP085�ڲ�����,��������
//*********************************************************
short Multiple_read(uchar ST_Address)
{   
	uchar msb, lsb;
	short _data;
    BMP085_Start();                          //��ʼ�ź�
    BMP085_SendByte(BMP085_SlaveAddress);    //�����豸��ַ+д�ź�
    BMP085_SendByte(ST_Address);             //���ʹ洢��Ԫ��ַ
    BMP085_Start();                          //��ʼ�ź�
    BMP085_SendByte(BMP085_SlaveAddress+1);         //�����豸��ַ+���ź�

    msb = BMP085_RecvByte();                 //BUF[0]�洢
    BMP085_SendACK(0);                       //��ӦACK
    lsb = BMP085_RecvByte();     
	BMP085_SendACK(1);                       //���һ��������Ҫ��NOACK

    BMP085_Stop();                           //ֹͣ�ź�
    Delay5ms();
    _data = msb << 8;
	_data |= lsb;	
	return _data;
}
//********************************************************************
long bmp085ReadTemp(void)
{

    BMP085_Start();                  //��ʼ�ź�
    BMP085_SendByte(BMP085_SlaveAddress);   //�����豸��ַ+д�ź�
    BMP085_SendByte(0xF4);	          // write register address
    BMP085_SendByte(0x2E);       	// write register data for temp
    BMP085_Stop();                   //����ֹͣ�ź�
	delay(10);	// max time is 4.5ms
	
	return (long) Multiple_read(0xF6);
}
//*************************************************************
long bmp085ReadPressure(void)
{
	long pressure = 0;

    BMP085_Start();                   //��ʼ�ź�
    BMP085_SendByte(BMP085_SlaveAddress);   //�����豸��ַ+д�ź�
    BMP085_SendByte(0xF4);	          // write register address
    BMP085_SendByte(0x34);       	  // write register data for pressure
    BMP085_Stop();                    //����ֹͣ�ź�
	delay(10);    	                  // max time is 4.5ms
	
	pressure = Multiple_read(0xF6);
	pressure &= 0x0000FFFF;
	
	return pressure;	
	//return (long) bmp085ReadShort(0xF6);
}

//**************************************************************

//��ʼ��BMP085��������Ҫ��ο�pdf�����޸�**************
void Init_BMP085()
{
	ac1 = Multiple_read(0xAA);
	ac2 = Multiple_read(0xAC);
	ac3 = Multiple_read(0xAE);
	ac4 = Multiple_read(0xB0);
	ac5 = Multiple_read(0xB2);
	ac6 = Multiple_read(0xB4);
	b1 =  Multiple_read(0xB6);
	b2 =  Multiple_read(0xB8);
	mb =  Multiple_read(0xBA);
	mc =  Multiple_read(0xBC);
	md =  Multiple_read(0xBE);
}
//***********************************************************************
void bmp085Convert()
{
	long ut;
	long up;
	long x1, x2, b5, b6, x3, b3, p;
	unsigned long b4, b7;
	long  temperature;
	long  pressure;
	
	ut = bmp085ReadTemp();
	ut = bmp085ReadTemp();	   // ��ȡ�¶�
	up = bmp085ReadPressure();
	up = bmp085ReadPressure();  // ��ȡѹǿ
	
	x1 = ((long)ut - ac6) * ac5 >> 15;
	x2 = ((long) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	 temperature = (b5 + 8) >> 4;

	 //*************

	 conversion(temperature);
	 DisplayOneChar(4,0,'T');       //�¶���ʾ
     DisplayOneChar(5,0,':'); 
     DisplayOneChar(7,0,bai);       
     DisplayOneChar(8,0,shi); 
     DisplayOneChar(9,0,'.'); 
	 DisplayOneChar(10,0,ge); 
	 DisplayOneChar(11,0,0XDF);     //�¶ȵ�λ
	 DisplayOneChar(12,0,'C'); 

	 
     //*************
	
	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;
	b3 = (((long)ac1 * 4 + x3) + 2)/4;
	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long) up - b3) * (50000 >> OSS);
	if( b7 < 0x80000000)
	     p = (b7 * 2) / b4 ;
           else  
		    p = (b7 / b4) * 2;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	 pressure = p + ((x1 + x2 + 3791) >> 4);

	 conversion(pressure);
     DisplayOneChar(4,1,'P');    //��ʾѹǿ
     DisplayOneChar(5,1,':'); 
	 DisplayOneChar(6,1,shiwan); 
	 DisplayOneChar(7,1,wan);    
     DisplayOneChar(8,1,qian); 
     DisplayOneChar(9,1,'.'); 
     DisplayOneChar(10,1,bai); 
     DisplayOneChar(11,1,shi); 
	 DisplayOneChar(12,1,'K');   //��ѹ��λ
	 DisplayOneChar(13,1,'p'); 
	 DisplayOneChar(14,1,'a'); 

}

//*********************************************************
//******������********
//*********************************************************
void main()
{ 
    delay(50);	                   //�ϵ���ʱ		
    InitLcd();                     //Һ����ʼ��
    Init_BMP085();                //��ʼ��BMP085 
  while(1)                         //ѭ��
  { 
    bmp085Convert();
	delay(1000); 
  }
} 