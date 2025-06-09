#ifndef TASTENSTEUERUNG_H
#define TASTENSTEUERUNG_H

#include "Config.h"

class TastenSteuerung {
private:
  // Entprellungs-Variablen
  static const unsigned long DEBOUNCE_DELAY = 200;
  
  bool _buttonMenu;
  bool _buttonSelect;
  bool _buttonNext;

  bool _buttonMenuLast;
  bool _buttonSelectLast;
  bool _buttonNextLast;

  unsigned long _timestampButton;
    
public:
  TastenSteuerung();
  
  // Button-Indizes
  static const uint8_t BTN_MENU = 0;
  static const uint8_t BTN_SELECT = 1;
  static const uint8_t BTN_NEXT = 2;

  // Button-Status
  static const uint8_t NOT_PRESSED = 0;
  static const uint8_t PRESSED = 1;
  static const uint8_t NO_CHANGE = 2;

  void init();
  uint8_t loop(unsigned long timestamp);
  
  // Öffentliche Button-Abfragen
  uint8_t buttonPressed(uint8_t buttonIndex);
};

#endif