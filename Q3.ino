// =========== Pin Definitions ===========
// Encoder pins
const int encoderA = 2;   // INT0 - Channel A
const int encoderB = 4;   // Channel B

// Motor control pins (L298N)
const int motorIN1 = 5;
const int motorIN2 = 6;
const int motorEN = 9;    // PWM pin for speed control

// =========== Encoder Variables ===========
volatile long encoderCount = 0;    // Updated inside interrupt
long lastCount = 0;                // Previous count for speed calc
unsigned long lastTime = 0;        // Last time we measured speed
float speedTicksPerSec = 0;        // Speed in ticks/second

// =========== Interrupt Function ===========
// Runs on every rising edge of Channel A
void handleEncoder() {
  // Check Channel B to know rotation direction
  if (digitalRead(encoderB) == LOW) {
    encoderCount++;   // Clockwise
  } else {
    encoderCount--;   // Counter-clockwise
  }
}

// =========== Motor Setup ===========
void setupMotor() {
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
  pinMode(motorEN, OUTPUT);
  
  // Motor starts stopped
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
  analogWrite(motorEN, 0);
}

// =========== Run Motor ===========
// speed: 0–255 PWM
// forward: true = forward, false = reverse
void runMotor(int speed, bool forward = true) {
  if (forward) {
    digitalWrite(motorIN1, HIGH);
    digitalWrite(motorIN2, LOW);
  } else {
    digitalWrite(motorIN1, LOW);
    digitalWrite(motorIN2, HIGH);
  }
  analogWrite(motorEN, speed);
}

// =========== Stop Motor ===========
void stopMotor() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
  analogWrite(motorEN, 0);
}

// =========== SETUP ================
void setup() {
  Serial.begin(9600);
  Serial.println("DC Motor with Encoder Test");
  
  // Encoder setup
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderA), handleEncoder, RISING);
  
  // Motor setup
  setupMotor();
  
  lastTime = millis();
  
  // Start motor at low speed for testing
  Serial.println("Starting motor...");
  runMotor(100, true);
  delay(1000);
}

// =========== MAIN LOOP =============
void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - lastTime;
  
  // Calculate speed every 1 second
  if (elapsed >= 1000) {
    long ticks = encoderCount - lastCount;
    speedTicksPerSec = ticks * (1000.0 / elapsed);
    
    Serial.print("Time: ");
    Serial.print(currentTime / 1000);
    Serial.print("s | Ticks: ");
    Serial.print(encoderCount);
    Serial.print(" | Speed: ");
    Serial.print(speedTicksPerSec);
    Serial.print(" ticks/sec");
    
    // Print direction
    if (ticks > 0) {
      Serial.println(" (Clockwise)");
    } else if (ticks < 0) {
      Serial.println(" (Counter-clockwise)");
    } else {
      Serial.println(" (Stopped)");
      
      // If motor stops, increase speed slightly
      static int motorSpeed = 100;
      motorSpeed += 10;
      if (motorSpeed > 255) motorSpeed = 100;
      runMotor(motorSpeed, true);
    }
    
    // Update values
    lastCount = encoderCount;
    lastTime = currentTime;
  }
  
  // Change direction every 5 seconds (test only)
  static unsigned long lastDirectionChange = 0;
  if (currentTime - lastDirectionChange > 5000) {
    lastDirectionChange = currentTime;
    static bool forward = true;
    forward = !forward;
    Serial.print("Changing direction to: ");
    Serial.println(forward ? "Forward" : "Reverse");
    runMotor(150, forward);
  }
}
