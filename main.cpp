#include <Arduino.h>

#define LCD_CS A3    // Chip Select goes to Analog 3
#define LCD_CD A2    // Command/Data goes to Analog 2
#define LCD_WR A1    // LCD Write goes to Analog 1
#define LCD_RD A0    // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
MCUFRIEND_kbv tft;

#include <TimeLib.h>

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

const char *scedule[] = {"CAR", "SOa", "SOb", "MAT", "COM", "KRa", "EN",
                         "EN",  "HAN", "SOa", "MUb", "PE",  "MAT", "EG",
                         "KRa", "EG", "MAT", "PE",  "SCI", "EN",  "CH",
                         "SCI", "KRa", "EXP", "EXP", "EN",  "COM", "MUa",
                         "HAN", "MUa", "MAT", "KRb", "CH",  "CH",  "CH"};

void setup()
{
    Serial.begin(9600);

    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.cp437(true);
    tft.setRotation(1);

    tft.fillScreen(BLACK);

    // setTime(1664289801);
    setTime(1664291029);
}

void loop()
{
    // tft.fillScreen(BLACK);

    if (Serial.available() > 0 && '0' <= Serial.peek() && Serial.peek() <= '9') // A int
    {
        uint32_t newtime = Serial.parseInt();
        setTime(newtime);
    }

    uint32_t sec = second();
    uint32_t min = minute();
    uint32_t hr = (hour() + 9)%12 /* utc -> korean time conversion */;
    if (hr == 0) hr = 12;
    bool am = (hour() + 9)%24 < 12;

    tft.setCursor(0, 0);
    tft.setTextColor(WHITE, BLACK);
    tft.setTextSize(3);

    tft.print((char)('0' + hr / 10 % 10));
    tft.print((char)('0' + hr % 10));

    tft.print(':');

    tft.print((char)('0' + min / 10 % 10));
    tft.print((char)('0' + min % 10));

    tft.print(':');

    tft.print((char)('0' + sec / 10 % 10));
    tft.print((char)('0' + sec % 10));

    tft.print(' ');

    if (am)
    {
        tft.print('A');
    }
    else
    {
        tft.print('P');
    }
    tft.print('M');

    tft.print(' ');
    tft.print('\n');
    tft.print('\n');

    uint32_t yr = year();
    uint32_t mn = month();
    uint32_t dy = day();
    uint32_t wd = weekday();

    tft.print((char)('0' + yr / 1000 % 10));
    tft.print((char)('0' + yr / 100 % 10));
    tft.print((char)('0' + yr / 10 % 10));
    tft.print((char)('0' + yr % 10));

    tft.print('/');

    tft.print((char)('0' + mn / 10 % 10));
    tft.print((char)('0' + mn % 10));

    tft.print('/');

    tft.print((char)('0' + dy / 10 % 10));
    tft.print((char)('0' + dy % 10));

    tft.print(' ');

    switch (wd)
    {
    case 1: // sun
        tft.print("SUN");
        break;
    case 2:
        tft.print("MON");
        break;
    case 3:
        tft.print("TUE");
        break;
    case 4:
        tft.print("WED");
        break;
    case 5:
        tft.print("THU");
        break;
    case 6:
        tft.print("FRI");
        break;
    case 7:
        tft.print("SAT");
        break;
    }

    int gyosi = 8;
    if ((hr == 8 && min > 50 || hr == 9 && min < 40) && am)
        gyosi = 1;
    else if ((hr == 9 && min > 50 || hr == 10 && min < 40) && am)
        gyosi = 2;
    else if ((hr == 10 && min > 50 || hr == 11 && min < 40) && am)
        gyosi = 3;
    else if (hr == 11 && min > 50 && am || hr == 12 && min < 40 && !am)
        gyosi = 4;
    else if ((hr == 1 && min > 40 || hr == 2 && min < 30) && !am)
        gyosi = 5;
    else if ((hr == 2 && min > 40 || hr == 3 && min < 30) && !am)
        gyosi = 6;
    else if ((hr == 3 && min > 50 || hr == 4 && min < 40) && !am)
        gyosi = 7;

    tft.print('\n');
    tft.print('\n');

    tft.setTextSize(2);

    if (wd != 1 && wd != 7)
    {
        uint32_t d = wd - 2;
        for (int i = 0; i < 7; i++)
        {
            if (gyosi - 1 == i)
            {
                tft.setTextColor(BLACK, WHITE);
                tft.print(scedule[d * 7 + i]);
                tft.setTextColor(WHITE, BLACK);
            }
            else
            {
                tft.print(scedule[d * 7 + i]);
            }
            tft.print(' ');
        }
    }
}
