#include <SPI.h>
#include <LoRa.h>

String abortCode = "abrt";

// the package
typedef struct
{
  int16_t packageNo = 0;
  float randomBullshitGo = 33.35;
  String mamma = "mia";
} LoRa_Package;

LoRa_Package data;

void setup()
{
  // LoRa init.
  if (!LoRa.begin(433E6))
  {
    while (1);
  }
  LoRa.setTxPower(20);
  LoRa.onReceive(onReceive);
  LoRa.receive();
}

void loop()
{
  if (runEvery(200))
  {
    sendMessage();
    data.packageNo++;
    LoRa.receive(); // Goes back to receive mode for an abort signal
  }
}

void sendMessage()
{
  LoRa.beginPacket();
  LoRa.write((uint8_t *)&data, sizeof(data));
  LoRa.endPacket(true);
}

void onReceive(int packageSize)
{
  if (packetSize == 0) return;
  String incoming = "";

  while (LoRa.available())
  {
    incoming += (char)LoRa.read();
  }

  if (incoming == abortCode)
  {
    // Abort sequence
  }
}

// returns true when interval amount of time passed
boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
