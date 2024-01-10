uint8_t buf[8] = {
  0
};  // Keyboard Report Buffer: 8 bytes


//#define SERIAL_DEBUG  // for serial debug: remove //
                        // for actual HID: put //
                        // Original Sourcecode: https://techtotinker.com/2020/07/14/tutorial-how-to-use-arduino-uno-as-hid-part-1-arduino-keyboard-emulation/


/*
BeamNG Keyboard Commands	(HID Keypress	Values)
Toggle Parking Brake(p and P) = Hex	0x13	Dec 19
Shift Down(z and Z)	= Hex 0x1d	Dec 29
Shift up(x and X)	= Hex 0x1b	Dec 27
headlights(n and N)	= Hex 0x11	Dec 17
left signal(, and <) = Hex	0x36	Dec 54
right signal(. and >)	= Hex 0x37	Dec 55
hazard sinal(/ and ?)	= Hex 0x38	Dec 56
Parking Brake(spacebar) = Hex	0x2c	Dec 44
shifter mode?(q and Q) = Hex 0x14	Dec 20
*/

const int GPIO_PIN[] = {2,3,4,5,6,7,8,9,10};      // Button Input GPIO Pins
const int keyID[] = {19,29,27,17,55,54,56,44,20}; // Keypress HID(s)
char keyValue[] = "P or p, Z or z, X or x, N or n, . or >, /, or <, / or ?, Spacebar, Q or q";  // Keypress ID for SerialDebug
bool currState[] = {HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH};  // Default button state
bool prevState[] = {HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH};  // Default button state
unsigned long prevTime[] = {0,0,0,0,0,0,0,0,0};   // Debounce timer initial
unsigned long waitTime[] = {50,50,50,50,50,50,50,50,50};  // Debounce timer wait time
const int buttonCount = 9;  // number of GPIO's and Keypress HID(s)
bool currRead[] = {0,0,0,0,0,0,0,0,0};  // Holds state(s) of current button Keypress' 
//bool toggle[] = {0,0,0,0,0,0,0,0,0};  // current button Keypress single shot each time button is depressed or released' 

void setup() 
{
  Serial.begin(9600);

  for(int i = 0; i < buttonCount; i++){    //declaring all the outputs and setting them high
    pinMode(GPIO_PIN[i], INPUT_PULLUP);
    delay(200);
  }    
}

void loop() 
{
  checkButton();
}

void checkButton() {

  for(int i = 0; i < buttonCount; i++){     // wait for keypress
    currRead[i] = digitalRead(GPIO_PIN[i]);

    if (currRead[i] != currState[i]) {
      currState[i] = currRead[i];
        // Send the code
      buf[2] = keyID[i];    // HID: key iD
        #ifdef SERIAL_DEBUG
          buf[2] = keyValue[i]; // Serial: key pressed
        #endif
      Serial.write(buf, 8); // Send keypress
      delay(100);
    } else {
        // Release the keyboard
        releaseKey();
    }
  }
 
}

void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8); // Release key  
}
