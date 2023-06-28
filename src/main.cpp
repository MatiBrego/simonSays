#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

#include "wifi_ruts.h"
#include "webpage.h"


// Create http server
WebServer server;

// Create web socket server
WebSocketsServer webSocket = WebSocketsServer(81);

// Define led pins
const uint8_t ledPins[] = {23, 17, 33, 14}; // {red, blue, yellow, green}

// Define speaker pin
#define SPEAKER_PIN 2

// Define max game length
#define MAX_GAME_LENGTH 100

// Define note frequencies for speaker
#define NOTE_G3 196
#define NOTE_C4 261.63
#define NOTE_E4 329.63
#define NOTE_G5 783.99
#define NOTE_G4 392.00
#define NOTE_E5 659.25
#define NOTE_C5 523.25
#define NOTE_D5 587.33
#define NOTE_DS5 622
#define NOTE_CS5 554

// Declare one tone for each Led
const double gameTones[] = { NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G5};

// Declare variables to handle the game logic
uint8_t gameSequence[MAX_GAME_LENGTH] = {0};
uint8_t gameIndex = 0;
boolean buttonPressed[] = {false, false, false, false};

// Handle user input from the web socket
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

    server.on("/", []() { server.send_P(200, "text/html", webpage); }); // Send webpage on http get request
    server.begin();

    webSocket.begin();
    webSocket.onEvent(webSocketEvent); // Function to handle sockets event

   for (byte i = 0; i < 4; i++) {
      pinMode(ledPins[i], OUTPUT);
    }
    pinMode(SPEAKER_PIN, OUTPUT);
    randomSeed(analogRead(A3));
}

// Function to light a led and play the corresponding tone
void lightLedAndPlayTone(byte ledIndex) {
  digitalWrite(ledPins[ledIndex], HIGH);
  tone(SPEAKER_PIN, gameTones[ledIndex]);
  delay(300);
  digitalWrite(ledPins[ledIndex], LOW);
  noTone(SPEAKER_PIN);
}

// Plays the sequence
void playSequence() {
  for (int i = 0; i < gameIndex; i++) {
    byte currentLed = gameSequence[i];
    lightLedAndPlayTone(currentLed);
    delay(50);
  }
}

// Loop that waits for user input through web socket.
byte readButtons() {
  while (true) {  
    webSocket.loop();
    server.handleClient();

    for (byte i = 0; i < 4; i++) {
      if (buttonPressed[i]) {
        return i;
      }
    }
    delay(1);
  }
}

// Plays level up music when user input is correct
void playLevelUpSound() {
  tone(SPEAKER_PIN, NOTE_E4);
  delay(150);
  tone(SPEAKER_PIN, NOTE_G4);
  delay(150);
  tone(SPEAKER_PIN, NOTE_E5);
  delay(150);
  tone(SPEAKER_PIN, NOTE_C5);
  delay(150);
  tone(SPEAKER_PIN, NOTE_D5);
  delay(150);
  tone(SPEAKER_PIN, NOTE_G5);
  delay(150);
  noTone(SPEAKER_PIN);
}

// Sends the score to the web socket client, resets the game and plays the game over music
void gameOver() {
  String score = (String) (gameIndex - 1);
  webSocket.broadcastTXT(score); // Send score to client
  gameIndex = 0;
  delay(200);

  // Play a Wah-Wah-Wah-Wah sound
  tone(SPEAKER_PIN, NOTE_DS5);
  delay(300);
  tone(SPEAKER_PIN, NOTE_D5);
  delay(300);
  tone(SPEAKER_PIN, NOTE_CS5);
  delay(300);
  tone(SPEAKER_PIN, NOTE_C5);
  delay(900);
  noTone(SPEAKER_PIN);

  delay(500);
}

// Waits for user input in from the readButtons() loop and checks it
bool checkUserSequence() {

  for (int i = 0; i < gameIndex; i++) {
    byte expectedButton = gameSequence[i];
    byte actualButton = readButtons();

    lightLedAndPlayTone(actualButton);

    for(int j = 0; j < 4 ; j++){ // Set all buttonPressed to false after reading
      buttonPressed[j] = false;
    }
    
    if (expectedButton != actualButton) {

      return false;
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
