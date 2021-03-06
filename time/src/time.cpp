#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <lwip/etharp.h>

const char *ssid = "Adesk";
const char *password = "tryourbest";

const IPAddress remote(192, 168, 0, 115);

int timezone = 8;
int dst = 0;


void init_wifi() {
    Serial.println("Connecting to WiFi");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("WiFi connected with ip ");
    Serial.println(WiFi.localIP());
}

void init_time() {
    configTime(timezone * 3600, dst * 0, "cn.ntp.org.cn", "ntp1.aliyun.com");
    time_t now;
    struct tm *timeinfo;
    time(&now);
    timeinfo = localtime(&now);
    Serial.println(timeinfo->tm_hour);
    delay(1000);
}

void setup() {
    Serial.begin(115200);
    delay(10);
    init_wifi();
}

void loop() {
    init_time();
    delay(1000);
}
