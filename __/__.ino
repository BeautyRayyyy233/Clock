  #include <ESP8266.h>   //引入所需库文件
  #include "dht11.h"                                               //引用dht11库文件，使得下面可以调用相关参数
#define DHT11PIN 8                                            //定义温湿度针脚号为8号引脚
dht11 DHT11;     
#include"DS1302.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  //配置LCD地址及行列

#define numdata_length 2
String comdata = "";
int numdata[18] = {0};
int flag = 0;
#define Buzz_Pin 5
unsigned int Alarm_Hour[9];
unsigned int Alarm_Minute[9];
bool Always=false;
//下列预编译指令用于探测使用的core还是core+以适配不同版本硬件


#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif

#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); 

#define EspSerial mySerial
#define UARTSPEED  9600
#endif

#define SSID        F("Godzilla") //WIFI名称
#define PASSWORD    F("00000000") //WIFI密码
#define DEVICEID    "575257071" //OneNet上的设备ID
String apiKey = "JKLvQT7T7MNsME7F8nfDxirhqz0=";//与你的设备绑定的APIKey

/***/
#define HOST_NAME   "api.heclouds.com"
#define HOST_PORT   (80)
#define INTERVAL_SENSOR   5000             //定义传感器采样时间间隔  597000
#define INTERVAL_NET      5000             //定义发送时间
static const byte  GETDATA[]  PROGMEM = {
  "GET https://api.heclouds.com/devices/575257071/datapoints?datastream_id=Alarm,&limit=1 HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key:JKLvQT7T7MNsME7F8nfDxirhqz0=\r\nConnection: close\r\n\r\n"
};

ESP8266 wifi(&EspSerial);
  
  int tiaojian=0;
  #define Do 262  
#define Re 294  
#define Mi 330  
#define Fa 349  
#define Sol 392  
#define La 440  
#define Si 494  
#define Do_h 523  
#define Re_h 587  
#define Mi_h 659  
#define Fa_h 698  
#define Sol_h 784  
#define La_h 880  
#define Si_h 988
int length;
int scale[]={Sol,Sol,La,Sol,Do_h,Si,
             Sol,Sol,La,Sol,Re_h,Do_h,
             Fa_h,Fa_h,Mi_h,Do_h,Re_h,Do_h};       
float durt[]=
{
  0.5,0.5,1,1,1,1+1,
  0.5,0.5,1,1,1,1+1,
   0.5,0.5,1,1,1,1,1,
    0.5,0.5,1,1,1,1+1,
};                                                        //音长
int tonepin=8;   //用8号引脚


void setup()
{
  pinMode(Buzz_Pin,OUTPUT);
  pinMode(tonepin,OUTPUT);
  length=sizeof(scale)/sizeof(scale[0]);   //计算长度

  pinMode(DHT11PIN,OUTPUT);   
  DS1302_Init();
  Serial.begin(9600); 
  Serial.println("Welcome to use!");
   while (!Serial); // wait for Leonardo enumeration, others continue immediately
  Serial.print(F("setup begin\r\n"));
    WifiInit(EspSerial, UARTSPEED);
    lcd.init();    //初始化LCD1602 

lcd.backlight();//液晶显示Welcometo use！ 
     DS1302_GetTime(&DS1302Buffer);
  Display_RTCC();
  dth11();
  delay(100);
   lcd.clear();    
  Serial.print(F("FW Version:"));
  Serial.println(wifi.getVersion().c_str()); //获取FW Version值

  if (wifi.setOprToStation()) {
    Serial.print(F("to station ok\r\n"));
  } else {
    Serial.print(F("to station err\r\n"));
  }

  if (wifi.joinAP(SSID, PASSWORD)) {  //如果连接成功，显示连接成功（Join AP success），以及IP和Mac地址
    Serial.print(F("Join AP success\r\n"));
    Serial.print(F("IP:"));
    Serial.println( wifi.getLocalIP().c_str());
  } else { //如果连接失败显示连接失败（Join AP success）
    Serial.print(F("Join AP failure\r\n"));
  };
  if (wifi.disableMUX()) {
    Serial.print(F("single ok\r\n"));
  } else {
    Serial.print(F("single err\r\n"));
  }

  Serial.print(F("setup end\r\n"));  
 
}
void Display_RTCC(){
 
    lcd.setCursor(0,0);

    switch(DS1302Buffer.Week)
  {
    case 1:
        Serial.println("Mon");
        lcd.print("Mon");//显示星期一
        break;
    case 2:
        Serial.println("Tue"); 
        lcd.print("Tue");//显示星期二
        break;
    case 3:
        Serial.println("Wed");
        lcd.print("Wed");//显示星期三
        break;
    case 4:
        Serial.println("Thu");
        lcd.print("Thu");//显示星期四
        break;
    case 5:
        Serial.println("Fri");     
        lcd.print("Fri"); //显示星期五
        break;
    case 6:
        Serial.println("Sat");
        lcd.print("Sat");//显示星期六
        break;
    case 7:
        Serial.println("Sun");  
        lcd.print("Sun");//显示星期日
        break;
    default : break;            
  }
lcd.setCursor(0,4);
  lcd.print(DS1302Buffer.Hour);
  lcd.print(':');
  lcd.print(DS1302Buffer.Minute);

}

