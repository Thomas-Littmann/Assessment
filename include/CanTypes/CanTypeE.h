/** 
 * @file CanTypeE.h
 * @brief Headerfile mit der gegebenen Klasse CanTypeE aus dem Assessment, welche die Hardware verkörpert. Diese wird über den Wrapper CanAdapterE an das Interface angebunden.
 */


#pragma once

#include <cstdint>


/** @brief Platzhalter Struct, welcher von der Spezialfunktion sendMessageStruct eingelesen wird. */
struct messageStruct {

};


/**
 * @class CanTypeE
 * @brief Hardware Klasse für CanTypeE welche in der Aufgabenstellung gegeben ist.
 */
class CanTypeE {
    public:

        bool initialize() { 
            initialized = true; 
            return true;
        }

        bool setBaudrate(uint32_t br) { 
            baudrate = br; 
            return true;
        }

        /** Prüft ob CanBus initialisiert und aktiv ist vor dem Senden */
        bool sendMessage(const uint8_t* message, uint8_t length) { return (initialized && active); }

        void setActive() { active = true; }

        void setPassive() { active = false; }

        /** @brief Hardwarespezifische Spezialfunktion. Direktzugriff von Applikation ist über Bypass möglich. */
        bool sendMessageStruct(messageStruct* message) {
            if (message == nullptr) { return false; }
            return true;
        }

    private:
        uint32_t baudrate = 0;
        bool initialized = false;
        bool active = false;

        bool handleMessage() { return true; }
};
