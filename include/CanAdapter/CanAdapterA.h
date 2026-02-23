/**
 * @file CanAdapterA.h
 * @brief Adapter (Wrapper) um Applikation über das einheitliche Interface Zugriff auf Hardware CanTypeA zu erlauben.
 * @details
 * Diese Klasse bindet die Hardware CanTypeA an das einheitliche Interface an.
 * Sie übersetzt die Enums für Modus und Baudrate und ruft hardware eigene Funktionen auf.
 */


#pragma once

#include <cstdint>

#include "../CanTypes/CanTypeA.h"
#include "../CanCommon.h"
#include "../ICanInterface.h"


/** 
 * @class CanAdapterA
 * @brief Adapter (Wrapper) um einheitlichen Zugriff auf die Hardware CanTypeA zu erlauben.
 */
class CanAdapterA : public ICanInterface {
    public:
        /** @brief Konstruktor welcher das Attribut m_hardwareA anlegt und diesem die Adresse des Hardwareobjekts zuweist. */
        explicit CanAdapterA(CanTypeA* hardware) : m_hardwareA(hardware) {}

        bool initialize() override {
            if (!m_hardwareA) return false;
            return m_hardwareA->initialize();
        }
        
        /** @brief Übersetzt das Enum CanMode in den entsprechenden Wert und setzt Status der Hardware (Active/ Passive) */
        bool setMode (CanMode mode) override {
            if (!m_hardwareA) return false;

            switch (mode) {
                case CanMode::Active:
                    m_hardwareA->setActive();
                    return true;
                case CanMode::Passive:
                    m_hardwareA->setPassive();
                    return true;
                default:
                    return false;
            }
        }

        /** @brief Übersetzt das Enum Baudrate in den entsprechenden Wert und setzt Baudrate der Hardware */
        bool setBaudrate(CanBaudrate br) override {
            if (!m_hardwareA) return false;
            
            uint32_t baudValue = 0;

            switch (br) {
                case CanBaudrate::BAUD_125k:
                    baudValue = 125000;
                    break;
                case CanBaudrate::BAUD_250k:
                    baudValue = 250000;
                    break;
                case CanBaudrate::BAUD_500k:
                    baudValue = 500000;
                    break;
                default:
                    return false;
            }

            return m_hardwareA->setBaudrate(baudValue);
        }

        /** @brief Prüft eingereichte Nachricht auf Gültigkeit und ruft Methode sendMessage(message, length) in Hardware auf, wenn gültig. */
        CanSyncResult sendMessage(const uint8_t* message, uint8_t length) override {
            if (!m_hardwareA) return CanSyncResult::INVALID_PARAMETER;
            if (length < 1 || length > 8 ) return CanSyncResult::INVALID_LENGTH;

            bool success = m_hardwareA->sendMessage(message, length);

            if (success) return CanSyncResult::OK;
            return CanSyncResult::NOT_ACTIVE;
        }


        /** @brief Adresse des Callbacks für den asynchronen Empfang wird hinterlegt */
        void registerReceiveCallback(CanReceiveCallback callback) override {
            m_rxCallback = callback;
        }
        /** @brief Adresse des Callbacks für die asynchrone Rückmeldung von Fehlern wird hinterlegt */
        void registerErrorCallback(CanErrorCallback callback) override {
            m_errorCallback = callback;
        }

        /** @brief Methode um Bypass von Applikation auf Hardware CanTypeA direkt zu ermöglichen.
         * @details In diesem Fall ist es bspw. für die Methode setBaudrate(eBaudrate baudrate), welche eine Spezialfunktion von CanTypeA ist. 
         * Per Downcast kann die Applikation auf diese Spezialfunktion an dem einheitlichem Interface vorbei zugreifen.
         */
        CanTypeA* getHardware() {return m_hardwareA; }

    private:
        /** @brief Adresse der Hardware von CanTypeA. Diese erlaubt den Direktzugriff des Adapters auf die Hardware.
         */
        CanTypeA* m_hardwareA; 

        /**@brief Speicher für Funktionsadressen zur Realisierung des Asynchronen Rückmeldens von Fehlern. */
        CanReceiveCallback m_rxCallback = nullptr;
        CanErrorCallback m_errorCallback = nullptr;
};