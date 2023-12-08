int latchPin = 5;
int clockPin = 3;
int dataPin = 7;

bool digits[10][8] = {
  {1,1,0,1,1,1,0,1},  // 0
  {0,1,0,1,0,0,0,0},  // 1
  {1,1,0,0,1,1,1,0},  // 2
  {1,1,0,1,1,0,1,0},  // 3
  {0,1,0,1,0,0,1,1},  // 4
  {1,0,0,1,1,0,1,1},  // 5
  {1,0,1,1,1,1,1,1},  // 6
  {1,1,0,1,0,0,0,0},  // 7
  {1,1,0,1,1,1,1,1},  // 8
  {1,1,1,1,1,0,1,1}   // 9
};

int currentDigits[4] = {0, 0, 0, 0};
unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  digitalWrite(clockPin, LOW);
  Serial.begin(9600);
}

void loop() {
  // Опрос Serial
  if (Serial.available() >= 4) {
    for (int i = 0; i < 4; i++) {
      char user_input = Serial.read(); 
      currentDigits[i] = (int)(user_input - '0');
    }
    displayDigits();
  }

  // Отсчет времени
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    incrementTime();
    displayDigits();
  }
}

void displayDigits() {
  digitalWrite(latchPin, LOW);
  for (int j = 0; j < 4; j++) {
    int digit = currentDigits[j];
    if (digit >= 0 && digit <= 9) {
        for(int i = 7; i >=0; i--) {
            shiftAndSet(digits[digit][i]);
        }
    }
  }
  digitalWrite(latchPin, HIGH);
}

void shiftAndSet(bool val) {
  digitalWrite(dataPin, val);
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);
}

void incrementTime() {
  currentDigits[3]++;
  if (currentDigits[3] >= 10) {
    currentDigits[3] = 0;
    currentDigits[2]++;
    if (currentDigits[2] >= 6) {
      currentDigits[2] = 0;
      currentDigits[1]++;
      if (currentDigits[1] >= 10) {
        currentDigits[1] = 0;
        currentDigits[0]++;
        if (currentDigits[0] >= 10) {
          currentDigits[0] = 0;
        }
      }
    }
  }
}
