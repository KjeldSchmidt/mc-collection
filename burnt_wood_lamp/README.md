# Run
## Terminal
pio run
pio run -t upload (oder pfeil button)
## Visual Studio Code
Install PlatformIo plugin
- Click Build
- Click Upload

build without wifi comment out:
- ; upload_protocol = espota
- ; upload_port = 192.168.178.26

# Requirements
## LED 
### Power Supply
- min 3A (60mA / LED)
- 5V
- power supply unit above ~ 100 LEDs

# ColorModes
## New
1. Think of a <Name> 
1.1 Add it to enum in modes/Mode.h
1.2 Add it to ModeToString in modes/ModeConverter.h
1.3 Add it to ModeFromString in modes/ModeConverter.h
2. Create the Mode by copying f.e. <SingleColorMode.h> in colorModes/SingleColormode.h & renaming it using 1.
2.1 Insert the Mode into factories/ColorModeFactory createColorMode
3. Test it by setting your mode to be the initialMode in led/LedManager 
4. Build & Upload