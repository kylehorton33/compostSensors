#include <SPI.h>
#include <RF24Network.h>
#include <RF24.h>

unsigned long time_now = 0;
RF24 radio(7, 8);
RF24Network network(radio);
const uint16_t this_node = 00;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node = 01;   // Address of the other node in Octal format


void setup(void) {
Serial.begin(115200);
radio.begin();
SPI.begin();
network.begin(90,this_node);
}

void loop(void) { 
  
    network.update();
    float temp = 0;
    
    while (network.available() ) {
        RF24NetworkHeader header;
        network.read(header,&temp,sizeof(float));
        time_now = millis()/1000;
        Serial.print("Current temperature is: ");
        Serial.print(temp);
        Serial.print(" C (after ");
        Serial.print(time_now);
        Serial.println(" s)");

    }
}
