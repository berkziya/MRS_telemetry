#include <SPI.h>
#include <LoRa.h>

// sck 13
// miso 12
// mosi 11
// nss 10
// reset 9
// dio0 2

// the package we are sending
typedef struct
{
  unsigned int packageNo = 0;
  unsigned int randomBullshitGo0 = 99;  // 0 to 65,535
  unsigned int randomBullshitGo1 = 33;
  unsigned int randomBullshitGo2 = 66;
  uint16_t checksum = 0;
} loraPacket;

loraPacket sent;
loraPacket data;

void setup()
{
  Serial.begin(9600);
  if (!LoRa.begin(433E6))  // LoRa init.
  {
    Serial.println("ERROR: LoRa failed to begin");
    while (1);
  }
  Serial.println("LoRa begun");
  // LoRa.enableCrc();
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa.receive();
}

void loop()
{
  if (runEvery(600   + random(-50, 50)))
  {
    sendMessage();
  }
}

void sendMessage()
{
  Serial.println("Sending package no: " + String(sent.packageNo));
  LoRa.beginPacket();
  LoRa.setTxPower(21);
  
  // Calculate the checksum for the packet
  sent.checksum = calculateChecksum(&sent);
  
  // Send the packet
  LoRa.write((uint8_t *)&sent, sizeof(sent));
  LoRa.endPacket(true);
  
  sent.packageNo++;
}

void onReceive(int packageSize)
{
  if (!packageSize) return;
  // Serial.println("                   Received a package with " + String(packageSize) + " bytes size");
  LoRa.readBytes((uint8_t *)&data, sizeof(data));
  
  // Check for checksum
  if (data.checksum != calculateChecksum(&data))
  {
    Serial.println("received a corrupted package");
    return
  }
  
  Serial.println("                   package: " + String(data.packageNo) + " | snr: " + String(LoRa.packetSnr()) + " | rssi: " + String(LoRa.rssi()));
  Serial.println("                   message: " + String(data.randomBullshitGo0) + " | " + String(data.randomBullshitGo1) + " | " + String(data.randomBullshitGo2));
}

void onTxDone()
{
  // Serial.println("TRANSMISSION DONE");
  LoRa.receive();  // goes back to listening for incoming signals
}

boolean runEvery(unsigned long interval)  // returns true when interval amount of time passed
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

uint16_t calculateChecksum(loraPacket *packet) {
  uint16_t checksum = 0;
  uint8_t *p = (uint8_t *)packet;
  for (int i = 0; i < sizeof(packet) - sizeof(packet->checksum); i++) {
    checksum += p[i];
  }
  return checksum;
}
