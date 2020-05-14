#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SenseBoxMCU.h"
#include <math.h>

#define OLED_RESET 4

String dir[8] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
int dirnum = 0;
int samplesize = 60;
int imageselector = 1;

HDC1080 hdc;
VEML6070 vml;
TSL45315 lux_sensor;
Adafruit_SSD1306 display(OLED_RESET);

//Bild-Hexcodes

// 'sensebox', 128x50px
const unsigned char logo1 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x70, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x70, 0x00, 0x00, 0x00, 0x01, 0xce, 0x00, 0x00, 
  0x00, 0x78, 0x1c, 0x39, 0xc0, 0xf0, 0x38, 0x38, 0x70, 0x70, 0x73, 0xe0, 0x07, 0xce, 0x01, 0x80, 
  0x00, 0xfe, 0x7e, 0x3f, 0xe1, 0xfc, 0x7e, 0x38, 0x71, 0xfc, 0xfb, 0xe0, 0x0e, 0xe0, 0x63, 0xc0, 
  0x01, 0xfe, 0xff, 0x3f, 0xf3, 0xfc, 0xff, 0x3f, 0xe3, 0xfe, 0xfb, 0xe0, 0x17, 0xe0, 0xe3, 0xe0, 
  0x01, 0xce, 0xe3, 0x1c, 0x73, 0x9d, 0xc7, 0x3f, 0xf3, 0x8e, 0x3d, 0x80, 0x3b, 0x6c, 0x41, 0xf0, 
  0x01, 0xc0, 0xc3, 0x9c, 0x73, 0xc1, 0xc3, 0x38, 0x73, 0x8e, 0x1f, 0x00, 0x5b, 0x6e, 0x1f, 0xf8, 
  0x01, 0xf8, 0xff, 0x9c, 0x71, 0xf9, 0xff, 0x38, 0x3b, 0x0e, 0x0f, 0x00, 0x5b, 0x74, 0x1f, 0xf8, 
  0x00, 0x7c, 0xff, 0x1c, 0x70, 0xfd, 0xff, 0x38, 0x3b, 0x0e, 0x1f, 0x00, 0x3b, 0x60, 0x8f, 0xf0, 
  0x03, 0x8e, 0xe0, 0x1c, 0x73, 0x1d, 0xc0, 0x38, 0x3b, 0x8e, 0x3b, 0x80, 0x17, 0x61, 0xc1, 0xe0, 
  0x03, 0x8e, 0xe3, 0x1c, 0x73, 0x1d, 0xe2, 0x38, 0xf3, 0x9e, 0x73, 0xc0, 0x07, 0xe0, 0xc3, 0xc0, 
  0x03, 0xfc, 0x7f, 0x3e, 0xfb, 0xfc, 0xff, 0x7f, 0xf1, 0xfc, 0xfd, 0xe0, 0x06, 0xc0, 0x01, 0x80, 
  0x01, 0xf8, 0x3e, 0x3e, 0xf9, 0xf0, 0x7e, 0x7f, 0xc0, 0xf8, 0xf9, 0xe0, 0x01, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x9f, 0xe0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf8, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf8, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'afg', 128x50px
const unsigned char logo2 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0xff, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0x00, 
  0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xfc, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0xc7, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xfc, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0x87, 0xff, 0xe7, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x7e, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0x87, 0xff, 0xe7, 0xff, 0xc0, 0xff, 0xff, 0xff, 0xf8, 0x18, 0x3e, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0x83, 0xff, 0xc7, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xf0, 0xfe, 0x1f, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0x83, 0xff, 0xc7, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xf1, 0xff, 0x3f, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0x01, 0xff, 0xcf, 0xff, 0xc7, 0xdf, 0xf7, 0xff, 0xe3, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0x7f, 0xff, 0x31, 0xff, 0xcf, 0xff, 0xc7, 0xff, 0xf7, 0xff, 0xe3, 0xff, 0xff, 0x80, 0x00, 
  0x00, 0x7f, 0xfe, 0x31, 0xff, 0xcf, 0xff, 0xc7, 0xff, 0xf3, 0xff, 0xe3, 0xff, 0xff, 0x80, 0x00, 
  0x00, 0x7f, 0xfe, 0x38, 0xff, 0x8f, 0xff, 0x8f, 0xff, 0xf3, 0xff, 0xe7, 0xff, 0xff, 0xc0, 0x00, 
  0x00, 0x7f, 0xfe, 0x38, 0xff, 0x8f, 0xff, 0x87, 0xff, 0xf1, 0xff, 0xe7, 0xff, 0xff, 0xc0, 0x00, 
  0x00, 0x7f, 0xfc, 0x78, 0xff, 0x9f, 0xff, 0x80, 0x7f, 0xf8, 0xff, 0xe7, 0xf8, 0x0f, 0xc0, 0x00, 
  0x00, 0x7f, 0xfc, 0x7c, 0x7f, 0x9f, 0xff, 0x80, 0x3f, 0xf8, 0xff, 0xe7, 0xf8, 0x0f, 0xe0, 0x00, 
  0x00, 0x7f, 0xf8, 0x7c, 0x7f, 0x1f, 0xff, 0x80, 0x3f, 0xf8, 0x7f, 0xe7, 0xff, 0x0f, 0xe0, 0x00, 
  0x00, 0x7f, 0xf8, 0x78, 0x7f, 0x1f, 0xff, 0x0f, 0xff, 0xf8, 0x7f, 0xe3, 0xff, 0x8f, 0xf0, 0x00, 
  0x00, 0x7f, 0xf8, 0x00, 0x3f, 0x1f, 0xff, 0x1f, 0xff, 0xf8, 0x3f, 0xe1, 0xff, 0x1f, 0xf0, 0x00, 
  0x00, 0x7f, 0xf8, 0x00, 0x3f, 0x3f, 0xff, 0x1f, 0xff, 0xf8, 0x3f, 0xf1, 0xff, 0x1f, 0xf0, 0x00, 
  0x00, 0x7f, 0xf0, 0xfe, 0x1e, 0x3f, 0xff, 0x1f, 0xff, 0xf8, 0x1f, 0xf0, 0xfe, 0x1f, 0xf8, 0x00, 
  0x00, 0x7f, 0xf1, 0xff, 0x1e, 0x3f, 0xff, 0x1f, 0xff, 0xf8, 0x0f, 0xf8, 0x7c, 0x3f, 0xfc, 0x00, 
  0x00, 0x7f, 0xe1, 0xff, 0x1e, 0x3f, 0xff, 0x1f, 0xff, 0xf8, 0x0f, 0xfc, 0x00, 0x7f, 0xfc, 0x00, 
  0x00, 0x7f, 0xe3, 0xff, 0x0e, 0x3f, 0xff, 0x1f, 0xff, 0xf8, 0x07, 0xfe, 0x01, 0xff, 0xfc, 0x00, 
  0x00, 0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0x1f, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xff, 0xfc, 0x00, 
  0x00, 0x7f, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0x1f, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xff, 0xfe, 0x00, 
  0x00, 0x1f, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xff, 0xfe, 0x00, 
  0x00, 0x0f, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};




