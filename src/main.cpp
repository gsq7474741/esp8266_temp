#include <Arduino.h>
//#include <U8g2lib.h>
#include <OneWire.h>
#include "string.h"
//#include <DallasTemperature.h>
#include "Wire.h"
#include "SPI.h"
#include "../.pio/libdeps/esp01_1m/U8g2/src/clib/u8g2.h"
#include "../.pio/libdeps/esp01_1m/U8g2/src/U8g2lib.h"
#include "../.pio/libdeps/esp01_1m/DallasTemperature/DallasTemperature.h"

#include "nodemcu_pin_map.h"

#define SWITCH_PIN D6
#define LED_PIN D2

#define SCLK D5 // EN
#define SID D7  // RW
#define CS D8   // RS
#define RS CS

#define ONE_WIRE_BUS_1 D0
#define ONE_WIRE_BUS_2 D1
#define ONE_WIRE_BUS_3 D3
#define ONE_WIRE_BUS_4 D4


//U8G2_ST7565_LX12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 13, /* cs=*/ 15, /* dc=*/ 12, /* reset=*/ 16);
//U8G2_ST7565_LX12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

// Feather HUZZAH ESP8266, E=clock=14, RW=data=13, RS=CS
//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 13, /* CS=*/ 15, /* reset=*/ 16);
// Feather HUZZAH ESP8266, E=clock=14, RW=data=13, RS=CS
U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ CS, /* reset=*/ U8X8_PIN_NONE);


OneWire oneWire_1(ONE_WIRE_BUS_1);
OneWire oneWire_2(ONE_WIRE_BUS_2);
//OneWire oneWire_3(ONE_WIRE_BUS_3);
//OneWire oneWire_4(ONE_WIRE_BUS_4);
DallasTemperature sensor_1(&oneWire_1);
DallasTemperature sensor_2(&oneWire_2);
//DallasTemperature sensor_3(&oneWire_3);
//DallasTemperature sensor_4(&oneWire_4);
//OneWire ds(ONE_WIRE_BUS);  // on pin 10 (a 4.7K resistor is necessary)

OneWire ds18x20[] = {ONE_WIRE_BUS_1, ONE_WIRE_BUS_2, /*ONE_WIRE_BUS_3, ONE_WIRE_BUS_4*/ };
const int oneWireCount = sizeof(ds18x20) / sizeof(OneWire);
DallasTemperature sensor[oneWireCount];

const String sensorName[] = {"sensor_1", "sensor_2", /*"sensor_3", "sensor_4"*/ };

bool powerOn = false;

void setup() {
    pinMode(SWITCH_PIN, INPUT);
//    bool powerOn = false;

    u8g2.begin();            //显示器初始化
    u8g2.enableUTF8Print();  //支持中文
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);  //设置字体


}

void loop() {
    auto key = digitalRead(SWITCH_PIN);

    if (key == LOW) //按键按下
    {
        if (!powerOn) {
            powerOn = true;
            digitalWrite(LED_PIN, HIGH); //将LED脚输出高电平，点亮LED灯

            Serial.begin(9600);
            Serial.println("Dallas Temperature Multiple Bus Control Library Simple Demo");
            Serial.print("============Ready with ");
            Serial.print(oneWireCount);
            Serial.println(" Sensors================");

            // Start up the library on all defined bus-wires
            DeviceAddress deviceAddress;
            for (int i = 0; i < oneWireCount; i++) {
                sensor[i].setOneWire(&ds18x20[i]);
                sensor[i].begin();
                if (sensor[i].getAddress(deviceAddress, 0))
                    sensor[i].setResolution(deviceAddress, 12);
            }

        } else {
            powerOn = false;
            digitalWrite(LED_PIN, LOW); //将LED脚输出低电平，熄灭LED灯
        }
        delay(1000); //延时1秒，防止按键抖动
    } else {
        if (powerOn) {
            u8g2.firstPage();
            do {
                Serial.print("Requesting temperatures...");
                for (int i = 0; i < oneWireCount; i++) {
                    sensor[i].requestTemperatures();
                }
                Serial.println("DONE");

                delay(1000);
                for (int i = 0; i < oneWireCount; i++) {
                    float temperature = sensor[i].getTempCByIndex(0);
                    Serial.print("Temperature for the sensor ");
                    Serial.print(i);
                    Serial.print(" is ");
                    Serial.println(temperature);

                    u8g2.setCursor(i, 0);
                    u8g2.print(sensorName[i]);
                    u8g2.print(temperature);
                }
                Serial.println();

//                u8g2.setCursor(0, 15);
//                u8g2.print("Hello World!");
//                u8g2.setCursor(0, 40);
//                u8g2.print("你好世界");


            } while (u8g2.nextPage());
            delay(500);
        }
    }
}