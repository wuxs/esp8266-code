#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>

#define WIFI_SSID "My_Wi-Fi"
#define WIFI_PASSWORD "my-awesome-password"

#define MQTT_HOST IPAddress(192, 168, 1, 10)
#define MQTT_PORT 1883

AsyncMqttClient client;
Ticker mqtt_reconnect_ticker;
Ticker wifi_reconnect_ticker;

WiFiEventHandler wifi_connect_handler;
WiFiEventHandler wifi_disconnect_handler;

void print(String msg) {
    Serial.print(msg);
    if (client.connected()) {

    }
}

void println(String msg) {
    Serial.println(msg);
    if (client.connected()) {

    }
}

void connect_to_wifi() {
    println("Connecting to Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}


void on_wifi_disconnect(const WiFiEventStationModeDisconnected &event) {
    println("Wi-Fi 已断开.");
    mqtt_reconnect_ticker.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
    wifi_reconnect_ticker.once(2, connect_to_wifi);
}

void connect_to_mqtt() {
    println("正在连接MQTT服务器...");
    client.connect();
}

void on_wifi_connect(const WiFiEventStationModeGotIP &event) {
    println("Wi-Fi 已连接.");
    connect_to_mqtt();
}

void on_mqtt_connect(bool sessionPresent) {
    println("MQTT 服务器已连接.");
    print("Session present: ");
    println((String) sessionPresent);
    uint16_t packetIdSub = client.subscribe("test/lol", 2);
    print("Subscribing at QoS 2, packetId: ");
    println((String) packetIdSub);
    client.publish("test/lol", 0, true, "test 1");
    println("Publishing at QoS 0");
    uint16_t packetIdPub1 = client.publish("test/lol", 1, true, "test 2");
    print("Publishing at QoS 1, packetId: ");
    println((String) packetIdPub1);
    uint16_t packetIdPub2 = client.publish("test/lol", 2, true, "test 3");
    print("Publishing at QoS 2, packetId: ");
    println((String) packetIdPub2);
}

void on_mqtt_disconnect(AsyncMqttClientDisconnectReason reason) {
    println("MQTT 服务器已断开.");

    if (WiFi.isConnected()) {
        mqtt_reconnect_ticker.once(2, connect_to_mqtt);
    }
}

void on_mqtt_subscribe(uint16_t packetId, uint8_t qos) {
    println("Subscribe acknowledged.");
    print("  packetId: ");
    println((String) packetId);
    print("  qos: ");
    println((String) qos);
}

void on_mqtt_unsubscribe(uint16_t packetId) {
    println("Unsubscribe acknowledged.");
    print("  packetId: ");
    println((String) packetId);
}

void on_mqtt_message(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index,
                     size_t total) {
    println("Publish received.");
    print("  topic: ");
    println(topic);
    print("  qos: ");
    println((String) properties.qos);
    print("  dup: ");
    println((String) properties.dup);
    print("  retain: ");
    println((String) properties.retain);
    print("  len: ");
    println((String) len);
    print("  index: ");
    println((String) index);
    print("  total: ");
    println((String) total);
}

void on_mqtt_publish(uint16_t packetId) {
    println("Publish acknowledged.");
    print("  packetId: ");
    println((String) packetId);
}

void setup() {
    Serial.begin(115200);
    println("");
    println("");

    wifi_connect_handler = WiFi.onStationModeGotIP(on_wifi_connect);
    wifi_disconnect_handler = WiFi.onStationModeDisconnected(on_wifi_disconnect);

    client.onConnect(on_mqtt_connect);
    client.onDisconnect(on_mqtt_disconnect);
    client.onSubscribe(on_mqtt_subscribe);
    client.onUnsubscribe(on_mqtt_unsubscribe);
    client.onMessage(on_mqtt_message);
    client.onPublish(on_mqtt_publish);
    client.setServer(MQTT_HOST, MQTT_PORT);

    connect_to_wifi();
}

void loop() {
}