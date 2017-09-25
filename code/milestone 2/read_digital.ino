void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  int lineOR = digitalRead(2); //line sensor, out right
  int lineOL = digitalRead(3); //line sensor, out left
  int wallL = digitalRead(4); //wall sensor left
  int wallR = digitalRead(5); //wall sensor right
  
  int front = analogRead(A5);


  Serial.print("line OR value ");
  Serial.print(lineOR);
  Serial.print("          ");
  Serial.print("line OL value ");
  Serial.print(lineOL);
  Serial.print("          ");
  Serial.print("wall L value ");
  Serial.print(wallL);
  Serial.print("          ");
  Serial.print("wall R value ");
  Serial.println(wallR);
  //Serial.print("wall R value ");
  //Serial.println(front);
}