// 获取串口输入
void Scan_Input()
{
  int j = 0;
//不断循环检测串口缓存，一个个读入字符串
while (Serial.available() > 0)
{
  comdata += char(Serial.read());
  delay(2);
  flag = 1;
}
//如果接收到数据则执行comdata分析操作，否则什么都不做
if(flag == 1) {
for(int i = 0; i < comdata.length() ; i++){
  if(comdata[i] == ':'){
    j++;
    }
  else{
    numdata[j] = numdata[j] * 10 + (comdata[i] - '0');
    }
  }
flag = 0;
Alarm_Hour[0]=numdata[0];
numdata[0]=0;
Alarm_Minute[0]=numdata[1];
numdata[1]=0;
}

  
}

//时间设置函数

//时间设置函数
void Set_Time(String &command)
{
DS1302_ON_OFF(0); //关闭振荡
DS1302Buffer.Year=command.substring(4,6).toInt();
DS1302Buffer.Month=command.substring(7,9).toInt();
DS1302Buffer.Day=command.substring(10,12).toInt();
DS1302Buffer.Week=command.substring(13,14).toInt();
DS1302Buffer.Hour=command.substring(15,17).toInt();
DS1302Buffer.Minute=command.substring(18).toInt();
DS1302_SetTime( DS1302_YEAR , DS1302Buffer.Year );
DS1302_SetTime( DS1302_MONTH , DS1302Buffer.Month );
DS1302_SetTime( DS1302_DAY , DS1302Buffer.Day );
DS1302_SetTime( DS1302_WEEK , DS1302Buffer.Week );
DS1302_SetTime( DS1302_HOUR , DS1302Buffer.Hour );
DS1302_SetTime( DS1302_MINUTE , DS1302Buffer.Minute );
DS1302_ON_OFF(1);

Display_RTCC();
}

//蜂鸣器报警函数
void buzz(int &times)
{
for(int x=0;x<length;x++)
{
tone(tonepin,scale[x]);
digitalWrite(ledpin,HIGH);
delay(500*durt[x]); //这里用来根据节拍调节延时，500这个指数可以自己调整
digitalWrite(ledpin,LOW);
delay(100*durt[x]);
noTone(tonepin);
}
delay(3000);

}

void dth11(){
   int chk = DHT11.read(DHT11PIN);                 //将读取到的值赋给chk

int tem=(float)DHT11.temperature;               //将温度值赋值给tem
int hum=(float)DHT11.humidity;                   //将湿度值赋给hum
 lcd.setCursor(2,12);
          lcd.print("TEM:");
          lcd.print(tem);
          lcd.print(" HUM:");
          lcd.print(hum);

  
  }
void getdata(){
   wifi.sendFromFlash(GETDATA, sizeof(GETDATA)); //从Flash读取发送内容，节约内存
  uint8_t buffer[1024] = {0};
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 20000);
  if (len > 0) {
    Serial.print(F("Received:["));
    for (uint32_t i = 0; i < len; i++) {
      Serial.print((char)buffer[i]);
    }
    Serial.print(F("]\r\n"));
  }
  
  }
void loop()
{
  Scan_Input();
  DS1302_GetTime(&DS1302Buffer);  

          Display_RTCC();
          dth11();
getdata();
  if (wifi.releaseTCP()) {
    Serial.print(F("release tcp ok\r\n"));
  } else {
    Serial.print(F("release tcp err\r\n"));
  }
while(tiaojian==1){
  if (wifi.releaseTCP()) {
    Serial.print(F("release tcp ok\r\n"));
  } else {
    Serial.print(F("release tcp err\r\n"));
  }
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    Serial.print(F("create tcp ok\r\n"));
  } else {
    Serial.print(F("create tcp err\r\n"));
  }
 
  if (wifi.releaseTCP()) {
    Serial.print(F("release tcp ok\r\n"));
  } else {
    Serial.print(F("release tcp err\r\n"));
  }
  tiaojian=0;
}
  //while (1);
  delay(100);
         

}


