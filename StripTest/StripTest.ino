#include "Ai_WS2811.h"

#define NUM_PIXELS 16
#define DATA_PIN 8

Ai_WS2811 ws2811;

byte next_t1 = 34;
byte next_t2 =  1;

byte colors = 13;
byte palette[] = {
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x1F,
  0x00, 0x00, 0x3F,
  0x00, 0x0F, 0x7F,
  0x00, 0x1F, 0x8F,
  0x00, 0x3F, 0x9F,
  0x00, 0x5F, 0xAF,
  0x00, 0x7F, 0xBF,
  0x3F, 0x9F, 0xCF,
  0x7F, 0xBF, 0xDF,
  0xBF, 0xDF, 0xEF,
  0xFF, 0xFF, 0xFF
};


struct CRGB {
  unsigned char g;
  unsigned char r;
  unsigned char b;
} *leds;

float *colorBuffer;

//some initial values
void setup() {
  colorBuffer = (float*)malloc(NUM_PIXELS * sizeof(float));
  for(byte i=0; i<NUM_PIXELS; i++) {
    colorBuffer[i] = 0.0;
  }
  
  ws2811.init(DATA_PIN,NUM_PIXELS);
  leds = (struct CRGB*)ws2811.getRGBData();
  
}

void loop(){
  rainbow();
}

/**
 * Color climb function
 **/
void rainbow(){
  while(1) {
    
      for (int i = 0; i < NUM_PIXELS; i++) {
           blendRGB(0, i, 100.0);
//        colorBuffer[i] = colorBuffer[i] - 2.0;
      }
      
      if(!(millis()%50)) blendRGB(random(0,255), random(0, NUM_PIXELS), 1.0);
             
      ws2811.sendLedData();
  }
}

void setRGB(int r, int g, int b, uint8_t n) {
  leds[n].r = constrain(r,0,255);
  leds[n].g = constrain(g,0,255);
  leds[n].b = constrain(b,0,255);
}

void blendRGB(int v, uint8_t n, float blending) {
  
    colorBuffer[n]+= (constrain(v,0,255)-colorBuffer[n])/blending;
  
    setHue((byte)colorBuffer[n]*2%255, n, colorBuffer[n]);

  
//  byte color_id = colorBuffer[n] / 255.0 * colors;
//  leds[n].r+= (palette[color_id*3+0]-leds[n].r)>>2;
//  leds[n].g+= (palette[color_id*3+1]-leds[n].g)>>2;
//  leds[n].b+= (palette[color_id*3+2]-leds[n].b)>>2;
      
//  leds[n].r = colorBuffer[n];
//  leds[n].g = colorBuffer[n];
//  leds[n].b = colorBuffer[n];
}

/**
 * HVS to RGB comversion (simplified to the range 0-255)
 **/
void setHue(int h, uint8_t n, float alpha) {
  //this is the algorithm to convert from RGB to HSV
  double r=0; 
  double g=0; 
  double b=0;

  double hf=h/42.5; // Not /60 as range is _not_ 0-360

  int i=(int)floor(h/42.5);
  double f = h/42.5 - i;
  double qv = 1 - f;
  double tv = f;

  switch (i)
  {
  case 0: 
    r = 1;
    g = tv;
    break;
  case 1: 
    r = qv;
    g = 1;
    break;
  case 2: 
    g = 1;
    b = tv;
    break;
  case 3: 
    g = qv;
    b = 1;
    break;
  case 4:
    r = tv;
    b = 1;
    break;
  case 5: 
    r = 1;
    b = qv;
    break;
  }

  leds[n].r = constrain((int)(alpha * r), 0, 255);
  leds[n].g = constrain((int)(alpha * g), 0, 255);
  leds[n].b = constrain((int)(alpha * b), 0, 255);
}
