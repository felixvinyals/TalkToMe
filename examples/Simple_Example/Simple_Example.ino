#include <TalkToMe.h>

TalkToMe ttm;

void setup() {
  Serial.begin(9600);
  ttm.begin();
  Serial.println("Begin!");
}

void loop() {
  String parameter, value;
  byte type;

  ttm.checkSerialForCommands(&type, &parameter, &value);
  if (type != 0) {
    if (type == 255) {
      Serial.println("Wrong command. Did you forget the '?' or '='?");
    }
    else if (type == 1) {
      Serial.println("'Get Parameter' command type recived.");
      Serial.println("&parameter: " + String(parameter));
      Serial.println("&value: " + String(value));
    }
    else if (type == 2) {
      Serial.println("'Set Parameter' command type recived.");
      Serial.println("&parameter: " + String(parameter));
      Serial.println("&value: " + String(value));
    }
    type = 0; // Reset the flag
  }
}
