#ifndef DYNA_CONNECT_H
#define DYNA_CONNECT_H

#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include "AutoConnect.h"

#define EEPROM_SIZE 1

class DynaConnect {
public:
    DynaConnect(WebServer &Server, AutoConnect &Portal);

    void handle_client();

private:
    void root_page();

    void start_page();

    void store_time_zone_index(uint8_t timezone_index);

    void load_time_zone_index();

    WebServer &Server;
    AutoConnect &Portal;
    AutoConnectConfig Config;
    AutoConnectAux Timezone;

    uint8_t time_zone_index;
};

#endif // DYNA_CONNECT_H
