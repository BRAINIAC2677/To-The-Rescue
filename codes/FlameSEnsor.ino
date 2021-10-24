#define FLAME 15
#define ALARM 2
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Flame sensor test");
  pinMode(FLAME,INPUT);
  pinMode(ALARM,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int fire = digitalRead(FLAME);
  Serial.println(fire);
  if(fire == HIGH){
    digitalWrite(ALARM,HIGH);
    Serial.println("Fire Fire!!");
  }
  else{
    Serial.println("Inside else");
    digitalWrite(ALARM,LOW);
    Serial.println("No Fire");
  }
  delay(200);
  

}
