#include <stdint.h>
#include <LCD.h>
#include <SPI.h>
#include <XPT2046.h>
#include <Touch.h>

TP_BUTTON Abutton(10,10,100,86);//A버튼 객체 생성
TP_BUTTON Bbutton(130,10,100,86);//B버튼 객체 생성
TP_BUTTON Cbutton(10,110,100,86);//C버튼 객체 생성
TP_BUTTON Dbutton(130,110,100,86);//D버튼 객체 생성
TP_BUTTON Ebutton(10,220,100,86);//E버튼 객체 생성
TP_BUTTON Fbutton(130,220,100,86);//F버튼 객체 생성

void setup()
{
  Serial.begin(9600);
  __SD_CS_DISABLE();

  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();

  Tft.lcd_init();// init TFT library
  Tp.tp_init();  // 터치패널 사용설정
  Tp.tp_adjust();


  //A~F버튼 사용설정
  Abutton.init();
  Bbutton.init(); 
  Cbutton.init();
  Dbutton.init(); 
  Ebutton.init();
  Fbutton.init(); 
 
}

void loop()
{
  Tp.update();//터치 입력을 갱신합니다.
  if(Abutton.istouched()){//터치 정보를 기반으로 버튼이 눌렸는지 확인
    Serial.println("A");//버튼이 눌리면 해당버튼의 문자를 프린트 합니다.
    while(Abutton.istouched()){//버튼이 눌리는 동안은 계속 프린트 되지 않도록 합니다.
      Tp.update();
    }
  }
  if(Bbutton.istouched()){//이하 동일
    Serial.println("B");
    while(Bbutton.istouched()){
      Tp.update();
    }
  }
  if(Cbutton.istouched()){
    Serial.println("C");
    while(Cbutton.istouched()){
      Tp.update();
    }
  }
  if(Dbutton.istouched()){
    Serial.println("D");
    while(Dbutton.istouched()){
      Tp.update();
    }
  }
  if(Ebutton.istouched()){
    Serial.println("E");
    while(Ebutton.istouched()){
      Tp.update();
    }
  }
  if(Fbutton.istouched()){
    Serial.println("F");
    while(Fbutton.istouched()){
      Tp.update();
    }
  }
  delay(100);
}
