#include <VSync.h>
#include <SPI.h>
#include <LoRa.h>

ValueReceiver<1> receiver;
ValueReceiver<2> sender;

int g_packageNo;
int abortte = 0;
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
  // LoRa init.
  if (!LoRa.begin(433E6))
  {
    while (1);
  }
  LoRa.setTxPower(18);
  LoRa.enableCrc();
  LoRa.receive();
  LoRa.onReceive(onReceive);

  // Processing connection init.
  Serial.begin(19200);
  receiver.observe(abortte);
  sender.observe(g_packageNo);
  sender.observe(rssiValue);
  sender.observe(snrValue);
}

void loop()
{
  receiver.sync();
  if (abortte == 1) // Abort sequence
  {
    while (1)
    {
      LoRa.beginPacket();
      LoRa.write("abrt");
      LoRa.endPacket();
      delay(100);
    }
  }

  sender.sync();
}

// When a package is received
void onReceive(int packageSize)
{
  if (packageSize)
  {
    LoRa.readBytes((uint8_t *)&data, sizeof(data));
    snrValue = int(LoRa.packetSnr() * 10);
    rssiValue = int(LoRa.rssi() * 10);
  }
}