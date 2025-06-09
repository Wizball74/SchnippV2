#include "MatrixDisplay.h"
#include "TastenSteuerung.h"

MatrixDisplay::MatrixDisplay() :
   display(MD_MAX72XX::FC16_HW, PIN_MATRIX_DATA, PIN_MATRIX_CLK, PIN_MATRIX_CS, MAX_DEVICES) {

    // Callback initialisieren
  _sendToMain = nullptr;
}

void MatrixDisplay::init() {
  Serial.println("Setup 3-Zonen-Matrix");
  
  if (!display.begin(3)) {
    Serial.println("FEHLER: Matrix konnte nicht initialisiert werden!");
    return;
  }
    
  // **3 ZONEN DEFINIEREN**
  setZones(3);
  
  // **ALLE ZONEN KONFIGURIEREN**
  for(uint8_t zone = 0; zone < 3; zone++) {
    display.setIntensity(zone, 15);           // Helligkeit 0-15
    display.setTextAlignment(zone, PA_CENTER); // Zentriert
    display.setSpeed(zone, 80);             // Animation-Geschwindigkeit
    display.setPause(zone, 5000);            // Pause zwischen Animationen
    
    display.addChar('$', SYMBOL_SKULL);
    display.addChar('#', SYMBOL_BALL_0);
    display.addChar('{', SYMBOL_BALL_1);
    display.addChar('@', SYMBOL_SMILEY_0);
    display.addChar('}', SYMBOL_SMILEY_1);
  }
  
  display.displayClear();
  
  _currentScore[0] = 0;
  _currentScore[1] = 0;

  _currentPlayer = 0;
  _currentPhase = 0;
  _currentJackpot = 0;
  
  _status = " ";
  _blink = true;
  
  _nextStatus = STATUS_WELCOME1;
  _menuStatus = MENU_OFF;

  delay(100);
  Serial.println("Matrixanzeige: 3 Zonen erfolgreich initialisiert");
}

void MatrixDisplay::setZones(uint8_t count) {
  display.displayClear();
  switch(count) {
    case 3:
      display.setZone(ZONE_PLAYER2, 0, 1);    // Zone 0: Module 0-1 (RECHTS! Spieler 2)
      display.setZone(ZONE_STATUS, 2, 9);      // Zone 1: Module 2-9 (Status)  
      display.setZone(ZONE_PLAYER1, 10, 11);  // Zone 2: Module 10-11 (LINKS! Spieler 1)
      break;

    default:
      display.setZone(ZONE_MENU, 0, 11);      // Fullscreen
      break;
  }
}

