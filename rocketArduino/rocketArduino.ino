#include <SPI.h>
#include <LoRa.h>

// sck 13
// miso 12
// mosi 11
// nss 10
// reset 9
// dio0 2

String abortCode = "abrt";

// the package we are sending
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
  if (!LoRa.begin(433E6)) // LoRa init.
  {
    Serial.println("ERROR: LoRa failed to begin");
    while (1);
  }
  Serial.println("LoRa begun");
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
  LoRa.setTxPower(20, PA_OUTPUT_RFO_PIN);
  LoRa.write((uint8_t *)&data, sizeof(data));
  LoRa.endPacket(true);
  data.packageNo++;
}

void onReceive(int packageSize)
{
  if (!packageSize) return; // Ignore if the package is empty
  Serial.println("Received a package with " + String(packageSize) + " bytes size");

  String incoming = "";
  while (LoRa.available())
  {
    incoming += (char)LoRa.read(); // Note: Not sure if this is async or takes time.
  }
  if (incoming == abortCode) abort();// We got abort signal
  Serial.println("Got this signal: " + incoming);
}

void onTxDone()
{
  Serial.println("BASAWI");
  LoRa.receive(); // goes back to listening for abort signals
}

void abort() // Abort sequence
{
  Serial.println("Initialising abort sequence");
  // while (1);
}

boolean runEvery(unsigned long interval) // returns true when interval amount of time passed
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
