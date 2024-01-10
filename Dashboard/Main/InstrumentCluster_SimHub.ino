//   xConnect © 2022 by InfoX1337 is licensed under CC BY-NC-SA 4.0 (Original Source Code)
// Contact hello@xstudios.one if you want to use xConnect commercially.
//                To view a copy of this license, visit: 
//          http://creativecommons.org/licenses/by-nc-sa/4.0/
// Special thanks to Vampyr Yannik#0001 for providing the CAN CODES
//          ___________
//  -  ----// --|||-- \\         
// ---- __//____|||____\\____   
//     | _|    " | "   --_  ||
// ----|/ \______|______/ \_|| uDayton ECE43/432 FA23/SP24
//______\_/_____________\_/_______
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// SIMHUB Custom serial device binding must be set
// NCalc formula: ';'+[Rpms]+':'+[SpeedMph]+':'+[OilTemperature]+':'+[Gear]+':'+[TurnIndicatorLeft]+':'+[TurnIndicatorRight]+':'+[Handbrake]+':'+[ABSActive]+':'+[CarDamagesAvg]+':'
// example serial output from Simhub to Arduino (without the parenthesis): ";834:0:180:N:0:0:100:0:0:"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Imports:
#include <mcp_can.h>
#include <SPI.h>
#include <math.h>

#define hi8(x) ((int) (x) >> (8) ) // keep upper 8 bits
#define lo8(x) ((int) (x) & (0xff) ) // keep lower 8 bits

// Configuration:
#define VERSION "1.2"
#define SERIAL_SPEED 115200
#define DEBUGGER_ENABLED 
#define SPI_CS_PIN 9
#define OK_BTN_PIN 3

// Create a new MCP_CAN instance with the SPI_CS_PIN defined in the config.
MCP_CAN CAN(SPI_CS_PIN);

// This function runs only once during init of the arduino.
void setup() {

  // Initiate Serial bus and print notice:
Serial.begin(SERIAL_SPEED);
  //DebugPrint("Serial output started with debugging");
  // Logo print
Serial.println("          ___________		");
Serial.println("  -  ----// --|||-- \\ 		");        
Serial.println(" ---- __//____|||____\\____ 	");  
Serial.println("     | _|    \" | \"   --_  ||	");
Serial.println(" ----|/ \\______|______/ \\_|| uDayton ECE43/432 FA23/SP24 ");
Serial.println("______\\_/_____________\\_/_______");	
Serial.println(" Bao Truong, Dillon Tipton, Bill Kaval  ");
  Serial.println("             **Adapted from the following source**");                    
  Serial.println("   xConnect © 2022 by InfoX1337 (licensed under CC BY-NC-SA 4.0)");
  Serial.println("                      hello@xstudios.one");
  Serial.println("                        Version: V" + String(VERSION) + "\n");
  Serial.println("               To view a copy of this license, visit: ");
  Serial.println("         http://creativecommons.org/licenses/by-nc-sa/4.0/");
  Serial.println("\nStarting initiation of program...");
  // Try and initiate the CAN BUS controller.
  CAN_INIT:
    if(CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) {
      DebugPrint("CAN BUS CONTROLLER initiation ok");
    } else {
      DebugPrint("CAN BUS CONTROLLER initiation failed, retrying in 500ms");
      delay(500);
      goto CAN_INIT;
    } 
  // Set CAN BUS mode to MCP_ANY
  CAN.setMode(MCP_ANY);

  // Configure interface buttons
  pinMode(OK_BTN_PIN, INPUT_PULLUP);
  
  // Ford cluster custom needle sweep
  DebugPrint("Starting needle sweep service...");
  FordSweep();
  DebugPrint("Needle sweep completed! Starting program...");
  Serial.println("Program starting...");
}  
// This is the 'main' function of the code, as its name suggests, it runs in a continuous loop.
// To avoid laggy CAN BUS, or cluster switching off please do not use the delay() here.

// Basic terminology needed when working with xConnect:
// dataPacket - a data packet sent by the xConnect PC software (not customizable). Its format is: RPM:SPEED:TEMP:FUEL:GEARSELECT:LOCALE:
// customProtocol - A custom protocol sent after a dataPacket, that is customizable to fit the developer needs, it can be formatted however you desire.
// LOCALE - There can be two values: European, American (euro -> Celcious and meters / american -> Fahrenheit and mph)

