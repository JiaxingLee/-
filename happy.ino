
#define SSID        "nova" //改为你的热点名称, 不要有中文
#define PASSWORD    "12345678"//改为你的WiFi密码Wi-Fi密码
#define DEVICEID    "577959342" //OneNet上的设备ID
String apiKey = "unPi6Fd5bf7omzCESWFsFJESRpI=";//与你的设备绑定的APIKey

#define HOST_NAME   "api.heclouds.com"
#define HOST_PORT   (80)

//传感器部分================================   
#include <Wire.h>    
#include <math.h> 
int BH1750address = 0x23; 
byte buff[2];                         

#include <ESP8266.h>
#include <I2Cdev.h>                                //调用库  

int TSlim=800;


#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 
                                   // received before closing the connection.  If you know the server
                                   // you're accessing is quick to respond, you can reduce this value.

//WEBSITE     
char buf[10];

#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();

float tempOLED, humiOLED, lightnessOLED;

#define INTERVAL_OLED 1000

String mCottenData;
String jsonToSend;


#include <Servo.h>//舵机
Servo myservo;
int pos = 0;
int addition;
//传感器值的设置 
#include <dht11.h>
dht11 DHT11;
#define tspin A0      //土壤湿度引脚
#define DHT11PIN 2//温湿度传感引脚
#define watpin 8//水泵引脚
#define ledpin 6//补光灯引脚

#define winpin 7//换风扇引脚
float sensor_tem, sensor_hum, sensor_lux=0,ts;                    //传感器温度、湿度、光照   
char  sensor_tem_c[7], sensor_hum_c[7], sensor_lux_c[7] ,ts_c[7];    //换成char数组传输


#include <SoftwareSerial.h>
#define EspSerial mySerial
#define UARTSPEED  9600
SoftwareSerial mySerial(12, 13);                                  //RX:D13, TX:D12 
ESP8266 wifi(&EspSerial);                                 //定义一个ESP8266（wifi）的对象
//unsigned long net_time1 = millis();                          //数据上传服务器时间
//unsigned long sensor_time = millis();                        //传感器采样时间计时器

int chk;                                   //用于存储传感器数据
String postString;                                //用于存储发送数据的字符串
//String jsonToSend;                                //用于存储发送的json格式参数


void setup(void)      
{  
      
  //初始化串口波特率  
    Wire.begin();
    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately
    Serial.print(F("setup begin\r\n"));
    delay(100);
   
    myservo.attach(9);//遮阳布
    myservo.write(pos);
    pinMode(8,OUTPUT);
   // pinMode(9,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(6,OUTPUT);
  WifiInit(EspSerial, UARTSPEED);
  Serial.print(F("FW Version:"));
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print(F("to station + softap ok\r\n"));
  } else {
    Serial.print(F("to station + softap err\r\n"));
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print(F("Join AP success\r\n"));
    Serial.print(F("IP:"));
    Serial.println( wifi.getLocalIP().c_str());
  } else {
    Serial.print(F("Join AP failure\r\n"));
  }

  if (wifi.disableMUX()) {
    Serial.print(F("single ok\r\n"));
  } else {
    Serial.print(F("single err\r\n"));
  }
   Serial.print(F("setup end\r\n"));
}
void loop(void)     //循环函数  
{   
  getSensorData();                                        //读串口中的传感器数据
  updateSensorData();                                     //将数据上传到服务器的函数
  doit();
 
  delay(1000);
}

void getSensorData(){ 
  int chk = DHT11.read(DHT11PIN);
  Serial.print("Read sensor: ");
  switch (chk)
   {
      case DHTLIB_OK: 
      Serial.println("OK"); 
      break;
      case DHTLIB_ERROR_CHECKSUM: 
      Serial.println("Checksum error"); 
      break;
      case DHTLIB_ERROR_TIMEOUT: 
      Serial.println("Time out error"); 
      break;
      default: 
      Serial.println("Unknown error"); 
      break;
      } 
   float sensor_hum= DHT11.humidity;//读取湿度
   float sensor_tem = DHT11.temperature;//读取温度
   float TS=analogRead(tspin);//读取土壤湿度串口
   float ts=inmap(TS,510,1024,0,100);//换算相对湿度
   BH1750_Init(BH1750address);//读取光强
   if(2==BH1750_Read(BH1750address))
    {
      sensor_lux=((buff[0]<<8)|buff[1])/1.2;
      }  
   
    delay(1000);
    Serial.println(sensor_tem);
    Serial.println(sensor_hum);
    Serial.println(sensor_lux);
    Serial.println(TS);
    Serial.println(ts);
    
    dtostrf(sensor_tem, 2, 1, sensor_tem_c);//转换float
    dtostrf(sensor_hum, 2, 1, sensor_hum_c);
    dtostrf(sensor_lux, 3, 1, sensor_lux_c);
    dtostrf(ts,3,1, ts_c);
}
void updateSensorData() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
    Serial.print("create tcp ok\r\n");

    jsonToSend="{\"Temperature\":";
  
    jsonToSend+="\""+String(sensor_tem_c)+"\"";
    jsonToSend+=",\"Humidity\":";
  
    jsonToSend+="\""+String(sensor_hum_c)+"\"";
    jsonToSend+=",\"Light\":";
    
  
    jsonToSend+="\""+String(sensor_lux_c)+"\"";
    jsonToSend+=",\"ts\":";

    jsonToSend+="\""+String( ts_c)+"\"";
    jsonToSend+="}";
Serial.println(jsonToSend);


    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";

  const char *postArray = postString.c_str();                 //将str转化为char数组
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println("send success");   
     if (wifi.releaseTCP()) {                                 //释放TCP连接
        Serial.print("release tcp ok\r\n");
        } 
     else {
        Serial.print("release tcp err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  
  } else {
    Serial.print("create tcp err\r\n");
  }
}
void doit(){
    if (sensor_tem>= 15 )              //对温度进行判断
            digitalWrite(winpin,LOW);     //排风扇
    else
            digitalWrite(winpin,HIGH);
 if(sensor_lux>= 600)               //遮阳布
           {
            int n=map(sensor_lux,500,1000,0,180);
           pos+=addition;
             if(pos>=n)
              {addition=0; }
            if(pos<n)
              {addition=8;
              Serial.println(pos);
              myservo.write(pos);}
           }
       else {if(pos>=0)pos=-pos;
       Serial.println(pos);
            myservo.write(pos);
            pos=0;
            } 
   //Serial.println(pos);
    if(sensor_lux<100)   //LED补光
      {
      int lightOUT=inmap(sensor_lux,100,0,0,225);
      digitalWrite(ledpin,lightOUT);
       }
        else digitalWrite(ledpin,LOW);       
     if(ts>80)digitalWrite(8,HIGH);
        else digitalWrite(8,LOW);
        }

int BH1750_Read(int address) 
{
  int i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available()) 
  {
    buff[i] = Wire.read();  //读取
    i++;
  }
  Wire.endTransmission();  
  return i;
}
void BH1750_Init(int address) 
{
  Wire.beginTransmission(address);
  Wire.write(0x10);
  Wire.endTransmission();
}
long inmap(long x, long in_min, long in_max, long out_min, long out_max) //反映射函数
{
  return out_max-(x-in_min)*(out_max-out_min)/(in_max-in_min);
}
