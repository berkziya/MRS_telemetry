#include <VSync.h>
#include <SPI.h>
#include <LoRa.h>

// nss 10
// reset 9
// dio0 2
// miso 12
// mosi 11
// sck 13

ValueReceiver<1> receiver;
ValueReceiver<4> sender;

bool isProcessing = false;

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
    if (!isProcessing) Serial.println("ERROR: LoRa failed to begin");
    while (1);
  }
  if (!isProcessing) Serial.println("LoRa begun");
  LoRa.enableCrc();
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa.receive();

  // Processing connection init.
  if (isProcessing)
  {
    receiver.observe(isAbort);
    sender.observe(p_packageSize);
    sender.observe(p_packageNo);
    sender.observe(rssiValue);
    sender.observe(snrValue);
  }
}

void loop()
{
  if (isProcessing) receiver.sync(); // Processing get values

  while (isAbort)
  {
    if (!isProcessing) Serial.println("Sending abort signal!");
    LoRa.beginPacket();
    LoRa.setTxPower(18, PA_OUTPUT_RFO_PIN);
    LoRa.print("abrt");
    LoRa.endPacket();
    delay(100);
  }

  if (isProcessing) sender.sync(); // Processing send values
}

// When a package is received
void onReceive(int packageSize)
{
  if (!packageSize) return;
  if (!isProcessing) Serial.println("Received a package with " + String(packageSize) + " bytes size");
  p_packageSize = packageSize;
  LoRa.readBytes((uint8_t *)&data, sizeof(data));
  snrValue = int(LoRa.packetSnr());
  rssiValue = int(LoRa.rssi());
  p_packageNo = data.packageNo;
  if (!isProcessing) Serial.println("package: " + String(data.packageNo) + " | snr: " + String(snrValue) + " | rssi: " + String(rssiValue));
}

void onTxDone()
{
  if (!isProcessing) Serial.println("BASAWI");
}
