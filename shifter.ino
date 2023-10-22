uint8_t buf[8] = { 0 }; //Keyboard report buffer

#define PIN_IN A0 //Input pin

// Depending on how the shifter sends signals, or if we need to map more buttons, may need to declare more input pins

// Voltage thresholds for each driving state (EXAMPLE VALUES, NOT CERTAIN AS OF YET)
int driveThreshold = 128;
int neutralThreshold = 96;
int reverseThreshold = 64;
int parkThreshold = 32;

// Gear states are represented as integers (MAY NEED TO SCALE OR REARRANGE THEM IF WE NEED TO SEND THE SIGNAL FOR LONGER)
int driveState = 4;
int neutralState = 3;
int reverseState = 2;
int parkState = 1;

int state = 1; // Target state read from input
int currentState = 1; // What the vehicle is currently set to
  
// Keyboard signals to send
int leftShift = (int) '<';
int rightShift = (int) '>';

void setup() {

  Serial.begin(9600); // Setup Serial communication

  //Set pinmode of Input pins

  pinMode(PIN_IN, INPUT);

}

void loop() {

  int input = analogRead(PIN_IN);

  // Set the target state (THESE HAVE TO BE IN DESCENDING ORDER)
  if(input >= driveThreshold) { // If the shifter is set to drive
    state = driveState;
  } else if(input >= neutralThreshold) { // If the shifter is set to neutral
    state = neutralState;
  } else if(input >= reverseThreshold) { // If the shifter is set to reverse
    state = reverseState;
  } else if(input >= parkThreshold) { // If the shifter is set to park
    state = parkState;
  }

  // If the current state is a lower gear than the target gear, go up one gear
  if (state > currentState) {
    Serial.write(buf, rightShift);
    releaseKey();
    currentState++;
  }

  // If the current state is a higher gear than the target gear, go down a gear
  if (state < currentState) {
    Serial.write(buf, leftShift);
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
