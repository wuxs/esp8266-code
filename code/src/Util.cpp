//
// Created by wxs on 19-3-15.
//
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Util.h>
#include "Util.h"


void Util::now_time() {
    time_t utime;
    struct tm *now;
    time(&utime);
    now = localtime(&utime);
    Serial.printf("%d-%d-%d %d:%d:%d", now->tm_year, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
}

bool Util::wake_on_lan() {
    WiFiUDP udp;
    udp.beginPacketMulticast(IPADDR_BROADCAST, WOL_PORT, WiFi.localIP());
    uint8_t package[102];
    make_magic_packet(wake_mac, package);
    udp.write(package, sizeof(package));
    udp.endPacket();
    return false;
}

bool Util::make_magic_packet(const uint8_t *mac, uint8_t *package) {
    for (int i = 0; i <= 16; i++) {
        if (i == 0) {
            package[i * 6 + 0] = 0xff;
            package[i * 6 + 1] = 0xff;
            package[i * 6 + 2] = 0xff;
            package[i * 6 + 3] = 0xff;
            package[i * 6 + 4] = 0xff;
            package[i * 6 + 5] = 0xff;
        } else {
            package[i * 6 + 0] = mac[0];
            package[i * 6 + 1] = mac[1];
            package[i * 6 + 2] = mac[2];
            package[i * 6 + 3] = mac[3];
            package[i * 6 + 4] = mac[4];
            package[i * 6 + 5] = mac[5];
        }
    }
    return false;
}

void Util::init_wifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);
    Serial.print("Connected to AP");
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.print("\nIP address: ");
    Serial.println(WiFi.localIP());
}

void Util::init_time() {
    configTime(8 * 3600,  0, "cn.ntp.org.cn", "ntp1.aliyun.com");
}

void Util::init_mqtt() {

}

bool Util::arping() {
    return false;
}

void Util::print(const char *msg) {
    Serial.print(msg);
}

void Util::println(const char *msg) {
    Serial.println(msg);
}

void Util::write_to_log() {

}

void Util::arp(const char *ip) {

}

void Util::str_to_bytes() {

}

void Util::init_server() {

}

void Util::register_handler() {

}

void Util::unregister_handler() {

}

