#include <Arduino.h>
#include <util/delay.h>

// Assume Arduino Uno, digital pin 8 lives in Port B

#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_PIN PINB
#define LED_BIT _BV(0)
#define NOP __asm__("nop\n\t")

class Ai_WS2811 
{
  private:
    int m_nLeds;
    unsigned char m_nDataRate;
    unsigned long m_nCounter;
    unsigned char *m_pData;
    unsigned char *m_pDataEnd;

  public:
    byte _r, _g, _b;
    
    uint8_t *led_arr;
    void init(uint8_t,uint16_t);
    void sendLedData(void);
    unsigned char *getRGBData() { return m_pData; } 
};