#define LED_1_PIN 12
#define LED_2_PIN 11

typedef void (*TaskFunction)();
TaskFunction taskList[3];

unsigned long previousTimeLED1 = 0;
unsigned long previousTimeLED2 = 0;
unsigned long LED1Interval = 500;
unsigned long LED2Interval = 1000;

byte LED1State = LOW;
byte LED2State = LOW;

String inputBuffer = "";
int targetLED = 0;
int newInterval = 0;
bool inputReady = false;

void setup() {
  Serial.begin(9600);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);

  taskList[0] = taskReadSerial;
  taskList[1] = taskBlinkLED1;
  taskList[2] = taskBlinkLED2;

  Serial.println("Enter: <LED number> <interval>");
  Serial.println("Example: 1 750");
}

void loop() {
  for (int i = 0; i < 3; i++) {
    taskList[i]();
  }
}

// Task 1: Read and parse serial input
void taskReadSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      inputReady = true;
    } else if (isPrintable(c)) {
      inputBuffer += c;
    }
  }

  if (inputReady) {
    inputReady = false;
    inputBuffer.trim();
    int spaceIndex = inputBuffer.indexOf(' ');
    if (spaceIndex > 0) {
      targetLED = inputBuffer.substring(0, spaceIndex).toInt();
      newInterval = inputBuffer.substring(spaceIndex + 1).toInt();

      if (targetLED == 1) {
        LED1Interval = newInterval;
        Serial.print("LED 1 interval set to ");
        Serial.println(LED1Interval);
      } else if (targetLED == 2) {
        LED2Interval = newInterval;
        Serial.print("LED 2 interval set to ");
        Serial.println(LED2Interval);
      } else {
        Serial.println("Invalid LED number.");
      }
    } else {
      Serial.println("Invalid input. Use format: 1 500");
    }
    inputBuffer = "";
  }
}

// Task 2: Blink LED 1
void taskBlinkLED1() {
  unsigned long now = millis();
  if (now - previousTimeLED1 >= LED1Interval) {
    previousTimeLED1 = now;
    LED1State = !LED1State;
    digitalWrite(LED_1_PIN, LED1State);
  }
}

// Task 3: Blink LED 2
void taskBlinkLED2() {
  unsigned long now = millis();
  if (now - previousTimeLED2 >= LED2Interval) {
    previousTimeLED2 = now;
    LED2State = !LED2State;
    digitalWrite(LED_2_PIN, LED2State);
  }
}
