# Schnipp - Arduino Flipper Game

Ein Arduino-basiertes Geschicklichkeitsspiel für 2 Spieler mit LED-Matrix-Anzeige, Servo-gesteuerter Kugelsortierung und Jackpot-System.

## 🎮 Spielbeschreibung

**Schnipp** ist ein Flipper-ähnliches Geschicklichkeitsspiel, bei dem zwei Spieler abwechselnd Kugeln mit dem Finger von unten nach oben "schnippen". Das Ziel ist es, durch geschicktes Zielen Punkte zu sammeln und als Erster 30 Punkte zu erreichen.

### Spielphasen
- **Phase 1 (0-9 Punkte)**: LED weiß, normale Punktevergabe
- **Phase 2 (10-19 Punkte)**: LED blau, +1 Ziele geben +2 Punkte
- **Phase 3 (20-30 Punkte)**: LED rot, +1 Punkte gehen an den Gegner!

## 🔧 Hardware-Komponenten

### Hauptkomponenten
- **Arduino Mega 2560** - Hauptcontroller
- **12x MAX7219 LED-Matrix Module** - Spielstand und Animationen
- **6x Servo-Motoren** - Kugelsortierung
- **10x Lichtschranken (LM393 + Laser)** - Treffererkennung
- **Nema 17 Stepper** - Drehscheiben
- **28BYJ-48 Stepper** - Fänger-Mechanismus

### Sensoren & Eingabe
- **3x Taster** - Menü, Auswahl, Weiter
- **Schwarzlicht LEDs** - Spezialeffekte in Phase 3
- **Diverse Status-LEDs**

### Mechanik
- **Magnete** - Kugeln "einfangen"
- **Drehscheiben** - Dynamische Hindernisse
- **Jackpot-System** - Kugeln sammeln und freigeben
- **6-Fach Sortierung** - Automatische Punktevergabe

## 📁 Projekt-Struktur

```
/src/
├── schnipp.ino           # Hauptsketch
├── Config.h              # Pin-Definitionen & Konstanten
├── Spielmanager.h/.cpp   # Kern-Spiellogik
├── Matrixanzeige.h/.cpp  # LED-Matrix Steuerung
├── Lichtschranken.h/.cpp # Sensor-System
├── Kugelsortierung.h/.cpp # Servo-Steuerung  
├── Jackpotsystem.h/.cpp  # Jackpot-Mechanismus
└── Tastensteuerung.h/.cpp # Menü & Eingabe
```

## 🚀 Installation & Setup

### Benötigte Libraries
```cpp
// In Arduino IDE installieren:
#include <Servo.h>           // Arduino Standard
#include <MD_Parola.h>       // LED Matrix Animationen
#include <MD_MAX72xx.h>      // MAX7219 Treiber
#include <SPI.h>             // Arduino Standard
```

### Hardware-Anschluss
Siehe `Config.h` für vollständige Pin-Belegung:

```cpp
// Beispiel wichtige Pins:
#define PIN_MATRIX_CS    53   // LED Matrix
#define PIN_BUT_MENU     30   // Menü-Taste
#define PIN_SERVO_6      45   // Jackpot-Servo
#define PIN_SCHWARZLICHT 29   // UV-LEDs
```

### Upload-Anleitung
1. Arduino IDE öffnen
2. Board: "Arduino Mega 2560" auswählen
3. Alle `.h` und `.cpp` Dateien in Projektordner
4. `schnipp.ino` öffnen und hochladen

## 🎯 Spielregeln

### Grundlagen
- **2 Spieler** abwechselnd
- **10 Kugeln** pro Spieler
- **Ziel: 30 Punkte** erreichen
- **Maximal 3 Versuche** wenn Kugel zurückrollt

### Punktevergabe
| Ziel | Phase 1 | Phase 2 | Phase 3 |
|------|---------|---------|---------|
| +1 Löcher | +1 | +2 | Gegner +1 |
| +2 Löcher | +2 | +2 | +2 |
| Jackpot | min. 3 | min. 6 | min. 3 |

### Jackpot-System
- Kugeln sammeln sich im Jackpot-Behälter
- Auslösung gibt minimum 3 Punkte
- Jede gesammelte Kugel = +1 Punkt extra
- In Phase 2: Doppelte Punkte

## 🎮 Menü-System

**Menü-Button**: Öffnet/schließt Menü  
**Next-Button**: Nächster Menüpunkt  
**Select-Button**: Auswahl bestätigen

### Hauptmenü
1. **Neues Spiel** - Spiel neu starten
2. **Spielstand** - Punkte manuell ändern
3. **Tests** - Hardware-Funktionstest
4. **Sound** - Lautstärke einstellen

### Test-Funktionen
- Lichtschranken-Status anzeigen
- Servos einzeln testen
- Drehscheiben & Fänger testen
- Schwarzlicht ein/aus
- LED-Strip Farbtest

## 🔧 Entwicklung & Anpassung

### Code-Struktur
- **Objektorientiert** mit separaten Klassen
- **Header/Source-Trennung** für bessere Organisation  
- **Test-Funktionen** in jeder Klasse
- **Umfangreiche Kommentierung**

### Erweiterungsmöglichkeiten
- **Sound-System** (DFPlayer Mini)
- **LED-Strip Steuerung** (IR-basiert)
- **Erweiterte Animationen**
- **Statistik-System**
- **Turnier-Modus**

### Debug-Ausgaben
Alle wichtigen Events werden über Serial Monitor ausgegeben:
```
Schnipp - Initialisierung...
Spielmanager: Bereit
Neues Spiel gestartet
Spieler 1: +2 = 2
JACKPOT AUSGELÖST! 5 Kugeln = 5 Punkte
```

## 📊 Status & TODOs

### ✅ Implementiert
- Grundlegende Spiellogik
- LED-Matrix mit Animationen
- Lichtschranken-System
- Servo-Steuerung
- Jackpot-Mechanismus
- Menü-System

### 🔄 In Arbeit
- Stepper-Motor Integration
- Sound-System
- LED-Strip Steuerung
- Erweiterte Tests

### 📝 Geplant
- Statistik-Speicherung
- Kalibrierungs-Menü
- Tournament-Modus
- Remote-Steuerung

## 🤝 Beitragen

Das Projekt ist modular aufgebaut - neue Features können einfach als separate Klassen hinzugefügt werden. 

**Coding-Standards:**
- Klassen in `.h/.cpp` aufteilen
- Test-Funktionen implementieren
- Deutsche Kommentare, englische Code-Namen
- Konsistente Namenskonvention

## 📄 Lizenz

Dieses Projekt ist für Lern- und Hobbyzwecke entwickelt. 

---

**Viel Spaß beim Schnippen! 🎯**