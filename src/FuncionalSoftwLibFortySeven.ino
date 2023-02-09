//FortySevenEffects y Library de lathoub
#include <BLEMIDI_Transport.h> //Libreria para el transporte bluethoot de contenido midi
#include <hardware/BLEMIDI_ESP32_NimBLE.h> // Libreria para ajustar la conexion BT a la placa

#include "MPU6050.h" //Libreria que facilita la lectura del acelerometro MPU6050
#include "Wire.h" //Libreria que facilita la comunicacion I2C

// Create and bind the MIDI interface to the default hardware Serial port
BLEMIDI_CREATE_DEFAULT_INSTANCE();

//Sensor MPU6050 - Giroscopio y Acelerometro
MPU6050 sensor;
short ax, ay, az; // Valores RAW del acelerometro  en los ejes x y z

//Joystick
const int SW_pin = 26; // IO17
const int X_pin = 36;  // IO36

//Base musical
int d = 170; //Velocidad de nota o volumen
int octava = 4; //Octavas que tienen el minimo a -1 y el maximo a 9
int note = 60;
int last_note = 60;


void setup()
{
    Serial.begin(115200);     //Inicialización de salida de serial a 115200 baudios
    Wire.begin();             //Iniciando I2C mediante la libreria Wire  

  //Arranque del MIDI
    MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages


  // //Sensores
    sensor.initialize();      //Iniciando el sensor MPU6050
    if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
    else Serial.println("Error al iniciar el sensor");

  // //Joystick
    pinMode(SW_pin, INPUT_PULLUP);

}


void loop()
{

  octava =  updateOctave(octava);
  int minNote = 12 + (octava * 12); // In MIDI 60 is equivalent to C4 or Do

  
  // //Si se pulsa el botón se envia una nota y se espera una cantidad de tiempo
  if (digitalRead(SW_pin) == 0) {
    note = minNote + getNoteFromMPU6050(); //Obtiene la nota a enviar con base en Do cuarta a Si en cuarta
    MIDI.sendNoteOn(note, d, 10); //nota velocidad canal
    last_note = note;
    while(digitalRead(SW_pin) == 0 ){}
  }
  MIDI.sendNoteOff(last_note, 0, 10);

  Serial.print("Switch:  ");
  Serial.print(digitalRead(SW_pin));

  Serial.print(" ");
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin));
  
  Serial.print(" ");
  Serial.print("Nota: ");
  Serial.print(minNote);
  Serial.print("\n");
  delay(100);
}

int updateOctave(int octava){
  if((analogRead(X_pin) > 4000) && (octava < 9)){octava = octava + 1;}
  if((analogRead(X_pin) < 100) && (octava > -1)){octava = octava - 1;}
  return octava;
}

int getNoteFromMPU6050() {
  sensor.getAcceleration(&ax, &ay, &az); //Lee los valores del MPU6050
  float accel_ang_x=atan(ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);   //Calcula la inclinacion de eje X
  int valueNote = map(accel_ang_x, 80, -80, 0, 11); //Mapea los valores y devuelve entre 0 y 11 dependiendo del valor 
  return valueNote;
}