#include "TastenSteuerung.h"

TastenSteuerung::TastenSteuerung() {
}

void TastenSteuerung::init() {
  Serial.println("TastenSteuerung: Initialisiere...");
  
  // Pins als Input mit Pull-up konfigurieren
  pinMode(PIN_BUT_MENU, INPUT_PULLUP);
  pinMode(PIN_BUT_SELECT, INPUT_PULLUP);
  pinMode(PIN_BUT_NEXT, INPUT_PULLUP);
  
  _buttonMenuLast = digitalRead(PIN_BUT_MENU);
  _buttonSelectLast = digitalRead(PIN_BUT_SELECT);
  _buttonNextLast = digitalRead(PIN_BUT_NEXT);

  Serial.println("TastenSteuerung: Bereit");
}

uint8_t TastenSteuerung::loop(unsigned long timestamp) {
  // Alle Button-States aktualisieren
  if (timestamp - _timestampButton > 300) {
    _buttonMenu = digitalRead(PIN_BUT_MENU);
    _buttonSelect = digitalRead(PIN_BUT_SELECT);
    _buttonNext = digitalRead(PIN_BUT_NEXT);

    if (_buttonMenu == LOW) {
      _timestampButton = timestamp;
      return 1;
    } else if (_buttonSelect == LOW) {
      _timestampButton = timestamp;
      return 2;
    } else if (_buttonNext == LOW) {
      _timestampButton = timestamp;
      return 3;
    }
  }
  return 0;
}

uint8_t TastenSteuerung::buttonPressed(uint8_t buttonIndex) {
  switch(buttonIndex) {
    case BTN_MENU:
      if (_buttonMenuLast != _buttonMenu) {
        _buttonMenuLast = _buttonMenu;
        if (_buttonMenu) {
          return PRESSED;
        } else {
          return NOT_PRESSED;
        }
      } else {
        return NO_CHANGE;
      }
      break;
    case BTN_SELECT:
      if (_buttonSelectLast != _buttonSelect) {
        _buttonSelectLast = _buttonSelect;
        if (_buttonSelect) {
          return PRESSED;
        } else {
          return NOT_PRESSED;
        }
      } else {
        return NO_CHANGE;
      }
      break;
    case BTN_NEXT:
      if (_buttonNextLast != _buttonNext) {
        _buttonNextLast = _buttonNext;
        if(_buttonNext) {
          return PRESSED;
        } else {
          return NOT_PRESSED;
        }
      } else {
        return NO_CHANGE;
      }
      break;
    default:
      return NO_CHANGE;
      break;
  }
  return NO_CHANGE;
}