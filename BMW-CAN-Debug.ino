#include <SPI.h>
#include "mcp2515_can.h"

const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;

mcp2515_can CanBus(SPI_CS_PIN); // Set CS pin
#define MAX_DATA_SIZE 8

unsigned char msg[MAX_DATA_SIZE] = { 0x00, 0xF3, 0x04, 0x3F, 0xFF, 0xF0, 0xFF, 0xFF};

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  while (CAN_OK != CanBus.begin(CAN_500KBPS)) {             // init can bus : baudrate = 500k
      Serial.println("CanBus init failed; retrying...");
      delay(100);
  }

  CanBus.init_Mask(0, 0, 0x3ff);                         // there are 2 mask in mcp2515, you need to set both of them
  CanBus.init_Mask(1, 0, 0x3ff);

 // CAN.init_Filt(0, 0, 0x0A5);                          // there are 6 filter in mcp2515
  CanBus.init_Filt(0, 0, 0x23A);                          // there are 6 filter in mcp2515
  CanBus.init_Filt(1, 0, 0x21A);                          // there are 6 filter in mcp2515
  
  Serial.println("CanBus init ok!");
}

uint32_t id;
uint8_t  type; // bit0: ext, bit1: rtr
uint8_t  len;
byte data[MAX_DATA_SIZE] = {0};

void send_test_msg() {
  CanBus.sendMsgBuf(0x23A, 0, 8, msg);
}


void loop() {
  if (Serial.available() > 0) {
      byte in = Serial.read();

      Serial.print("Received input: ");
      Serial.println(in, DEC);

      if (in == 49) {
        send_test_msg();
        Serial.println("Sending test data.");
      }
  }

  if (CAN_MSGAVAIL != CanBus.checkReceive()) {
    return;
  }

  char prbuf[32 + MAX_DATA_SIZE * 3];
  int i, n;

  unsigned long t = millis();
  // read data, len: data length, buf: data buf
  CanBus.readMsgBuf(&len, data);

  id = CanBus.getCanId();
  type = (CanBus.isExtendedFrame() << 0) | (CanBus.isRemoteRequest() << 1);

  n = sprintf(prbuf, "%04lu.%03d ", t / 1000, int(t % 1000));

  //static const byte type2[] = {0x00, 0x02, 0x30, 0x32};
  //n += sprintf(prbuf + n, "RX: [%08lX](%02X) ", (unsigned long)id, type2[type]);
  n += sprintf(prbuf, "RX: [%08lX](%02X) ", id, type);

  for (i = 0; i < len; i++) {
    n += sprintf(prbuf + n, "%02X ", data[i]);
  }
  
  Serial.println(prbuf);
}
