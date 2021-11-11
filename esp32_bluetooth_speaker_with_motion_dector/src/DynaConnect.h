#ifndef DYNA_CONNECT_H
#define DYNA_CONNECT_H

#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include "AutoConnect.h"

#define EEPROM_SIZE 128

class DynaConnect {
public:
    DynaConnect(WebServer &Server, AutoConnect &Portal);

    void handle_client();

private:
    void root_page();

    void start_page();

    WebServer &Server;
    AutoConnect &Portal;
    AutoConnectConfig Config;
    AutoConnectAux Timezone;
};

#endif // DYNA_CONNECT_H
