#include<SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <OneWire.h>
#include <DallasTemperature.h>

static char send_payload[256];

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int min_payload_size = 4;
const int max_payload_size = 32;
const int payload_size_increments_by = 1;
int next_payload_size = min_payload_size;

RF24 radio(9,10);
const int role_pin=5;

const uint64_t pipes[2]={0xF0F0F0F0F0E1LL,0XF0F0F0F0D2LL};
char receive_payload[max_payload_size+1];
void setup()
{
  Serial.begin(115200);
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setRetries(5,15);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
}

void loop(void)
{
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  Serial.print("Sending Data: ");
  Serial.print(temperature);
  Serial.println(" C");
  delay(3000);
  String temp = String(temperature);
  static char send_payload[50];
  temp.toCharArray(send_payload,50);
  Serial.println(send_payload);

  radio.stopListening();

  Serial.print(F("Now sending length "));
  Serial.println(next_payload_size);
  radio.write(send_payload, next_payload_size);
  radio.startListening();
}
