#include <Arduino.h>
#include <SPI.h>

void initiateButtonChallenge(char selectedColor);
void evaluateButtonPress();
void lightUpLED(char selectedColor);
void turnOffAllLEDs();

volatile char spiCommand = '\0';
volatile bool newSPICommand = false;

const int analogButtonPin = A0;
const int buzzerControlPin = 2;
const int commonSmallLEDPin = 3;

const int p1RedLEDPin = 4;
const int p1GreenLEDPin = 5;
const int p1BlueLEDPin = 6;
const int p2RedLEDPin = 7;
const int p2GreenLEDPin = 8;
const int p2BlueLEDPin = 9;

unsigned long buttonPressStartTime = 0;
bool isWaitingForButtonPress = false;
bool isPlayerOneActive = true;

void setup() {
  Serial.begin(28800);         
  SPCR |= bit(SPE);            
  pinMode(MISO, OUTPUT);       
  SPI.attachInterrupt();       

  pinMode(buzzerControlPin, OUTPUT);
  pinMode(commonSmallLEDPin, OUTPUT);
  pinMode(p1RedLEDPin, OUTPUT);
  pinMode(p1GreenLEDPin, OUTPUT);
  pinMode(p1BlueLEDPin, OUTPUT);
  pinMode(p2RedLEDPin, OUTPUT);
  pinMode(p2GreenLEDPin, OUTPUT);
  pinMode(p2BlueLEDPin, OUTPUT);

  digitalWrite(commonSmallLEDPin, HIGH); 
}


ISR(SPI_STC_vect) {
  char receivedData = SPDR;
  if (receivedData != '#') { 
    spiCommand = receivedData;
    newSPICommand = true;
  }
}

void loop() {
  if (newSPICommand) {
    newSPICommand = false;

    
    if (spiCommand == 'r' || spiCommand == 'g' || spiCommand == 'b') {
      initiateButtonChallenge(spiCommand);
    } else {
      SPDR = '$'; 
    }
  }

  if (isWaitingForButtonPress) {
    evaluateButtonPress();
  }
}

void initiateButtonChallenge(char selectedColor) {
  Serial.print("Received command: ");
  Serial.print(spiCommand);
  Serial.println(isPlayerOneActive ? " for player 1" : " for player 2");

  buttonPressStartTime = millis();
  isWaitingForButtonPress = true;
  lightUpLED(selectedColor);
}

void evaluateButtonPress() {
  unsigned long elapsedTime = millis() - buttonPressStartTime;
  int buttonReadValue = analogRead(analogButtonPin);
  Serial.print("Button value: ");
  Serial.println(buttonReadValue);

  bool isButtonPressed = false;

  if ( buttonReadValue <= 50) {
    return; 
  }

  if (isPlayerOneActive) {
    if ((spiCommand == 'b' && buttonReadValue >= 120 && buttonReadValue <= 140) ||
        (spiCommand == 'g' && buttonReadValue >= 85 && buttonReadValue <= 100) ||
        (spiCommand == 'r' && buttonReadValue >= 955 && buttonReadValue <= 970)) {
      isButtonPressed = true;
    }
  } else {
    if ((spiCommand == 'b' && buttonReadValue >= 80 && buttonReadValue <= 90) ||
        (spiCommand == 'g' && buttonReadValue >= 935 && buttonReadValue <= 945) ||
        (spiCommand == 'r' && buttonReadValue >= 970 && buttonReadValue <= 980)) {
      isButtonPressed = true;
    }
  }

  if (isButtonPressed) {
    isWaitingForButtonPress = false;
    char scoreGrade;

    if (elapsedTime <= 300) {
      scoreGrade = 'e'; 
    } else if (elapsedTime <= 600) {
      scoreGrade = 'g';
    } else if (elapsedTime <= 1000) {
      scoreGrade = 'm';
    } else {
      scoreGrade = 'i'; 
    }

    SPDR = scoreGrade; 
    if (scoreGrade != 'i') {
      tone(buzzerControlPin, 659, 200); 
    }
    isPlayerOneActive = !isPlayerOneActive; 
  } else if (elapsedTime > 900) {
    isWaitingForButtonPress = false;
    SPDR = 'i';
    isPlayerOneActive = !isPlayerOneActive; 
  }
}

void lightUpLED(char selectedColor) {
  turnOffAllLEDs();
  int targetLEDPin;

  if (isPlayerOneActive) {
    targetLEDPin = (selectedColor == 'r') ? p1RedLEDPin :
                   (selectedColor == 'g') ? p1GreenLEDPin :
                   p1BlueLEDPin;
    Serial.print("Lighting Player 1 LED: ");
  } else {
    targetLEDPin = (selectedColor == 'r') ? p2RedLEDPin :
                   (selectedColor == 'g') ? p2GreenLEDPin :
                   p2BlueLEDPin;
    Serial.print("Lighting Player 2 LED: ");
  }

  Serial.println(targetLEDPin);
  digitalWrite(targetLEDPin, HIGH);
}

void turnOffAllLEDs() {
  digitalWrite(p1RedLEDPin, LOW);
  digitalWrite(p1GreenLEDPin, LOW);
  digitalWrite(p1BlueLEDPin, LOW);
  digitalWrite(p2RedLEDPin, LOW);
  digitalWrite(p2GreenLEDPin, LOW);
  digitalWrite(p2BlueLEDPin, LOW);
}
