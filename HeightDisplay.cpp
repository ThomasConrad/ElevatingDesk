#include "HeightDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

HeightDisplay::HeightDisplay()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}

void HeightDisplay::init() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        return;
    }
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

void HeightDisplay::showHeight(float heightMM) {
    clearDisplay();
    
    display.setCursor(0, 0);
    display.print(heightMM, 1);
    display.println(" mm");
    
    display.display();
}

void HeightDisplay::showMessage(const char* message) {
    clearDisplay();
    
    display.setCursor(0, 0);
    display.println(message);
    
    display.display();
}

void HeightDisplay::clearDisplay() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
}
