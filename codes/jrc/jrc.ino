#include <LiquidCrystal.h>
#include <dht.h>

#define analogLow 0
#define analogHigh 1024
#define flameReadPin1 0
#define flameReadPin2 4
#define gasReadPin 33
#define dhtReadPin 32

LiquidCrystal lcd(5,13,14,15,16,17);
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
  delay(1000);
}

void updateDisplay(){
  lcd.setCursor(0,0);
  lcd.print(line0);
  lcd.setCursor(0,1);
  lcd.print(line1);
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
  }
  else{
    line0 = "Status: Normal";
  }

  line1 = String("FS: ") + String("GS: ") + String(DH.temperature) + String("C ") + String(DH.humidity) + "%";
  updateDisplay();
  delay(2000);
  
}
