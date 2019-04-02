#include <Arduino.h>
#include "TalkToMe.h"


TalkToMe::TalkToMe(){
	bytesRecived = 0;
	lastCharIsCR = false;
	recivedSTX = false;
}

void TalkToMe::begin() {
}

void TalkToMe::checkSerialForCommands(byte* _type, String* _parameter, String* _value) {
	Serial.flush();
	findEndOfCommand();
	*_type = type;
	*_parameter = parameter;
	*_value = value;
	type = 0;
	parameter = "";
}


void TalkToMe::findEndOfCommand() {
  // Keeps storing chars in the array until ETX or CrLf is detected

  byte index; // An index to move in the loops
  byte charactersOnRecivingBuffer = 0; // How many characters (bytes) do we have in the reciving buffer
  char serialRecivedCharacter = 0; // Where we store what we read from the buffer
  
  charactersOnRecivingBuffer = Serial.available();
  Serial.println("Serial.available(): " + String(charactersOnRecivingBuffer));
  if (charactersOnRecivingBuffer > 0) { // There is something in the buffer
    for (index = 0; index < charactersOnRecivingBuffer; index++) { // Let's check all what we've recived
      serialRecivedCharacter = Serial.read(); // Get a character from the buffer
      bytesRecived++; // One more byte recived
      
      if (bytesRecived >= serialReciveBufferSize) { // Buffer overflow error
        Serial.println("Buffer overflow");
        clearSerialRecivingBuffer();
      }
      
      if (serialRecivedCharacter == -1) { // The buffer is empty
        Serial.println("Nothing on the buffer");
      }
      else if (serialRecivedCharacter == 13) { // We got the Carriage Return character
        lastCharIsCR = true;
      }
      else if (serialRecivedCharacter == 2) { // The first recived byte is STX
        clearSerialRecivingBuffer();
		recivedSTX = true;
      }
      
      // End of command is CrLf
      else if ((serialRecivedCharacter == 10) && (lastCharIsCR == true)) {
		analyzeCommand();
		clearSerialRecivingBuffer();
      }
      // End of command is ETX
      else if ((serialRecivedCharacter == 3) && (recivedSTX == true)) {
		analyzeCommand();
		clearSerialRecivingBuffer();
      }

      else {
        lastCharIsCR = false;
        for (index = 0; index < serialReciveBufferSize; index++) { // Let's move through the array to find the first empty space
          if (serialRecivingBufferArray[index] == 0) { // Empty space found
            serialRecivingBufferArray[index] = serialRecivedCharacter; // Fill the empty space
            break; // Stop the for
          }
        }
      }
    }
  }
}

void TalkToMe::analyzeCommand() {
  byte index;
  byte equalCharPosition;
  
  for (index = serialReciveBufferSize - 1; index != 0; index--) {
	
    if (serialRecivingBufferArray[index] != 0) {
		if (serialRecivingBufferArray[index] == '?') { // Get parameter command
			serialRecivingBufferArray[index] = 0;
			type = 1;
			parameter = String(serialRecivingBufferArray);
			value = "";
			break;
		}
		else if (serialRecivingBufferArray[index] == '=') { // Set parameter command
			equalCharPosition = index;
			type = 2;
			parameter = String(serialRecivingBufferArray).substring(0, equalCharPosition);
			value = String(serialRecivingBufferArray).substring(equalCharPosition + 1, bytesRecived);
			break;
		}
    }
  }
  if (index == 0) {
	  type = 255; // 255 is the invalid command format id
  }
}

void TalkToMe::clearSerialRecivingBuffer() {
	byte index;
	
	bytesRecived = 0;
	lastCharIsCR = false;
	recivedSTX = false;
	
	for (index = 0; index < serialReciveBufferSize; index++) {
		serialRecivingBufferArray[index] = 0;
	}
}

void TalkToMe::printSerialRecivingBuffer() {
  byte index;
  for (index = 0; index < serialReciveBufferSize; index++) {
    if (serialRecivingBufferArray[index] != 0) {
      Serial.print(serialRecivingBufferArray[index]);
    }
  }
  Serial.println("");
}