uint8_t buf[8] = { 0 }; //Keyboard report buffer

#define PARK 4 //Input pin for park
#define REVERSE 5 //Input pin for reverse
#define NEUTRAL 6 //Input pin for neutral
#define DRIVE 7 //Input pin for drive

#define RESET 8 //Input pin for reset button

// Pins for the Gear specific LEDs
#define PARKLIGHT 9
#define REVERSELIGHT 10
#define NEUTRALLIGHT 11
#define DRIVELIGHT 12

// May need to add more if we are implementing more states

// Depending on how the shifter sends signals, or if we need to map more buttons, may need to declare more input pins

// Scaling factor
int scale = 1;

// Gear states are represented as integers (MAY NEED TO SCALE OR REARRANGE THEM IF WE NEED TO SEND THE SIGNAL FOR LONGER)
int driveState = 4*scale;
int neutralState = 3*scale;
int reverseState = 2*scale;
int parkState = 1*scale;

int resetState = 17;
int resetSim = 0;

int state = parkState; // Target state read from input
int currentState = state; // What the vehicle is currently set to

void setup() {

  Serial.begin(9600); // Setup Serial communication

  //Set pinmode of Input pins

  pinMode(PARK, INPUT);
  pinMode(REVERSE, INPUT);
  pinMode(NEUTRAL, INPUT);
  pinMode(DRIVE, INPUT);
  
  pinMode(RESET, INPUT);

  //Set pinmode of output RGB pins
  
  pinMode(PARKLIGHT, OUTPUT);
  pinMode(REVERSELIGHT, OUTPUT);
  pinMode(NEUTRALLIGHT, OUTPUT);
  pinMode(DRIVELIGHT, OUTPUT);

}

void loop() {

  // Set the target state (THESE HAVE TO BE IN DESCENDING ORDER)
  if(digitalRead(RESET) == HIGH && resetSim == 0) { // If the reset button is pressed
    resetSim = 1;
    state = resetState;
  } else if(digitalRead(DRIVE) == HIGH) { // If the shifter is set to drive
     state = driveState;
  } else if(digitalRead(NEUTRAL) == HIGH) { // If the shifter is set to neutral
     state = neutralState;
  } else if(digitalRead(REVERSE) == HIGH) { // If the shifter is set to reverse
    state = reverseState;
  } else if(digitalRead(PARK) == HIGH) { // If the shifter is set to park
    state = parkState;
  }

  if(digitalRead(RESET) == LOW) {
    resetSim = 0;
  }

  // If the reset button is pressed, reset both the sim and the state variables
  if (state == resetState) {
    buf[2] = 21; // Send an r in keycode representation
    Serial.write(buf, 8);
    releaseKey();
    state = parkState;
    currentState = state;
  }

  // If the current state is a lower gear than the target gear, go up one gear
  if (state > currentState) {
    buf[2] = 27; // Send an x in keycode representation
    Serial.write(buf, 8);
    releaseKey();
    currentState++;
  }

  // If the current state is a higher gear than the target gear, go down a gear
  if (state < currentState) {
    buf[2] = 29; // Send a z in keycode representation
    Serial.write(buf, 8);
    releaseKey();
    currentState--;
  }

  // Light up corresponding ring LED
  if(currentState == parkState) {
    digitalWrite(PARKLIGHT, HIGH);
    digitalWrite(REVERSELIGHT, LOW);
    digitalWrite(NEUTRALLIGHT, LOW);
    digitalWrite(DRIVELIGHT, LOW);
  } else if(currentState == reverseState) {
    digitalWrite(PARKLIGHT, LOW);
    digitalWrite(REVERSELIGHT, HIGH);
    digitalWrite(NEUTRALLIGHT, LOW);
    digitalWrite(DRIVELIGHT, LOW);
  } else if(currentState == neutralState) {
    digitalWrite(PARKLIGHT, LOW);
    digitalWrite(REVERSELIGHT, LOW);
    digitalWrite(NEUTRALLIGHT, HIGH);
    digitalWrite(DRIVELIGHT, LOW);
  } else if(currentState == driveState) {
    digitalWrite(PARKLIGHT, LOW);
    digitalWrite(REVERSELIGHT, LOW);
    digitalWrite(NEUTRALLIGHT, LOW);
    digitalWrite(DRIVELIGHT, HIGH);
  }

}

// Function for Key Release

void releaseKey() {

  buf[0] = 0;

  buf[2] = 0;

  Serial.write(buf, 8); // Send Release key

}
