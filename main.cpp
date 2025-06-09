#include <Arduino.h>
#include "Config.h"
#include "MatrixDisplay.h"
#include "TastenSteuerung.h"
//#include "SpielManager.h"

// ==============================================================
// GLOBALE OBJEKTE
// ==============================================================
TastenSteuerung tasten;
MatrixDisplay matrix;

unsigned long timestamp;
uint8_t butPressed = BUTTON_NONE;

// ==============================================================
// DUMMY-FUNKTIONEN
// ==============================================================

void matrixCallback(uint8_t nr);

// ==============================================================
// SETUP & LOOP
// ==============================================================

// SETUP
void setup() {
  Serial.begin(115200);
  delay(500);

  // **STARTUP-DELAY FÜR EXTERNE STROMVERSORGUNG - BLOCKING**
  Serial.println("Schnipp - Warte auf Hardware-Stabilisierung...");

  // SETZE PINS
  // **ALLE PINS SOFORT SICHER AUSSCHALTEN**
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_SCHWARZLICHT, OUTPUT);
  digitalWrite(PIN_LED_1, LOW);
  digitalWrite(PIN_SCHWARZLICHT, HIGH);
  
  // **1 SEKUNDE WARTEN MIT LED-BLINKEN**
  for(int i = 0; i < 2; i++) {  // 2 x 500ms = 1 Sekunde
    for (int u = 0; u < 3; u++) {
      digitalWrite(PIN_LED_1, HIGH);
      delay(200);  // 125ms an
      digitalWrite(PIN_LED_1, LOW);
      delay(300);  // 125ms aus
    }
  }
  
  // **STARTUP ABGESCHLOSSEN**
  digitalWrite(PIN_LED_1, LOW);        // LED definitiv aus
  digitalWrite(PIN_SCHWARZLICHT, LOW); // UV-Licht definitiv aus

  // **STARTUP**
  Serial.println("Stabilisierung abgeschlossen, initialisiere Hardware...");
  matrix.init();
  matrix.setSendToMainCallback(matrixCallback);
  
  tasten.init();
}

void loop() {
  timestamp = millis();
  // Tasten-Eingaben verarbeiten
  butPressed = tasten.loop(timestamp);

  // Matrix-Display
  matrix.loop(butPressed);

}  

// ==============================================================
// FUNKTIONEN
// ==============================================================

void matrixCallback(uint8_t menuItem) {
  Serial.print("Matrix-Callback aufgerufen mit Parameter: ");
  Serial.println(menuItem);
  
  switch(menuItem) {
    case MENU_0_0_NEW_GAME:
      matrix.showStatus(ZONE_MENU, "-> NEW GAME", 1000, PA_PRINT, PA_PRINT);
      break;
    case MENU_1_0_P1_PLUS:
      matrix.showStatus(ZONE_MENU, "-> P1 + 1", 1000, PA_PRINT, PA_PRINT);
      break;
    case MENU_1_1_P1_MINUS:
      matrix.showStatus(ZONE_MENU, "-> P1 - 1", 1000, PA_PRINT, PA_PRINT);
      break;
    case MENU_1_2_P2_PLUS:
      matrix.showStatus(ZONE_MENU, "-> P2 + 1", 1000, PA_PRINT, PA_PRINT);
      break;
    case MENU_1_3_P2_MINUS:
      matrix.showStatus(ZONE_MENU, "-> P2 - 1", 1000, PA_PRINT, PA_PRINT);
      break;
    case MENU_2_0_LIGHT_BARRIERS:
      matrix.showStatus(ZONE_MENU, "-> Test Light", 1000, PA_PRINT, PA_PRINT);
      break;
    case MENU_2_1_SERVOS:
      matrix.showStatus(ZONE_MENU, "-> Test Servo", 1000, PA_PRINT, PA_PRINT);
      break;
    case MENU_2_2_DISKS:
      matrix.showStatus(ZONE_MENU, "-> Test Disks", 1000, PA_PRINT, PA_PRINT);
      break;
    case MENU_2_3_UV_LIGHT:
      matrix.showStatus(ZONE_MENU, "-> Test UV", 1000, PA_PRINT, PA_PRINT);
      break;
    case MENU_3_0_VOLUME_PLUS:
      matrix.showStatus(ZONE_MENU, "-> Volume +", 1000, PA_PRINT, PA_PRINT);
      break;
    case MENU_3_1_VOLUME_MINUS:
      matrix.showStatus(ZONE_MENU, "-> Volume -", 1000, PA_PRINT, PA_PRINT);
      break;
    case MENU_3_2_SOUND_TEST:
      matrix.showStatus(ZONE_MENU, "-> SOUND TEST", 1000, PA_PRINT, PA_PRINT);
      break;

    default:
      Serial.println("-> Unbekannter Callback-Parameter!");
      break;
  }
}
