#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

char inputBuffer[16];
int bufferIndex = 0;
char operation = 0;
long firstNumber = 0;
long secondNumber = 0;
boolean secondNumStart = false;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.display();
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    if (key >= '0' && key <= '9') {
      if (bufferIndex < sizeof(inputBuffer) - 1) {
        inputBuffer[bufferIndex++] = key;
        inputBuffer[bufferIndex] = '\0';
        displayInput();
      }
    } else if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
      if (bufferIndex > 0 && !secondNumStart) {
        firstNumber = atol(inputBuffer);
        operation = key;
        secondNumStart = true;
        clearBuffer();
      }
    } else if (key == '#') {
      if (secondNumStart) {
        secondNumber = atol(inputBuffer);
        long result = performCalculation(firstNumber, secondNumber, operation);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(result);
        display.display();
        clearBuffer();
        secondNumStart = false;
      }
    } else if (key == '*') {
      clearBuffer();
      display.clearDisplay();
      display.display();
      secondNumStart = false;
    }
  }
}

void displayInput() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(inputBuffer);
  display.display();
}

void clearBuffer() {
  memset(inputBuffer, 0, sizeof(inputBuffer));
  bufferIndex = 0;
}

long performCalculation(long num1, long num2, char op) {
  switch (op) {
    case 'A': return num1 + num2;
    case 'B': return num1 - num2;
    case 'C': return num1 * num2;
    case 'D': if (num2 != 0) return num1 / num2; else return 0;
    default: return 0;
  }
}
