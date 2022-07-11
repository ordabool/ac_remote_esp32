#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>
#include <string.h>
#include <IRsend.h>
#include <WiFi.h>
#include "Electra-AC-Remote/electraAcRemoteEncoder.h"
#include "Electra-AC-Remote/electraAcRemoteEncoder.c"

const char* ssid = "EnchantedBunny";
const char* password = "cowrocks";

WiFiServer server(80);
String header;
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

int transmitter = 33;
IRsend irsend(transmitter);

const int FAN_LOW = kAirwellFanLow;
const int FAN_MEDIUM = kAirwellFanMedium;
const int FAN_HIGH = kAirwellFanHigh;
const int FAN_AUTO = kAirwellFanAuto;

const int COOL_MODE = kAirwellCool;
const int HEAT_MODE = kAirwellHeat;
const int DRY_MODE = kAirwellDry;
const int FAN_MODE = kAirwellFan;
const int AUTO_MODE = kAirwellAuto;

ac my_ac;
int modeValue = COOL_MODE;
int fanValue = FAN_MEDIUM;
int temperatureValue = 24;

void updateAC() {
    int* code = getCodes(&my_ac, fanValue, modeValue, temperatureValue, ON, SWING_OFF);
    unsigned  mask;
    mask = (1 << 32) - 1;
    int croppedState = my_ac.fullState & mask;
    irsend.send(AIRWELL, croppedState, kAirwellBits, kAirwellMinRepeats);
    Serial.println("Sent cropped code to AC!");
}

void setup() {
    Serial.begin(9600);
    irsend.begin();

    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
}

