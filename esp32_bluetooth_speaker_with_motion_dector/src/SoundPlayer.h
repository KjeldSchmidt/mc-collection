//
// Created by kjeld on 18/06/2020.
//

#ifndef VINYL_RECORD_PLAYER_SOUND_PLAYER_H
#define VINYL_RECORD_PLAYER_SOUND_PLAYER_H

#include <Arduino.h>
#include "../.pio/libdeps/wemos_d1_mini32/MD_YX5300/src/MD_YX5300.h"

class SoundPlayer {
public:
	SoundPlayer( Stream &stream );

	void play_current_folder( uint8_t folder );

	void play_random_from_folder( uint8_t folder );

	void begin();

	bool check();

	void play_next();

	static uint16_t SERIAL_BPS;

private:
	void handle_message();

	void play_current();

	MD_YX5300 mp3;
	uint8_t current_file_index = 0;
	uint8_t current_folder_index = 0;
	uint16_t files_in_current_folder = 0;
};


#endif //VINYL_RECORD_PLAYER_SOUND_PLAYER_H
