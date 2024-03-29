#include <BLEMIDI_Transport.h> //Libreria para el transporte bluethoot de contenido midi
#include <hardware/BLEMIDI_ESP32_NimBLE.h> // Libreria para ajustar la conexion BT a la placa
#include "MPU6050.h" //Libreria que facilita la lectura del acelerometro MPU6050
#include "Wire.h" //Libreria que facilita la comunicacion I2C

BLEMIDI_CREATE_DEFAULT_INSTANCE();  // Crear y vincular, la interfaz MIDI al Serial port por defecto del hardware

MPU6050 sensor; //Sensor MPU6050 - Giroscopio y Acelerometro
short ax, ay, az; // Valores RAW del acelerometro  en los ejes x y z

//Joystick
const int SW_pin = 26;  // IO26
const int X_pin = 36;   // IO36 Eje de la escala
const int Y_pin = 4;    // IO4 Eje del pitch

//Base musical
int canal = 10;
int d = 170; //Velocidad de nota o volumen
int octava = 4; //Octavas que tienen el minimo a -1 y el maximo a 9
int note = 60;
int last_note = 60;
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

void setup()
{
    Serial.begin(38400);     //Inicialización de salida de serial a 115200 baudios
    Wire.begin();             //Iniciando I2C mediante la libreria Wire  

  //Arranque del MIDI
    MIDI.begin(/*MIDI_CHANNEL_OMNI*/);  // Escucha los mensajes de entrada
    tiempo1 = millis();

  // //Sensores
    sensor.initialize();      //Iniciando el sensor MPU6050
    if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
    else Serial.println("Error al iniciar el sensor");

    sensor.setXAccelOffset(-671);
    sensor.setYAccelOffset(-804);
    sensor.setZAccelOffset(1452);
    sensor.setXGyroOffset (-378);
    sensor.setYGyroOffset (-66);
    sensor.setZGyroOffset (-61);

  // //Joystick
    pinMode(SW_pin, INPUT_PULLUP);
}


void loop()
{
  octava =  updateOctave(octava);
  int minNote = 12 + (octava * 12); // In MIDI 60 is equivalent to C4 or Do

  
  // //Si se pulsa el botón se envia una nota y se espera una cantidad de tiempo
  if (digitalRead(SW_pin) == 0) {
    Serial.print("Switch:  ");
    Serial.print(digitalRead(SW_pin));

    MIDI.sendNoteOff(last_note, 0, canal);
    note = minNote + getNoteFromMPU6050(); //Obtiene la nota a enviar con base en Do cuarta a Si en cuarta
    Serial.print(note + "\n");
    MIDI.sendNoteOn(note, d, canal); //nota velocidad canal
    last_note = note;
    while(digitalRead(SW_pin) == 0 ){
        int pitch = map(analogRead(Y_pin), 0, 4095, -8192, 8191);
        MIDI.sendPitchBend(pitch, canal);
      }
  }

  Serial.print(digitalRead(SW_pin));
  
  Serial.print(" ");
  Serial.print("Nota: ");
  Serial.print(minNote);
  Serial.print("  Octava: ");
  Serial.print(octava);
  Serial.print("\n");
}


int updateOctave(int octava){
  tiempo2 = millis();
  if((analogRead(X_pin) < 2000) && (octava < 9) && (tiempo2>tiempo1+1000)){octava = octava + 1; tiempo1 = millis();}
  if((analogRead(X_pin) > 3000) && (octava > -1) && (tiempo2>tiempo1+1000)){octava = octava - 1; tiempo1 = millis();}
  return octava;
}

int getNoteFromMPU6050() {
  sensor.getAcceleration(&ax, &ay, &az); //Lee los valores del MPU6050
  //float accel_ang_x=atan(ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);   //Calcula la inclinacion de eje X
  int valueNote = map(ay, 18000, -13000, 0, 11); //Mapea los valores y devuelve entre 0 y 11 dependiendo del valor 
  return valueNote;
}