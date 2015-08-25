#include <Arduino.h>
#include <SoftwareSerial.h>
#include <espduino.h>
#include <rest.h>

/////////////////////////////////
// SETUP
const char* WifiSsid = "YourWifiSsid";
const char* WifiPass = "YourWifiPass";

// IP address of the server running flask_tester.py
const char* FlaskIpAddress = "XXX.XXX.XXX.XXX";

const char* testString = "This is my espduino test string!";

// Pin connected to button
const int buttonPin = 7;
int buttonState = 0;

// SoftwareSerial ports
// esp8266 will be using 0 and 1
const int SS_RX = 2;
const int SS_TX = 3;

// END SETUP
/////////////////////////////////

SoftwareSerial debugPort(SS_RX, SS_TX);
ESP esp(&Serial, &debugPort, 4);

REST rest(&esp);

boolean wifiConnected = false;

void wifiCb(void* response)
{
    uint32_t status;
    RESPONSE res(response);

    if (res.getArgc() == 1) {
        res.popArgs((uint8_t*)&status, 4);
        if (status == STATION_GOT_IP) {
            debugPort.println("WIFI CONNECTED");
            wifiConnected = true;
        } else {
            wifiConnected = false;
        }
    }
}

void setup() {
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(SS_RX, INPUT);
    pinMode(SS_TX, OUTPUT);

    Serial.begin(19200);
    debugPort.begin(19200);
    debugPort.println("Beginning setup.");
    esp.enable();
    delay(500);
    esp.reset();
    delay(500);
    while (!esp.ready());

    debugPort.println("ARDUINO: setup rest client");

    if (!rest.begin(FlaskIpAddress, 5000, false)) {
        debugPort.println("ARDUINO: failed to setup rest client");
        while (1);
    }
    rest.setContentType("application/x-www-form-urlencoded");

    /*setup wifi*/
    debugPort.println("ARDUINO: setup wifi");
    esp.wifiCb.attach(&wifiCb);
    esp.wifiConnect(WifiSsid, WifiPass);
    debugPort.println("ARDUINO: system started");
}

void loop() {
    char data_buf[256];
    esp.process();

    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {

        if (wifiConnected) {
            for (int testNum = 0; testNum < 100; testNum++) {
                sprintf(data_buf, "test_string=%s&test_num=%d", testString, testNum);
                debugPort.print("Data length: ");
                debugPort.println(strlen(data_buf));

                debugPort.println(data_buf);
                rest.post("/", (const char*)data_buf);

                debugPort.println("ARDUINO: send post");

                if (rest.getResponse(data_buf, 256) == HTTP_STATUS_OK) {
                    debugPort.println("ARDUINO: POST successful");

                } else {
                    debugPort.println("ARDUINO: POST error");
                }
            }
        }
    }
}
