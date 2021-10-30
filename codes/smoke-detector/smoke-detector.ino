
int smokePin = 26;
int smokeDigPin = 15;
// Your threshold value
int smokeLimit = 50;

int detectorPin = 2;

void setup() {
  pinMode(smokePin, INPUT);
  //pinMode(smokeDigPin, INPUT);
  //pinMode(detectorPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int smokeRead = analogRead(smokePin);
  //int smokeDigRead = digitalRead(smokeDigPin);

  Serial.print("Pin A0: ");
  Serial.println(smokeRead);
  Serial.print("Pin D0: ");
  //Serial.println(smokeDigRead);

  if(smokeRead > smokeLimit)
  {
    //digitalWrite(detectorPin, HIGH);
    Serial.print("Yesss");
    //alerting code here
  }
  else
  {
    //digitalWrite(detectorPin, LOW);
    Serial.print("NOOO");
  }
  delay(1000);
}
