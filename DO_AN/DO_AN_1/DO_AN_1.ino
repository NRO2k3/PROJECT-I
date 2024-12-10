#define BLYNK_TEMPLATE_ID "TMPL60OW4hZsO"
#define BLYNK_TEMPLATE_NAME "TEST"
#define BLYNK_AUTH_TOKEN "qhOsy_q4JaIeqtc1QtOLwMS5f_pKAbiD"
#include <WiFi.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#define DHTTYPE DHT22
#define DHTPIN 16
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
DHT dht(DHTPIN, DHTTYPE);
unsigned long thoi_gian,hien_tai=0;
int  LED = 17, LED_do=25, LED_xanh=26, LED_vang=27, bam = 18, do_am_dat=34, luu_luong_nuoc=35, Van=19, luu_luong_nuoc_1=32;
int button, bam1, che_do, time_on, time_off, t_on, t_off, do_am, nguong_tren, nguong_duoi, count;
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Hai San Bao Linh";
char pass[] = "141887bl";
bool trangthai=1,trangthai_1=1;
int h, m, s;
float nhietdo, doam;
int  gia_tri_doc, gia_tri_doc_1, gia_tri_doc_2, the_tich_1=0, the_tich=0;
String Web_app_url= "https://script.google.com/macros/s/AKfycbxBA3-NjIji4YSwq4d8qX6ghEjVDru9FNkmCmRtVHnZa7ZFz0NWDMvj98iI6JePDAiY/exec";
String Van_Bom = "OFF";
String Van_Tuoi = "OFF";
String Status1="Success";
int cho_phep=0;
int tght,tgqk=0;
void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  // Serial.print("Đang kết nối..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    // Serial.print(".");
  }
  timeClient.begin();
  timeClient.setTimeOffset(25200); 
  lcd.init();
  lcd.backlight();
  dht.begin();
  pinMode(LED, OUTPUT);
  pinMode(Van, OUTPUT);
  pinMode(LED_do, OUTPUT);
  pinMode(LED_xanh, OUTPUT);
  pinMode(LED_vang, OUTPUT);
  pinMode(bam, INPUT_PULLUP);
  Blynk.begin(auth, ssid, pass);
  digitalWrite(LED, LOW);
  digitalWrite(Van, LOW);
  digitalWrite(LED_do, LOW);
  digitalWrite(LED_xanh, LOW);
  digitalWrite(LED_vang, LOW);
}
BLYNK_WRITE(V0)
{
  che_do = param.asInt();
}
BLYNK_WRITE(V1) 
{
  button = param.asInt();
}
BLYNK_WRITE(V5) 
{ 
  time_on = param.asInt();
  nguong_tren = param.asInt();
}
BLYNK_WRITE(V6) 
{
  time_off = param.asInt();
  nguong_duoi = param.asInt();
}
void man_hinh_chinh()
{
  timeClient.update();
  nhietdo=dht.readTemperature(); 
  gia_tri_doc=analogRead(do_am_dat); 
  do_am=map(gia_tri_doc,0,4095,0,100);
  lcd.setCursor(0,0);
  lcd.print(nhietdo);
  lcd.print((char)223);
  lcd.setCursor(8,0);
  lcd.print(do_am);
  lcd.print("%");
  h = timeClient.getHours();
  m = timeClient.getMinutes();
  s = timeClient.getSeconds(); 
  lcd.setCursor(0,1);
  lcd.print("    ");
  if(h>=0&&h<=9)
  {
    lcd.setCursor(4,1);
    lcd.print("0");
    lcd.setCursor(5,1);
    lcd.print(h);
  }
  else
  {
    lcd.setCursor(4,1);
    lcd.print(h);
  }
  lcd.setCursor(6,1); 
  lcd.print(":");
  if(m>=0&&m<=9)
  {
    lcd.setCursor(7,1);
    lcd.print("0");
    lcd.setCursor(8,1);
    lcd.print(m);
  }
  else
  {
    lcd.setCursor(7,1);
    lcd.print(m);
  } 
  lcd.setCursor(9,1); 
  lcd.print(":");
  if(s>=0&&s<=9)
  {
    lcd.setCursor(10,1);
    lcd.print("0");
    lcd.setCursor(11,1);
    lcd.print(s);
  }
  else
  {
    lcd.setCursor(10,1); 
    lcd.print(s);
  }
  lcd.setCursor(12,1);
  lcd.print("    ");
}
void check()
{
  gia_tri_doc_1=analogRead(luu_luong_nuoc);
  the_tich_1=map(gia_tri_doc_1,0,4095,0,100);
  gia_tri_doc_2=analogRead(luu_luong_nuoc_1);
  the_tich=map(gia_tri_doc_2,0,4095,0,10000);
  if(the_tich<=4000)
  {
    Van_Bom="ON";
    digitalWrite(Van, HIGH);
  }
  if(the_tich>=9000)
  {
    Van_Bom="OFF";
    digitalWrite(Van, LOW);
  }
  if(the_tich_1==0)
  {
    if(cho_phep == 1)
    {
      Van_Bom="OFF";
      Van_Tuoi="OFF";
      send_google_sheets();
      cho_phep=0;
    }
    Van_Bom="OFF";
    Van_Bom="OFF";
    digitalWrite(LED, LOW);
    digitalWrite(Van, LOW);
    trangthai=1;
    button=0;
    Blynk.virtualWrite(V1,0);
  }
  tght=millis();
  if(tght-tgqk>=20000)
  {
    tgqk=tght;
    send_google_sheets();
  }
  // Serial.print(the_tich_1);
  // Serial.print(" ");
  // Serial.println(the_tich);
}
void che_do_1()
{
  if(button == 0) 
    {
      digitalWrite(LED, LOW);
      digitalWrite(Van, LOW);
      if(cho_phep == 1)
      {
        Van_Bom="OFF";
        Van_Tuoi="OFF";
        send_google_sheets();
        cho_phep=0;
      }
    }
    else 
    { 
      digitalWrite(LED, HIGH);
      cho_phep=1;
      Van_Tuoi="ON";
      gia_tri_doc=analogRead(do_am_dat); 
      do_am=map(gia_tri_doc,0,4095,0,100);
      Blynk.virtualWrite(V8,do_am);
      check();
    }
    bam1 = digitalRead(bam);
    if(bam1 == 0)
    {
      if(trangthai == 1)
      { 
        Van_Tuoi="ON";
        cho_phep=1;
        digitalWrite(LED, HIGH);
        trangthai=0;
        button=1;
        Blynk.virtualWrite(V1,1);
        check();
      }
      else
      {
        Van_Tuoi="OFF";
        digitalWrite(LED,LOW);
        digitalWrite(Van, LOW);
        trangthai=1;
        button=0;
        Blynk.virtualWrite(V1,0);
        if(cho_phep == 1)
        {
          Van_Bom="OFF";
          Van_Tuoi="OFF";
          send_google_sheets();
          cho_phep=0;
        }
      }
    }
}
void che_do_2()
{
  gia_tri_doc=analogRead(do_am_dat); 
  do_am=map(gia_tri_doc,0,4095,0,100);
  Blynk.virtualWrite(V8,do_am);
  if(button==1)
  {
    if(do_am < nguong_duoi)
    {
      Van_Tuoi="ON";
      cho_phep=1;
      digitalWrite(LED, HIGH);
      count=0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  DANG TUOI CAY ");
      lcd.setCursor(0,1);
      lcd.print("................");
      while(count!=60)
      { 
        if(button==0)
        {
          digitalWrite(LED, LOW);
          digitalWrite(LED_do, LOW);
          digitalWrite(LED_xanh, LOW);
          digitalWrite(LED_vang, LOW);
          if(cho_phep == 1)
          {
            Van_Bom="OFF";
            Van_Tuoi="OFF";
            send_google_sheets();
            cho_phep=0;
          }
          break;
        }
        gia_tri_doc=analogRead(do_am_dat); 
        do_am=map(gia_tri_doc,0,4095,0,100);
        Blynk.virtualWrite(V8,do_am);
        check();
        count++;
        if(do_am < nguong_duoi)
        {
          // Serial.print("Trường hợp 1: ");
          digitalWrite(LED_do, HIGH);
          digitalWrite(LED_xanh, LOW);
          digitalWrite(LED_vang, LOW);
        }
        else if(nguong_duoi <= do_am && do_am <= nguong_tren)
        {
          // Serial.print("Trường hợp 2: ");
          digitalWrite(LED_do, LOW);
          digitalWrite(LED_xanh, HIGH);
          digitalWrite(LED_vang, LOW);
        }
        else if( nguong_tren < do_am)
        {
          // Serial.print("Trường hợp 3: ");
          digitalWrite(LED, LOW);
          digitalWrite(LED_do, LOW);
          digitalWrite(LED_xanh, LOW);
          digitalWrite(LED_vang, HIGH);
          if(cho_phep == 1)
          {
            Van_Bom="OFF";
            Van_Tuoi="OFF";
            send_google_sheets();
            cho_phep=0;
          }
          delay(10000);
          break;
        }
        // Serial.print(che_do);
        // Serial.print(" ");
        // Serial.print(nguong_tren);
        // Serial.print(" ");
        // Serial.print(nguong_duoi);
        // Serial.print(" ");
        // Serial.print(count);
        // Serial.print(" ");
        // Serial.println(do_am);
        delay(1000);
      }
      if(cho_phep == 1)
      {
        Van_Bom="OFF";
        Van_Tuoi="OFF";
        send_google_sheets();
        cho_phep=0;
      }
      digitalWrite(LED, LOW);
      digitalWrite(Van, LOW);
      digitalWrite(LED_do, LOW);
      digitalWrite(LED_xanh, LOW);
      digitalWrite(LED_vang, LOW);
      lcd.clear();
    }
  }
}
void che_do_3()
{ 
  t_on=time_on*60000;
  t_off=time_off*60000;
  if(button==1)
  { 
    thoi_gian=millis();
    if(trangthai_1==1)
    { 
      cho_phep=1;
      Van_Tuoi="ON";
      digitalWrite(LED, HIGH);
      gia_tri_doc=analogRead(do_am_dat); 
      do_am=map(gia_tri_doc,0,4095,0,100);
      Blynk.virtualWrite(V8,do_am);
      check();
      if(thoi_gian-hien_tai>=t_on)
      {
        trangthai_1=0;
        hien_tai=millis();
        digitalWrite(LED, LOW);
        digitalWrite(Van, LOW);
        if(cho_phep == 1)
        {
          Van_Bom="OFF";
          Van_Tuoi="OFF";
          send_google_sheets();
          cho_phep=0;
        }
      }
    }
    else
    {
      if(thoi_gian-hien_tai>=t_off)
      {
        hien_tai=millis();
        trangthai_1=1;
        digitalWrite(LED, HIGH); 
      }
    }
    // Serial.print(thoi_gian);
    // Serial.print(" ");
    // Serial.println(hien_tai);
  }
  else
  {
    digitalWrite(LED, LOW);
    digitalWrite(Van, LOW);
    if(cho_phep == 1)
    {
      Van_Bom="OFF";
      Van_Tuoi="OFF";
      send_google_sheets();
      cho_phep=0;
    }
  }
}
void send_google_sheets()
{
  String Send_data_url= Web_app_url+"?tt=write";
  Send_data_url =  Send_data_url + "&sts="+ Status1;
  Send_data_url = Send_data_url + "&tmp="+ String(nhietdo);
  Send_data_url = Send_data_url +"&humd="+ String(do_am);
  Send_data_url = Send_data_url +"&vt="+ Van_Tuoi;
  Send_data_url = Send_data_url + "&vb="+ Van_Bom;
  // Serial.print("URL : ");
  // Serial.println(Send_data_url);
  HTTPClient http;
  http.begin(Send_data_url.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET(); 
  // Serial.print("HTTP Status Code : ");
  // Serial.println(httpCode);
  String payload;
  if (httpCode > 0) 
  {
    payload = http.getString();
    // Serial.println("Payload : " + payload);    
  }
  http.end();
}
void loop()
{
  Blynk.run();
  man_hinh_chinh();
  if(che_do == 1)
  {
    che_do_1();
  }
  else if(che_do == 2)
  {
    che_do_2();
  }
  else if(che_do == 3)
  {
    che_do_3();
  }
}