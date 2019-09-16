#include <SPI.h>
#include <RF24Network.h>
#include <RF24.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

RF24 radio(7, 8);
RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;        // Address of our node
const uint16_t other_node = 00;       // Address of other node
const unsigned long interval = 2000;  // How often to send packets to the other unit

unsigned long last_sent;             // When did we last send?

void setup(void) {
  Serial.begin(115200);
  Serial.println("RF24Network/examples/helloworld_tx/");
  sensors.begin();
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);  // Channel set to 90, writing node set to "this_node"
}

void loop(void)
{

    network.update();                          // Check the network regularly

  
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;
    sensors.requestTemperatures();
    float temperature = sensors.getTempCByIndex(0);
    Serial.print("Sending...");
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header,&temperature,sizeof(float));
    if (ok){
      Serial.print("ok. Sent ");
      Serial.println(temperature);}
    else
      Serial.println("failed.");
  }
    
    
}
