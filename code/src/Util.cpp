//
// Created by wxs on 19-3-15.
//

#include <stdarg.h>
#include <HardwareSerial.h>
#include "Util.h"


void Util::now_time() {

}

bool Util::wake_on_lan() {
    return false;
}

bool Util::make_magic_packet() {
    return false;
}

void Util::init_wifi() {

}

void Util::init_time() {

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

