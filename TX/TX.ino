#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

const int led_pin = 5;
const int BUTTON_PIN = 6;
int buttonState = 0;
int state = 0;

int RX() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();

  unsigned long previousMillis = millis();  // 初始化 previousMillis

  while(millis() - previousMillis < 3000) {  // 非阻塞循環
    if (radio.available()) {
      char text[32] = "";
      radio.read(&text, sizeof(text));

      Serial.println(text);
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
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, 1);
  delay(1000);
  digitalWrite(led_pin, 0);

  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(BUTTON_PIN);  //讀取按鍵的狀態
  digitalWrite(led_pin, 1);
  if (buttonState == LOW) {
    if (state == 0) {
      const char text[] = "0";
      state = 1;
      radio.write(&text, sizeof(text));
      delay(500);
    } else {
      const char text[] = "1";
      state = 0;
      radio.write(&text, sizeof(text));
      delay(500);
    }

  }
  digitalWrite(led_pin, 0);

}