void MatrixDisplay::loop(uint8_t but) {
  if (but > 0) {
    if (_menuStatus == MENU_OFF) {
      showMenu();
      setMenuText();
    } else {
      switch (but) {
        case BUTTON_MENU:
          hideMenu();
          break;

        case BUTTON_NEXT:
          doButtonNext();
          break;

        case BUTTON_SELECT:
          menuSelected();
          break;
      }
    }
  }
  
  if (display.displayAnimate()) {

    // Zone 2 und 0 (links und rechts - nur wenn nicht im Menü)
    if (_nextStatus != STATUS_MENU) {
      if (display.getZoneStatus(ZONE_PLAYER1)) {
        switch(_nextStatus) {
          case STATUS_WELCOME1:
          case STATUS_WELCOME2:
          case STATUS_WELCOME3:
            display.displayZoneText(ZONE_PLAYER1, ">>", PA_CENTER, 80, 1500, PA_SCROLL_RIGHT, PA_SCROLL_LEFT);
            display.displayZoneText(ZONE_PLAYER2, "<<", PA_CENTER, 80, 1500, PA_SCROLL_LEFT, PA_SCROLL_RIGHT);
            display.displayReset(ZONE_PLAYER1);
            display.displayReset(ZONE_PLAYER2);
            break;

          case STATUS_PHASE:
            switch(_currentPhase) {
              case 0:
                display.displayZoneText(ZONE_PLAYER1, "I", PA_CENTER, 120, 3000, PA_FADE);
                display.displayZoneText(ZONE_PLAYER2, "I", PA_CENTER, 120, 3000, PA_FADE);
                break;

              case 1:
                display.displayZoneText(ZONE_PLAYER1, "II", PA_CENTER, 120, 3000, PA_FADE);
                display.displayZoneText(ZONE_PLAYER2, "II", PA_CENTER, 120, 3000, PA_FADE);
                break;

              case 2:
                display.displayZoneText(ZONE_PLAYER1, "III", PA_CENTER, 120, 3000, PA_FADE);
                display.displayZoneText(ZONE_PLAYER2, "III", PA_CENTER, 120, 3000, PA_FADE);
                break;
            }
            display.displayReset(ZONE_PLAYER1);
            display.displayReset(ZONE_PLAYER2);

            break;

          case STATUS_IDLE:
            showScore();
            break;

          default:
            break;
          }
      }
    }

    //Statusanzeige setzen
    int8_t changeStatusTo = -1;

    if (display.getZoneStatus(ZONE_STATUS)) {
      //Nächste idle-Animation starten
      switch(_nextStatus) {
        case STATUS_MENU:
          setMenuText();
          break;
        case STATUS_WELCOME1:
          _spriteData = ghost;
          _spriteWidth = W_GHOST;
          _spriteFrames = F_GHOST;
          display.setSpriteData(ZONE_STATUS, _spriteData, _spriteWidth, _spriteFrames,
            _spriteData, _spriteWidth, _spriteFrames);
          display.displayZoneText(ZONE_STATUS, "WELCOME TO", PA_CENTER, 30, 1500, PA_OPENING_CURSOR, PA_SPRITE);
          display.displayReset(ZONE_STATUS);

          changeStatusTo = STATUS_WELCOME2;
          break;

        case STATUS_WELCOME2:
            // Initialize sprite data
          _spriteData = ball_0;
          _spriteWidth = W_BALL_0;
          _spriteFrames = F_BALL_0;
          display.setSpriteData(ZONE_STATUS, _spriteData, _spriteWidth, _spriteFrames,
            _spriteData, _spriteWidth, _spriteFrames);

          display.displayZoneText(ZONE_STATUS, "SCHNIPP", PA_CENTER, 30, 1500, PA_OPENING_CURSOR, PA_SPRITE);
          display.displayReset(ZONE_STATUS);

          changeStatusTo = STATUS_WELCOME3;
          break;

        case STATUS_WELCOME3:
          display.setSpriteData(ZONE_STATUS, _spriteData, _spriteWidth, _spriteFrames,
            _spriteData, _spriteWidth, _spriteFrames);

          display.displayZoneText(ZONE_STATUS, "HABT SPASS!", PA_CENTER, 30, 1500, PA_SPRITE, PA_CLOSING_CURSOR);
          display.displayReset(ZONE_STATUS);
          changeStatusTo = STATUS_PHASE;
          break;

        case STATUS_PHASE:
          //STATUS
          _status = "PHASE " + String(_currentPhase + 1);
          display.displayZoneText(ZONE_STATUS, _status.c_str(), PA_CENTER, 80, 3000, PA_OPENING_CURSOR, PA_CLOSING_CURSOR);
          display.displayReset(ZONE_STATUS);
          changeStatusTo = STATUS_IDLE;
          break;

        case STATUS_01_BALLS:
          _status = "NOCH 5 x {";
          display.displayZoneText(ZONE_STATUS, _status.c_str(), PA_CENTER, 80, 3000, PA_OPENING_CURSOR, PA_CLOSING_CURSOR);
          display.displayReset(ZONE_STATUS);
          changeStatusTo = STATUS_02_PHASE;
          break;

        case STATUS_02_PHASE:
          if (_currentScore[0] > _currentScore[1]) {
            _status = "§            }";
            display.displayZoneText(ZONE_STATUS, _status.c_str(), PA_CENTER, 80, 3000, PA_OPENING_CURSOR, PA_CLOSING_CURSOR);
            display.displayReset(ZONE_STATUS);
          } else if (_currentScore[0] < _currentScore[1]) {
            _status = "}            §";
            display.displayZoneText(ZONE_STATUS, _status.c_str(), PA_CENTER, 80, 3000, PA_OPENING_CURSOR, PA_CLOSING_CURSOR);
            display.displayReset(ZONE_STATUS);
          } else {
            _status = "}            }";
            display.displayZoneText(ZONE_STATUS, _status.c_str(), PA_CENTER, 80, 3000, PA_OPENING_CURSOR, PA_CLOSING_CURSOR);
            display.displayReset(ZONE_STATUS);
          }
          changeStatusTo = STATUS_IDLE;
          break;

        case STATUS_03_BLA:
          break;

        case STATUS_IDLE:
        case STATUS_IDLE2:
          //idle-Ani
          _status = " ";
            // Initialize sprite data
          _spriteData = wave;
          _spriteWidth = W_WAVE;
          _spriteFrames = F_WAVE;
          display.setSpriteData(1, _spriteData, _spriteWidth, _spriteFrames,
                    _spriteData, _spriteWidth, _spriteFrames);
          display.displayZoneText(ZONE_STATUS, _status.c_str(), PA_CENTER, 50, 0, PA_SPRITE, PA_SPRITE);        //Animation
          display.displayReset(ZONE_STATUS);
          if (changeStatusTo == STATUS_IDLE) {
            _timestamp = millis();
            changeStatusTo = STATUS_IDLE2;
          } else if (millis() - _timestamp > IDLE_ANIMATION_ZEIT) {
            changeStatusTo = STATUS_01_BALLS;
          }
          break;

        default:
            //
          break;
      }

      if (changeStatusTo >= 0) {
        _nextStatus = changeStatusTo;
      }
    }
  }
}

