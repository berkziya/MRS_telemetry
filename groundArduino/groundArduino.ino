#include <VSync.h>
#include <SPI.h>
#include <LoRa.h>

ValueReceiver<1> receiver;
ValueReceiver<4> sender;

int isAbort = 0;
int p_packageNo = 0;
int p_packageSize = 0;
int snrValue = 0, rssiValue = 0;

// the package
typedef struct
{
  int16_t packageNo;
  float randomBullshitGo;
  String mamma;
} LoRa_Package;

LoRa_Package data;

void setup()
{
  Serial.begin(9600);
  // LoRa init.
  if (!LoRa.begin(433E6))
  {
    while (1);
  }
  LoRa.setTxPower(18);
  LoRa.enableCrc();
  LoRa.onReceive(onReceive);
  LoRa.receive();

  // Processing connection init.
  receiver.observe(isAbort);
  sender.observe(p_packageSize);
  sender.observe(p_packageNo);
  sender.observe(rssiValue);
  sender.observe(snrValue);
}

void loop()
{
  receiver.sync(); // Processing get values

  if (isAbort == 1) // Abort sequence
  {
    while (1)
    {
      LoRa.beginPacket();
      LoRa.print("abrt");
      LoRa.endPacket();
      delay(100);
    }
  }

  sender.sync(); //Processing send values
}

// When a package is received
void onReceive(int packageSize)
{
  if (!packageSize) return;
  p_packageSize = packageSize;
  LoRa.readBytes((uint8_t *)&data, sizeof(data));
  snrValue = int(LoRa.packetSnr());
  rssiValue = int(LoRa.rssi());
}