#include <SPI.h>
#include <MFRC522.h>
#include <AltSoftSerial.h>

#define SS_PIN 10   //RFID SS(SDA:ChipSelect) PIN
//#define RST_PIN 9   //RFID Reset PIN
#define RST_PIN 7
MFRC522 rfid(SS_PIN, RST_PIN); //RFID 라이브러리

/* 등록된 RF CARD ID */
#define CARD_1 0xE9
#define CARD_2 0x59
#define CARD_3 0x79

AltSoftSerial HM10;
//SoftwareSerial HM10(2, 3); // RX, TX

void setup() { 
  //시리얼 모니터 시작
//  Serial.begin(9600);
  HM10.begin(9600);
  SPI.begin(); // SPI 시작
  rfid.PCD_Init(); // RF 모듈 시작
}
 
void loop() {
  //카드가 인식 안되었다면 더이상 진행하지 말고 빠져나감
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  //RF카드의 ID가 인식 안되었다면 더이상 진행하지 말고 빠져나감
  if ( ! rfid.PICC_ReadCardSerial())
    return;


//  Serial.print(F("PICC type: "));
  //PICC 타입 읽어오기
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
//  Serial.println(rfid.PICC_GetTypeName(piccType));


  //MIFARE 방식이 아니라면 더이상 진행하지 말고 빠져나감
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
//    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }


//ID가 등록된 ID와 동일하다면
if (rfid.uid.uidByte[0] == CARD_1 ||
    rfid.uid.uidByte[0] == CARD_2 ||)
  {
    if (rfid.uid.uidByte[0] == CARD_1)
    {
      HM10.write("AT+CONA810871D107A");
      delay(1000);
      HM10.print("\nCard 1 detected\n");
      delay(1000);
      HM10.write("AT+RESET");
      delay(1000);
    }
    else if (rfid.uid.uidByte[0] == CARD_2 )
    {
      HM10.write("AT+CONA810871B48C3");
      delay(1000);
      HM10.print("\nCard 2 detected\n");
      delay(1000);
      HM10.write("AT+RESET");
      delay(1000);
    }
  }
  
  //종료
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  //다시 시작!
}

