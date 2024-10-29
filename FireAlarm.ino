#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// const String PHONE_1 = "+919600834966";
// const String PHONE_2 = "";  //optional
// const String PHONE_3 = "";  //optional

#define rxPin 2
#define txPin 3
SoftwareSerial sim800L(rxPin, txPin);

int gas_read=0;

const int THRESHOLD_PPM = 250;
boolean fire_flag = false;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define gas A0
#define Sensor 8
#define Buzzer 10
#define LED 6

void setup() {
  pinMode(Sensor ,INPUT);
  pinMode(gas ,INPUT);
  pinMode(LED , OUTPUT);
  pinMode(Buzzer, OUTPUT);
  Serial.begin(115200);
  sim800L.begin(9600);

  lcd.init();
  lcd.backlight();
  Serial.println("Initializing...");
  sim800L.println("AT");
  delay(1000);
  sim800L.println("AT+CMGF=1");
  delay(1000);
}

void send_sms(String text) {
  Serial.println("sending sms....");
  delay(50);
  sim800L.print("AT+CMGF=1\r");
  delay(1000);
  sim800L.print("AT+CMGS=\+917598066712\"\r");
  delay(1000);
  sim800L.print(text);
  delay(100);
  sim800L.write(0x1A);
  delay(5000);
  Serial.println("Message sent..");  
}

void make_call() {
  Serial.println("calling....");
  sim800L.println("ATD+917598066712;");
  delay(20000);
  sim800L.println("ATH");
  delay(1000);
}


void loop() {
  while (sim800L.available()) {
    Serial.println(sim800L.readString());
  }

  Serial.print("PPM: ");

  gas_read = analogRead(gas);
  Serial.println(gas_read);
  delay(300);
  if( gas_read >= 300)
  {
    digitalWrite(LED, HIGH);
    digitalWrite(Buzzer, HIGH);  
    delay(1000);
    send_sms("Gas Leackage Detected!!....");  
    make_call();
  }
  else{
    digitalWrite(LED, LOW);
    digitalWrite(Buzzer, LOW);
  }

  bool value = digitalRead(Sensor);
    Serial.println(value);

  Serial.println(value);
  delay(1000);

  if (value == 0) {
    digitalWrite(LED, HIGH);
    digitalWrite(Buzzer, HIGH);
    delay(1000);
    if (fire_flag == 0) {
      Serial.println("Fire Detected.");
      fire_flag == 1;
      send_sms("Fire.....!!");
      make_call();
    }
  } else {
    digitalWrite(LED, LOW);
    digitalWrite(Buzzer, LOW);
    fire_flag = 0;
  }
}

// void send_multi_sms() {
//   if (PHONE_1 != "") {
//     Serial.print("Phone 1: ");
//     send_sms("Fire is Detected", PHONE_1);
//   }
//   if (PHONE_2 != "") {
//     Serial.print("Phone 2: ");
//     send_sms("Fire is Detected", PHONE_2);
//   }
//   if (PHONE_3 != "") {
//     Serial.print("Phone 3: ");
//     send_sms("Fire is Detected", PHONE_3);
//   }
// }

// void make_multi_call() {
//   if (PHONE_1 != "") {
//     Serial.print("Phone 1: ");
//     make_call(PHONE_1);
//   }
//   if (PHONE_2 != "") {
//     Serial.print("Phone 2: ");
//     make_call(PHONE_2);
//   }
//   if (PHONE_3 != "") {
//     Serial.print("Phone 3: ");
//     make_call(PHONE_3);
//   }
// }

// void send_sms(String text, String phone) {
//   Serial.println("sending sms....");
//   delay(50);
//   sim800L.print("AT+CMGF=1\r");
//   delay(1000);
//   sim800L.print("AT+CMGS=\"" + phone + "\"\r");
//   delay(1000);
//   sim800L.print(text);
//   delay(100);
//   sim800L.write(0x1A);
//   delay(5000);
// }


int analogToPPM(int analogValue) {
  return analogValue;
}