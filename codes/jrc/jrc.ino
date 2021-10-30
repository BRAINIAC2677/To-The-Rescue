#include <LiquidCrystal.h>
#include <dht.h>
#include <analogWrite.h>
#include <SoftwareSerial.h>

#define analogLow 0
#define analogHigh 1024
#define gasReadLimit 1000

#define flameReadPin1 0
#define flameReadPin2 4
#define gasReadPin 33
#define dhtReadPin 32
#define motor1pin1 2
#define motor2pin1 5
#define motor1speed 34
#define motor2speed 35

#define STATUS 0
#define SWITCHON 1
#define SWITCHOFF 2

//lib instances
LiquidCrystal lcd(18,19,23,15,16,17);
dht DH;
SoftwareSerial mySerial(15, 17); //SIM800L Tx & Rx is connected to Arduino #3 & #2

//global variables
String line0 = "Status: Normal";
String line1 = "F1: 1, F2: 1, 29C, 45%";
int command = -1;

void handleCommand(){
  switch(command){
    case STATUS:
      sendMsg(line0+line1);
      break;
    case SWITCHON:
      motorOn();
      break;
    case SWITCHOFF:
      motorOff();
      break;
  }
}

void sendMsg(String msg){
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+8801633927378\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print(msg); //text content
  updateSerial();
  mySerial.write(26);
}

void sendCall(){
  mySerial.println("AT"); //Once the handshake test is successful, i t will back to OK
  updateSerial();
  
  mySerial.println("ATD+ +8801633927378;"); //  change ZZ with country code and xxxxxxxxxxx with phone number to dial
  updateSerial();
  delay(20000); // wait for 20 seconds...
  mySerial.println("ATH"); //hang up
  updateSerial();
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
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

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  lcd.begin(16,2);
  
  lcd.clear();
  
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
    motorOff();
  }

  if(gasAnalogRead > gasReadLimit){
    
  }
  else{
    
  }

  line1 = String("FS: ") + String("GS: ") + String(DH.temperature) + String("C ") + String(DH.humidity) + "%";
  updateDisplay();
  delay(2000);
  
}
