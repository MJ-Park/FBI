#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

#define RST_PIN         49           // Configurable, see typical pin layout above
#define SS_PIN          53          // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); //RFID 라이브러리

/* 등록된 RF CARD ID */
#define CARD_1 0xE9
#define CARD_2 0x59
//#define CARD_3 0x79

int BOOKING_STATE = 0;

/*  BOOKING STATE
 *  0 : 예약자 없는 상태       [ ]
 *  1 : 1번 혼자 사용중        [1]
 *  2 : 2번 혼자 사용중        [2]
 *  3 : 1번 2번 순서로 사용중  [1, 2]
 *  4 : 2번 1번 순서로 사용중  [2, 1]
 */
