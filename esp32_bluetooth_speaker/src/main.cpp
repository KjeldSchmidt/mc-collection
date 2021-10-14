#include <Arduino.h>
#include "../lib/ESP32-A2DP-1.4.0/src/BluetoothA2DPSink.h"

BluetoothA2DPSink a2dp_sink;

/**
 * To use this, you need:
 * - An ESP32
 * - An I2S DAC, e.g. the UDA1334
 *
 * Make these connections:
 * I2S DAC <-> ESP32
 * Bitclock (BCLK) <-> IO26
 * Word Select (WSEL) <-> IO25
 * Data In (DIN) <-> IO22
 *
 * For significantly extended configuration options, check here:
 * https://github.com/pschatzmann/ESP32-A2DP
 */

void setup() {
	a2dp_sink.start( "ESP32 Blueetooth Speaker" );
}

void loop() {
}