#ifndef MATRIXDISPLAY_H
#define MATRIXDISPLAY_H

#include "Config.h"
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// triangular wave / worm
static const uint8_t F_WAVE = 14;
static const uint8_t W_WAVE = 14;
static const uint8_t PROGMEM wave[F_WAVE * W_WAVE] =
{
  0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10,
  0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20,
  0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40,
  0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
  0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
  0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
  0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10,
  0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08,
  0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04,
  0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02,
  0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02,
  0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04,
  0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08,
};

const uint8_t SYMBOL_SKULL[] = { 8, 14, 27, 187, 175, 175, 187, 27, 14 };
const uint8_t SYMBOL_SMILEY_1[] = { 8, 0, 6, 7, 211, 219, 31, 14, 0 };
const uint8_t SYMBOL_SMILEY_0[] = { 8, 60, 118, 179, 159, 159, 179, 118, 60 };
const uint8_t SYMBOL_BALL_1[] = { 8, 60, 110, 223, 255, 253, 253, 122, 60 };
const uint8_t SYMBOL_BALL_0[] = { 8, 60, 102, 255, 255, 255, 253, 114, 60 };

const uint8_t F_BALL_1 = 1;
const uint8_t W_BALL_1 = 8;
static const uint8_t PROGMEM ball_1[F_BALL_1 * W_BALL_1] =
{
 0x3c, 0x6e, 0xdf, 0xff, 0xfd, 0xfd, 0x7a, 0x3c
};

const uint8_t F_BALL_0 = 1;
const uint8_t W_BALL_0 = 8;
static const uint8_t PROGMEM ball_0[F_BALL_0 * W_BALL_0] =
{
 0x3c, 0x66, 0xff, 0xff, 0xff, 0xfd, 0x72, 0x3c
};

const uint8_t F_GHOST = 2;
const uint8_t W_GHOST = 7;
static const uint8_t PROGMEM ghost[F_GHOST * W_GHOST] =
{
 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe,
};

class MatrixDisplay {
private:
  MD_Parola display;

  // Menü-Texte
  static const char* MENU_ITEMS_ROOT[];
  static const char* MENU_ITEMS_SPIEL[];
  static const char* MENU_ITEMS_TEST[];
  static const char* MENU_ITEMS_SOUND[];
  

  // Sprite data members (replaced problematic struct array)
  const uint8_t* _spriteData;
  uint8_t _spriteWidth;
  uint8_t _spriteFrames;

  // Spielstand-Anzeige
  uint8_t _currentScore[2];
  uint8_t _currentPlayer;
  uint8_t _currentPhase;
  uint8_t _currentJackpot;

  uint8_t _lastScore[2];
  uint8_t _lastPlayer;
  uint8_t _lastPhase;
  uint8_t _lastJackpot;

  bool _blink;
  int16_t _intensity = 150;
  bool _dirUp = false;

  // Statusmeldungen
  //ToDo

  // **Welches Menu anzeigen?**
  uint8_t _menuStatus;
  unsigned long _menuTimestamp = 0;
  uint8_t _menuCurrentSelection = 0;

  uint8_t _nextStatus;
  String _status;
  unsigned long _timestamp;
  uint8_t _bla = 0;

  // Private Methoden
  void setZones(uint8_t zone);
  void menuSelected();
  void doButtonNext();
  
  // Callback-Funktionen
  void (*_sendToMain)(uint8_t nr);

public:
  MatrixDisplay();

  void init();
  void loop(uint8_t but);
  
  // Callback-Funktionen für Menü-Aktionen
  void setSendToMainCallback(void (*callback)(uint8_t nr));
  
  // Haupt-Display Funktionen
  void setData(uint8_t punkte1, uint8_t punkte2, uint8_t aktiverSpieler, uint8_t phase);
  void showScore();
  void showMenu();
  void setMenuText();
  void showStatus(uint8_t zone, String statusText, int duration, textEffect_t effectIn, textEffect_t effectOut);

  // Menü-Funktionen
  void hideMenu();
  
    // Test-Funktionen
  void test();
  void clear();
};

#endif