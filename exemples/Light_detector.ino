void setup() {
  pinMode(8,INPUT);
  pinMode(13,OUTPUT);
  Serial.begin(9600); //initialise serial monitor

}

void loop() {
  int temp=digitalRead(8);      //assign value of LDR sensor to a temporary variable
  Serial.println("Intensity="); //print on serial monitor using ""
  Serial.println(temp);         //display output on serial monitor
  delay(300);
  if(temp==HIGH)               //HIGH means,light got blocked
    digitalWrite(13,HIGH);        //if light is not present,LED on
  else
    digitalWrite(13,LOW);         //if light is present,LED off
}
