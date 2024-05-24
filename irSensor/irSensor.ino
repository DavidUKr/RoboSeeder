// Define the pins
const int analogPin = 33; // A0 
const int digitalPin = 12; // D0 

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set digital pin as input
  pinMode(digitalPin, INPUT_PULLUP); // Use INPUT_PULLUP to enable internal pull-up resistor
}

void loop() {
  // Read analog value from IR sensor
  int analogValue = analogRead(analogPin);

  // Read digital value from IR sensor
  int digitalValue = digitalRead(digitalPin);

  // Print values to serial monitor
  Serial.print("Analog value: ");
  Serial.println(analogValue);
  
  Serial.print("Digital value: ");
  Serial.println(digitalValue);
  
  delay(100);
}