void setup() {
  Serial.begin(115200);
  Wire.begin();
  senseBoxIO.statusRed();
  senseBoxIO.powerNone();
  while (!Serial);
  if (Serial) {
    senseBoxIO.statusNone();
    senseBoxIO.statusGreen();
    delay(250);
    senseBoxIO.powerXB1(true);
    delay(250);
    senseBoxIO.powerUART(true);
    delay(250);
    senseBoxIO.powerI2C(true);
    delay(250);
    senseBoxIO.powerXB2(true);
    //Serial.print("Samplesize: ");
    //Serial.println(samplesize);
  }
  hdc.begin();
  vml.begin();
  lux_sensor.begin();
  delay(2000);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.display();
  delay(100);
  display.clearDisplay();
}

void loop() {
  int speeds[60];
  int dirs[60];
  int temps[60];
  int humis[60];
  int uvs[60];
  int lights[60];
  for (int i; i <= samplesize; i ++) {
    speeds[i] = random(80);
    dirs[i] = random(8);
    temps[i] = hdc.getTemperature();
    humis[i] = hdc.getHumidity();
    uvs[i] = vml.getUvIntensity();
    lights[i] = lux_sensor.getIlluminance();
    Serial.print("Took Sample Num.: ");
    Serial.println(i);
    delay(1000);
  }
  int avgdir = round(average(dirs, 60));
  if (avgdir > 8){
    avgdir = 8;
  }
  int avgtemp = round(average(temps, 60));
  int avghumi = round(average(humis, 60));
  int avguv = round(average(uvs, 60));
  int avglight = round(average(lights, 60));
  
  Serial.print("Wind ");
  Serial.println(avgdir);
  Serial.print("Speed ");
  Serial.println(round(average(speeds, 60)));
  Serial.print("Temp ");
  Serial.println(avgtemp);
  Serial.print("Humi ");
  Serial.println(avghumi);
  Serial.print("UV-raw ");
  Serial.println(avguv);
  Serial.print("UV-Index" );
  Serial.println(getUVI(avguv));
  Serial.print("Lux ");
  Serial.println(avglight);
  Serial.println();

  switch(imageselector){
    case 1:
      display.clearDisplay(); //for Clearing the display
      display.drawBitmap(0, 0, logo1, 128, 50, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
      display.display();
      imageselector ++;
      break;
    case 2:
      display.clearDisplay(); //for Clearing the display
      display.drawBitmap(0, 0, logo2, 128, 50, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
      display.display();
      imageselector ++;
      break;
    default:
      imageselector = 1;
      break;
  }
  if (!Serial) {
    display.clearDisplay();
    senseBoxIO.statusNone();
    senseBoxIO.statusRed();
    delay(250);
    senseBoxIO.powerXB1(false);
    delay(250);
    senseBoxIO.powerUART(false);
    delay(250);
    senseBoxIO.powerI2C(false);
    delay(250);
    senseBoxIO.powerXB2(false);
    setup();
  }
}

float average (int * array, int len)  // assuming array is int.
{
  long sum = 0L ;  // sum will be larger than an item, long for safety.
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return  ((float) sum) / len ;  // average will be fractional, so float may be appropriate.
}

float getUVI(int uv) {//vml into Uv-Index

  float refVal = 0.4; // Referenzwert: 0,01 W/m^2 ist äquivalent zu 0.4 als UV-Index
  float uvi = refVal * (uv * 5.625) / 1000;
  return uvi;
}
