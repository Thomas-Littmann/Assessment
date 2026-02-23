/** 
 * @file CanTypeB.h
 * @brief Headerfile mit der gegebenen Klasse CanTypeB aus dem Assessment, welche die Hardware verkörpert. Diese wird über den Wrapper CanAdapterB an das Interface angebunden.
 */


#pragma once

#include <cstdint>

/**
 * @class CanTypeB
 * @brief Hardware Klasse für CanTypeB welche in der Aufgabenstellung gegeben ist.
 */
class CanTypeB {
    public:
        bool initialize() { 
            initialized = true;
            return true; 
        }

        bool setBaudrate(uint32_t br) {
            baudrate = br;
            return true;
        }

        /** @brief Prüft ob CanBus initialisiert und aktiv ist vor dem Senden */
        bool sendMessage(const uint8_t* message, uint8_t length) { return (initialized && active); }

        bool setState (bool state) {
            active = state;
            return true;
        }
        

    private:
        uint32_t baudrate = 0;
        bool initialized = false;
        bool active = false;

        bool handleMessage() { return true; }
    
        void reset() {
            initialized = false;
            active = false;
            baudrate = 0;
        }      
};