uint8_t buf[8] = { 0 }; //Keyboard report buffer

#define PARK 4 //Input pin for park
#define REVERSE 5 //Input pin for reverse
#define NEUTRAL 6 //Input pin for neutral
#define DRIVE 7 //Input pin for drive

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

}

void loop() {

  // Set the target state (THESE HAVE TO BE IN DESCENDING ORDER)
  if(digitalRead(DRIVE) == HIGH) { // If the shifter is set to drive
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

}

// Function for Key Release

void releaseKey() {

  buf[0] = 0;

  buf[2] = 0;

  Serial.write(buf, 8); // Send Release key

}
