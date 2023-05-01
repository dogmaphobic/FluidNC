#pragma once

#include "Config.h"
#include "Configuration/Configurable.h"
#include "Machine/I2CBus.h"

namespace Machine {
    class INA219 : public Configuration::Configurable {
    public:
        void init();
        void loop(); 
        void report_realtime_status(LogStream& msg);
        void validate() override {}
        void afterParse() override;
        void group(Configuration::HandlerBase& handler) override {
            handler.item("i2c_num", _i2c_num);
            handler.item("sda_pin", _sda_pin);
            handler.item("scl_pin", _scl_pin);
            handler.item("i2c_freq", _i2c_clock);
            handler.item("low_bat", _low_battery, 10.0f, 48.0f);
        }
    private:
        TwoWire* _twoWire        = nullptr;
        uint8_t  _i2c_num        = 1;
        uint8_t  _sda_pin        = 0;
        uint8_t  _scl_pin        = 4;
        uint32_t _i2c_clock      = 100000;
        float    _low_battery    = 12.8f;
        //-- I could not get this to work with the Adafruit library
        //   and I certainly didn't feel taking the time to retrofit
        //   the entire standard TwoWire model into FluidNC's
        //   version of it.
        //I2CBus*  _i2c          = nullptr;
        bool     _error          = false;
        bool     _inited         = false;
        float    _shuntvoltage   = 0;
        float    _busvoltage     = 0;
        float    _current_mA     = 0;
        float    _loadvoltage    = 0;
        float    _power_mW       = 0;
        uint32_t _timer          = 0;
    };
}