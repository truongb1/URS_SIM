// Define the baud rate
#define BAUD_RATE 115200

// Variables to store the received serial data
char serialData[50]; // Assuming the message won't exceed 50 characters

// Function to parse and extract values from the message
void extractValues(char *message, float &speed, float &rpm, float &temp) {
  // Check if the message starts with "Data="
  if (strncmp(message, "Data=", 5) == 0) {
    // Move the pointer to the actual data part
    char *dataPart = message + 5;

    // Assuming the format is "float:float:float"
    char *token = strtok(dataPart, ":");
    if (token != NULL) {
      speed = atof(token);
      token = strtok(NULL, ":");
      if (token != NULL) {
        rpm = atof(token);
        token = strtok(NULL, ":");
        if (token != NULL) {
          temp = atof(token);
          return; // Return after successfully extracting all values
        }
      }
    }
  }

  // Set values to a default if the format is incorrect
  speed = -1.0;
  rpm = -1.0;
  temp = -1.0;
}

void setup() {
  // Start the serial communication
  Serial.begin(BAUD_RATE);
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the data into the serialData array
    Serial.readBytesUntil('\n', serialData, sizeof(serialData));
    
    // Extract and print values if the message is in the correct format
    float speed, rpm, temp;
    extractValues(serialData, speed, rpm, temp);

    if (speed >= 0 && rpm >= 0 && temp >= 0) {
      Serial.print("SPEED: ");
      Serial.print(speed, 2); // Print speed with 2 decimal places

      Serial.print(" RPM: ");
      Serial.print(rpm, 2); // Print rpm with 2 decimal places

      Serial.print(" TEMP: ");
      Serial.println(temp, 2); // Print temp with 2 decimal places
     
    }
  }
}

