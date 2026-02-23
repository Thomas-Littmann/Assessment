/** 
 * @file CanTypeA.h
 * @brief Headerfile mit der gegebenen Klasse CanTypeA aus dem Assessment, welche die Hardware verkörpert. Diese wird über den Wrapper CanAdapterA an das Interface angebunden.
 */


#pragma once

#include <cstdint>

/**
 * @class CanTypeA
 * @brief Hardware Klasse für CanTypeA welche in der Aufgabenstellung gegeben ist.
 */
class CanTypeA {
    public:

        /** @brief Enum für hardwarespezifische Spezialfunktion. Hier wurde angenommen, dass CanTypeA als einzige Hardware eine Baudrate von 1000kBaud zulässt. */
        enum class eBaudrate : uint32_t {
            BAUD_125k = 125000,
            BAUD_250k = 250000,
            BAUD_500k = 500000,
            BAUD_1000k = 1000000
        };


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

        void setActive() { active = true; }

        void setPassive() { active = false; }

        /** @brief Hardwarespezifische Spezialfunktion. Direktzugriff von Applikation ist über Bypass möglich. */
        bool setBaudrate(eBaudrate br) {
            baudrate = static_cast<uint32_t>(br); 
            return true; 
        }

    private:
        uint32_t baudrate = 0;
        bool initialized = false;
        bool active = false;

        bool handleMessage() { return true; }
};
