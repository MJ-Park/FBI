#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <TouchScreen.h>

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

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

Adafruit_GFX_Button timerButton;
Adafruit_GFX_Button upButton;
Adafruit_GFX_Button downButton;

char textfield[3] = "";
uint8_t textfield_i = 0;
uint8_t countNum = 60;

////////////////////////  SETUP /////////////////////                  
void setup(void)
{
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();
  if(identifier==0x0101)
      identifier=0x9341;
  
  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.4\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);
  //tft.setRotation(2);
  tft.fillScreen(BLACK);
  
  ///////////// CREATE TIMER BUTTON ////////////////
  char timerButton_Label[6] = "TIMER";
  char upButton_Label[2] = "+";
  char downButton_label[2] = "-";
  
  #define timberButton_TEXTSIZE 2
  #define timerButton_X 125
  #define timerButton_Y 240
  #define timerButton_W 70
  #define timerButton_H 70

  timerButton.initButton(&tft, timerButton_X, timerButton_Y, 
                          timerButton_W, timerButton_H, WHITE, RED, WHITE,
                          timerButton_Label, timberButton_TEXTSIZE);
  upButton.initButton(&tft, 200, 240, 
                          50, 50, WHITE, 0xC618, WHITE,
                          upButton_Label, 3);
  downButton.initButton(&tft, 50, 240, 
                          50, 50, WHITE, 0xC618, WHITE,
                          downButton_label, 3);
  timerButton.drawButton();
  downButton.drawButton();
  upButton.drawButton();
  
  ///////////// CREATE TEXT FIELDS /////////////////
  #define TEXT1_X 10
  #define TEXT1_Y 10
  #define TEXT_W 220
  #define TEXT_H 80
  #define TEXT_TSIZE 2
  #define TEXT_TIMER_X 80
  #define TEXT_TIMER_Y 110
  #define TEXT_TIMER_W 80
  #define TEXT_TIMER_H 80
  
  tft.drawRect(TEXT1_X, TEXT1_Y, TEXT_W, TEXT_H, WHITE);
  tft.drawRect(80, 100, 90, 90, WHITE);
  tft.setCursor(TEXT_TIMER_X+20, TEXT_TIMER_Y+20);
  tft.setTextColor(RED,BLACK);
  tft.setTextSize(5);
  tft.print(countNum);  tft.setCursor(TEXT_TIMER_X+20, TEXT_TIMER_Y+20);
  tft.setTextColor(RED,BLACK);
  tft.setTextSize(5);
  tft.print(countNum);
}


////////////////////////////////// SETUP END ///////////////////////////////////


// Print something in the mini status bar with either flashstring
void status(const __FlashStringHelper *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}
// or charstring
void status(char *msg) {
  tft.fillRect(STATUS_X, STATUS_Y, 240, 8, BLACK);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.print(msg);
}

void timerStart() {
  for( int i = countNum ; i >=0 ; i-- )
  {
//    long prev = millis();
    tft.setCursor(TEXT_TIMER_X+20, TEXT_TIMER_Y+20);
    tft.setTextColor(RED,BLACK);
    tft.setTextSize(5);
    tft.print(i);
    delay(1000);
  }
  makeSound();
  
  delay(3000);
  tft.setCursor(TEXT_TIMER_X+20, TEXT_TIMER_Y+20);
  tft.setTextColor(RED,BLACK);
  tft.setTextSize(5);
  tft.print(countNum);
  
}

void timerUP() {
  countNum += 15;
  tft.setCursor(TEXT_TIMER_X+20, TEXT_TIMER_Y+20);
  tft.setTextColor(RED, BLACK);
  tft.setTextSize(5);
  tft.print(countNum);
}

void timerDOWN() {
  if (countNum > 15) {
    countNum -= 15;
    tft.setCursor(TEXT_TIMER_X+20, TEXT_TIMER_Y+20);
    tft.setTextColor(RED, BLACK);
    tft.setTextSize(5);
    tft.print(countNum);
  }
}

void makeSound() {
  
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop(void) {
  
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

   if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
   {
    // scale from 0->1023 to tft.width
    p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
   }
   
  // check if they were pressed or released
  if (timerButton.contains(p.x, p.y))
  {
    timerButton.press(true);
  }  else  {
    timerButton.press(false);  }
    
  if (upButton.contains(p.x, p.y))
  {
    upButton.press(true);
  } else { upButton.press(false); }

  if (downButton.contains(p.x, p.y))
  {
    downButton.press(true);
  } else { downButton.press(false);}

  if (timerButton.justReleased()) {
    timerButton.drawButton();  // draw normal
    makeSound();
  }
  
  if (timerButton.justPressed()) {
      timerStart();
      timerButton.drawButton(true);  // draw invert!
    delay(100); // UI debouncing
  }
  if (upButton.justPressed()) {
    timerUP();
  }
  if (downButton.justPressed()) {
    timerDOWN();
  }
}
