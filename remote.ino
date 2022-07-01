#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>
#include <string.h>
#include <IRsend.h>
#include "Electra-AC-Remote/electraAcRemoteEncoder.h"
#include "Electra-AC-Remote/electraAcRemoteEncoder.c"

int receiver = 13;
int transmitter = 33;
int switchButton = 27;
const uint16_t MinUnknownSize = 12;

IRrecv irrecv(receiver);
decode_results results;

IRsend irsend(transmitter);

int lastButtonState = LOW;
int currentButtonState;

ac my_ac;
int fanState = 1;

void setup() {
    Serial.begin(9600);
    irrecv.setUnknownThreshold(MinUnknownSize);
    irrecv.enableIRIn();
    irsend.begin();
    pinMode(switchButton, INPUT_PULLUP);

}

void loop() {
    if (irrecv.decode(&results)) {
        Serial.println("--- Entering measurement: ---");
        Serial.println(typeToString(results.decode_type, results.repeat));
        Serial.println(resultToHexidecimal(&results));
        irrecv.resume();
    }

    currentButtonState = digitalRead(switchButton);
    // if (lastButtonState == LOW && currentButtonState == HIGH) {
    if (true) {
        // irsend.send(AIRWELL, 0x2704001AE, kAirwellBits, kAirwellMinRepeats);
        // Serial.println("Clicked!");

        int* code = getCodes(&my_ac, fanState, COOL, 23, ON, SWING_OFF);
        fanState = 4 - fanState;

        Serial.println("Code: ");-
        Serial.println(my_ac.fullState);
        Serial.println(*code);

        irsend.send(AIRWELL, my_ac.fullState, kAirwellBits, kAirwellMinRepeats); 
        
    }

    lastButtonState = currentButtonState;
        delay(2000);
}
