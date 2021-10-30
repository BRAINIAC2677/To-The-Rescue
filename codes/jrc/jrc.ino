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
LiquidCrystal lcd(18,19,23,13,14,16);
dht DH;
SoftwareSerial mySerial(15, 17); //SIM800L Tx & Rx is connected to Arduino #3 & #2

//global variables
int curDisp = 0;

String line11 = "Status: Normal";
String line12 = "Flame   Gas";
String line13 = "0 0";
String line14 = "Temp   Humidity";
String line15 = "0 0";
String flameOutput = "Safe";
String gasOutput = "Safe";

int command = -1;

void handleCommand(){
  switch(command){
    case STATUS:
      sendMsg(line11+line12+line13+line14+line15);
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
  lcd.clear();
  if(curDisp == 0){
    lcd.setCursor(0,0);
    lcd.print(line11);
  }
  else if(curDisp == 1){
    lcd.setCursor(0,0);
    lcd.print(line12);
    lcd.setCursor(0,1);
    lcd.print(line13);
  }
  else{
    lcd.setCursor(0,0);
    lcd.print(line14);
    lcd.setCursor(0,1);
    lcd.print(line15);
  }
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

  sendMsg("Hola");
  delay(1000);
}



void loop() {
  curDisp = (curDisp + 1)%3;

  //read sensor
  int flameRead1 = digitalRead(flameReadPin1);
  int flameRead2 = digitalRead(flameReadPin2);
  int gasAnalogRead = analogRead(gasReadPin);
  DH.read11(dhtReadPin);
  //end of read sensor

  /*Serial.print("flameRead1: ");
  Serial.println(flameRead1);
  Serial.print("flameRead2: ");
  Serial.println(flameRead2);
  Serial.print("gasAnalogRead: ");
  Serial.println(gasAnalogRead);
  Serial.print("humidity = ");
  Serial.println(DH.humidity);
  Serial.print("temperature: ");
  Serial.print(DH.temperature); 
  Serial.println("C  ");*/

  //flame out
  if(flameRead1 == LOW or flameRead2 == LOW){
    line11 = "Status: Fire!";
    flameOutput = "Fire";
    motorOn();
    sendMsg(line11+line12+line13+line14+line15);
  }
  else{
    line11 = "Status: Normal";
    flameOutput = "Safe";
    motorOff();
  }
  //end of flame out

  //gas leakage
  if(gasAnalogRead > gasReadLimit){
    line11 = "Status: Gas Leak!";
    gasOutput = "Danger";
    motorOn();
    sendMsg(line11+line12+line13+line14+line15);
  }
  else{
    line11 = "Status: Normal";
    gasOutput = "Safe";
    motorOff();
  }
  //end of gas leakage
  
  //display update
  line13 = flameOutput + String("    ")+String(gasOutput);
  line15 =  String(DH.temperature) + String("C ") + String(DH.humidity) + "%";
  updateDisplay();
  
  delay(1500);
  
}
