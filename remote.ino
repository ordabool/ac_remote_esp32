#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>
#include <string.h>

// #define FULL_STATE_MASK 0x200000002
// #define ON_STATE_MASK   0x1
// #define FAN_MASK        0x3
// #define SWING_MASK      0x3
// #define MODE_MASK       0x7
// #define TEMP_MASK       0xF
// #define COOL            0x1
// #define HEAT            0x2
// #define SWING_OFF       0x0
// #define SWING_ON	    0x1
// #define SWING_SINGLE    0x2
// #define OFF             0x0
// #define ON              0x1

// #define DATA_BITS_LENGTH 34

int receiver = 13;
const uint16_t MinUnknownSize = 20;

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







// Serial.println(resultToHumanReadableBasic(&results));
// Serial.println(resultToSourceCode(&results));


// 22 degrees
// ON/OFF: 0x2603800A2             001001100000001110000000000010100010
// Minus to 22: 0x603800A2             01100000001110000000000010100010
// Plus to 23: 0x604000A2              01100000010000000000000010100010
// Plus to 24: 0x604800A2              01100000010010000000000010100010




// https://github.com/nryhbhue/Electra-AC-Remote
// https://www.instructables.com/Reverse-engineering-of-an-Air-Conditioning-control/