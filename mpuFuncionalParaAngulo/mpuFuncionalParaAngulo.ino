// Librerias I2C para controlar el mpu6050
// la libreria MPU6050.h necesita I2Cdev.h, I2Cdev.h necesita Wire.h
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 sensor;

// Valores RAW (sin procesar) del acelerometro  en los ejes x,y,z
short ax, ay, az;
int minNote = 60; // In MIDI 60 is equivalent to C or Do
void setup() {
  Serial.begin(57600);    //Iniciando puerto serial
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor

  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
}

void loop() {
  // Leer las aceleraciones 
  sensor.getAcceleration(&ax, &ay, &az);
  //Calcular los angulos de inclinacion:
  float accel_ang_x=atan(ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);
  float accel_ang_y=atan(ay/sqrt(pow(ax,2) + pow(az,2)))*(180.0/3.14);

  int valueNote = map(accel_ang_x, 80, -80, 0, 11);
  int note = minNote + valueNote;

  //Este bloque es solo para visualizar el correcto funcionamiento
  // basta con enviar el midi con note 
  switch (note) {
    case 60:
      Serial.println("DO");
      break;
    case 61:
      Serial.println("Do#");
      break;
    case 62:
      Serial.println("RE");
      break;
    case 63:
      Serial.println("Re#");
      break;
    case 64:
      Serial.println("MI");
      break;
    case 65:
      Serial.println("FA");
      break;
    case 66:  
      Serial.println("Fa#");
      break;
    case 67:  
      Serial.println("SOL");
      break;
    case 68:
      Serial.println("Sol#");
      break;
    case 69:
      Serial.println("LA");
      break;
    case 70:  
      Serial.println("La#");
      break;
    case 71:  
      Serial.println("SI");
      break;
  }

  //Mostrar los angulos separadas por un [tab]
  Serial.print("   Inclinacion en X: ");
  Serial.print(accel_ang_x); 
  Serial.print("   Inclinacion en Y:");
  Serial.println(accel_ang_y);
  delay(100);
}