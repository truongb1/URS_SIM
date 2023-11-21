// Define the baud rate
#define BAUD_RATE 9600

// Variables to store the received serial data
char serialData[50]; // Assuming the message won't exceed 50 characters

// Function to parse and extract speed value from the message
float extractSpeed(char *message) {
  // Assuming the format is "Speed:xx.xx"
  char *speedStart = strchr(message, ':');
  if (speedStart != NULL) {
    return atof(speedStart + 1); // Convert the substring after ':' to a float
  } else {
    return -1.0; // Return a negative value if the format is incorrect
  }
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

    // Extract and print the speed if the message is in the correct format
    float speed = extractSpeed(serialData);
    if (speed >= 0) {
      Serial.print("Received Speed: ");
      Serial.println(speed, 2); // Print speed with 2 decimal places
    } else {
      Serial.println("Invalid format");
    }
  }
}