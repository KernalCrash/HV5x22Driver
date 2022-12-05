/*
 * Description: Example sketch to demonstrate the HV5x22 driver library usuage
 * Author     : Michael Scholz
 * eMail      : Michael_Scholz@hotmail.com 
 * Creation   : 2022.12.02 
 * Modified   : ---
*/
#include <HV5x22Driver.h>

// Shift Register MCU interface, ESP32 NodeMCU DevBoard
#define SHR_DAT 13
#define SHR_STR 25
#define SHR_CLK 33
#define SHR_OE   4

// Shift Register object
HV5x22Driver shiftReg = HV5x22Driver(SHR_DAT, SHR_CLK, SHR_OE, SHR_STR);

void setup() {
  Serial.begin(115200);
  Serial.printf("HV5x22 Shift Register Test\n");
  
  // Shift register clear / reset
  shiftReg.clearShr();
}

void loop() {
  for(int i=0; i<10; i++) {
    shiftReg.send((uint32_t)(1<<i), HV5x22Driver::BitOrder::MSBITFIRST);
    vTaskDelay(500);
  }
}
