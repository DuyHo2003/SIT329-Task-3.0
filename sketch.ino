extern "C" {
  #include "GPIO_DEAKIN.h"   // Your low-level GPIO library
}

#define NUM_LEDS 6

// Struct to hold Port/Pin mapping
typedef struct {
  char port;
  char pin;
} LedPin;

// Example LED mapping (adjust according to Nano 33 IoT datasheet!)
LedPin leds[NUM_LEDS] = {
  {'A', 10}, // D2
  {'A', 11}, // D3
  {'B', 10}, // D4
  {'B', 11}, // D5
  {'A', 20}, // D6
  {'A', 21}  // D7
};

// Timers
unsigned long timer1 = 0;  
unsigned long timer2 = 0;  
unsigned long timer3 = 0;  

// Mode state
int mode = 0;

// Mode 1 (Knight Rider)
int pos = 0;
int dir = 1;

// Mode 2 (Catch Me If You Can)
int leftPos = 0;
int rightPos = NUM_LEDS - 1;
int leftDir = 1;
int rightDir = -1;

void setup() {
  Serial.begin(9600);
  while (!Serial);  // Wait for Serial Monitor to open


  // Configure GPIO pins for LEDs
  for (int i = 0; i < NUM_LEDS; i++) {
    Config_GPIO(leds[i].port, leds[i].pin, OUTPUT);
    Write_GPIO(leds[i].port, leds[i].pin, LOW); // LEDs off
  }

  Serial.println("Select Mode: 1 = Knight Rider, 2 = Catch Me If You Can");
  while (Serial.available() == 0); // wait for user input
  mode = Serial.parseInt();

  Serial.print("Mode Selected: ");
  Serial.println(mode);
}

void loop() {
  unsigned long now = millis();

  if (mode == 1) {
    // Knight Rider: step every 1s
    if (now - timer1 >= 1000) {
      timer1 = now;
      runKnightRider();
    }
  }
  else if (mode == 2) {
    // Left runner: step every 1s
    if (now - timer2 >= 1000) {
      timer2 = now;
      moveLeftRunner();
    }
    // Right runner: step every 0.5s
    if (now - timer3 >= 500) {
      timer3 = now;
      moveRightRunner();
    }
  }
}

// -------- Mode Functions --------
void runKnightRider() {
  clearLeds();
  Write_GPIO(leds[pos].port, leds[pos].pin, HIGH);
  pos += dir;
  if (pos == NUM_LEDS - 1 || pos == 0) dir = -dir;
}

void moveLeftRunner() {
  clearLeds();
  Write_GPIO(leds[leftPos].port, leds[leftPos].pin, HIGH);
  leftPos += leftDir;
  if (leftPos == NUM_LEDS - 1 || leftPos == 0) leftDir = -leftDir;
}

void moveRightRunner() {
  Write_GPIO(leds[rightPos].port, leds[rightPos].pin, HIGH);
  rightPos += rightDir;
  if (rightPos == NUM_LEDS - 1 || rightPos == 0) rightDir = -rightDir;
}

// Turn off all LEDs
void clearLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    Write_GPIO(leds[i].port, leds[i].pin, LOW);
  }
}
