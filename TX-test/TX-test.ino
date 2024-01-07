#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

const int led_pin = 5;
const int BUTTON_PIN = 6;
int buttonState = 0;
int state = 0;


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

//void loop() {
//  if(state == 0){
//          const char text[] = "0";
//          state = 1;
//          radio.write(&text, sizeof(text));
//          delay(1000);
//        }else{
//          const char text[] = "1";
//          state = 0;
//          radio.write(&text, sizeof(text));
//          delay(1000);
//        }
//    
//}

// testing

void loop(){
  const char text[] = "1";
  radio.write(&text, sizeof(text));
  delay(100);
}
