#include "Config.h"
#include <Wire.h>
#include <Adafruit_INA219.h>
#include "Machine/MachineConfig.h"  // config

#include "ina219.h"

Adafruit_INA219 ina219;

namespace Machine {

    void INA219::afterParse() {
        /*
        Not using FluidNC i2c
        if (!config->_i2c[_i2c_num]) {
            log_error("i2c" << _i2c_num << " section must be defined for ina219");
            _error = true;
        }
        */
    }

    void INA219::init() 
    {
        //if(!_error) {
            //_i2c = config->_i2c[_i2c_num];
            //if(_i2c) {
                _twoWire = new TwoWire(_i2c_num);
                if(_twoWire->setPins(_sda_pin, _scl_pin)) {
                    if(_twoWire->begin()) {
                        if (ina219.begin(_twoWire)) {
                            ina219.setCalibration_32V_2A();
                            return;
                        } else {
                            log_error("Failed to init Adafruit_INA219");
                        }
                    } else {
                        log_error("Failed to init TwoWire for INA219 chip");
                    }
                } else {
                    log_error("Failed to init i2c pins");
                }
            //} else {
            //    log_error("Failed to collect i2c for INA219 chip");
            //}
            if(_twoWire) {
                delete _twoWire;
                _twoWire = nullptr;
            }
            _error = true;
            log_error("Failed to init INA219 chip");
        //}
    }

    void INA219::loop() 
    {
        uint32_t now = millis();
        if(now - _timer > 2000) {
            if(!_error) {
                _shuntvoltage    = ina219.getShuntVoltage_mV();
                _busvoltage      = ina219.getBusVoltage_V();
                _current_mA      = ina219.getCurrent_mA();
                _power_mW        = ina219.getPower_mW();
                _loadvoltage     = _busvoltage + (_shuntvoltage / 1000);
            }
            _timer = now;
        }
    }

    void INA219::report_realtime_status(LogStream& msg)
    {
        msg <<  "|Vstat:";
        msg << _busvoltage;
        msg << ":";
        msg << _current_mA;
        msg << ":";
        msg << _power_mW;
        msg << ":";
        msg << _loadvoltage;
    }

}