void MatrixDisplay::setData(uint8_t punkte1, uint8_t punkte2, uint8_t aktiverSpieler, uint8_t phase) {
  if(punkte1 != _currentScore[0] || punkte2 != _currentScore[1] || aktiverSpieler != _currentPlayer || phase != _currentPhase) {
    _currentScore[0] = punkte1;
    _currentScore[1] = punkte2;
    _currentPlayer = aktiverSpieler;
    _currentPhase = phase;
  }
}

void MatrixDisplay::showScore() {
  // Statische char-Arrays verwenden statt String-Objekte
  static char p1Buffer[10];
  static char p2Buffer[10];

  //SPIELER 1 (LINKS)
  if(_blink && _currentPlayer == 0) {
      sprintf(p1Buffer, "   ");
  } else {
      sprintf(p1Buffer, "%02d", _currentScore[0]);
  }
  display.displayZoneText(ZONE_PLAYER1, p1Buffer, PA_LEFT, 80, 800, PA_PRINT);
  display.displayReset(ZONE_PLAYER1);
  
  // SPIELER 2 (Rechts)
  if(_blink && _currentPlayer == 1) {
      sprintf(p2Buffer, "   ");
  } else {
      sprintf(p2Buffer, "%02d", _currentScore[1]);
  }
  
  display.displayZoneText(ZONE_PLAYER2, p2Buffer, PA_RIGHT, 80, 800, PA_PRINT);
  display.displayReset(ZONE_PLAYER2);
  _blink = !_blink;

}

void MatrixDisplay::showStatus(uint8_t zone, String statusText, int duration, textEffect_t effectIn, textEffect_t effectOut) {
  display.displayZoneText(zone, statusText.c_str(), PA_CENTER, 80, duration, effectIn, effectOut);
  display.displayReset(zone);
}

void MatrixDisplay::showMenu() {
  _nextStatus = STATUS_MENU;
  _menuStatus = MENU_0_0_NEW_GAME;

  setZones(1);
  Serial.println("Matrix: Menü aktiviert");
}

void MatrixDisplay::hideMenu() {
  _nextStatus = STATUS_PHASE;
  _menuStatus = MENU_OFF;
  setZones(3);
  //showScore();
  Serial.println("Matrix: Menü deaktiviert");
}

void MatrixDisplay::doButtonNext() {
  switch(_menuStatus) {
    case MENU_2_3_UV_LIGHT:
      _menuStatus = MENU_2_0_LIGHT_BARRIERS;
      break;

    case MENU_2_2_DISKS:
      _menuStatus = MENU_2_3_UV_LIGHT;
      break;

    case MENU_2_1_SERVOS:
      _menuStatus = MENU_2_2_DISKS;
      break;

    case MENU_2_0_LIGHT_BARRIERS:
      _menuStatus = MENU_2_1_SERVOS;
      break;

    case MENU_1_0_P1_PLUS:
      _menuStatus = MENU_1_1_P1_MINUS;
      break;

    case MENU_1_1_P1_MINUS:
      _menuStatus = MENU_1_2_P2_PLUS;
      break;

    case MENU_1_2_P2_PLUS:
      _menuStatus = MENU_1_3_P2_MINUS;
      break;

    case MENU_1_3_P2_MINUS:
      _menuStatus = MENU_1_0_P1_PLUS;
      break;

    case MENU_0_0_NEW_GAME:
      _menuStatus = MENU_0_1_CORRECTIONS;
      break;

    case MENU_0_1_CORRECTIONS:
      _menuStatus = MENU_0_2_TESTS;
      break;

    case MENU_0_2_TESTS:
      _menuStatus = MENU_0_3_SOUNDS;
      break;

    case MENU_0_3_SOUNDS:
      _menuStatus = MENU_0_0_NEW_GAME;
      break;

    default:
      //Shouldnt happen
      Serial.print("!doButtonNext failed! --> ");
      Serial.println(_menuStatus);
      return;
      break;
  }
  setMenuText();
}

