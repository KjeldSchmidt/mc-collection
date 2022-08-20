#ifndef BURNT_WOOD_LAMP_LEDSTRIPE_H
#define BURNT_WOOD_LAMP_LEDSTRIPE_H

#include <string>

class LedStripe {

    public:
      const std::string Name() const {return _name;}
      const int StartLed() const {return _startLED; }
      const int LedCount() const  {return _ledCount; }
    
      explicit LedStripe(std::string name, int ledStart, int ledCount){
           _name = name;
           _startLED = ledStart;
           _ledCount = ledCount;
      }
      ~LedStripe();

    private:
      std::string _name;
      int _startLED;
      int _ledCount;
};

#endif