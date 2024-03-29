#include "DynaConnect.h"

static const char AUX_TIMEZONE[] PROGMEM = R"(
{
  "title": "Timezone",
  "uri": "/timezone",
  "menu": true,
  "element": [
    {
      "name": "caption",
      "type": "ACText",
      "value": "Sets the time zone to get the current local time.",
      "style": "font-family:Arial;font-weight:bold;text-align:center;margin-bottom:10px;color:DarkSlateBlue"
    },
    {
      "name": "timezone",
      "type": "ACSelect",
      "label": "Select TZ name",
      "option": [],
      "selected": 10
    },
    {
      "name": "newline",
      "type": "ACElement",
      "value": "<br>"
    },
    {
      "name": "start",
      "type": "ACSubmit",
      "value": "OK",
      "uri": "/start"
    }
  ]
}
)";

typedef struct {
	const char *zone;
	const char *ntpServer;
	int8_t tzoff;
} Timezone_t;

static const Timezone_t TZ[] = {
		{ "Europe/London",        "europe.pool.ntp.org",        0 },
		{ "Europe/Berlin",        "europe.pool.ntp.org",        1 },
		{ "Europe/Helsinki",      "europe.pool.ntp.org",        2 },
		{ "Europe/Moscow",        "europe.pool.ntp.org",        3 },
		{ "Asia/Dubai",           "asia.pool.ntp.org",          4 },
		{ "Asia/Karachi",         "asia.pool.ntp.org",          5 },
		{ "Asia/Dhaka",           "asia.pool.ntp.org",          6 },
		{ "Asia/Jakarta",         "asia.pool.ntp.org",          7 },
		{ "Asia/Manila",          "asia.pool.ntp.org",          8 },
		{ "Asia/Tokyo",           "asia.pool.ntp.org",          9 },
		{ "Australia/Brisbane",   "oceania.pool.ntp.org",       10 },
		{ "Pacific/Noumea",       "oceania.pool.ntp.org",       11 },
		{ "Pacific/Auckland",     "oceania.pool.ntp.org",       12 },
		{ "Atlantic/Azores",      "europe.pool.ntp.org",        -1 },
		{ "America/Noronha",      "south-america.pool.ntp.org", -2 },
		{ "America/Araguaina",    "south-america.pool.ntp.org", -3 },
		{ "America/Blanc-Sablon", "north-america.pool.ntp.org", -4 },
		{ "America/New_York",     "north-america.pool.ntp.org", -5 },
		{ "America/Chicago",      "north-america.pool.ntp.org", -6 },
		{ "America/Denver",       "north-america.pool.ntp.org", -7 },
		{ "America/Los_Angeles",  "north-america.pool.ntp.org", -8 },
		{ "America/Anchorage",    "north-america.pool.ntp.org", -9 },
		{ "Pacific/Honolulu",     "north-america.pool.ntp.org", -10 },
		{ "Pacific/Samoa",        "oceania.pool.ntp.org",       -11 }
};

DynaConnect::DynaConnect(WebServer &Server, AutoConnect &Portal) : Server(Server), Portal(Portal) {
    Timezone.load( AUX_TIMEZONE );
	load_time_zone_index();

    AutoConnectSelect &tz = Timezone[ "timezone" ].as<AutoConnectSelect>();
	for ( uint8_t n = 0; n < sizeof( TZ ) / sizeof( Timezone_t ); n++ ) {
		tz.add( String( TZ[ n ].zone ));
	}

	Portal.join( { Timezone } );

	Server.on( "/", [&] (){ this->root_page(); } );
	Server.on( "/start", [&] (){ this->start_page(); } );

	Serial.println( "Creating portal and trying to connect..." );

	if ( Portal.begin()) {
		Serial.println( "WiFi connected: " + WiFi.localIP().toString());
		Serial.println( WiFi.getHostname());
    }
}

void DynaConnect::root_page() {
	String content =
			"<html>"
			"<head>"
			"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
			"<script type=\"text/javascript\">"
			"setTimeout(\"location.reload()\", 1000);"
			"</script>"
			"</head>"
			"<body>"
			"<h2 align=\"center\" style=\"color:blue;margin:20px;\">Dschungelmusik</h2>"
			"<h3 align=\"center\" style=\"color:gray;margin:10px;\">{{DateTime}}</h3>"
			"<p style=\"text-align:center;\">Click the gear to enter settings</p>"
			"<p></p><p style=\"padding-top:15px;text-align:center\">" AUTOCONNECT_LINK( COG_24 ) "</p>"
			"</body>"
			"</html>";
	static const char *wd[7] = { "Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat" };
	struct tm *tm;
	time_t t;
	char dateTime[26];

	t = time( NULL );
	tm = localtime( &t );
	sprintf( 
		dateTime,
		"%04d/%02d/%02d (%s) %02d:%02d:%02d",
		tm->tm_year + 1900,
		tm->tm_mon + 1,
		tm->tm_mday,
		wd[ tm->tm_wday ],
		tm->tm_hour,
		tm->tm_min,
		tm->tm_sec 
	);

	content.replace( "{{DateTime}}", String( dateTime ));
	Server.send( 200, "text/html", content );
}

void DynaConnect::start_page() {
	// Retrieve the value of AutoConnectElement with arg function of WebServer class.
	// Values are accessible with the element name.
	String tz = Server.arg( "timezone" );

	for ( uint8_t n = 0; n < sizeof( TZ ) / sizeof( Timezone_t ); n++ ) {
		String tzName = String( TZ[ n ].zone );
		if ( tz.equalsIgnoreCase( tzName )) {
			store_time_zone_index( n );
			Serial.println( "Time zone: " + tz );
			Serial.println( "ntp server: " + String( TZ[ n ].ntpServer ));
			break;
		}
	}

	// The /start page just constitutes timezone,
	// it redirects to the root page without the content response.
	Server.sendHeader( "Location", String( "http://" ) + Server.client().localIP().toString() + String( "/" ));
	Server.send( 302, "text/plain", "" );
	Server.client().flush();
	Server.client().stop();
}


void DynaConnect::handle_client() {
    Portal.handleClient();
}

void DynaConnect::store_time_zone_index( uint8_t new_time_zone_index ) {
	if ( time_zone_index == new_time_zone_index ) return;

	EEPROM.begin(1);

	configTime( TZ[ new_time_zone_index ].tzoff * 3600, 0, TZ[ new_time_zone_index ].ntpServer );
	EEPROM.write(0, new_time_zone_index);

	EEPROM.end();
}

void DynaConnect::load_time_zone_index() {
	EEPROM.begin(1);
	time_zone_index = EEPROM.read( 0 );
	EEPROM.end();

	if (time_zone_index > 23) {
		time_zone_index = 1;
	} 

	configTime( TZ[ time_zone_index ].tzoff * 3600, 0, TZ[ time_zone_index ].ntpServer );
}