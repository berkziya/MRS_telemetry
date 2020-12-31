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
}

void loop() {
  LoRa.beginPacket();
  LoRa.write((uint8_t*)&data, sizeof(data));
  LoRa.endPacket();
  delay(5000);

  data.packageNo++;
}