void MatrixDisplay::setMenuText() {
  switch(_menuStatus) {
    case MENU_0_0_NEW_GAME:
      _status = "1/4: NEUSTART";
      break;
    case MENU_0_1_CORRECTIONS:
      _status = "2/4: UNDO";
      break;
    case MENU_0_2_TESTS:
      _status = "3/4: TESTS";
      break;
    case MENU_0_3_SOUNDS:
      _status = "4/4: SOUND";
      break;
    case MENU_1_0_P1_PLUS:
      _status = "<---P1 +1";
      break;
    case MENU_1_1_P1_MINUS:
      _status = "<---P1 -1";
      break;
    case MENU_1_2_P2_PLUS:
      _status = "+1 P2 --->";
      break;
    case MENU_1_3_P2_MINUS:
      _status = "-1 P2 --->";
      break;
    case MENU_2_0_LIGHT_BARRIERS:
      _status = "LICHTSCHRANKEN";
      break;
    case MENU_2_1_SERVOS:
      _status = "SERVOS";
      break;
    case MENU_2_2_DISKS:
      _status = "DISKS";
      break;
    case MENU_2_3_UV_LIGHT:
      _status = "UV-LICHT";
      break;
    case MENU_3_0_VOLUME_PLUS:
      _status = "VOLUME +";
      break;
    case MENU_3_1_VOLUME_MINUS:
      _status = "VOLUME -";
      break;
    case MENU_3_2_SOUND_TEST:
      _status = "SOUNDTEST";
      break;
    default:
      _status = " ";
      Serial.print("!setMenuText failed! --> ");
      Serial.println(_menuStatus);
      break;
      
  }
  display.displayZoneText(ZONE_MENU, _status.c_str(), PA_CENTER, 30, 60000, PA_PRINT);
  display.displayReset(ZONE_MENU);
}

void MatrixDisplay::menuSelected() {
  switch(_menuStatus) {
    case MENU_0_0_MAIN_MENU:
      _menuStatus = MENU_0_0_NEW_GAME;
      break;

    case MENU_0_1_CORRECTIONS:
      _menuStatus = MENU_1_0_P1_PLUS;
      break;

    case MENU_0_2_TESTS:
      _menuStatus = MENU_2_0_LIGHT_BARRIERS;
      break;

    case MENU_0_3_SOUNDS:
      _menuStatus = MENU_3_0_VOLUME_PLUS;
      break;

    default:
      _sendToMain(_menuStatus);
      break;
  }
}

void MatrixDisplay::test() {
  Serial.println("=== MATRIX-TEST ===");
  
  // Test alle 3 Zonen
  display.displayZoneText(ZONE_PLAYER1, "P1", PA_CENTER, 100, 2000, PA_SCROLL_UP);
  delay(500);
  display.displayZoneText(ZONE_STATUS, "TEST", PA_CENTER, 100, 2000, PA_SCROLL_LEFT);
  delay(500);
  display.displayZoneText(ZONE_PLAYER2, "P2", PA_CENTER, 100, 2000, PA_SCROLL_DOWN);
  
  // Warte bis Animationen fertig
  unsigned long testStart = millis();
  while(millis() - testStart < 5000) {
    if (display.displayAnimate()) {
      // Animation läuft
    }
    delay(10);
  }
  
  Serial.println("Matrix-Test beendet");
}

void MatrixDisplay::clear() {
  display.displayClear();
  Serial.println("Matrix: Display gelöscht");
}

void MatrixDisplay::setSendToMainCallback(void (*callback)(uint8_t nr)) {
  _sendToMain = callback;
}

