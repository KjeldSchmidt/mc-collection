//
// Created by kjeld on 30/01/2021.
//

#include "string"
#include "LampWebServer.h"
#include "modes/Mode.h"
#include "modes/ModeConverter.h"

void LampWebServer::registerHandlers() {
	server->on( "/setMode", [ & ]() { this->setMode(); } );
	server->on( "/getModes", [ & ]() { this->getModes(); } );
}

void LampWebServer::initServer() {
	registerHandlers();
	server->begin();
}

void LampWebServer::setMode() {
	// CORS
	server->sendHeader( "Access-Control-Allow-Origin", "*" );
    // Get Mode from data
    std::string newModeName = server->arg( "newMode" ).c_str();
    Mode newMode = ModeFromString(newModeName);

    // Maybe get optional color
    uint32_t optionalColor = 0;
    bool isSingleColorMode = newMode == Mode::SingleColorColorMode;
    if (isSingleColorMode) {

        // Check has optional Color
        bool hasOptionalColor = server->hasArg( "color" );
        if (!hasOptionalColor) {
            server->send( 400, "text/plain", "SingleColor(color) requires a hex arg" );
            return;
        }

        // Get
		const char *color_string = server->arg( "color" ).c_str();
	    optionalColor = strtol( color_string, nullptr, 16 );

        // Check optional Color is set
        bool optionalColorIs0 = optionalColor == 0;
        bool optionalColorIsBlack = strcmp( color_string, "0x000000" );
        bool optionalColorIsUnparseable = optionalColorIs0 && !optionalColorIsBlack;
        if (optionalColorIsUnparseable) {
            server->send( 400, "text/plain", "Color has parsed to 0" );
            return;
        }
	}

    // Set Mode
    bool success = lightManager->setMode( newMode, optionalColor);

    // Send Response
    int statusCode = success ? 200 : 400;
    server->send(statusCode);
}

void LampWebServer::handleClient() {
	server->handleClient();
}

void LampWebServer::getModes() {
	const char * modes = LightManager::getModes();
	server->send( 200, "text/plain", modes);
}

LampWebServer::LampWebServer( LightManager *lightManager, ESP8266WebServer *server )
		: lightManager( lightManager ),
		  server( server ) {}
