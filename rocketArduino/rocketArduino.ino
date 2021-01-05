#include <SPI.h>
#include <LoRa.h>

// nss 10
// reset 9
// dio0 2
// miso 12
// mosi 11
// sck 13

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
  Serial.begin(9600);
  // LoRa init.
  if (!LoRa.begin(433E6))
  {
    Serial.println("ERROR: LoRa failed to begin");
    while (1);
  }
  Serial.println("LoRa begun");
  LoRa.setTxPower(18);
  LoRa.enableCrc();
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa.receive();
}

void loop()
{
  if (runEvery(500))
  {
    sendMessage();
  }
}

void sendMessage()
{
  Serial.println("Sending package no: " + String(data.packageNo));
  LoRa.beginPacket();
  LoRa.write((uint8_t *)&data, sizeof(data));
  LoRa.endPacket(true);
  data.packageNo++;
}

void onReceive(int packageSize)
{
  if (!packageSize) return;
  Serial.println("Received a package with " + String(packageSize) + " bytes size");
  String incoming = "";
  while (LoRa.available())
  {
    incoming += (char)LoRa.read(); //Note: Not sure if this is async or takes time.
  }
  if (incoming == abortCode)
  {
    // Abort sequence
    Serial.println("Initialising abort sequence");
    while (1);
  }
  Serial.println("Got the signal of: " + incoming);
}

void onTxDone()
{
  Serial.println("BASAWI");
  LoRa.receive(); // goes back to listening for abort signals
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
