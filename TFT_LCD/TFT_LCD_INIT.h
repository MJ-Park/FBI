#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <TouchScreen.h>
//#include <MsTimer2.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define LCD_CS A3   // Chip Select goes to Analog 3
#define LCD_CD A2  // Command/Data goes to Analog 2
#define LCD_WR A1  // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

//Param For 2.4"_9341_V3.0
#define TS_MINX 123
#define TS_MAXX 901

#define TS_MINY 82
#define TS_MAXY 904
// We have a status line for like, is FONA working
#define STATUS_X 10
#define STATUS_Y 65

#define TEXT1_X 10
#define TEXT1_Y 10
#define TEXT_W 220
#define TEXT_H 80
#define TEXT_TSIZE 2
#define TEXT_TIMER_X 80
#define TEXT_TIMER_Y 110
#define TEXT_TIMER_W 80
#define TEXT_TIMER_H 80
#define timberButton_TEXTSIZE 2
#define timerButton_X 125
#define timerButton_Y 240
#define timerButton_W 70
#define timerButton_H 70

#define MINPRESSURE 10
#define MAXPRESSURE 1000

char timerButton_Label[6] = "TIMER";
char upButton_Label[2] = "+";
char downButton_label[2] = "-";

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

Adafruit_GFX_Button timerButton;
Adafruit_GFX_Button upButton;
Adafruit_GFX_Button downButton;

uint8_t countNum = 60;
