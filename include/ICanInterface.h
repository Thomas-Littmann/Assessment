/**
 * @file ICanInterface.h
 * @brief Headerfile welches das Einheitliche Interface ICanInterface enthält.
 * @details
 * Dieses Interface stellt die Einheitliche Schnittstelle dar, welche in dem Assessment gefordert wird.
 * Von diesem werden die gemeinsamen Funktionen der verschiedenen Hardwares (CanTypeA, CanTypeB, ..., CanTypeE) zur Verfügung gestellt.
 * Gepaart mit hardwarespezifischen Adaptern (Wrappern) ermöglicht das Interface der Applikation, mittels einheitlicher Methoden mit verschiedenen Hardwares zu interagieren.
 * Neue Hardware kann mit wenig Aufwand angebunden werden.
 */


#pragma once

#include <cstdint>
#include "CanCommon.h"


/** @brief Aliase für die Callbacks zur besseren Lesbarkeit */
using CanReceiveCallback = void (*)(const uint8_t* data, uint8_t length);
using CanErrorCallback = void (*)(CanAsyncResult);


class ICanInterface {
	public:
		virtual ~ICanInterface() = default;

		virtual bool initialize() = 0;

		/** @brief Implementierung einer einheitlichen Methode setMode. 
		 *  @details Setzt Hardware auf aktiv oder passive mit den hardwarespezifischen Methoden.
		 * 	Diese sind bisher: setState(bool active) und setActive(), bzw. setPassive().
		 * 	CanMode wurde implemeniert, um noch andere Modi für neue Hardware zu unterstützen.
		 * */ 
		virtual bool setMode(CanMode mode) = 0;

		/** @brief Implementierung einer einheitlichen Methode um festgelegte Übertragungsraten einzustellen (125kBaud, 250kBaud und 500kBaud). */
		virtual bool setBaudrate(CanBaudrate baudrate) = 0; 

		/** @brief Synchrones Senden inklusive Synchroner Rückmeldung. */ 
		virtual CanSyncResult sendMessage(const uint8_t* message, uint8_t length) = 0;

		/** @brief Asynchrones Empfangen (Registrierung eines Callbacks). 
		 * @details Damit dies funktioniert, müsste sich der Adapter bei der Hardware als Empfänger von Nachrichten registrieren.
		 * Ist der jeweilige Adapter bei der Hardware hinterlegt, können bei Erhalt von Nachrichten diese registrierten Callbacks weitergeleitet werden.
		 * Bei CanTypeC könnte das Attribut messageReceiver dafür angedacht sein.
		 */
		virtual void registerReceiveCallback(CanReceiveCallback callback) = 0;

		/** @brief Asynchrone Asynchroner Empfang von Fehlern (Registrierung eines Callbacks für Fehler). */
		virtual void registerErrorCallback(CanErrorCallback callback) = 0;
};