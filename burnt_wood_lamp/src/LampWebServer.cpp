//
// Created by kjeld on 30/01/2021.
//

#include "LampWebServer.h"

void LampWebServer::registerHandlers() {
	server->on( "/setMode", [ & ]() { this->setMode(); } );
	server->on( "/getModes", [ & ]() { this->getModes(); } );
}

void LampWebServer::initServer() {
	registerHandlers();
	server->begin();
}

void LampWebServer::setMode() {
	bool success = false;
	String newModeName = server->arg( "newMode" );
	server->sendHeader( "Access-Control-Allow-Origin", "*" );

	if ( server->args() == 1 ) {
		success = lightManager->setMode( newModeName );
	} else if ( newModeName == "SingleColor" ) {
		if ( !server->hasArg( "color" )) {
			server->send( 400, "text/plain", "SingleColor(color) requires a hex arg" );
			return;
		}
		const char *color_string = server->arg( "color" ).c_str();
		uint32_t color = strtol( color_string, nullptr, 16 );
		if ( color == 0 && ( strcmp( color_string, "0x000000" ) != 0 )) {
			server->send( 400, "text/plain", "Color has parsed to 0" );
			return;
		}
		success = lightManager->setMode( newModeName, color );
	}

	if ( success ) {
		server->send( 200 );
	} else {
		server->send( 400 );
	}
}

void LampWebServer::handleClient() {
	server->handleClient();
}

void LampWebServer::getModes() {
	server->send( 200, "text/plain", LightManager::getModes());
}

LampWebServer::LampWebServer( LightManager *lightManager, ESP8266WebServer *server )
		: lightManager( lightManager ),
		  server( server ) {}
