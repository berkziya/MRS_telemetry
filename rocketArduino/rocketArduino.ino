#include <SPI.h>
#include <LoRa.h>

typedef struct {
  int16_t packageNo = 0;
  float randomBullshitGo = 33.35;
  String mamma = "mia";
}
LoRa_Package;

LoRa_Package data;

void setup() {
  if (!LoRa.begin(433E6)) {
    while (1);
  }
  LoRa.setTxPower(20);
}

void loop() {
  if(runEvery(300)) {
    LoRa.beginPacket();
    LoRa.write((uint8_t*)&data, sizeof(data));
    LoRa.endPacket(true);
  }

  data.packageNo++;
}

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
