#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);

const byte addresses[][6] = { "1Node", "2Node" };
int id = 0xff;

#include <Servo.h>  // подключаем библиотеку для серво
Servo myservo_x;
Servo myservo_y;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  radio.startListening(); 

  myservo_x.attach(7);
  myservo_y.attach(6);
  myservo_x.write(40);
  myservo_y.write(40);
}


int x = 40;  // начальное положение двух координат лазера
int y = 40;

void loop() {
  // 1. Ждем входящих данных
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    if (strstr(text, "init") != NULL) {
      Serial.println(("инициализация"));
      //init system (тут должен быть код с инициализацией)
      y=0;myservo_y.write(y);
      delay(200);
      y=80;myservo_y.write(y);
      delay(200);
      x=0;myservo_x.write(x);
      delay(200);
      x=80;myservo_x.write(x);
      delay(200);
      x = 80;
      y = 80;
      myservo_x.write(x);
      myservo_y.write(y);
      delay(200);
      x = 0;
      y = 0;
      myservo_x.write(x);
      myservo_y.write(y);
      delay(200);
      x = 0;
      y = 80;
      myservo_x.write(x);
      myservo_y.write(y);
      delay(200);
      x = 80;
      y = 0;
      myservo_x.write(x);
      myservo_y.write(y);
      delay(200);
      x = 40;
      y = 40;
      myservo_x.write(x);
      myservo_y.write(y);
      // конец инита
      //String reply = "init done " + String(x) + "," + String(y) + " ID:" + String(id);  // текст для передачи где мы передаем положение лазера и идентификационный номер кубсата(демо реальных условий)
      char reply[32];
      snprintf(reply, sizeof(reply), "init done %d,%d ID: %d", x, y, id);  
      radio.stopListening();                                               
      radio.write(&reply, sizeof(reply));                              
      radio.startListening();                                              
      Serial.println(reply);
    }
    if (text[0] == 'x') {
      int newX = atoi(text + 3);
      newX = constrain(newX, 0, 180);
      x = newX;
      myservo_x.write(x);
    }

    if (text[0] == 'y') {
      int newY = atoi(text + 3);
      newY = constrain(newY, 0, 180);
      y = newY;
      myservo_y.write(y);
    }
  }
}