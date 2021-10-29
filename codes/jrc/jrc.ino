#include <LiquidCrystal.h>
#include <dht.h>
#include <analogWrite.h>

#define analogLow 0
#define analogHigh 1024
#define flameReadPin1 0
#define flameReadPin2 4
#define gasReadPin 33
#define dhtReadPin 32
#define motor1pin1 2
#define motor2pin1 5
#define motor1speed 34
#define motor2speed 35

LiquidCrystal lcd(18,19,23,15,16,17);
dht DH;

String line0 = "Status: Normal";
String line1 = "F1: 1, F2: 1, 29C, 45%";

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  
  Serial.begin(9600);
  Serial.println("Serial Monitor Started.");
  pinMode(flameReadPin1,INPUT);
  pinMode(flameReadPin2,INPUT);
  pinMode(gasReadPin, INPUT);
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor1speed, OUTPUT);
  pinMode(motor2speed, OUTPUT);
  delay(1000);
}

void updateDisplay(){
  lcd.setCursor(0,0);
  lcd.print(line0);
  lcd.setCursor(0,1);
  lcd.print(line1);
}

void motorOn(){
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor2pin1, HIGH);
  analogWrite(motor1speed, 255);
  analogWrite(motor2speed, 255);
}

void motorOff(){
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor2pin1, LOW);
  analogWrite(motor1speed, 0);
  analogWrite(motor2speed, 0);
}

void loop() {
  int flameRead1 = digitalRead(flameReadPin1);
  int flameRead2 = digitalRead(flameReadPin2);
  int gasAnalogRead = analogRead(gasReadPin);
  DH.read11(dhtReadPin);


  Serial.print("flameRead1: ");
  Serial.println(flameRead1);
  Serial.print("flameRead2: ");
  Serial.println(flameRead2);
  Serial.print("gasAnalogRead: ");
  Serial.println(gasAnalogRead);
  Serial.print("humidity = ");
  Serial.println(DH.humidity);
  Serial.print("temperature: ");
  Serial.print(DH.temperature); 
  Serial.println("C  ");

  if(flameRead1 == LOW or flameRead2 == LOW){
    line0 = "Status: Fire!";
    motorOn();
  }
  else{
    line0 = "Status: Normal";
    motorOn();
  }

  line1 = String("FS: ") + String("GS: ") + String(DH.temperature) + String("C ") + String(DH.humidity) + "%";
  updateDisplay();
  delay(2000);
  
}
