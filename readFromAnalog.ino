#define PIN_IN A0 //Input pin

// Use this code to read analog input and print out the read value

void setup() {

  Serial.begin(9600); // Setup Serial communication

  //Set pinmode of Input pins

  pinMode(PIN_IN, INPUT);

}

void loop() {

  int input = analogRead(PIN_IN);

  Serial.println(input);
  delay(50);
  
}
