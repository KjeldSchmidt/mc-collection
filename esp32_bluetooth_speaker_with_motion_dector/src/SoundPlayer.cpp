//
// Created by kjeld on 18/06/2020.
//

#include "SoundPlayer.h"

uint16_t SoundPlayer::SERIAL_BPS = MD_YX5300::SERIAL_BPS;

SoundPlayer::SoundPlayer( Stream &stream ) : mp3( MD_YX5300( stream )) {}


void SoundPlayer::play_current_folder( uint8_t folder ) {
	current_folder_index = folder;
	current_file_index = 1;
	play_current();
}

void SoundPlayer::play_next() {
	current_file_index++;
	play_current();
}

void SoundPlayer::begin() {
	mp3.begin();
	mp3.setSynchronous( true );
	mp3.setCallback( nullptr );
}

bool SoundPlayer::check() {
	bool check = mp3.check();
	if ( check ) handle_message();
	return check;
}

void SoundPlayer::handle_message() {
	const MD_YX5300::cbData *status = mp3.getStatus();
	MD_YX5300::status_t status_code = status->code;

	Serial.println( status_code, HEX );
	Serial.println( status->data );

	if ( status_code != MD_YX5300::STS_ACK_OK ) {
		Serial.print(F( "STS_0x" ));
		Serial.println( status_code, HEX );
	}

	switch ( status_code ) {
		case MD_YX5300::STS_FILE_END:
		case MD_YX5300::STS_VERSION: // Boldly attempt to ignore errors.
			Serial.println( "Song done" );
			break;
		case MD_YX5300::STS_ERR_FILE:
			Serial.println( "File Not Found" );
			break;
		case MD_YX5300::STS_FLDR_FILES:
			Serial.println( status->data );
			files_in_current_folder = status->data;
			break;
		default:
			break;
	}
	Serial.println();
}

void SoundPlayer::play_current() {
	Serial.print("Now playing song ");
	Serial.println(current_file_index);
	mp3.playSpecific( current_folder_index, current_file_index );
}

void SoundPlayer::play_random_from_folder( uint8_t folder ) {
	current_folder_index = folder;
	
	Serial.print("Playing random file from folder ");
	Serial.println(folder);
	if (folder == 1) {
		files_in_current_folder = 25;
	} else if (folder == 2)
	{
		files_in_current_folder = 1;
	} else {
		Serial.println("This folder index is not available in this project");
		return;
	}
	
	Serial.print("Got folder file count ");
	Serial.println(files_in_current_folder);
	current_file_index = random(files_in_current_folder) + 1;
	play_current();
}
