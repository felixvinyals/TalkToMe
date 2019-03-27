/* 
	TalkToMe.h - Simple command interpreter
	Author: Felix Vinyals
	
	Command begin and end:
	[command]CrLf
	or
	<STX>[command]<ETX>
	
	Command structure:
	Get parameter:
	[parameter]?
	Set parameter: 
	[parameter]=[value]
	
	Constraits:
	Max lenght of command: 64 bytes
	Only use <STX> and <ETX> to construct commands
	Only use '=' for setting parameters
	Only use '?' for getting a parameter
	Do not use spaces on '=' or '?'
	Casesensitive
	Do not use null character in your commands

*/

#ifndef TalkToMe_h
#define TalkToMe_h

#include "Arduino.h"

#define serialReciveBufferSize 64 // Serial reciving buffer size

class TalkToMe {
	public:
		// Variables:
		/* struct ttmCommandStruct {
			byte commandType;	
			String commandParameter;
			String commandValue;
		}; */
		
		// Constructor:
		TalkToMe();
		
		// Functions:
		void begin();
		void checkSerialForCommands(byte* type, String* parameter, String* value);
	
	private:
		// Variables:
		char serialRecivingBufferArray[serialReciveBufferSize]; // Reciving data array buffer
		boolean lastCharIsCR; // If the last char recived was a CR
		boolean recivedSTX; // If the first char recived was a STX
		byte bytesRecived; // Total recived bytes before end CrLF or ETX
		
		String parameter, value;
		byte type; // 255: command not recognized 0: no new data 1: Get parameter 2: Set parameter
		
		// Functions:
		void findEndOfCommand();
		void analyzeCommand();
		void clearSerialRecivingBuffer();
		void printSerialRecivingBuffer();
	
};

#endif