#include <VSync.h>
#include <SPI.h>
#include <LoRa.h>

ValueReceiver<1>receiver;
ValueReceiver<2> sender;

int g_packageNo;
int abortte = 0;
int snrValue, rssiValue;

typedef struct {
  int16_t packageNo;
  float randomBullshitGo;
  String mamma;
}
LoRa_Package;

LoRa_Package data;

void setup() {
  Serial.begin(19200);
  
  if (!LoRa.begin(433E6)) {
    while (1);
  }
  
  LoRa.receive();
  LoRa.onReceive(weGotPackage);
  
  receiver.observe(abortte);

  sender.observe(g_packageNo);
  sender.observe(rssiValue);
  sender.observe(snrValue);
}

void loop() {
  receiver.sync();
  
  rssiValue = int(LoRa.rssi()*10);
  
  sender.sync();
}

void weGotPackage(int packageSize) {  
  if (packageSize) {
    LoRa.readBytes((uint8_t *)&data, packageSize);
  }
  
  snrValue = int(LoRa.packetSnr()*10);
}
