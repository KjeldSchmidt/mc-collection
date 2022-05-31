#pragma once

#include "ColorMode.h"

class TimExistentialDreadMode : public ColorMode {
// MODES
enum Modes {strobo, ambient, river, switchsort};
struct ModeConfig {
    Modes mode;
    uint32 minRuntime;
    uint32 maxRuntime;
    uint16 minDelay;
    uint16 maxDelay;
    bool init;
    uint8 chanceInPercent;
    //void updateColors(CRGB *leds_out);
    void set_function(std::function<void(CRGB*)> in) {
        SetColors = in;
    }
    std::function<void(CRGB*)> SetColors;
};

    // NOTES
    // LED-Order
    // Bottom Left -> Bottom Right -> Top Right -> Top left

    // MODES
    private: bool init = false;
    private: static const uint8 modeConfigsCount = 4;
    private: ModeConfig modeConfigs[modeConfigsCount];
    private: ModeConfig currentModeConfig;
    private: uint16 modeTimePassedInMs = 0;
    private: uint16 modeTimeMax = -1;

    // Part of TimExistentialDreadMode
    // MODE: STROBO
    // Colors
    private: CRGB stroboOff = CRGB(20, 20, 25);
    private: static const uint8 stroboColorCount = 7;
    private: CRGB stroboColors[stroboColorCount] {};
    // Caching
    private: bool stroboToggle = false;
    private: uint8 lastStroboColorIndex = -1;

    // MODE: SINGLE
    // Colors

    // MODE: RIVER
    // Colors

    // MODE: AMBIENT
    // Colors

    // Override Update
    // Return (uint 16) -> Delay in MilliSeconds
    public: uint16 Update(CRGB *leds_out) override {
        // Init
        Init();

        // Update ModeConfig
        UpdateModeConfig();

        // Set Colors
        currentModeConfig.SetColors(leds_out);

        // Set Random Delay
        const uint8 delay = random16(currentModeConfig.minDelay, currentModeConfig.maxDelay);
        modeTimePassedInMs += delay;

        return delay;
    }

    private: void Init() {
        if(init){
            return;
        }

        init = true;
        // Init Modes
        // STROBO
        modeConfigs[0] = {
                strobo,
                   500, // minRuntime
                  5000, // maxRuntime
                    10, // minDelay
                    30, // maxDelay
                    false, //init
                    10, // chanceInPercent
        };
        modeConfigs[0].set_function([ & ](CRGB* leds_out) { this->SetStroboColor(leds_out); });

        // AMBIENT
        modeConfigs[1] = {
                   ambient,
                     2000, // minRuntime
                     6000, // maxRuntime
                      200, // minDelay
                      200, // maxDelay
                      false, // init
                      100, // chanceInPercent
        };
        modeConfigs[1].set_function([ & ](CRGB* leds_out) { this->SetAmbientColor(leds_out); });

        // RIVER
        modeConfigs[2] = {
                river,
                10000,// minRuntime
                20000,// maxRuntime
                   10,// minDelay
                   10,// maxDelay
                false, // Init
                  100, // chanceInPercent

        };
        modeConfigs[2].set_function([ & ](CRGB* leds_out) { this->SetRiverColor(leds_out); });

        // SWITCH-SORT
        modeConfigs[3] = {
                switchsort,
                100000,// minRuntime
                100000,// maxRuntime
                    100,// minDelay
                    100,// maxDelay
                false, // Init
                   100, // chanceInPercent

        };
        modeConfigs[3].set_function([ & ](CRGB* leds_out) { this->SetSwitchSortColor(leds_out); });

        // Init Strobo
        stroboColors[0] = CRGB(255,   0,   0);
        stroboColors[1] = CRGB(  0, 255,   0);
        stroboColors[2] = CRGB(  0,   0, 255);
        stroboColors[3] = CRGB(255, 255,   0);
        stroboColors[4] = CRGB(255,   0, 255);
        stroboColors[5] = CRGB(  0, 255, 255);
        stroboColors[6] = CRGB(255, 255, 255);

        // Set initial Mode
        SetModeConfig(modeConfigs[0]);
    };
    private: void UpdateModeConfig() {
        if(modeTimePassedInMs < modeTimeMax){
            return;
        }

        // Set Next Mode randomly
        ModeConfig newModeConfig = currentModeConfig;
        uint8 random100 = random8(0, 100);

        SetModeConfig(modeConfigs[3]);
    }

