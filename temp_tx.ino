/*
  Transmission module for continuous read of temperature data
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//Initialize Temperature
#define ONE_WIRE_BUS 2               // Data wire (yellow) to Digital 10 on Arduino
OneWire oneWire(ONE_WIRE_BUS);        // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature. 

//Initialize Radio Variables
RF24 radio(7,8);                    // nRF24L01+ radio attached using Getting Started board 
RF24Network network(radio);          // Network uses that radio
const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format
const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit
unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already

struct payload_t {                  // Structure of our payload
  unsigned long ms;
  unsigned long counter;
  float temp;
};

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Setting up digital temperature read...");
  sensors.begin();
  Serial.println("Setting up radio transmission..."); 
  SPI.begin();
  radio.begin();
  network.begin(90,this_node);
}

void loop() {
  
  network.update();                          // Check the network regularly  
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    Serial.print("Sending...");
    sensors.requestTemperatures();
    Serial.println(sensors.getTempCByIndex(0),4);
    payload_t payload = {millis(), packets_sent++, sensors.getTempCByIndex(0)};
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header,&payload,sizeof(payload));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }
}
