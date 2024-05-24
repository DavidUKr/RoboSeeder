#define ANALOG_PIN_1 14
#define DIGITAL_PIN_1 27
#define ANALOG_PIN_2 15
#define DIGITAL_PIN_2 17
#define ANALOG_PIN_3 35
#define DIGITAL_PIN_3 26

#define INTERRUPT_THRESHOLD 1000  
#define TRIGGER_PIN  12  
#define ECHO_PIN     13  
#define SERVO_PIN    33 
#define MAX_DISTANCE 200 
#define DETECTION_THRESHOLD 50 // Distance threshold in cm to detect an obstacle
#define SCAN_ANGLE_STEP 5 

volatile bool objectDetected1 = false;
volatile bool objectDetected2 = false;
volatile bool objectDetected3 = false;

void IRAM_ATTR handleInterrupt1() {
  objectDetected1 = true;
}

void IRAM_ATTR handleInterrupt2() {
  objectDetected2 = true;
}

void IRAM_ATTR handleInterrupt3() {
  objectDetected3 = true;
}

void rotateServo(int angle) {
  int pulseWidth = map(angle, 0, 180, 500, 2400);
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(SERVO_PIN, LOW);
  delay(20);
}

long measureDistance() {
  long duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void setup() {
  Serial.begin(115200);

  pinMode(DIGITAL_PIN_1, INPUT);
  pinMode(DIGITAL_PIN_2, INPUT);
  pinMode(DIGITAL_PIN_3, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SERVO_PIN, OUTPUT);

  
  attachInterrupt(digitalPinToInterrupt(DIGITAL_PIN_1), handleInterrupt1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DIGITAL_PIN_2), handleInterrupt2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DIGITAL_PIN_3), handleInterrupt3, CHANGE);
}

void loop() {
  if (objectDetected1) {
    int analogValue1 = analogRead(ANALOG_PIN_1);
    Serial.print("Object detected on sensor 1! Analog Value: ");
    Serial.println(analogValue1);
    objectDetected1 = false;
  }

  if (objectDetected2) {
    int analogValue2 = analogRead(ANALOG_PIN_2);
    Serial.print("Object detected on sensor 2! Analog Value: ");
    Serial.println(analogValue2);
    objectDetected2 = false;
  }

  if (objectDetected3) {
    int analogValue3 = analogRead(ANALOG_PIN_3);
    Serial.print("Object detected on sensor 3! Analog Value: ");
    Serial.println(analogValue3);
    objectDetected3 = false;
  }

  static bool direction = true; // true for left to right, false for right to left

  const int numAngles = 180 / SCAN_ANGLE_STEP + 1;
  int angles[numAngles];
  long distances[numAngles];

  int index = 0;
  if (direction) {
    for (int angle = 0; angle <= 180; angle += SCAN_ANGLE_STEP) {
      rotateServo(angle);
      long distance = measureDistance();
      
      angles[index] = angle;
      distances[index] = distance;
      index++;

      Serial.print("Angle: ");
      Serial.print(angle);
      Serial.print(" Distance: ");
      Serial.println(distance);

      if (distance <= DETECTION_THRESHOLD) {
        Serial.print("Obstacle detected at angle ");
        Serial.print(angle);
        Serial.print(" with distance ");
        Serial.println(distance);
      }
    }
  } else {
    for (int angle = 180; angle >= 0; angle -= SCAN_ANGLE_STEP) {
      rotateServo(angle);
      long distance = measureDistance();
      
      angles[index] = angle;
      distances[index] = distance;
      index++;

      Serial.print("Angle: ");
      Serial.print(angle);
      Serial.print(" Distance: ");
      Serial.println(distance);

      if (distance <= DETECTION_THRESHOLD) {
        Serial.print("Obstacle detected at angle ");
        Serial.print(angle);
        Serial.print(" with distance ");
        Serial.println(distance);
      }
    }
  }

  direction = !direction; 

  Serial.println("Angle-Distance Array:");
  for (int i = 0; i < numAngles; i++) {
    Serial.print("Angle: ");
    Serial.print(angles[i]);
    Serial.print(" Distance: ");
    Serial.println(distances[i]);
  }

  delay(1000); 
}
