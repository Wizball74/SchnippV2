#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ==============================================================
// PIN DEFINITIONEN und KONSTANTEN
// ==============================================================

// Lichtschranken (obere Spielhälfte)
#define PIN_LS_JACKPOT_FUELLEN    42  // Kugel landet im Jackpot
#define PIN_LS_PLUS_1             40  // +1 Punkt
#define PIN_LS_PLUS_2             41  // +2 Punkte  
#define PIN_LS_JACKPOT_AUSLOESEN  39  // Jackpot auslösen

// Lichtschranken (untere Fächer)
#define PIN_LS_FACH_1  22  // Spieler 1, Vorrat
#define PIN_LS_FACH_2  23  // Spieler 1, +1
#define PIN_LS_FACH_3  24  // Spieler 1, +2
#define PIN_LS_FACH_4  25  // Spieler 2, +2
#define PIN_LS_FACH_5  26  // Spieler 2, +1
#define PIN_LS_FACH_6  27  // Spieler 2, Vorrat

// Servos (Kugelsortierung)
#define PIN_SERVO_1  44  // Spieler 2, +1 Punkt
#define PIN_SERVO_2  46  // Spieler 2, +2 Punkte
#define PIN_SERVO_3  48  // Spieler 1, +2 Punkte
#define PIN_SERVO_4  47  // Spieler 1, +1 Punkt
#define PIN_SERVO_5  49  // Spieler 1, Vorrat
#define PIN_SERVO_6  45  // Jackpot öffnen

// Buttons
#define PIN_BUT_MENU    30
#define PIN_BUT_SELECT  32
#define PIN_BUT_NEXT    31

// Drehscheiben - 28BYJ-48 Stepper Motoren (2x)
// Motor 1 (Linke Drehscheibe) 
#define PIN_MOTOR1_IN1   8   // ULN2003 IN1
#define PIN_MOTOR1_IN2   9   // ULN2003 IN2  
#define PIN_MOTOR1_IN3   10  // ULN2003 IN3
#define PIN_MOTOR1_IN4   11  // ULN2003 IN4

// Motor 2 (Rechte Drehscheibe)
#define PIN_MOTOR2_IN1   12  // ULN2003 IN1
#define PIN_MOTOR2_IN2   13  // ULN2003 IN2
#define PIN_MOTOR2_IN3   14  // ULN2003 IN3  
#define PIN_MOTOR2_IN4   15  // ULN2003 IN4

// LEDs
#define PIN_LED_1        43
#define PIN_SCHWARZLICHT 29  // Relais für UV-LEDs

// Matrix Display
#define PIN_MATRIX_CLK   52
#define PIN_MATRIX_DATA  51
#define PIN_MATRIX_CS    53

#define NUM_BALLS_PER_PHASE      10

// Servo Positionen
#define SERVO_GESCHLOSSEN         90
#define SERVO_OFFEN               30
#define SERVO_OFFEN_JACKPOT       130

// Timing
#define TASTEN_TIMEOUT      400   // 0.4 Sekunden
#define MENU_TIMEOUT        5000  // 5 Sekunden
#define JACKPOT_OFFEN_ZEIT  3000  // 3 Sekunden
#define UV_LICHT_ZEIT       2000  // 1 Sekunde
#define SIEGER_ANZEIGE_ZEIT 60000 // 1 Minute
#define IDLE_ANIMATION_ZEIT 8000  // 8 Sekunden

// Matrix
#define MAX_DEVICES 12
#define MATRIX_BREITE 32
#define MATRIX_HOEHE 8

// Drehscheiben Konstanten
#define MOTOR_STEPS_PER_REV  2048  // 28BYJ-48 mit Getriebe
#define MOTOR_DEFAULT_SPEED  8     // U/min
#define MOTOR_MAX_SPEED      10    // Maximale empfohlene Geschwindigkeit

//Debugging
#define DEBUG                0    // 1 = Debug an, 0 = Debug aus

