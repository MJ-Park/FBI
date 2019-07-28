#include "RFID_HM10_INIT.h"
#include "TFT_LCD_INIT.h"

int USER_ARRAY[3] = {9,9,9};
uint8_t BOOKED_USER = 0;
const int MAX_BOOKED_USER = 2;

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

  HM10.begin(9600);
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
      rfid.uid.uidByte[0] == CARD_2 ||
      rfid.uid.uidByte[0] == CARD_3)
    {
      if (rfid.uid.uidByte[0] == CARD_1)
      {
        makeSound();
        card_Detected('1');
        Serial.println("1번 찍힘");
        Serial.print("BOOKED_USER : ");
        Serial.println(BOOKED_USER);
      }
      else if (rfid.uid.uidByte[0] == CARD_2 )
      {
        makeSound();
        card_Detected('2');
        Serial.println("2번 찍힘");
        Serial.print("BOOKED_USER : ");
        Serial.println(BOOKED_USER);
      }
      else if (rfid.uid.uidByte[0] == CARD_3 )
      {
        makeSound();
        card_Detected('3');
        Serial.println("3번 찍힘");
        Serial.print("BOOKED_USER : ");
        Serial.println(BOOKED_USER);
      }
    }
    Serial.print("ARRAY[0] : ");
    Serial.println(USER_ARRAY[0]);
    Serial.print("ARRAY[1] : ");
    Serial.println(USER_ARRAY[0]);
    Serial.print("ARRAY[2] : ");
    Serial.println(USER_ARRAY[0]);
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
  tone(39, 784, 300);
}

/////////////////////// RFID, HM10 함수 //////////////////

void connectHM10(char cardNum) {
  if(cardNum == '1') {
    HM10.write("AT+CONA810871D107A");
    delay(1000);
    HM10.print("\nCard 1 detected\n");
    delay(1000);
    HM10.write("AT+RESET");
    delay(1000);
  }
  else if(cardNum == '2') {
    HM10.write("AT+CONA810871B48C3");
    delay(1000);
    HM10.print("\nCard 2 detected\n");
    delay(1000);
    HM10.write("AT+RESET");
    delay(1000);
  }
  else if (cardNum == '3') {
    HM10.write("AT+CONA810871B488A");
    delay(1000);
    HM10.print("\nCard 3 detected\n");
    delay(1000);
    HM10.write("AT+RESET");
    delay(1000);
  }
}

int isBookedUser(char cardNum) {
  for(int i=0;i<MAX_BOOKED_USER;i++) {
    if(USER_ARRAY[i] == cardNum) return i;  // 이미 예약되어있으면 그자리 
  }
  return 0;   
}

void card_Detected(char cardNum) {
  if(BOOKED_USER == 0) {    // 예약자가 아무도 없는 경우에
    connectHM10(cardNum);
    USER_ARRAY[BOOKED_USER] = cardNum;
    BOOKED_USER++;
  }
  else if (isBookedUser(cardNum) > 0)    // 예약되어있는 사람이 카드를 찍은 경우,
  {                                 
    BOOKED_USER--;
    if(isBookedUser(cardNum) == 1) {  // 첫번째 사람이 운동 끝나고 찍은경우라면
      if(BOOKED_USER > 0) {
        USER_ARRAY[0] = USER_ARRAY[1];
        USER_ARRAY[1] = USER_ARRAY[2];
        USER_ARRAY[2] = ' ';
        connectHM10(USER_ARRAY[0]);
      }
      else {                          // 그 이후의 사람이 찍은 경우라면 취소 (추후 구현)
        
      }
    }
  }
  else                    // 예약자가 아닌 사람이 찍은 경우
  {
    USER_ARRAY[BOOKED_USER] = cardNum;
    BOOKED_USER++;
  }
}
