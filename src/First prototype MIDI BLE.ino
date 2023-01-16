#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32_NimBLE.h>
BLEMIDI_CREATE_INSTANCE("CustomName", MIDI)

const int btnPin = 0;

int t = 0;
int tl = 50;  
int d = 1000;  
int btnState = 0;    

void setup()
{
  Serial.begin(115200);  
  pinMode(btnPin, INPUT);
  BLEMIDI.setHandleConnected(OnConnected);
  BLEMIDI.setHandleDisconnected(OnDisconnected);
  
  MIDI.begin();
}

void loop()
{
  btnState = digitalRead(btnPin);

  if (btnState == HIGH) {t=0;} 
  else {
    if(t == tl){
      MIDI.sendNoteOn(36, 100, 10);
      delay(d);
    }else{t++;}
  }

}

void OnConnected() {
  Serial.print("Connected");
}

void OnDisconnected() {
  Serial.print("Disconnected");
}