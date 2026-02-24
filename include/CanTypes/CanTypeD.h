/** 
 * @file CanTypeD.h
 * @brief Headerfile mit der gegebenen Klasse CanTypeD aus dem Assessment, welche die Hardware verkörpert. Diese wird über den Wrapper CanAdapterD an das Interface angebunden.
 */


#pragma once

#include <cstdint>

/**
 * @class CanTypeD
 * @brief Hardware Klasse für CanTypeD welche in der Aufgabenstellung gegeben ist.
 */
class CanTypeD {
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
        
        /** @brief ANNAHME 
         * Für die Integration des Empfangs wird hier angenommen, dass die  Hardware CanTypeD eine Methode getMessage() bereitstellt, welche die aktuell empfangene Nachricht zurückgibt.
         * @details Diese Methode erhält Adresse eines Puffers und ist hier pseudomäßig impementiert. 
         * Es wird angenommen, dass die empfangene Nachricht in den Puffer geschrieben und die Länge der Nachricht zurückgegeben wird.
         * Wenn eine Nachricht empfangen wurde, wird true zurückgegeben, andernfalls false.
         */
        bool getMessage(uint8_t* buffer, uint8_t& length);

    private:
        uint32_t baudrate = 0;
        bool initialized = false;
        bool active = false;

        bool handleMessage() { return true; }
    
        bool reset() {
            initialized = false;
            active = false;
            baudrate = 0;
        }      
};