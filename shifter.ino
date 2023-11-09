uint8_t buf[8] = { 0 }; //Keyboard report buffer

#define PARK 4 //Input pin for park
#define REVERSE 5 //Input pin for reverse
#define NEUTRAL 6 //Input pin for neutral
#define DRIVE 7 //Input pin for drive

#define RESET 8 //Input pin for reset button

// Pins for the RGB LED
#define RED A0
#define GREEN A1
#define BLUE A2

// May need to add more if we are implementing more states

// Depending on how the shifter sends signals, or if we need to map more buttons, may need to declare more input pins

// Scaling factor
int scale = 1;

// Gear states are represented as integers (MAY NEED TO SCALE OR REARRANGE THEM IF WE NEED TO SEND THE SIGNAL FOR LONGER)
int driveState = 4*scale;
int neutralState = 3*scale;
int reverseState = 2*scale;
int parkState = 1*scale;

int state = neutralState; // Target state read from input
int currentState = neutralState; // What the vehicle is currently set to

void setup() {

  Serial.begin(9600); // Setup Serial communication

  //Set pinmode of Input pins

  pinMode(PARK, INPUT);
  pinMode(REVERSE, INPUT);
  pinMode(NEUTRAL, INPUT);
  pinMode(DRIVE, INPUT);
  
  pinMode(RESET, INPUT);

  //Set pinmode of output RGB pins
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

}

void loop() {

  // Set the target state (THESE HAVE TO BE IN DESCENDING ORDER)
  if(digitalRead(RESET) == HIGH) { // If the reset button is pressed, reset the sim and the state variables
    buf[2] = 21;
    Serial.write(buf, 8);
    releaseKey();
    state = neutralState;
    currentState = neutralState;
  } else if(digitalRead(DRIVE) == HIGH) { // If the shifter is set to drive
    state = driveState;
  } else if(digitalRead(NEUTRAL) == HIGH) { // If the shifter is set to neutral
    state = neutralState;
  } else if(digitalRead(REVERSE) == HIGH) { // If the shifter is set to reverse
    state = reverseState;
  } else if(digitalRead(PARK) == HIGH) { // If the shifter is set to park
    state = parkState;
  }

  // If the current state is a lower gear than the target gear, go up one gear
  if (state > currentState) {
    buf[2] = 27;
    Serial.write(buf, 8);
    releaseKey();
    currentState++;
  }

  // If the current state is a higher gear than the target gear, go down a gear
  if (state < currentState) {
    buf[2] = 29;
    Serial.write(buf, 8);
    releaseKey();
    currentState--;
  }

  if(currentState == parkState) {
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 255);
  } else if(currentState == reverseState) {
    analogWrite(RED, 255);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
  } else if(currentState == neutralState) {
    analogWrite(RED, 255);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 255);
  } else if(currentState == driveState) {
    analogWrite(RED, 0);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 0);
  }

}

// Function for Key Release

void releaseKey() {

  buf[0] = 0;

  buf[2] = 0;

  Serial.write(buf, 8); // Send Release key

}
