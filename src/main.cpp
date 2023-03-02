#include <Arduino.h>
//#include <U8g2lib.h>
#include <OneWire.h>
//#include <DallasTemperature.h>
#include "Wire.h"
#include "SPI.h"
#include "../.pio/libdeps/esp01_1m/U8g2/src/clib/u8g2.h"
#include "../.pio/libdeps/esp01_1m/U8g2/src/U8g2lib.h"
#include "../.pio/libdeps/esp01_1m/DallasTemperature/DallasTemperature.h"

#include "nodemcu_pin_map.h"

#define SCLK D5
#define SID D7
#define CS D8
#define RS CS
#define RST D1
#define ONE_WIRE_BUS_1 2
#define ONE_WIRE_BUS_2 4
#define ONE_WIRE_BUS_3 14
#define ONE_WIRE_BUS_4 12

//U8G2_ST7565_LX12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 13, /* cs=*/ 15, /* dc=*/ 12, /* reset=*/ 16);
//U8G2_ST7565_LX12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

// Feather HUZZAH ESP8266, E=clock=14, RW=data=13, RS=CS
//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 13, /* CS=*/ 15, /* reset=*/ 16);
// Feather HUZZAH ESP8266, E=clock=14, RW=data=13, RS=CS
U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ CS, /* reset=*/ RST);



OneWire oneWire_1(ONE_WIRE_BUS_1);
//OneWire oneWire_2(ONE_WIRE_BUS_2);
//OneWire oneWire_3(ONE_WIRE_BUS_3);
//OneWire oneWire_4(ONE_WIRE_BUS_4);
DallasTemperature sensor_1(&oneWire_1);
//DallasTemperature sensor_2(&oneWire_2);
//DallasTemperature sensor_3(&oneWire_3);
//DallasTemperature sensor_4(&oneWire_4);
//OneWire ds(ONE_WIRE_BUS);  // on pin 10 (a 4.7K resistor is necessary)


void setup() {
    u8g2.begin();            //显示器初始化
    u8g2.enableUTF8Print();  //支持中文
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);  //设置字体

    Serial.begin(9600); //串口初始化
    Serial.println("Dallas Temperature Control Library Demo - TwoPin_DS18B20"); //打印标题

    sensor_1.begin(); //初始化传感器
    sensor_outhouse.begin();    //初始化传感器
}

void loop() {
    u8g2.firstPage();        //此为显示循环, 更加详细的内容可以参照下面给出的链接
    do {
        u8g2.drawStr(0, 20, "MadonG");        //在指定位置显示字符 MadonG
    } while (u8g2.nextPage());
    delay(1000);        //延时1秒

    Serial.print("Requesting temperatures...");
    sensor_1.requestTemperatures();   //请求温度
//    sensor_2.requestTemperatures();  //请求温度
    Serial.println(" done");

    Serial.print("Inhouse: ");
    Serial.println(sensor_1.getTempCByIndex(0));  //打印温度

    Serial.print("Outhouse: ");
    Serial.println(sensor_outhouse.getTempCByIndex(0));   //打印温度

}