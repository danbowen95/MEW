/* This writes the data
 * 
 * Upload to the MEGA
 * SC pin = 10
 */
 
#include <SPI.h>
#include <mcp2515.h>

int i;

struct can_frame canMsg1;
struct can_frame canMsg2;
MCP2515 mcp2515(53);


void setup() {
  canMsg1.can_id  = 0x0F6; // 246
  canMsg1.can_dlc = 8;     // 8
  canMsg1.data[0] = 0x8E;  
  canMsg1.data[1] = 0x87;
  canMsg1.data[2] = 0x32;
  canMsg1.data[3] = 0xFA;
  canMsg1.data[4] = 0x26;
  canMsg1.data[5] = 0x8E;
  canMsg1.data[6] = 0xBE;
  canMsg1.data[7] = 0x86;

  canMsg2.can_id  = 0x036; // 54
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 0x0E;
  canMsg2.data[1] = 0x00;
  canMsg2.data[2] = 0x00;
  canMsg2.data[3] = 0x08;
  canMsg2.data[4] = 0x01;
  canMsg2.data[5] = 0x00;
  canMsg2.data[6] = 0x00;
  canMsg2.data[7] = 0xA0;
  
  while (!Serial);
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("Example: Write to CAN");
}

void loop() {
  mcp2515.sendMessage(&canMsg1);
  mcp2515.sendMessage(&canMsg2);

  Serial.print("Messages sent, i = ");
  Serial.println(i);
  i = i + 1;
  
  delay(3000);
}
