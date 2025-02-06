// Define motor driver pins
int IN1 = 9;
int IN2 = 10;
int EN = 11;  // Enable pin for PWM (Motor speed)

// Switch pins
int switch1 =   2; // S1 - Pin for switch 1 (100 PWM)
int switch2 =   3; // S2 - Pin for switch 2 (240 PWM)

// Define motor speed values
int motorSpeed = 0;  // Default motor speed (this can be changed with switch S1 and S2)

int signSpeed =  250;

// Define sign speed thresholds (in PWM)
int stopSpeed = 0;
int limitSpeed20 = 40;
int limitSpeed30 = 60;
int limitSpeed40 = 80;
int limitSpeed60 = 120;
int limitSpeed70 = 140;
int limitSpeed80 = 160;
int limitSpeed90 = 180;
int limitSpeed100 = 200;

// Initialize serial communication
String receivedData = "";

void setup() {
  Serial.begin(9600);  // Start serial communication
  
  pinMode(IN1, OUTPUT);  // Motor driver control pin
  pinMode(IN2, OUTPUT);  // Motor driver control pin
  pinMode(EN, OUTPUT);   // Motor driver enable pin for PWM
  
  pinMode(switch1, INPUT);  // Set Switch 1 (S1) pin as input
  pinMode(switch2, INPUT);  // Set Switch 2 (S2) pin as input

  // Set motor speed based on switch input
  analogWrite(EN, motorSpeed);  // Set initial motor speed to 100 PWM or 240 PWM

  // Set motor direction (rotate in one direction)
  digitalWrite(IN1, LOW);   // Set IN1 HIGH
  digitalWrite(IN2, LOW);    // Set IN2 LOW to rotate in one direction
}

void loop() {
  if (Serial.available()) {  // Check if data is available from Python
    receivedData = Serial.readStringUntil('\n');  // Read the incoming sign name
    
    // Debugging output
    Serial.println("Received Sign: " + receivedData);

    // Set the sign speed based on the detected traffic sign
    signSpeed = getSignSpeed(receivedData);

    // Compare current motor speed with sign speed and take action
    if (motorSpeed > signSpeed) {
      motorSpeed = signSpeed;  // Reduce speed if the detected sign requires a lower speed
      analogWrite(EN, motorSpeed);
    }
    // If the current speed is equal to or less than the detected sign speed, do nothing
  }

  // Optionally: Adjust motor speed based on switches
  if (digitalRead(switch1) == HIGH) {
    if (100 > signSpeed ) {
      motorSpeed = signSpeed;
      analogWrite(EN, motorSpeed);
      digitalWrite(IN1, HIGH);   // Set IN1 HIGH
      digitalWrite(IN2, LOW);    // Set IN2 LOW to rotate in one direction
    }
    else {
      motorSpeed = 100;  // S1 pressed, set speed to 100 PWM
      analogWrite(EN, motorSpeed);
      digitalWrite(IN1, HIGH);   // Set IN1 HIGH
      digitalWrite(IN2, LOW);    // Set IN2 LOW to rotate in one direction

    }
    
  }
  if (digitalRead(switch2) == HIGH) {
    if (240 > signSpeed ) {
      motorSpeed = signSpeed;
      analogWrite(EN, motorSpeed);
      digitalWrite(IN1, HIGH);   // Set IN1 HIGH
      digitalWrite(IN2, LOW);    // Set IN2 LOW to rotate in one direction
    }
    else {
      motorSpeed = 240;  // S1 pressed, set speed to 100 PWM
      analogWrite(EN, motorSpeed);
      digitalWrite(IN1, HIGH);   // Set IN1 HIGH
      digitalWrite(IN2, LOW);    // Set IN2 LOW to rotate in one direction

    }
  }
}

// Function to get sign speed corresponding to detected traffic sign
int getSignSpeed(String sign) {
  if (sign == "Stop") {
    return stopSpeed;
  } else if (sign == "Speed Limit 20") {
    return limitSpeed20;
  } else if (sign == "Speed Limit 30") {
    return limitSpeed30;
  } else if (sign == "Speed Limit 40") {
    return limitSpeed40;
  } else if (sign == "Speed Limit 60") {
    return limitSpeed60;
  } else if (sign == "Speed Limit 80") {
    return limitSpeed80;
  } else if (sign == "Speed Limit 80") {
    return limitSpeed80;
  } else if (sign == "Speed Limit 90") {
    return limitSpeed90;
  } else if (sign == "Speed Limit 100") {
    return limitSpeed100;
  }
  return 0;  // Default to stop if no match
}
