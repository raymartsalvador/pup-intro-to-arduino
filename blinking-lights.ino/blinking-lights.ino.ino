void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  Serial.println("currently on");
  delay(1000);
  digitalWrite(13, LOW);
  Serial.println("currently off");
  delay(1000);
}
