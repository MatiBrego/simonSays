#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

#include "wifi_ruts.h"
#include "webpage.h"

WebServer server;
WebSocketsServer webSocket = WebSocketsServer(81);

const uint8_t ledPins[] = {23, 17, 33, 14};
const uint8_t buttonPins[] = {22, 16, 32, 27};

#define SPEAKER_PIN 2

#define MAX_GAME_LENGTH 100

#define NOTE_G3 196
#define NOTE_C4 261.63
#define NOTE_E4 329.63
#define NOTE_G5 783.99

const double gameTones[] = { NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G5};

uint8_t gameSequence[MAX_GAME_LENGTH] = {0};
uint8_t gameIndex = 0;
boolean buttonPressed[] = {false, false, false, false};

static void
webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    String color;
    size_t i;

    if (type == WStype_TEXT)
    {
      for (i = 0; i < length; i++)
      color += (char)payload[i];
      if(color == "red"){
        buttonPressed[0] = true;
      }

      if(color == "blue"){
        buttonPressed[1] = true;
      }

      if(color == "yellow"){
        buttonPressed[2] = true;
      }

      if(color == "green"){
        buttonPressed[3] = true;
      }
    }
}

void
setup(void)
{
   Serial.begin(BAUD);

    wifi_connect();

    server.on("/", []() { server.send_P(200, "text/html", webpage); });
    server.begin();
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

   for (byte i = 0; i < 4; i++) {
      pinMode(ledPins[i], OUTPUT);
      pinMode(buttonPins[i], INPUT_PULLUP);
    }
    pinMode(SPEAKER_PIN, OUTPUT);
    randomSeed(analogRead(A3));
}

void lightLedAndPlayTone(byte ledIndex) {
  digitalWrite(ledPins[ledIndex], HIGH);
  tone(SPEAKER_PIN, gameTones[ledIndex]);
  delay(300);
  digitalWrite(ledPins[ledIndex], LOW);
  noTone(SPEAKER_PIN);
}

void playSequence() {
  for (int i = 0; i < gameIndex; i++) {
    byte currentLed = gameSequence[i];
    lightLedAndPlayTone(currentLed);
    delay(50);
  }
}

byte readButtons() {
  while (true) {  
    webSocket.loop();
    server.handleClient();

    for (byte i = 0; i < 4; i++) {
      byte buttonPin = buttonPins[i];
      if (buttonPressed[i]) {
        return i;
      }
    }
    delay(1);
  }
}

void playLevelUpSound() {
//   tone(SPEAKER_PIN, NOTE_E4);
//   delay(150);
//   tone(SPEAKER_PIN, NOTE_G4);
//   delay(150);
//   tone(SPEAKER_PIN, NOTE_E5);
//   delay(150);
//   tone(SPEAKER_PIN, NOTE_C5);
//   delay(150);
//   tone(SPEAKER_PIN, NOTE_D5);
//   delay(150);
//   tone(SPEAKER_PIN, NOTE_G5);
  delay(150);
  noTone(SPEAKER_PIN);
}


void gameOver() {
  String score = (String) (gameIndex - 1);
  webSocket.broadcastTXT(score);
  gameIndex = 0;
  delay(200);

  // Play a Wah-Wah-Wah-Wah sound
//   tone(SPEAKER_PIN, NOTE_DS5);
  delay(300);
//   tone(SPEAKER_PIN, NOTE_D5);
  delay(300);
//   tone(SPEAKER_PIN, NOTE_CS5);
  delay(300);
  for (byte i = 0; i < 10; i++) {
    for (int pitch = -10; pitch <= 10; pitch++) {
      // tone(SPEAKER_PIN, NOTE_C5 + pitch);
      delay(5);
    }
  }
  noTone(SPEAKER_PIN);

//   sendScore(DASH, DASH);
  delay(500);
}


bool checkUserSequence() {

  for(int j = 0; j < 4 ; j++){
      buttonPressed[j] = false;
  }

  for (int i = 0; i < gameIndex; i++) {
    byte expectedButton = gameSequence[i];
    byte actualButton = readButtons();
    lightLedAndPlayTone(actualButton);
    if (expectedButton != actualButton) {
      for(int j = 0; j < 4 ; j++){
        buttonPressed[j] = false;
      }
      return false;
    }

    for(int j = 0; j < 4 ; j++){
      buttonPressed[j] = false;
    }
  }
  return true;
}



void
loop(void)
{
   gameSequence[gameIndex] = random(0, 4);
   gameIndex++;
   if (gameIndex >= MAX_GAME_LENGTH) {
      gameIndex = MAX_GAME_LENGTH - 1;
   }

   playSequence();
   if (!checkUserSequence()) {
      gameOver();
   }

   delay(300);

   if (gameIndex > 0) {
      playLevelUpSound();
      delay(300);
  }
}
