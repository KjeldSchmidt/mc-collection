//
// Created by kjeld on 30/01/2021.
//

#include "LampWebServer.h"

void LampWebServer::registerHandlers() {
	server->on("/setMode", [ & ]() { this->setMode(); });
	server->on("/getModes", [ & ]() { this->getModes(); });
	server->on("/ColorFromPayload", HTTP_POST, [ & ]() { this->colorFromPayload(); });
	server->on("/status", [ & ]() { this->getStatus(); });
}

void LampWebServer::initServer() {
	registerHandlers();
	server->begin();
}


void LampWebServer::colorFromPayload() const {
	if ( !server->hasArg( "plain" )) {
		server->send( 400, "text/plain", "ColorFromPaylopad requires a payload." );
		return;
	}

	// size_t length = server->arg("plain").length();
	const uint8_t* payload = reinterpret_cast<const uint8_t*>(server->arg("plain").c_str());

	lightManager->setMode( "ColorFromPayload", 0, 0, payload );
	server->send( 200 );
}

void LampWebServer::setMode() const {
	bool success = false;
	String newModeName = server->arg( "newMode" );
	server->sendHeader( "Access-Control-Allow-Origin", "*" );

	if ( newModeName == "SingleColor" ) {
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

	if ( server->args() == 1 ) {
		success = lightManager->setMode( newModeName );
	}

	if ( success ) {
		server->send( 200 );
	} else {
		server->send( 400, "text/plain", "Unknown mode" );
	}
}

void LampWebServer::handleClient() {
	server->handleClient();
}

void LampWebServer::getModes() const {
	server->send( 200, "text/plain", LightManager::getModes());
}

void LampWebServer::getStatus() const {
	server->send( 200, "text/plain", lightManager->getCurrentMode());
}

LampWebServer::LampWebServer( LightManager *lightManager, ESP8266WebServer *server )
		: lightManager( lightManager ),
		  server( server ) {}
