/**
 * @file CanCommon.h
 * @brief Headerfile für Enums, welche Werte für CanBaudrate, CanMode, CanSyncResult und CanAsyncResult speichern.
 * @details 
 * * CanBaudrate: Unterstützt die Anforderung, dass die Baudraten 125kBaud, 250kBaud und 500kBaud für die CAN-Anbindungen verwendet werden sollen.
 * * CanMode: Ermöglicht die Steuerung des Hardware Zustands.
 * Bisher sind Active und Passive möglich, dies kann für zukünftige Hardware aber auch erweitert werden.
 * * CanSyncResult: Fehlermeldungen für synchrone Rückmeldung von Fehlern.
 * * CanAsyncResult: Fehlermeldungen für asynchrone Rückmeldung von Fehlern.
 */

#pragma once
#include <cstdint>


/**
* @brief Übertragungsraten für den CAN-Bus.
*/
enum class CanBaudrate {
	BAUD_125k,
	BAUD_250k,
	BAUD_500k
};

/**
* @brief Modus auf welchen CAN gesetzt werden kann. Bisher Active und Passive.
*/
enum class CanMode {
	Active,
	Passive
};

/**
* @brief Rückmeldungen für Status nach Senden einer Nachricht (Synchron).
*/
enum class CanSyncResult {
	OK,
	NOT_INITIALIZED,
	INVALID_LENGTH,
	INVALID_PARAMETER,
	NOT_ACTIVE
};


/**
* @brief Rückmeldungen für Fehler nach Senden oder beim Empfang (Asynchron).
*/
enum class CanAsyncResult {
	TX_FAILED,
	RX_OVERFLOW,
	BUS_ERROR
};