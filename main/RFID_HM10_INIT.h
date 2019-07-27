#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

#define RST_PIN         49           // Configurable, see typical pin layout above
#define SS_PIN          53          // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); //RFID 라이브러리

/* 등록된 RF CARD ID */
#define CARD_1 0xE9
#define CARD_2 0x59
#define CARD_3 0x79

SoftwareSerial HM10(40, 41); // RX, TX

