
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>

// Connect SDA to i2c data  - Analog 4
// Connect SCL to i2c clock - Analog 5

#define __mainLoop loop
//#define __testLoop loop



#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_BMP085 bmp;
int counter;
char buf[32];
float testData[5];
int fragmentX=90, fragmentY=0;
int dCX=112, dCY=27;
int pressureY=49;

void setup()   {                
  Serial.begin(9600);
  Serial.println("Init start");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  if (!bmp.begin()) {
    Serial.println("BMP error");
  }
  display.clearDisplay();
  display.display();
  counter=0;
  testData[0]=0.0;
  testData[1]=22.10;
  testData[2]=-5.57;
  testData[3]=-15.03;
  testData[4]=666.33;
}


void show(Print& out,float temp,int32_t press,int i) {
  out.print(i);out.println(".");
  out.print("Temperature=");
  out.print(temp);
  out.println(" *C");
    
  out.print("Pressure=");
  out.print(press);
  out.println(" Pa");
  
}

void printLargeWhole(int w) {
  int baseY=-2;
  sprintf(buf,"%2d",w);
  String iS=String(buf);
  int textSize=9;
  int width=fragmentX+1;
  int height=calculateHeight(iS,textSize);
  while (width>fragmentX) {
    --textSize;
    width=calculateWidth(iS,textSize);
    height=calculateHeight(iS,textSize);
  }
  display.setTextSize(textSize);
  int y=(pressureY-height-1)/2;
  display.setCursor(fragmentX-2-width,y);
  display.print(buf);
}

void printFragment(int f) {
  display.setTextSize(3);
  display.setCursor(fragmentX,fragmentY); 
  sprintf(buf,"%02d",f);
  display.print(buf);
}

void printDegreeCelsius() {
  display.setCursor(dCX,dCY); 
  display.print("C");
  display.drawCircle(dCX-10,dCY+4,5,WHITE);
}

void showTemperature(float temp) {
  display.setTextColor(WHITE);
  int i=int(temp);
  int f=abs(int((temp-i)*100));
  printLargeWhole(i);
  printFragment(f);
  printDegreeCelsius();
}

int calculateWidth(const String& s,int textSize){
  return s.length()*6*textSize;
}

int calculateHeight(const String& s,int textSize) {
  return textSize*8;
}

void showPressure(int32_t pressure,int count) {
  int textSize=2;
  display.setTextColor(WHITE);
  display.setTextSize(textSize);
  String sPressure="";
  if (abs(count)%10 <5) {
    dtostrf(pressure/100.0,1,2,buf);
    sPressure+=buf;
    sPressure+=" hPa";
  } else {
    dtostrf(pressure/1333.22,1,2,buf);
    sPressure+=buf;
    sPressure+=" Hgcm";
  }
  int w=calculateWidth(sPressure,textSize);
  display.setCursor((128-w)/2,pressureY);
  display.print(sPressure);
}


void __testLoop() {
  display.clearDisplay();
  showPressure(99885,counter);
  showTemperature(testData[counter%5]);
  display.display();
  ++counter;
  delay(1000);    
}

void __mainLoop() {
  int32_t press=bmp.readPressure();
  float temp=bmp.readTemperature();
  show(Serial,temp,press,counter);
  display.clearDisplay();
//  display.drawRect(0,0,127,63,WHITE);
  showPressure(press,counter);
  showTemperature(temp);
  display.display();
  ++counter;
  delay(1000);    
}
  
