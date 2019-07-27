#include "TFT_LCD_INIT.h"

void setup(void)
{
  Serial.begin(9600);

  tft.reset();
  uint16_t identifier = tft.readID();
  if(identifier==0x0101)
      identifier=0x9341;
  
  tft.begin(identifier);
  tft.fillScreen(BLACK);
  makeObj();
}

void loop(void) {
  
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

   if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
   {
    p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
   }
   
  if (timerButton.contains(p.x, p.y)) timerButton.press(true); else timerButton.press(false);   
  if (upButton.contains(p.x, p.y)) upButton.press(true); else upButton.press(false); 
  if (downButton.contains(p.x, p.y)) downButton.press(true); else downButton.press(false);
  
  if (timerButton.justReleased()) {
    timerButton.drawButton();
    unsigned long startTime = millis();
    unsigned long presentTime = millis();
    while (presentTime - startTime <= countNum * 1000)
    {
      presentTime = millis();
      drawTimer(presentTime - startTime);
    }
    makeSound();
    delay(1000);
    TIMER_INIT();
  }
  
  if (timerButton.justPressed()) {
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

void makeObj() {
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
  tft.drawRect(TEXT1_X, TEXT1_Y, TEXT_W, TEXT_H, WHITE);
  tft.drawRect(80, 100, 90, 90, WHITE);
  drawFREE();
  TIMER_INIT();
}

void TIMER_INIT() {
  tft.setCursor(TEXT_TIMER_X+20, TEXT_TIMER_Y+20);
  tft.setTextColor(RED,BLACK);
  tft.setTextSize(5);
  tft.print(countNum);
}

void drawFREE() {
  tft.setCursor(TEXT1_X+25, TEXT1_Y+27);
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(3.8);
  tft.print("   FREE   " );
}

void drawUSER(int user) {
  tft.setCursor(TEXT1_X+25, TEXT1_Y+27);
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(3.8);
  tft.print("USER : ");
  tft.print(user);
  if(user < 10) {
    tft.print(" ");
  }
}

void drawTimer(unsigned long inputNum) {
  uint8_t tmpNum = inputNum / 1000;
  if(countNum - tmpNum >= 10) {
    tft.setCursor(TEXT_TIMER_X+20, TEXT_TIMER_Y+20);
    tft.setTextColor(RED,BLACK);
    tft.setTextSize(5);
    tft.print(countNum - tmpNum);
  }
  else {
    tft.setCursor(TEXT_TIMER_X, TEXT_TIMER_Y+20);
    tft.setTextColor(RED,BLACK);
    tft.setTextSize(5);
    tft.print(" ");
    tft.print(countNum - tmpNum);
    tft.print(" ");
    tft.drawRect(80, 100, 90, 90, WHITE);
  }
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