    private: void SetModeConfig(ModeConfig modeConfig){
        // Reset time passed
        modeTimePassedInMs = 0;
        modeConfig.init = false;

        // Set max play time
        modeTimeMax = random16(modeConfig.minRuntime, modeConfig.maxRuntime);
        currentModeConfig = modeConfig;
    }

    private: void SetSingleColor( CRGB *leds_out, CRGB color ) {
        for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
            leds_out[i] = color;
        }
    }

    private: void SetStroboColor( CRGB *leds_out ) {
        // Toggle
        stroboToggle = !stroboToggle;
        // Get Color
        CRGB color = stroboToggle ? GetStroboColor() : stroboOff;
        // Set Color
        SetSingleColor(leds_out, color);
    }

    private: void SetAmbientColor( CRGB *leds_out ) {
        // Determine Color
        const CRGB color = CRGB(
                random(0, 255),
                random(0, 255),
                random(0, 255)
        );

        // Set Color
        SetSingleColor(leds_out, color);
    }

    private: void SetRiverColor( CRGB *leds_out ) {
        // Init
        if(!currentModeConfig.init){
            currentModeConfig.init = true;

            // Generate Colors
            const uint8 min = 255 - NUM_LEDS;

            // Use random Color Combination
            uint8 rMultiplier = random(2);
            uint8 gMultiplier = random(2);
            uint8 bMultiplier = random(2);
            // But at least one color
            if(rMultiplier == 0 && gMultiplier == 0 && bMultiplier == 0){
                bMultiplier = 1;
            }

            for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
                const uint8 val = min+i;
                CRGB color = CRGB(
                        rMultiplier * val,
                        gMultiplier * val,
                        bMultiplier * val
                        );
                leds_out[ i ] = color;
            }
        }

        ShiftColors(leds_out);
    }

    private: void SetSwitchSortColor( CRGB *leds_out ) {
        // Init
        if(!currentModeConfig.init){
            currentModeConfig.init = true;

            // Generate Colors
            for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
                leds_out[ i ] = GetRandomColor();
            }
        }

        SwitchSortColors(leds_out);
    }

    private: uint8 lastIndex = 1;
    private: uint8 colorIndex = 0;
    private: void SwitchSortColors(CRGB *leds_out) {

        for ( uint8_t i = lastIndex; i < NUM_LEDS; i++ ) {
            CRGB val1 = leds_out[i-1];
            CRGB val2 = leds_out[i];

            // No need to switch
            if(val1.r <= val2.r ) {
                continue;
            }
            else {

                // Switch, set lastIndex & wait for next step
                leds_out[i - 1] = CRGB(val2.r, val1.g, val1.b);// val2;
                leds_out[i] = CRGB(val1.r, val2.g, val2.b);
                lastIndex = i + 1;
            }
        }

        // Completed without switching so current color is sorted -> do next color
        colorIndex++;
        lastIndex = 1;

        // If all 3 colors have completed end mode
        if(colorIndex > 2){
            modeTimePassedInMs += 1000000;
        }
    }

    private: CRGB GetRandomColor() {
        const CRGB color = CRGB(
                random(0, 255),
                random(0, 255),
                random(0, 255)
        );
        return color;
    }

    private: void ShiftColors(CRGB *leds_out){
        // leds_out[0] = CRGB(133,33,72);
        CRGB nextColor = leds_out[NUM_LEDS-1];
        for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
            CRGB currentColor = nextColor;
            nextColor = leds_out[i];
            leds_out[ i ] = currentColor;
        }
    }

    private: CRGB GetStroboColor(){
        uint8 newStroboColorIndex = lastStroboColorIndex;
        while(newStroboColorIndex == lastStroboColorIndex){
            newStroboColorIndex = random8(0, stroboColorCount);
        }

        lastStroboColorIndex = newStroboColorIndex;
        CRGB stroboColor = stroboColors[newStroboColorIndex];
        return stroboColor;
    }
};