#include <AltSoftSerial.h>

AltSoftSerial altSerial;

void setup() {
  altSerial.begin(9600);
  pinMode(6,OUTPUT);
  digitalWrite(6,LOW);
}

void loop() {
  char c;
  if (altSerial.available()) {
    c = altSerial.read();
    digitalWrite(6,HIGH);
    delay(400);
    digitalWrite(6,LOW);
  }
}