// Variable definitions needed for input code: (do not touch, user definitions below)
int rpm = 0;
int speed = 0; 
int temp = 100; // Temperature values: (100-200) the formula from celcious is: TEMP+90 the formula for fahrenheit is: TEMP-50
int fuel = 0;
String gear = "P";
int Telem; // dummy vaiable to capture beginning serial Lcalc message from simhub

// Custom variable definitions:
int rpmgate = 96; // <- rpm gate for ford rpm (96-115)
int finetune = 0; // <- rpm fine tuning inside of gate for ford rpm (0-255) 
int distance = 0;
byte speedH = 0x00;
byte speedL = 0x00;

byte PB_ON = 0x00;
byte PB_WARN = 0x00;
byte BRK_WARN = 0x00;
byte ABS = 0x00;
byte TCS = 0x00;
byte LOW_TP = 0x00;
byte CEL = 0x00;

byte CRUISE = 0x00;
byte blinkR = 0x00;
byte blinkL = 0x00;
boolean toggle1 = 0;
boolean parkingbrake = 0;
boolean ABSwarn = 0;
boolean TCSwarn = 0;
boolean TirePwarn = 0;
boolean CELwarn = 0;
boolean turnL = 0;
boolean turnR = 0;

void loop() {
  // Data reader loop
  while(Serial.available() > 0) {
    // Example data to send: Telem:734:0:180:N:0:0:100:0:0
  
    Serial.readStringUntil(';').toInt();
    rpm = Serial.readStringUntil(':').toInt();
    speed = Serial.readStringUntil(':').toInt();
    temp = Serial.readStringUntil(':').toInt();
    gear = Serial.readStringUntil(':');
    turnL = Serial.readStringUntil(':').toInt();
    turnR = Serial.readStringUntil(':').toInt();
    parkingbrake = Serial.readStringUntil(':').toInt();
    ABSwarn = Serial.readStringUntil(':').toInt();
    TCSwarn = Serial.readStringUntil(':').toInt();
    //TirePwarn = Serial.readStringUntil(':').toInt();
    CELwarn = Serial.readStringUntil(':').toInt();
    
    rpmgate = 96 + (int)(rpm / 500); // Calculate the gate and cast to int
    finetune = (int)(rpm%500)/2;
    if(rpmgate > 115) {
      rpmgate = 115;
    } 
    
    speedL = lo8(speed*.625); // lower bits used for speed in MPH
    //speedH = hi8(speed*.625);


    // clear warning lights
    PB_WARN = 0x00;
    ABS = 0x00;
    TCS = 0x00;
    LOW_TP = 0x00;
    CEL = 0x00;
    blinkR = 0x00;    
    blinkL = 0x00;


    // set warning lights 
    if(turnR) {
      blinkR = 0x08;
    }else blinkR = 0x00;
    if(turnL) {
      blinkL = 0x40;
    }else blinkL = 0x00;
    if(parkingbrake) PB_WARN = 0xFF;
    if(ABSwarn) ABS = 0x80;
    if(TCSwarn) TCS = 0x06;
    if(TirePwarn) LOW_TP = 0x04;
    if(CELwarn) CEL = 0x04;
  }
  
  // Cluster opSends to keep cluster alive and functional

  //Ignition
  opSend(0x3b3, 0x45, 0x00, 0x00, 0x8E, 125, 0x00, 0x13, 0x00);
  //Byte 5: Outside Temp Value -> 125=22°C
   
  //Airbag
  opSend(0x04C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

  //Parking Brake und Lampen
  opSend(0x3c3, 0x00, 0x00, PB_WARN, PB_ON, 0x00, 0x00, 0x00, 0x00);

  // Doors, Turnsignals etc.
  opSend(0x3B2, 0x00, 0x00, 0x00, 0x00, blinkR, 0x00, blinkL, 0x00);

  // ABS
  opSend(0x416, 0x0E, 0x00, 0x00, 0x00, 0x00, TCS, ABS, 0x00);
  //Byte 6: 0x04=Slow flashing TC, 0x06=Fast flahing TC, 0x06=Solid TC Light
  //Byte 7: 0xC0=Slow flashing ABS, 0x80=Rapid flashing ABS, 0x60=Solid ABS

  //Navigation Compass
  opSend(0x466, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

  //Coolant
  opSend(0x156, (temp - 50), 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00);

  //Power Steering
  opSend(0x877, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x00, 0x00);

  //Charging System?
  opSend(0x42c, CRUISE, 0x00, 0xA6, 0x00, 0x00, 0x00, 0x00, 0x00); 
  // First byte is Cruise Control light (0x90)
  
  //Dieselmotor
  opSend(0x421, CEL, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
  //Byte 1: 0x04=Solid CEL, 0x08=flahing CEL 

  //Parking Brake
  opSend(0x213, 0x00, 0x00, 0x00, PB_ON, BRK_WARN, 0x00, 0x00, 0x00); 

  //Tire Pressure
  opSend(0x3b4, LOW_TP, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
  //Byte 1: 0x04=Solid Low TP, 0x08=flahing Low TP

  //AWD
  opSend(0x261, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

  //Key Status
  opSend(0x38D, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

  // RPM
  opSend(0x204, 0x00, 0x00, 0x00, rpmgate, finetune, finetune, 0x00, 0x00);

  // Speedometer
  //distance += speed*1.12;
  //if(distance > speed*1.12) distance = 0;
  opSend(0x202, 0x40, 0x00, 0x00, 115, 115,  speedL, speedL, 0x00);  //opSend(0x202, 0x40, 0x00, 0x00, distance, distance, 0x00, speedL, 0x00);

  // Gears
  if(gear.equalsIgnoreCase("P")) {
    opSend(0x171, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04);
  } else if(gear.equalsIgnoreCase("R")) {
    opSend(0x171, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24);
  } else if(gear.equalsIgnoreCase("N")) {
    opSend(0x171, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46);    
  } else opSend(0x171, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64); 

    /*
  } else if(gear.equalsIgnoreCase("D")) {
    opSend(0x171, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64); 
  } else if(gear.equalsIgnoreCase("S")) {
    opSend(0x171, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96);  
  }
  */

  // Button Events
  if(digitalRead(OK_BTN_PIN) == LOW) {
    opSend(0x881, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    while(digitalRead(OK_BTN_PIN) == LOW) {}
    opSend(0x881, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
  }
  
}

// This is the opSend function, it allows you to send CAN BUS codes via the interface.
// The addr is the address of the CAN BUS code.
// The bytes a-h are the data bytes required on CAN BUS.
void opSend(short addr, byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte h) {
  unsigned char data[8] = { a, b, c, d, e, f, g, h };
  CAN.sendMsgBuf(addr, 0, 8, data);
}

void FordSweep() {
  DebugPrint("Waiting for cluster to start...");
  for(int i = 0; i < 500; i++) {
    opSend(0x3b3, 0x45, 0x00, 0x00, 0x8E, 0x00, 0x00, 0x13, 0x00); //Ignition
    opSend(0x04C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //Airbag
    opSend(0x3c3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //Parking Brake
    opSend(0x416, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //ABS
    opSend(0x466, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //Navigation Compass
    opSend(0x156, 100, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00);  //Coolant
    opSend(0x877, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x00, 0x00); //Power Steering
    opSend(0x42c, 0x00, 0x00, 0xA6, 0x00, 0x00, 0x00, 0x00, 0x00); //Charging System?
    opSend(0x421, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //Motor stuff
    opSend(0x213, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //Parking Brake
    opSend(0x3b4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //Tire Pressure
    opSend(0x261, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //AWD
    opSend(0x38D, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //Key Status
    opSend(0x3B2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); // Doors, Turnsignals
  } 
  DebugPrint("Cluster started! Starting needle sweep...");
  opSend(0x202, 0x40, 0x00, 0x00, 115, 115, 255, 255, 0x00);
  delay(1550);
  opSend(0x204, 0x00, 0x00, 0x00, 115, 255, 255, 0x00, 0x00);
  delay(200);
  opSend(0x156, 180, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00);
  delay(500);
  opSend(0x202, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
  delay(150);
  opSend(0x204, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
  opSend(0x156, 100, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00);
}

void DebugPrint(String output) {
  #ifdef DEBUGGER_ENABLED;
  Serial.println("DEBUG: " + output);
  #endif
}