void loop() {
    WiFiClient client = server.available();   // Listen for incoming clients
    if (client) {                             // If a new client connects,
        currentTime = millis();
        previousTime = currentTime;
        Serial.println("New Client.");          // print a message out in the serial port
        String currentLine = "";                // make a String to hold incoming data from the client
        while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
        currentTime = millis();
        if (client.available()) {             // if there's bytes to read from the client,
            char c = client.read();             // read a byte, then
            Serial.write(c);                    // print it out the serial monitor
            header += c;
            if (c == '\n') {                    // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
                // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                // and a content-type so the client knows what's coming, then a blank line:
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();
                
                if (header.indexOf("GET /power/toggle") >= 0) {
                    irsend.send(AIRWELL, my_ac.fullState, kAirwellBits, kAirwellMinRepeats);
                    Serial.println("Sent code to AC!");
                }

                if (header.indexOf("GET /mode/cool") >= 0) {
                    modeValue = COOL_MODE;
                    updateAC();
                } else if (header.indexOf("GET /mode/heat") >= 0) {
                    modeValue = HEAT_MODE;
                    updateAC();
                } else if (header.indexOf("GET /mode/dry") >= 0) {
                    modeValue = DRY_MODE;
                    updateAC();
                } else if (header.indexOf("GET /mode/fan") >= 0) {
                    modeValue = FAN_MODE;
                    updateAC();
                } else if (header.indexOf("GET /mode/auto") >= 0) {
                    modeValue = AUTO_MODE;
                    updateAC();
                }

                if (header.indexOf("GET /fan/low") >= 0) {
                    fanValue = FAN_LOW;
                    updateAC();
                } else if (header.indexOf("GET /fan/medium") >= 0) {
                    fanValue = FAN_MEDIUM;
                    updateAC();
                } else if (header.indexOf("GET /fan/high") >= 0) {
                    fanValue = FAN_HIGH;
                    updateAC();
                } else if (header.indexOf("GET /fan/auto") >= 0) {
                    fanValue = FAN_AUTO;
                    updateAC();
                }

                if (header.indexOf("GET /temp/16") >= 0) {
                    temperatureValue = 16;
                    updateAC();
                } else if (header.indexOf("GET /temp/17") >= 0) {
                    temperatureValue = 17;
                    updateAC();
                } else if (header.indexOf("GET /temp/18") >= 0) {
                    temperatureValue = 18;
                    updateAC();
                } else if (header.indexOf("GET /temp/19") >= 0) {
                    temperatureValue = 19;
                    updateAC();
                } else if (header.indexOf("GET /temp/20") >= 0) {
                    temperatureValue = 20;
                    updateAC();
                } else if (header.indexOf("GET /temp/21") >= 0) {
                    temperatureValue = 21;
                    updateAC();
                } else if (header.indexOf("GET /temp/22") >= 0) {
                    temperatureValue = 22;
                    updateAC();
                } else if (header.indexOf("GET /temp/23") >= 0) {
                    temperatureValue = 23;
                    updateAC();
                } else if (header.indexOf("GET /temp/24") >= 0) {
                    temperatureValue = 24;
                    updateAC();
                } else if (header.indexOf("GET /temp/25") >= 0) {
                    temperatureValue = 25;
                    updateAC();
                } else if (header.indexOf("GET /temp/26") >= 0) {
                    temperatureValue = 26;
                    updateAC();
                } else if (header.indexOf("GET /temp/27") >= 0) {
                    temperatureValue = 27;
                    updateAC();
                } else if (header.indexOf("GET /temp/28") >= 0) {
                    temperatureValue = 28;
                    updateAC();
                } else if (header.indexOf("GET /temp/29") >= 0) {
                    temperatureValue = 29;
                    updateAC();
                } else if (header.indexOf("GET /temp/30") >= 0) {
                    temperatureValue = 30;
                    updateAC();
                }
                
                // Display the HTML web page
                client.println("<!DOCTYPE html><html>");
                client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                client.println("<link rel=\"icon\" href=\"data:,\">");
                client.println("<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.0-beta1/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-0evHe/X+R7YkIZDRvuzKMRqM+OrBnVFBL6DOitfPri4tjfHxaWutUpFmBp4vmVor\" crossorigin=\"anonymous\">");
                client.println("<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.0-beta1/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-pprn3073KE6tl6bjs2QrFaJGz5/SUsLqktiwsUTF55Jfv3qYSDhgCecCxMW52nD2\" crossorigin=\"anonymous\"></script>");
                client.println("<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.1.1/css/all.min.css\" integrity=\"sha512-KfkfwYDsLkIlwQp6LFnl8zNdLGxu9YAA1QvwINks4PhcElQSvqcyVLLD9aMhXd13uQjoXtEKNosOWaZqXgel0g==\" crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\" />");
                
                // Web Page Heading
                client.println("<body>");


                client.println("<nav class=\"navbar bg-primary navbar-dark\">");
                client.println("    <div class=\"container-fluid\">");
                client.println("        <span class=\"navbar-brand mb-0 h1\">AC Web Remote (ESP32)</span>");
                client.println("    </div>");
                client.println("</nav>");

                client.println("<div class=\"container\">");

                client.println("<div class=\"row\" style=\"margin: 35px 30px;\">");
                client.println("    <a href=\"/power/toggle\" class=\"btn btn-danger\" style=\"width: 100%;\"><i class=\"fa-solid fa-power-off\"></i> Power</a>");
                client.println("</div>");

                client.println("<div class=\"row\" style=\"margin: 15px;\">");
                client.println("        <div class=\"btn-group\">");
                if (modeValue == COOL_MODE) {
                    client.println("            <a href=\"/mode/cool\" class=\"btn btn-primary active\" aria-current=\"page\"><i class=\"fa-solid fa-snowflake\"></i> Cool</a>");
                    client.println("            <a href=\"/mode/heat\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-sun\"></i> Heat</a>");
                    client.println("            <a href=\"/mode/dry\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-wind\"></i> Dry</a>");
                    client.println("            <a href=\"/mode/fan\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\"></i> Fan</a>");
                    client.println("            <a href=\"/mode/auto\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-robot\"></i> Auto</a>");
                } else if (modeValue == HEAT_MODE) {
                    client.println("            <a href=\"/mode/cool\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-snowflake\"></i> Cool</a>");
                    client.println("            <a href=\"/mode/heat\" class=\"btn btn-primary active\" aria-current=\"page\"><i class=\"fa-solid fa-sun\"></i> Heat</a>");
                    client.println("            <a href=\"/mode/dry\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-wind\"></i> Dry</a>");
                    client.println("            <a href=\"/mode/fan\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\"></i> Fan</a>");
                    client.println("            <a href=\"/mode/auto\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-robot\"></i> Auto</a>");
                } else if (modeValue == DRY_MODE) {
                    client.println("            <a href=\"/mode/cool\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-snowflake\"></i> Cool</a>");
                    client.println("            <a href=\"/mode/heat\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-sun\"></i> Heat</a>");
                    client.println("            <a href=\"/mode/dry\" class=\"btn btn-primary active\" aria-current=\"page\"><i class=\"fa-solid fa-wind\"></i> Dry</a>");
                    client.println("            <a href=\"/mode/fan\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\"></i> Fan</a>");
                    client.println("            <a href=\"/mode/auto\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-robot\"></i> Auto</a>");
                } else if (modeValue == FAN_MODE) {
                    client.println("            <a href=\"/mode/cool\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-snowflake\"></i> Cool</a>");
                    client.println("            <a href=\"/mode/heat\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-sun\"></i> Heat</a>");
                    client.println("            <a href=\"/mode/dry\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-wind\"></i> Dry</a>");
                    client.println("            <a href=\"/mode/fan\" class=\"btn btn-primary active\" aria-current=\"page\"><i class=\"fa-solid fa-fan\"></i> Fan</a>");
                    client.println("            <a href=\"/mode/auto\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-robot\"></i> Auto</a>");
                } else if (modeValue == AUTO_MODE) {
                    client.println("            <a href=\"/mode/cool\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-snowflake\"></i> Cool</a>");
                    client.println("            <a href=\"/mode/heat\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-sun\"></i> Heat</a>");
                    client.println("            <a href=\"/mode/dry\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-wind\"></i> Dry</a>");
                    client.println("            <a href=\"/mode/fan\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\"></i> Fan</a>");
                    client.println("            <a href=\"/mode/auto\" class=\"btn btn-primary active\" aria-current=\"page\"><i class=\"fa-solid fa-robot\"></i> Auto</a>");
                }
                client.println("        </div>");
                client.println("</div>");

                client.println("<div class=\"row\" style=\"margin: 15px;\">");
                client.println("        <div class=\"btn-group\">");
                if (fanValue == FAN_LOW) {
                    client.println("            <a href=\"/fan/low\" class=\"btn btn-primary active\" aria-current=\"page\"><i class=\"fa-solid fa-fan\" style=\"font-size: 30%;\"></i> Low</a>");
                    client.println("            <a href=\"/fan/medium\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\" style=\"font-size: 70%;\"></i> Medium</a>");
                    client.println("            <a href=\"/fan/high\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\"></i> High</a>");
                    client.println("            <a href=\"/fan/auto\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-robot\"></i> Auto</a>");
                } else if (fanValue == FAN_MEDIUM) {
                    client.println("            <a href=\"/fan/low\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\" style=\"font-size: 30%;\"></i> Low</a>");
                    client.println("            <a href=\"/fan/medium\" class=\"btn btn-primary active\" aria-current=\"page\"><i class=\"fa-solid fa-fan\" style=\"font-size: 70%;\"></i> Medium</a>");
                    client.println("            <a href=\"/fan/high\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\"></i> High</a>");
                    client.println("            <a href=\"/fan/auto\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-robot\"></i> Auto</a>");
                } else if (fanValue == FAN_HIGH) {
                    client.println("            <a href=\"/fan/low\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\" style=\"font-size: 30%;\"></i> Low</a>");
                    client.println("            <a href=\"/fan/medium\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\" style=\"font-size: 70%;\"></i> Medium</a>");
                    client.println("            <a href=\"/fan/high\" class=\"btn btn-primary active\" aria-current=\"page\"><i class=\"fa-solid fa-fan\"></i> High</a>");
                    client.println("            <a href=\"/fan/auto\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-robot\"></i> Auto</a>");
                } else if (fanValue == FAN_AUTO) {
                    client.println("            <a href=\"/fan/low\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\" style=\"font-size: 30%;\"></i> Low</a>");
                    client.println("            <a href=\"/fan/medium\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\" style=\"font-size: 70%;\"></i> Medium</a>");
                    client.println("            <a href=\"/fan/high\" class=\"btn btn-primary\" aria-current=\"page\"><i class=\"fa-solid fa-fan\"></i> High</a>");
                    client.println("            <a href=\"/fan/auto\" class=\"btn btn-primary active\" aria-current=\"page\"><i class=\"fa-solid fa-robot\"></i> Auto</a>");
                }
                client.println("        </div>");
                client.println("</div>");

                client.println("<div class=\"row\" style=\"margin: 35px 15px 15px 15px;\">");
                client.println("        <div class=\"btn-group\">");

                for (int i=16; i<=20; i++) {
                    if (temperatureValue == i) {
                        client.printf("            <a href=\"/temp/%d\" class=\"btn btn-primary active\" aria-current=\"page\">%d&#8451</a>", i, i);
                    } else {
                        client.printf("            <a href=\"/temp/%d\" class=\"btn btn-primary\" aria-current=\"page\">%d&#8451</a>", i, i);
                    }
                }

                client.println("        </div>");
                client.println("</div>");

                client.println("<div class=\"row\" style=\"margin: 15px;\">");
                client.println("        <div class=\"btn-group\">");

                for (int i=21; i<=25; i++) {
                    if (temperatureValue == i) {
                        client.printf("            <a href=\"/temp/%d\" class=\"btn btn-primary active\" aria-current=\"page\">%d&#8451</a>", i, i);
                    } else {
                        client.printf("            <a href=\"/temp/%d\" class=\"btn btn-primary\" aria-current=\"page\">%d&#8451</a>", i, i);
                    }
                }

                client.println("        </div>");
                client.println("</div>");

                client.println("<div class=\"row\" style=\"margin: 15px;\">");
                client.println("        <div class=\"btn-group\">");

                for (int i=26; i<=30; i++) {
                    if (temperatureValue == i) {
                        client.printf("            <a href=\"/temp/%d\" class=\"btn btn-primary active\" aria-current=\"page\">%d&#8451</a>", i, i);
                    } else {
                        client.printf("            <a href=\"/temp/%d\" class=\"btn btn-primary\" aria-current=\"page\">%d&#8451</a>", i, i);
                    }
                }

                client.println("        </div>");
                client.println("</div>");

                client.println("</div>");
        

                // The HTTP response ends with another blank line
                client.println();
                // Break out of the while loop
                break;
            } else { // if you got a newline, then clear currentLine
                currentLine = "";
            }
            } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
            }
        }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");

    }

}
