#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

const int led_pin = 5;
const int BUTTON_PIN = 6;
int buttonState = 0;
int state = 0;

void RX_init() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

void TX_init() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
}

int RX(int duration = 10000) {
  RX_init();

  unsigned long previousMillis = millis();  // 初始化 previousMillis

  while (millis() - previousMillis < duration) { // 非阻塞循環
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

void TX(char charToSend, int duration, int delay_time = 100) {
  TX_init();

  int times = duration / delay_time;
  Serial.print("sending: \"");
  Serial.print(charToSend);
  Serial.print("\" (");
  Serial.print(String(times));
  Serial.print(" times, delay ");
  Serial.print(String(delay_time));
  Serial.println("ms)");
  for (int i = 0; i < times; i++) {
    radio.write(&charToSend, sizeof(charToSend));

    delay(delay_time);
  }
}

void setup() {
  TX_init();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, 1);
  delay(1000);
  digitalWrite(led_pin, 0);

  Serial.begin(9600);
  Serial.println("-----Start TX Service-----");
}

void loop() {
  buttonState = digitalRead(BUTTON_PIN);  //讀取按鍵的狀態
  digitalWrite(led_pin, 1);
  if (buttonState == LOW) {
    if (state == 0) {
      const char text[] = "0";
      state = 1;
      Serial.print("Messege sent: ");
      Serial.println(text);
      TX(text[0], 3000); //send mes


      Serial.println(RX(500)); //receive ACK
      TX_init();
    } else {
      const char text[] = "1";
      state = 0;
      Serial.print("Messege sent: ");
      Serial.println(text);
      TX(text[0], 3000); //send mes
      //radio.write(&text, sizeof(text));


      Serial.println(RX(500));//receive ACK
      TX_init();
    }

  }
  digitalWrite(led_pin, 0);

}
