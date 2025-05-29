#define BLYNK_TEMPLATE_ID "TMPL6eNDnypoc"
#define BLYNK_TEMPLATE_NAME "SISTEM PENDETEKSI KEBAKARAN DINI"
#define BLYNK_AUTH_TOKEN "5ERzS-XifTk3R1e7ysBOWGYqV0tdaYs9"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Servo.h> // Gunakan Servo.h biasa untuk ESP8266

#define DHTPIN D4       // Sesuaikan dengan pin ESP8266 (misal: D2, D4, dll)
#define DHTTYPE DHT22

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const int ledMerah = D5;
const int ledHijau = D6;
const int servoPin = D7;
const int BuzzerPin = D3;

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;
WidgetLCD lcd1(V2);

BLYNK_WRITE(V3) {
  int switchValue = param.asInt();
  if (switchValue == 1) {
    digitalWrite(ledMerah, HIGH);
    digitalWrite(ledHijau, LOW);
    tone(BuzzerPin, 250);
    myservo.write(180); 
    lcd1.clear();
    lcd1.print(0, 0, "Ada Api Berbahaya!!");
    Blynk.logEvent("kebakaran", "Ada Api Berbahaya!!!");
  } else {
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledHijau, HIGH);
    noTone(BuzzerPin);
    myservo.write(0); 
    lcd1.clear();
    lcd1.print(0, 0, "Aman Dari Api");
  }
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("Sensor DHT22");
  lcd.setCursor(0, 1);
  lcd.print("Membaca nilai...");

  pinMode(ledMerah, OUTPUT);
  pinMode(ledHijau, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);

  dht.begin();
  myservo.attach(servoPin);
  myservo.write(0);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  Blynk.run();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Kelembaban: ");
  lcd.print(humidity);
  lcd.print(" %");

  if (temperature > 40) {
    digitalWrite(ledMerah, HIGH);
    digitalWrite(ledHijau, LOW);
    tone(BuzzerPin, 250);
    myservo.write(180); 
    lcd1.clear();
    lcd1.print(0, 0, "Ada Api Berbahaya!!");
    Blynk.logEvent("kebakaran", "Ada Api Berbahaya!!!");
  } else {
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledHijau, HIGH);
    noTone(BuzzerPin);
    myservo.write(0); 
    lcd1.clear();
    lcd1.print(0, 0, "Aman Dari Api");
  }

  Blynk.virtualWrite(V1, temperature); 
  Blynk.virtualWrite(V0, humidity);    

  delay(2000);
}
