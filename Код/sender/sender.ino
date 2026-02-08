#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);

#define pinX A0
#define pinY A1
#define swknopka 2

const byte addresses[][6] = { "1Node", "2Node" };

void setup() {
  pinMode(pinX, INPUT);
  pinMode(swknopka, INPUT_PULLUP);
  pinMode(pinY, INPUT);

  Serial.begin(9600);
  radio.begin();
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MIN);

  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);

  radio.stopListening(); 
}

int flag = 0;
int X;

void loop() {
  if (flag == 1) {
    radio.stopListening();
    int X = map(analogRead(pinX), 0, 1023, 80, 0);
    int Y = map(analogRead(pinY), 0, 1023, 0, 80);
    char x_text[10]; char y_text[10];
    snprintf(x_text, sizeof(x_text), "x: %d", X);
    snprintf(y_text, sizeof(y_text), "y: %d", Y);
    radio.write(&x_text, sizeof(x_text));
    radio.write(&y_text, sizeof(y_text));
  } else {
    const char text[] = "init";
    if (digitalRead(swknopka) == 0) {
      radio.stopListening();
      char text[] = "init";
      radio.write(&text, sizeof(text));
      Serial.println(("отправка: " + String(text)));
      radio.startListening();

      unsigned long started_waiting_at = millis();
      bool timeout = false;
      while (!radio.available()) {
        if (millis() - started_waiting_at > 2500) {
          timeout = true;
          break;
        }
      }


      if (timeout) {
        Serial.println("Таймаут! Ответ не получен.");
      } else {
        char reply[32] = "";
        radio.read(&reply, sizeof(reply));
        Serial.print("Ответ от приемника: ");
        Serial.println(reply);
        flag = 1;
      }
    }
  }
}