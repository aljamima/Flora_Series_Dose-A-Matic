#include <Wire.h>
#include <U8g2lib.h>
#include <Encoder.h>

// OLED setup
U8G2_SH1106_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

// Encoder
Encoder encoder(2, 3);  // CLK, DT

#define ENCODER_BTN 4
#define CONFIRM_BTN 5
#define BACK_BTN 6

#define RELAY_PINK   9
#define RELAY_PURPLE 8
#define RELAY_GREEN  7

long lastEncoderPos = 0;
int modeIndex = 0;
int gallonAmount = 1;

const int TIME_PER_5ML_MS = 2650;  // Calibrated timing per 5ml

const char* modeItems[] = {"AllPurpose", "Grow", "Bloom"};
const int modeCount = sizeof(modeItems) / sizeof(modeItems[0]);

enum ScreenState {
  MODE_SELECT,
  GALLON_SELECT,
  CONFIRM_SCREEN
};
ScreenState screenState = MODE_SELECT;

struct Dosing {
  int pink;
  int purple;
  int green;
};

Dosing dosingProfiles[] = {
  {1, 1, 1},    // AllPurpose
  {1, 2, 3},    // Grow
  {3, 2, 1}     // Bloom
};

void setup() {
  Serial.begin(115200);
  display.begin();
  display.setFont(u8g2_font_6x10_tr);

  pinMode(ENCODER_BTN, INPUT_PULLUP);
  pinMode(CONFIRM_BTN, INPUT_PULLUP);
  pinMode(BACK_BTN, INPUT_PULLUP);

  pinMode(RELAY_PINK, OUTPUT);
  pinMode(RELAY_PURPLE, OUTPUT);
  pinMode(RELAY_GREEN, OUTPUT);

  digitalWrite(RELAY_PINK, HIGH);
  digitalWrite(RELAY_PURPLE, HIGH);
  digitalWrite(RELAY_GREEN, HIGH);

  encoder.write(0);
  lastEncoderPos = 0;
  updateDisplay();
}

void loop() {
  long newPos = encoder.read() / 4;
  int delta = newPos - lastEncoderPos;

  if (delta != 0) {
    lastEncoderPos = newPos;
    if (screenState == MODE_SELECT) {
      modeIndex -= delta;
      if (modeIndex < 0) modeIndex = modeCount - 1;
      if (modeIndex >= modeCount) modeIndex = 0;
    } else if (screenState == GALLON_SELECT) {
      gallonAmount -= delta;
      if (gallonAmount < 1) gallonAmount = 100;
      if (gallonAmount > 100) gallonAmount = 1;
    }
    updateDisplay();
  }

  if (digitalRead(ENCODER_BTN) == LOW || digitalRead(CONFIRM_BTN) == LOW) {
    delay(150);
    if (screenState == MODE_SELECT) {
      screenState = GALLON_SELECT;
    } else if (screenState == GALLON_SELECT) {
      screenState = CONFIRM_SCREEN;
    } else if (screenState == CONFIRM_SCREEN) {
      runDosing(modeIndex, gallonAmount);
      screenState = MODE_SELECT;
    }
    encoder.write(0);
    lastEncoderPos = 0;
    updateDisplay();
  }

  if (digitalRead(BACK_BTN) == LOW) {
    delay(150);
    if (screenState == GALLON_SELECT) {
      screenState = MODE_SELECT;
    } else if (screenState == CONFIRM_SCREEN) {
      screenState = GALLON_SELECT;
    }
    encoder.write(0);
    lastEncoderPos = 0;
    updateDisplay();
  }
}

void runDosing(int modeIndex, int gallons) {
  Dosing d = dosingProfiles[modeIndex];

  unsigned long pinkTime   = d.pink * gallons * TIME_PER_5ML_MS;
  unsigned long purpleTime = d.purple * gallons * TIME_PER_5ML_MS;
  unsigned long greenTime  = d.green * gallons * TIME_PER_5ML_MS;

  display.firstPage();
  do {
    display.drawStr(0, 12, "Dosing Pink...");
  } while (display.nextPage());
  digitalWrite(RELAY_PINK, LOW); delay(pinkTime); digitalWrite(RELAY_PINK, HIGH);

  display.firstPage();
  do {
    display.drawStr(0, 12, "Dosing Purple...");
  } while (display.nextPage());
  digitalWrite(RELAY_PURPLE, LOW); delay(purpleTime); digitalWrite(RELAY_PURPLE, HIGH);

  display.firstPage();
  do {
    display.drawStr(0, 12, "Dosing Green...");
  } while (display.nextPage());
  digitalWrite(RELAY_GREEN, LOW); delay(greenTime); digitalWrite(RELAY_GREEN, HIGH);

  display.firstPage();
  do {
    display.drawStr(0, 12, "Dosing Complete.");
  } while (display.nextPage());
  delay(2000);
}

void updateDisplay() {
  display.firstPage();
  do {
    display.setFont(u8g2_font_6x10_tr);
    if (screenState == MODE_SELECT) {
      display.drawStr(0, 12, "Select Mode:");
      for (int i = 0; i < modeCount; i++) {
        char line[32];
        sprintf(line, "%c %s", (i == modeIndex ? '>' : ' '), modeItems[i]);
        display.drawStr(0, 24 + i * 10, line);
      }
    } else if (screenState == GALLON_SELECT) {
      display.drawStr(0, 12, "How many gallons?");
      char g[10];
      sprintf(g, "%d", gallonAmount);
      display.drawStr(30, 32, g);
    } else if (screenState == CONFIRM_SCREEN) {
      char line1[32], line2[32];
      sprintf(line1, "Mode: %s", modeItems[modeIndex]);
      sprintf(line2, "Gallons: %d", gallonAmount);
      display.drawStr(0, 12, "Confirm:");
      display.drawStr(0, 24, line1);
      display.drawStr(0, 36, line2);
      display.drawStr(0, 54, "Press confirm to run");
    }
  } while (display.nextPage());
}