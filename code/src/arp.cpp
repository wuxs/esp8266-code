#include <Arduino.h>
#include <IPAddress.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266Ping.h>
#include <lwip/etharp.h>
#include <lwip/netif.h>
#include <netif/etharp.h>
#include <ESP8266WebServer.h>

/* 连接您的WIFI SSID和密码 */
#define WIFI_SSID       "HUAWEI-TFN7F5"
#define WIFI_PASSWD     "949625012"
//#define PC_MAC          "4C:ED:FB:BD:DC:E6"
#define NOTIFY_HOST     "sc.ftqq.com"
#define NOTIFY_PORT      80
#define NOTIFY_URL      "/SCU44583Te387109551e70dc28790757bf3796aa05c643c7f5ac0c.send"
#define MASK            IPAddress(0xFF, 0xFF, 0xFF, 0xFF)
#define WOL_PORT        9

bool status = false;

uint8_t PC_MAC[6] = {0x4C, 0xED, 0xFB, 0xBD, 0xDC, 0xE6};

IPAddress MOBILE_IP(192, 168, 3, 52);
IPAddress PC_IP(192, 168, 3, 49);

ESP8266WebServer server(80);

struct config{

};



void handleRoot() {
    server.send(200, "text/plain", "hello from esp8266!");
}

void handleStatus() {
    server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}


void server_init() {
    server.on("/", handleRoot);
    server.on("/status", handleStatus);
    server.on("/inline", []() {
        server.send(200, "text/plain", "this works as well");
    });

    server.onNotFound(handleNotFound);
}

void wifi_init() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    Serial.print("Connected to AP");
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.print("\nIP address: ");
    Serial.println(WiFi.localIP());
}

void send_to_wechat(String text) {
    String url = NOTIFY_URL "?text=" + text;
    WiFiClient client;
    if (!client.connect(NOTIFY_HOST, NOTIFY_PORT)) {
        Serial.println("Connection Failed");
        delay(1000);
        return;
    }
    String request = "GET " + url + " HTTP/1.1\r\n" + "Host: " + NOTIFY_HOST + "\r\n\r\n";
    client.print(request);
    client.stop();
}

void make_magic_packet(const uint8_t *mac, uint8_t *package) {
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
}

void send_magic_packet() {
    WiFiUDP udp;
    udp.beginPacketMulticast(IPADDR_BROADCAST, WOL_PORT, WiFi.localIP());
    uint8_t package[102];
    make_magic_packet(PC_MAC, package);
    udp.write(package, sizeof(package));
    udp.endPacket();
}

bool arp_check() {
    eth_addr *mac;
    etharp_tmr();
    const ip_addr_t *remote_ip;
    etharp_find_addr(netif_default, MOBILE_IP, &mac, &remote_ip);
    char mac_str[6];
    sprintf(mac_str, MACSTR, MAC2STR(mac->addr));
    Serial.printf("MAC =>" MACSTR "\n", MAC2STR(mac->addr));
    return strcmp("F4:60:E2:76:BE:52", mac_str) == 0;
}

void check() {
    bool result = Ping.ping(MOBILE_IP);
    if (result && !status) {
        send_magic_packet();
        send_to_wechat(MOBILE_IP.toString() + "已连接WiFi!正在启动电脑!!");
        Serial.println(MOBILE_IP.toString() + " 已连接!");
        status = true;
    } else if (!result && !arp_check()) {
        Serial.println(MOBILE_IP.toString() + " 已断开连接!");
        if (Ping.ping(PC_IP)) {
            send_to_wechat(MOBILE_IP.toString() + "已断开wifi连接!电脑未关闭!!");
        } else {
            send_to_wechat(MOBILE_IP.toString() + "已断开wifi连接!");
        }
        status = false;
    }
}

void setup() {
    Serial.begin(115200);
    wifi_init();
    server_init();
}


void loop() {
    check();
    delay(1000);
}
