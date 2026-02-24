/**
 * @file CanAdapterB.h
 * @brief Adapter (Wrapper) um Applikation über das einheitliche Interface Zugriff auf Hardware CanTypeB zu erlauben.
 * @details
 * Diese Klasse bindet die Hardware CanTypeB an das einheitliche Interface an.
 * Sie übersetzt die Enums für Modus und Baudrate und ruft hardware eigene Funktionen auf.
 */


#pragma once

#include <cstdint>

#include "../CanTypes/CanTypeB.h"
#include "../CanCommon.h"
#include "../ICanInterface.h"


/** 
 * @class CanAdapterB
 * @brief Adapter (Wrapper) um einheitlichen Zugriff auf die Hardware CanTypeB zu erlauben.
 */
class CanAdapterB : public ICanInterface {
    public:
        /** @brief Konstruktor welcher das Attribut m_hardwareB anlegt und diesem die Adresse des Hardwareobjekts zuweist. */
        explicit CanAdapterB(CanTypeB* hardware) : m_hardwareB(hardware) {}

        bool initialize() override {
            if (!m_hardwareB) return false;
            return m_hardwareB->initialize();
        }
        
        /** @brief Übersetzt das Enum CanMode in den entsprechenden Wert und setzt Status der Hardware (Active/ Passive) */
        bool setMode (CanMode mode) override {
            if (!m_hardwareB) return false;

            switch (mode) {
                case CanMode::Active:
                    m_hardwareB->setState(true);
                    return true;
                case CanMode::Passive:
                    m_hardwareB->setState(false);
                    return true;
                default:
                    return false;
            }
        }

        /** @brief Übersetzt das Enum Baudrate in den entsprechenden Wert und setzt Baudrate der Hardware */
        bool setBaudrate(CanBaudrate br) override {
            if (!m_hardwareB) return false;
            
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

            return m_hardwareB->setBaudrate(baudValue);
        }

        /** @brief Prüft eingereichte Nachricht auf Gültigkeit und ruft Methode sendMessage(message, length) in Hardware auf, wenn gültig. */
        CanSyncResult sendMessage(const uint8_t* message, uint8_t length) override {
            if (!m_hardwareB) return CanSyncResult::INVALID_PARAMETER;
            if (length < 1 || length > 8 ) return CanSyncResult::INVALID_LENGTH;

            bool success = m_hardwareB->sendMessage(message, length);

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

        /**
         * @brief Zyklische Verarbeitung. Ermöglicht Polling für den Empfang von Nachrichten.
         * @details Zyklische Abfrage von Nachrichten bei der Hardware. Bei Erhalt von Nachrichten werden diese an die registrierte Callback-Funktion weitergeleitet.
         * Hinweis: Die aktuelle Hardware-API bietet keinen öffentlichen Zugriff auf handleMessage()) oder Empfangsregister
         */
        void receive() override {
            if (!m_hardwareB) return;

            uint8_t rxBuffer[8];
            uint8_t rxLength = 0;

            if (m_hardwareB->getMessage(rxBuffer, rxLength)) {

                if (m_rxCallback && rxLength > 0 && rxLength <= 8) {
                    m_rxCallback(rxBuffer, rxLength);
                }
            }

                /** @brief Rückmeldung von Fehlern über Callback. Hier wird geprüft, ob die Hardware initialisiert ist und ob die Nachricht nicht zu lang ist. */ 
            if (m_errorCallback) {

                if (!m_hardwareB->initialize()) {
                    m_errorCallback(CanAsyncResult::BUS_ERROR);
                }

                if (rxLength > 8) {
                    m_errorCallback(CanAsyncResult::RX_OVERFLOW);
                }
            }
        }

        /** @brief Methode um Bypass von Applikation auf Hardware CanTypeB direkt zu ermöglichen. */
        CanTypeB* getHardware() {return m_hardwareB; }

    private:
        /** @brief Adresse der Hardware von CanTypeB. Diese erlaubt den Direktzugriff der Applikation auf die Hardware. */
        CanTypeB* m_hardwareB; 

        /**@brief Speicher für Funktionsadressen zur Realisierung des Asynchronen Rückmeldens von Fehlern. */
        CanReceiveCallback m_rxCallback = nullptr;
        CanErrorCallback m_errorCallback = nullptr;
};