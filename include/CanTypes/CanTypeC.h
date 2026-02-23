/** 
 * @file CanTypeC.h
 * @brief Headerfile mit der gegebenen Klasse CanTypeC aus dem Assessment, welche die Hardware verkörpert. Diese wird über den Wrapper CanAdapterC an das Interface angebunden.
 */


#pragma once

#include <cstdint>

/**
 * @class CanTypeC
 * @brief Hardware Klasse für CanTypeC welche in der Aufgabenstellung gegeben ist.
 */
class CanTypeC {
    public:
        bool initialize() { 
            initialized = true;
            return true; 
        }

        bool setBaudrate(uint32_t br) {
            baudrate = br;
            return true;
        }

        /** @brief Prüft ob CanBus initialisiert und aktiv ist vor dem Senden und verschickt Nachricht*/
        bool sendMessage(const uint8_t* message, uint8_t length) { return (initialized && active); }

        void setActive() { active = true; }

        void setPassive() { active = false; }
        

    private:
        uint32_t baudrate = 0;
        bool initialized = false;
        bool active = false;

        bool handleMessage() { return true; }

        bool initTypeSpecific() { return true; }


    protected:
        void* messageReceiver = nullptr;
};