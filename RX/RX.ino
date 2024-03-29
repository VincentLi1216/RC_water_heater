#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

const int blue_led_pin = 7;
const int buzzer_pin = 6;

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


void setup() {
  RX_init();

  pinMode(blue_led_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(blue_led_pin, 0);
  digitalWrite(buzzer_pin, 0);

  Serial.begin(9600);
  Serial.println("-----Start RX Service-----");
  //  Serial.println(String(RX()));
}



void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));

    Serial.println(text);

    if (strcmp(text, "0") == 0) {
      Serial.println("Off");
      digitalWrite(buzzer_pin, 1);
      delay(200);
      digitalWrite(buzzer_pin, 0);
      digitalWrite(blue_led_pin, 0);
      const char text[] = "2";
      TX(text[0], 2000); //send ACK
      RX_init();
    }

    if (strcmp(text, "1") == 0) {
      Serial.println("On");
      digitalWrite(buzzer_pin, 1);
      delay(200);
      digitalWrite(buzzer_pin, 0);
      digitalWrite(blue_led_pin, 1);
      const char text[] = "3";
      TX(text[0], 2000); //send ACK
      RX_init();
    }


  }
}
