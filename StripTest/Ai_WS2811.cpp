#include "Ai_WS2811.h"

void Ai_WS2811::init(uint8_t pin, uint16_t nPixels) 
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  m_nLeds = nPixels * 3; 
  m_nCounter = 0; 
  m_pData = (unsigned char*)malloc(m_nLeds); 
  memset(m_pData,0,m_nLeds); 
  m_pDataEnd = m_pData + m_nLeds; 
}

void Ai_WS2811::sendLedData(void) 
{
  cli();
  register byte *p = m_pData;
  register byte *e = m_pDataEnd;
  volatile uint8_t b;
  while(p != e) 
  { 
    b   = *p++;    // Current byte value
    byte i=8;
    do {
      if ((b&0x80)==0) {
        // Send a '0'
        if (F_CPU==16000000L) {
          LED_PIN=LED_BIT;NOP;// Hi (start)
          NOP;NOP;            // Hi
          LED_PIN=LED_BIT;NOP;// Lo (250ns)
          NOP;NOP;            // Lo
          NOP;NOP;            // Lo (500ns)
        }   
        else if (F_CPU==8000000L) {
          LED_PIN = LED_BIT;  // Hi (start)
          NOP;                // Hi
          LED_PIN = LED_BIT;  // Lo (250ns)
          NOP;                // Lo
          NOP;                // Lo (500ns)
          NOP;                // Lo (data bit here!)  
          NOP;                // Lo (750ns)
        }   
      }   
      else {
        // Send a '1'
        if (F_CPU==16000000L) {
          LED_PIN=LED_BIT;NOP;// Hi (start)
          NOP;NOP;            // Hi
          NOP;NOP;            // Hi (250ns)
          NOP;NOP;            // Hi
          NOP;NOP;            // Hi (500ns)
          LED_PIN=LED_BIT;    // Lo (625ns)
        }   
        else if (F_CPU==8000000L) {
          LED_PIN = LED_BIT;  // Hi (start)
          NOP;                // Hi
          NOP;                // Hi (250ns)
          NOP;                // Hi
          NOP;                // Hi (500ns)
          NOP;                // Hi (data bit here!)
          LED_PIN = LED_BIT;  // Lo (750ns)
        }   
      }   
      b = b+b;
    } while (--i!=0);
  }
  sei();
}