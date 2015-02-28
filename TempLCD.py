#!/usr/bin/python

import Adafruit_BMP.BMP085 as BMP085
import time
import Adafruit_GPIO.SPI as SPI
import Adafruit_SSD1306
import Image
import ImageFont
import ImageDraw



sensor = BMP085.BMP085()
display = Adafruit_SSD1306.SSD1306_128_64(rst=24)

fragmentX=90
fragmentY=-1
dCX=112
dCY=27
pressureY=48

def showTemperature(value):
  whole=int(value)
  text="{0:2d}".format(whole)
  for font in fonts[::-1]:
    (w,h)=draw.textsize(text,font=font)
    if w<fragmentX:
      y=(pressureY-h)/2
      x=fragmentX-2-w
      draw.text((x,y),text,font=font,fill=255)
      break
  text="{0:2d}".format(abs(int((value-whole)*100)))
  draw.text((fragmentX,fragmentY),text,font=fonts[1],fill=255)
  draw.text((fragmentX,dCY),unichr(176)+"C",font=fonts[1],fill=255)

def showPressure(value,iteration):
 if (abs(iteration % 10) < 5):
   text="{0:0.2f} hPa".format(value/100.0) 
 else:
   text="{0:0.2f} Hgmm".format(value//1333.22)
 w,h=draw.textsize(text,font=fonts[0])
 draw.text(((width-w)/2,pressureY),text,font=fonts[0],fill=255)

display.begin()
width = display.width
height = display.height
image = Image.new('1', (width, height))
fonts = [ImageFont.truetype("arial.ttf",i*8) for i in range(2,9)]
draw = ImageDraw.Draw(image)
iteration=0

while True:
  draw.rectangle((0,0,width,height), outline=0, fill=0)
  display.clear()
  showPressure(sensor.read_pressure(),iteration)
  showTemperature(sensor.read_temperature())
  display.image(image)
  display.display()
  time.sleep(1)
  iteration+=1


