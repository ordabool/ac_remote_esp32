#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>
#include <string.h>

int receiver = 13;
const uint16_t MinUnknownSize = 12;

IRrecv irrecv(receiver);
decode_results results;

void setup() {
Serial.begin(9600);
irrecv.setUnknownThreshold(MinUnknownSize);
irrecv.enableIRIn();
}

void loop() {
    if (irrecv.decode(&results)) {
        Serial.println("--- Entering measurement: ---");
        Serial.println(typeToString(results.decode_type, results.repeat));
        Serial.println(resultToHexidecimal(&results));
        irrecv.resume();
    }
    delay(500);
}
