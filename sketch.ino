extern "C" {
  #include "GPIO_DEAKIN.h"   // Custom GPIO control library
}

#define NUM_LEDS 6   // Total number of LEDs in the display

// Struct to hold mapping of each LED to a specific port/pin
typedef struct {
  char port;   // Port letter (e.g., 'A' or 'B')
  char pin;    // Pin number within the port
} LedPin;

// LED mapping 
LedPin leds[NUM_LEDS] = {
  {'A', 10}, // LED1 on pin PA10 (D2)
  {'A', 11}, // LED2 on pin PA11 (D3)
  {'B', 10}, // LED3 on pin PB10 (D4)
  {'B', 11}, // LED4 on pin PB11 (D5)
  {'A', 20}, // LED5 on pin PA20 (D6)
  {'A', 21}  // LED6 on pin PA21 (D7)
};


// millis() is used to track time for each LED action
unsigned long timer1 = 0;  // Knight Rider timing
unsigned long timer2 = 0;  // Left runner timing
unsigned long timer3 = 0;  // Right runner timing


int mode = 0;  // Stores mode (1 or 2)

// Knight Rider (Mode 1) 
int pos = 0;     // Current LED index
int dir = 1;     // Direction (1 = right, -1 = left)

// Catch Me If You Can (Mode 2) 
int leftPos = 0;                // Left runner starting at index 0
int rightPos = NUM_LEDS - 1;    // Right runner starting at last index
int leftDir = 1;                // Left runner direction
int rightDir = -1;              // Right runner direction


void setup() {
  Serial.begin(9600);
  while (!Serial);  // Wait until Serial Monitor is opened

  // Configure each LED GPIO pin as OUTPUT and turn them off
  for (int i = 0; i < NUM_LEDS; i++) {
    Config_GPIO(leds[i].port, leds[i].pin, OUTPUT);
    Write_GPIO(leds[i].port, leds[i].pin, LOW);
  }

  // Ask user for mode selection
  Serial.println("Select Mode: 1 = Knight Rider, 2 = Catch Me If You Can");
  while (Serial.available() == 0); // Wait until user enters a number
  mode = Serial.parseInt();        // Read mode input

  Serial.print("Mode Selected: ");
  Serial.println(mode);
}


void loop() {
  unsigned long now = millis(); // Current time in milliseconds

  if (mode == 1) {
    // Knight Rider mode
    if (now - timer1 >= 1000) {   // Run every 1 second
      timer1 = now;
      runKnightRider();
    }
  }
  else if (mode == 2) {
    // Catch Me If You Can mode
    if (now - timer2 >= 1000) {   // Left runner every 1 second
      timer2 = now;
      moveLeftRunner();
    }
    if (now - timer3 >= 500) {    // Right runner every 0.5 second
      timer3 = now;
      moveRightRunner();
    }
  }
}


// Knight Rider: LED moves left and right
void runKnightRider() {
  clearLeds(); // Turn off all LEDs
  Write_GPIO(leds[pos].port, leds[pos].pin, HIGH); // Light up current LED
  pos += dir; // Move position
  // Reverse direction at ends
  if (pos == NUM_LEDS - 1 || pos == 0) dir = -dir;
}

// Left Runner: Moves slowly from left to right and back
void moveLeftRunner() {
  clearLeds();
  Write_GPIO(leds[leftPos].port, leds[leftPos].pin, HIGH);
  leftPos += leftDir;
  // Reverse at ends
  if (leftPos == NUM_LEDS - 1 || leftPos == 0) leftDir = -leftDir;
}

// Right Runner: Moves faster from right to left and back
void moveRightRunner() {
  Write_GPIO(leds[rightPos].port, leds[rightPos].pin, HIGH);
  rightPos += rightDir;
  // Reverse at ends
  if (rightPos == NUM_LEDS - 1 || rightPos == 0) rightDir = -rightDir;
}

// Turn off all LEDs
void clearLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    Write_GPIO(leds[i].port, leds[i].pin, LOW);
  }
}
