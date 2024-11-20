#include <Arduino.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>

const int lcdRS = 9;
const int lcdE = 8;
const int lcdD4 = 5;
const int lcdD5 = 4;
const int lcdD6 = 3;
const int lcdD7 = 2;

LiquidCrystal lcd(lcdRS, lcdE, lcdD4, lcdD5, lcdD6, lcdD7);
Servo gameServo;

const int startButtonPin = 7;
const int servoControlPin = 6;
const int servoMaxPosition = 180;
const unsigned long totalGameTime = 30000; 

char availableColors[] = {'r', 'g', 'b'};

unsigned long startGameTime = 0;
unsigned long lastTurnTime = 0;
unsigned long turnInterval = 3000; 
bool gameInProgress = false;
bool waitingForResponse = false;
char receivedResponse;

String playerOneName, playerTwoName;
int playerOneScore = 0;
int playerTwoScore = 0;
int roundNumber = 0;
bool isPlayerOneTurn = true;


void startGame();
void checkGameStatus();
void processTurn();
void displayFinalScores();
void resetGameSettings();
void transmitColorCommand();
char exchangeSPICommand(char command);
void updatePlayerScore(char scoreResponse);
String readPlayerName();

void setup() {
  Serial.begin(28800);
  Serial.println("testare");
  SPI.begin();
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  pinMode(startButtonPin, INPUT_PULLUP);
  Serial.println("testare");
  
  lcd.begin(16, 2);
  gameServo.attach(servoControlPin);
  lcd.print("Press to start!");
  Serial.println("testare");
}

void loop() {
  
  if (!gameInProgress && digitalRead(startButtonPin) == LOW) {
    delay(200);
    startGame();
  }

 
  if (gameInProgress) {
    checkGameStatus();
  }
}

void startGame() {
  gameInProgress = true;
  lcd.clear();
  lcd.print("Enter player 1:");
  Serial.println("Enter player 1 name:");

 
  playerOneName = readPlayerName();

  lcd.clear();
  lcd.print("Enter player 2:");
  Serial.println("Enter player 2 name:");


  playerTwoName = readPlayerName();

  
  startGameTime = millis();
  lcd.clear();
  lcd.print("game starting!");
  delay(1000);
  lcd.clear();
}

String readPlayerName() {
  String playerName = "";
  while (true) {
    if (Serial.available() > 0) {
      char incomingChar = Serial.read();
      if (incomingChar == '\n') {
        break; 
      }
      
      if (isPrintable(incomingChar)) {
        playerName += incomingChar;
        lcd.setCursor(playerName.length() - 1, 1); 
        lcd.print(incomingChar); 
      }
    }
  }
  return playerName;
}

void checkGameStatus() {
  unsigned long currentMillis = millis();

  if (currentMillis - startGameTime >= totalGameTime) {
    gameInProgress = false;
    displayFinalScores();
    resetGameSettings();
    return;
  }

  int servoAngle = map(currentMillis - startGameTime, 0, totalGameTime, 0, servoMaxPosition);
  gameServo.write(servoAngle);

  if (currentMillis - lastTurnTime >= turnInterval) {
    processTurn();
    lastTurnTime = currentMillis;
  }

  if (waitingForResponse) {
    Serial.println((isPlayerOneTurn ? playerTwoName : playerOneName) + " received score:");
    Serial.println(receivedResponse);
    updatePlayerScore(receivedResponse);
    waitingForResponse = false;
  }
}

void processTurn() {
  if (roundNumber < 30) {
    lcd.clear();
    lcd.print((isPlayerOneTurn ? playerOneName : playerTwoName) + " turn");
    transmitColorCommand();
    delay(1000);
    waitingForResponse = true;
    isPlayerOneTurn = !isPlayerOneTurn;
    roundNumber++;
  }
}

void transmitColorCommand() {

  char colorToSend = availableColors[random(0, 3)];
  exchangeSPICommand(colorToSend);
  delay(50); 
  Serial.print("Sent color: ");
  Serial.println(colorToSend);
}

char exchangeSPICommand(char command) {

  digitalWrite(SS, LOW);
  char response = SPI.transfer(command);
  digitalWrite(SS, HIGH);
  delay(50); 
  return response;
}

void updatePlayerScore(char scoreResponse) {
  int scorePoints = 0;

  switch (scoreResponse) {
    case 'e': scorePoints = 100; break; 
    case 'g': scorePoints = 50; break;  
    case 'm': scorePoints = 25; break;  
    case 'i': scorePoints = 0; break;   
  }

  if (isPlayerOneTurn) {
    playerTwoScore += scorePoints;
  } else {
    playerOneScore += scorePoints;
  }
}

void displayFinalScores() {

  lcd.clear();
  lcd.print(playerOneName + " score:");
  lcd.setCursor(0, 1);
  lcd.print(playerOneScore);
  delay(2000);

  lcd.clear();
  lcd.print(playerTwoName + " score:");
  lcd.setCursor(0, 1);
  lcd.print(playerTwoScore);
  delay(2000);

  lcd.clear();
  if (playerOneScore > playerTwoScore) {
    lcd.print("winner: " + playerOneName);
  } else if (playerTwoScore > playerOneScore) {
    lcd.print("winner: " + playerTwoName);
  } else {
    lcd.print("it's a draw!");
  }
  delay(2000);
}

void resetGameSettings() {
  playerOneScore = 0;
  playerTwoScore = 0;
  roundNumber = 0;
  isPlayerOneTurn = true;
  lcd.clear();
  lcd.print("Press to start!");
}