/**
 * @file CanAdapterC.h
 * @brief Adapter (Wrapper) um Applikation über das einheitliche Interface Zugriff auf Hardware CanTypeC zu erlauben.
 * @details
 * Diese Klasse bindet die Hardware CanTypeC an das einheitliche Interface an.
 * Sie übersetzt die Enums für Modus und Baudrate und ruft hardware eigene Funktionen auf.
 */


#pragma once

#include <cstdint>

#include "../CanTypes/CanTypeC.h"
#include "../CanCommon.h"
#include "../ICanInterface.h"


/** 
 * @class CanAdapterC
 * @brief Adapter (Wrapper) um einheitlichen Zugriff auf die Hardware CanTypeC zu erlauben.
 */
class CanAdapterC : public ICanInterface {
    public:
        /** @brief Konstruktor welcher das Attribut m_hardwareC anlegt und diesem die Adresse des Hardwareobjekts zuweist. */
        explicit CanAdapterC(CanTypeC* hardware) : m_hardwareC(hardware) {}

        bool initialize() override {
            if (!m_hardwareC) return false;
            return m_hardwareC->initialize();
        }
        
        /** @brief Übersetzt das Enum CanMode in den entsprechenden Wert und setzt Status der Hardware (Active/ Passive) */
        bool setMode (CanMode mode) override {
            if (!m_hardwareC) return false;

            switch (mode) {
                case CanMode::Active:
                    m_hardwareC->setActive();
                    return true;
                case CanMode::Passive:
                    m_hardwareC->setPassive();
                    return true;
                default:
                    return false;
            }
        }

        /** @brief Übersetzt das Enum Baudrate in den entsprechenden Wert und setzt Baudrate der Hardware */
        bool setBaudrate(CanBaudrate br) override {
            if (!m_hardwareC) return false;
            
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

            return m_hardwareC->setBaudrate(baudValue);
        }

        /** @brief Prüft eingereichte Nachricht auf Gültigkeit und ruft Methode sendMessage(message, length) in Hardware auf, wenn gültig. */
        CanSyncResult sendMessage(const uint8_t* message, uint8_t length) override {
            if (!m_hardwareC) return CanSyncResult::INVALID_PARAMETER;
            if (length < 1 || length > 8 ) return CanSyncResult::INVALID_LENGTH;

            bool success = m_hardwareC->sendMessage(message, length);

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

        /** @brief Methode um Bypass von Applikation auf Hardware CanTypeC direkt zu ermöglichen. */
        CanTypeC* getHardware() {return m_hardwareC; }

    private:
        /** @brief Adresse der Hardware von CanTypeA. Diese erlaubt den Direktzugriff der Applikation auf die Hardware. */
        CanTypeC* m_hardwareC; 

        /**@brief Speicher für Funktionsadressen zur Realisierung des Asynchronen Rückmeldens von Fehlern. */
        CanReceiveCallback m_rxCallback = nullptr;
        CanErrorCallback m_errorCallback = nullptr;
};