#include <Arduino.h>
#include <U8g2lib.h>
#include <stdio.h>
#include "activity_32_32_28f.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

int cpuusa = 0;
int ramuse = 0;
int gpuuse = 0;
int vramuse = 0;
String serialLine = "";

void updateAnimations() {
    activity_32_32_28f_frame = millis() / 42 % 28;
}

void drawAnimation_activity_32_32_28f(void) {
    u8g2.setDrawColor(1);
    u8g2.drawXBMP(96, -8, 32, 32, activity_32_32_28f_frames[activity_32_32_28f_frame]);
}

void drawScreen_1(void) {
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.drawLine(0, 0, 0, 0);
    drawAnimation_activity_32_32_28f();
    u8g2.drawLine(0, 9, 97, 9);
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(2, 7, "Usage");
    u8g2.drawLine(64, 14, 64, 63);
    u8g2.drawLine(0, 36, 128, 36);
    u8g2.setFont(u8g2_font_6x13_tr);
    u8g2.drawStr(2, 26, "CPU");
    u8g2.drawStr(2, 50, "GPU");
    u8g2.drawStr(67, 26, "RAM");
    u8g2.drawStr(67, 51, "VRAM");
    u8g2.drawFrame(2, 27, 60, 6);
    u8g2.drawFrame(2, 53, 60, 6);
    u8g2.drawFrame(67, 27, 60, 6);
    u8g2.drawFrame(67, 53, 60, 6);
    u8g2.setFont(u8g2_font_6x12_tr);
    char buf[8];
    sprintf(buf, "%d%%", cpuusa);
    u8g2.drawStr(44, 26, buf);
    sprintf(buf, "%d%%", ramuse);
    u8g2.drawStr(109, 26, buf);
    sprintf(buf, "%d%%", gpuuse);
    u8g2.drawStr(44, 51, buf);
    sprintf(buf, "%d%%", vramuse);
    u8g2.drawStr(109, 51, buf);
    u8g2.drawBox(3, 28, map(constrain(cpuusa, 0, 100), 0, 100, 0, 58), 4);
    u8g2.drawBox(68, 28, map(constrain(ramuse, 0, 100), 0, 100, 0, 58), 4);
    u8g2.drawBox(3, 54, map(constrain(gpuuse, 0, 100), 0, 100, 0, 58), 4);
    u8g2.drawBox(68, 54, map(constrain(vramuse, 0, 100), 0, 100, 0, 58), 4);
}

void readSerialLine() {
    while (Serial.available() > 0) {
        char incoming = Serial.read();
        if (incoming == '\n') {
            int c, r, g, v;
            if (sscanf(serialLine.c_str(), "%d,%d,%d,%d", &c, &r, &g, &v) == 4) {
                cpuusa = constrain(c, 0, 100);
                ramuse = constrain(r, 0, 100);
                gpuuse = constrain(g, 0, 100);
                vramuse = constrain(v, 0, 100);
            }
            serialLine = "";
        } else if (incoming != '\r' && serialLine.length() < 80) {
            serialLine += incoming;
        }
    }
}

void setup() {
    Serial.begin(115200);
    u8g2.setI2CAddress(0x3C * 2);
    u8g2.begin();
}

void loop() {
    readSerialLine();
    updateAnimations();
    u8g2.firstPage();
    do {
        drawScreen_1();
    } while (u8g2.nextPage());
    delay(30);
}
