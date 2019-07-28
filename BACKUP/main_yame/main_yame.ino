#include "RFID_HM10_INIT.h"
#include "TFT_LCD_INIT.h"

void setup() {
  Serial.begin(9600);
  pinMode(39,OUTPUT);
  tft.reset();
  uint16_t identifier = tft.readID();
  if(identifier==0x0101)
      identifier=0x9341;
  
  tft.begin(identifier);
  tft.fillScreen(BLACK);
  makeObj();

//  HM10.begin(9600);
  Serial1.begin(9600); // HM10 시작
  SPI.begin(); // SPI 시작
  rfid.PCD_Init(); // RF 모듈 시작
  makeSound();
}

void loop() {
  
if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
{
  //PICC 타입 읽어오기
//  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  //ID가 등록된 ID와 동일한지 체크
  if (rfid.uid.uidByte[0] == CARD_1 ||
      rfid.uid.uidByte[0] == CARD_2   )
    {
      if(rfid.uid.uidByte[0] == CARD_1)      card_Detected(1);
      else                                   card_Detected(2);
    }
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}
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
//    delay(1000);
    waitSecs(1);
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
//  delay(1000);
  waitSecs(1);
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
  tft.setCursor(TEXT_TIMER_X+20, TEXT_TIMER_Y+20);
  tft.setTextColor(RED,BLACK);
  tft.setTextSize(5);
  tft.print(countNum - tmpNum);
  if(countNum - tmpNum < 10)  tft.print(" ");
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
//  tone(39, 784, 300);
  tone(39, 800, 400);
}

/////////////////////// RFID, HM10 함수 //////////////////

void connectHM10(int cardNum) {
  if(cardNum == 1) {
    Serial1.write("AT+CONA810871D107A");
//    delay(1000);
    waitSecs(1);
    Serial1.print("\nCard 1 detected\n");
//    delay(1000);
    waitSecs(1);
    Serial1.write("AT+RESET");
    waitSecs(1);
//    delay(1000);
  }
  else if(cardNum == 2) {
    Serial1.write("AT+CONA810871B48C3");
//    delay(1000);
    waitSecs(1);
    Serial1.print("\nCard 2 detected\n");
//    delay(1000);
    waitSecs(1);
    Serial1.write("AT+RESET");
//    delay(1000);
    waitSecs(1);
  }
}

void card_Detected(int cardNum) {
  makeSound();
  tft.setCursor(TEXT1_X+25, TEXT1_Y+27);
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(3.8);
  tft.print("        ");
  
  tft.setCursor(TEXT1_X+25, TEXT1_Y+30);
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(2.8);
  tft.print("USER ");
  tft.print(cardNum);
  tft.print(" ");
  
  if(cardNum==1) {
    switch(BOOKING_STATE) {
      case 0:             // 아무도 쓰고 있지 않을때 바로 1번 사용
      BOOKING_STATE = 1;   
      tft.print("starts");
      drawUSER(1);
      break;

      case 1:             // 1번 혼자 쓰고 난 후 사용 종료
      BOOKING_STATE = 0;
      tft.print("ends");
      drawFREE();
      break;

      case 2:             // 
      BOOKING_STATE = 4;
      tft.print("booked");
      drawUSER(2);
      break;

      case 3:
      BOOKING_STATE = 2;
      tft.print("ends");
      drawUSER(2);
      connectHM10(2);     // char -> int 바꾸기
//      사용자 : 2번 (HM10 호출)
      break;

      case 4:
      BOOKING_STATE = 2;
      tft.print("cancled");
      drawUSER(2);
//      사용자 : 2번
      break;
    }
  }
  else if(cardNum==2) {
    switch(BOOKING_STATE) {
      case 0:
      BOOKING_STATE = 2;
      tft.print("starts");
      drawUSER(2);
      break;

      case 1:
      BOOKING_STATE = 3;
      tft.print("booked");
      drawUSER(1);
      break;

      case 2:
      BOOKING_STATE = 0;
      tft.print("ends");
      drawFREE();
      break;

      case 3:
      BOOKING_STATE = 1;
      tft.print("cancled");
      drawUSER(1);
      break;

      case 4:
      BOOKING_STATE = 1;
      tft.print("ends");
      drawUSER(1);
      connectHM10(1);
//      사용자 : 1번 (HM10으로 1번호출)
      break;
    }
  }
  else {
    tft.print("cardNum ERROR");
  }
}

waitSecs(float num); {
    unsigned long Ts = millis();    // 시작시간
    unsigned long Tn = millis();    // 현재시간
    while (Tn - Ts <= num * 1000)
    {
      Tn = millis();
    }
}
