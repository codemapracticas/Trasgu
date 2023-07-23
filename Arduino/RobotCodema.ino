#include "Servo.h"

Servo servoR, servoL;
Servo servoRT, servoLT;

//Delays
int defaultDelay = 200, lineasDelay = 0;
int giro_180 = 1028, giro_90 = 514, giro_45 = 257, giro_30 = 200; //Delays para diferentes ángulos

int pinBuz = 12;        // pin del Claxon

int traccion4R = true;  // Estado tracción 4 ruedas

String inString = ""; // String para lectura/escritura desde Protocoder

void setup() {


  Serial.begin(19200); //NO MODIFICAR!!!!!!!!!!!
  Serial.flush();

  servoL.attach(9);   // Servo rueda izquierda delantera
  servoR.attach(10);  // Servo rueda derecha delantera
  servoLT.attach(5);  // Servo rueda izquierda trasera
  servoRT.attach(6);  // Servo rueda derecha trasera
  
  servoR.write(90);   // Parar los servos
  servoL.write(90);
  servoRT.write(90);
  servoLT.write(90);
}

//Funciones lectura/escritura desde Appinventor
void readFromAndroid() {
  char inChar;
  while (Serial.available() > 0) {
    inChar = (char) Serial.read();
    Serial.flush();
    if (inChar == '=')
      inString = "";
    else if (inChar != '+')
      inString += inChar;
  }
}
void writeData() {
  if (inString == "avanzar") {
    avanzar();
    delay(defaultDelay);
    inString = "";
  }
  if (inString == "retroceder") {
    retroceder();
    delay(defaultDelay);
    inString = "";
  }
  if (inString == "izquierda") {
    izquierda();
    delay(defaultDelay);
    inString = "";
  }
  if (inString == "derecha") {
    derecha();
    delay(defaultDelay);
    inString = "";
  }
  if (inString == "tracAct") {
    traccion4R = true;
    inString = "";
  }
  if (inString == "tracDes") {
    traccion4R = false;
    inString = "";
  }
  if (inString == "parar") {
    parar();
    inString = "";
  }
  if (inString == "180") {
    giro_180_grados();
    inString = "";
  }
  if (inString == "autodestruccion") {
    destroySelf();
    inString = "";
  }
  if (inString == "pitido") {
    claxon();
    inString = "";
  }
}
// Funciones de movimiento
// -------------------------------------------------------------
// Girar 90º representa el paro del servo
// Girar 180º representa el 100% sentido horario
// Girar 0º representa el 100% sentido antihorario
// Hay que combinar los giros de las ruedas en sentido contrario
// ya que los servos están montados al revés en el lado contrario
//
void avanzar () {
  //Serial.println("Avanzar!");
  servoR.write(180);    
  servoL.write(0);
  servoRT.write(180);
  servoLT.write(0);
}

void retroceder() {
  servoR.write(0);
  servoL.write(180);
  servoRT.write(0);
  servoLT.write(180);
}
void parar() {
  servoR.write(90);
  servoL.write(90);
  servoRT.write(90);
  servoLT.write(90);
}
void izquierda() {
  servoR.write(180);
  servoL.write(180);
  if (traccion4R) {
    servoRT.write(180);
    servoLT.write(180);
  }
}
void derecha() {
  servoR.write(0);
  servoL.write(0);
  if (traccion4R) {
    servoRT.write(0);
    servoLT.write(0);
  }
}

void giro_180_grados() {
  servoR.write(180);
  servoL.write(180);
  delay(giro_180);
  parar();
}

void claxon() {
  //generar tono de 440Hz durante 1000 ms
  tone(pinBuz, 440);
  delay(1000);

  //detener tono durante 500ms  
  noTone(pinBuz);
  delay(500);
}



void destroySelf() {

  int nextTime, intervale = 10000;

  nextTime = millis() + intervale;

  do {
    if ( (nextTime - millis() )> 4000) {
      izquierda();
      delay(giro_30/2);
      parar();
      tone(pinBuz, 329, 500);
      delay(1000);
      derecha();
      delay(giro_30/2);
    } else {
      avanzar();
      delay(giro_30/2);
      parar();
      tone(pinBuz, 329, 200);
      retroceder();
      delay(giro_30/2);
      parar();
      delay(400);      
    }
  } while (millis() < nextTime);
}



void loop() {
  if (Serial.available() > 0) {
    readFromAndroid();
  }
  writeData();
}
