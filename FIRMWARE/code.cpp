#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <time.h>

const char* ssid = "my_wifi_name";
const char* password = "my_wifi_pass";

#define TFT_SCLK 10
#define TFT_MOSI 11
#define TFT_RST 9
#define TFT_DC 5
#define TFT_CS 6
#define TFT_BL 7

class MyST7789 : public Adafruit_ST7789 {
public:
    MyST7789(
        int8_t cs,
        int8_t dc,
        int8_t mosi,
        int8_t sclk,
        int8_t rst
    ) : Adafruit_ST7789(cs, dc, mosi, sclk, rst) {}

    void setOffsets(uint8_t col, uint8_t row) {
        _colstart = _colstart2 = col;
        _rowstart = _rowstart2 = row;
    }
};

MyST7789 clockscreen(
    TFT_CS,
    TFT_DC,
    TFT_MOSI,
    TFT_SCLK,
    TFT_RST
);

void setup() {
    Serial.begin(115200);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    clockscreen.init(76, 284);
    clockscreen.setOffsets(82, 18);
    clockscreen.invertDisplay(false);
    clockscreen.setRotation(1);
    clockscreen.fillScreen(ST77XX_BLACK);

    Serial.println("TFT Initialized!");

    clockscreen.setCursor(0, 0);
    clockscreen.print("WORKING!");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");

    configTime(3600, 3600, "pool.ntp.org", "time.nist.gov");
}

void loop() {
    struct tm timeinfo;

    if (getLocalTime(&timeinfo)) {
        Serial.printf(
            "%02d:%02d:%02d\n",
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec
        );
    }

    delay(1000);
}