// ==============================================================
// Konstanten
// ==============================================================

// Display-Zonen (3 Bereiche)
static const uint8_t ZONE_PLAYER1 = 2;    // Links: Module 0-1
static const uint8_t ZONE_STATUS = 1;      // Mitte: Module 2-9
static const uint8_t ZONE_PLAYER2 = 0;    // Rechts: Module 10-11
static const uint8_t ZONE_MENU = 0;    // Nur eine Zone beim Menu

// ==============================================================
// ENUMS
// ==============================================================

enum Fach {
  FACH_1_P1_VORRAT = 0,
  FACH_2_P1_PLUS1 = 1,
  FACH_3_P1_PLUS2 = 2,
  FACH_4_P2_PLUS2 = 3,
  FACH_5_P2_PLUS1 = 4,
  FACH_6_P2_VORRAT = 5
};

enum HitType {
  HIT_NORMAL = 0,
  HIT_PLUS_1 = 1,
  HIT_PLUS_2 = 2,
  HIT_PLUS_JACKPOT = 3,
  HIT_JACKPOT = 4
};

// Sound-Effekte (für künftige DFPlayer Mini Integration)
enum SoundEffect {
  SOUND_START = 1,
  SOUND_END = 2,
  SOUND_BUTTON_PRESS = 3,
  SOUND_HIT_PLUS_ONE = 4,
  SOUND_HIT_PLUS_TWO = 5,
  SOUND_HIT_PLUS_JACKPOT = 6,
  SOUND_HIT_JACKPOT = 7,
  SOUND_PHASE_CHANGE = 8
};

// LED-Strip Farben (für künftige IR-Integration)  
enum LEDColor {
  LED_WHITE = 1,
  LED_BLUE = 2,
  LED_RED = 3,
  LED_GREEN = 4,
  LED_RAINBOW = 5,
  LED_OFF = 6
};

enum Status {
  STATUS_WELCOME1 = 0,
  STATUS_GET_READY = 1,
  STATUS_01_BALLS = 2,
  STATUS_02_PHASE = 13,
  STATUS_03_BLA = 14,
  STATUS_PHASE = 3,
  STATUS_WIN = 10,
  STATUS_WELCOME2 = 11,
  STATUS_WELCOME3 = 12,
  STATUS_IDLE = 100,
  STATUS_IDLE2 = 101,
  STATUS_MENU = 110
};

enum MenuCurrentStatus {
  MENU_OFF = 0,               //Menü aus
  MENU_0_0_MAIN_MENU = 100,   //Hauptmenu
  MENU_0_0_NEW_GAME = 1,      //Neues Spiel
  MENU_0_1_CORRECTIONS = 2,   //Korrekturen
  MENU_0_2_TESTS = 3,         //Tests
  MENU_0_3_SOUNDS = 4,        //Sounds
  MENU_1_0_P1_PLUS = 5,       //Spieler 1 +1
  MENU_1_1_P1_MINUS = 6,      //Spieler 1 -1
  MENU_1_2_P2_PLUS = 7,       //Spieler 2 +1
  MENU_1_3_P2_MINUS = 8,      //Spieler 2 -1
  MENU_2_0_LIGHT_BARRIERS = 10,//Lichtschranken
  MENU_2_1_SERVOS = 11,       //Servos
  MENU_2_2_DISKS = 12,        //Drehscheiben
  MENU_2_3_UV_LIGHT = 13,     //Schwarzlicht
  MENU_3_0_VOLUME_PLUS = 15,  //Volume +
  MENU_3_1_VOLUME_MINUS = 16, //Volume -
  MENU_3_2_SOUND_TEST = 17,   //Sound-Test
};

enum ButtonPressed {
  BUTTON_NONE = 0,
  BUTTON_MENU = 1,
  BUTTON_SELECT = 2,
  BUTTON_NEXT = 3
};

#endif