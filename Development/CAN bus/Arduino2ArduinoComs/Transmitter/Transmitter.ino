/* This is the transmitter
 *  On the MEGA
 */

#include <SPI.h>
#include <mcp_can.h>

#define CAN_2515

const int spiCSPin = 53;
int ledHIGH    = 1;
int ledLOW     = 0;

//MCP_CAN CAN(spiCSPin);

#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(spiCSPin); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(spiCSPin); // Set CS pin
#endif

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))
    {
        Serial.println("CAN BUS init Failed");
        delay(100);
    }
    Serial.println("CAN BUS Shield Init OK!");
}

unsigned char stmp[8] = {ledHIGH, 1, 2, 3, ledLOW, 5, 6, 7};
    
void loop()
{   
  Serial.println("In loop");
  CAN.sendMsgBuf(0x43, 0, 8, stmp);
  delay(1000);
}
