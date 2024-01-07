#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

const int blue_led_pin = 7;
const int buzzer_pin = 6;

int RX() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  unsigned long previousMillis = millis();  // 初始化 previousMillis

  while (millis() - previousMillis < 10000) { // 非阻塞循環
    if (radio.available()) {
      char text[32] = "";
      radio.read(&text, sizeof(text));

      //Serial.println(text);
      if (strcmp(text, "1") == 0) {
        return 1;
      }

      if (strcmp(text, "0") == 0) {
        return 0;
      }
    }
  }

  return -1;
}

void setup() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  pinMode(blue_led_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(blue_led_pin, 0);
  digitalWrite(buzzer_pin, 0);

  Serial.begin(9600);
  Serial.println("start!!!");
  Serial.println(String(RX()));
}



void loop() {
  //    if (radio.available()) {
  //        char text[32] = "";
  //        radio.read(&text, sizeof(text));
  //
  //        Serial.println(text);
  //        if (strcmp(text, "1") == 0){
  //          Serial.println("On");
  //          digitalWrite(buzzer_pin, 1);
  //          delay(200);
  //          digitalWrite(buzzer_pin, 0);
  //          digitalWrite(blue_led_pin,1);
  //        }
  //
  //        if (strcmp(text, "0") == 0){
  //          Serial.println("Off");
  //          digitalWrite(buzzer_pin, 1);
  //          delay(200);
  //          digitalWrite(buzzer_pin, 0);
  //          digitalWrite(blue_led_pin,0);
  //        }
  //    }
}
