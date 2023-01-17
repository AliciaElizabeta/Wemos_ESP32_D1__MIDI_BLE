# Wemos_ESP32_D1__MIDI_BLE
Repository for my end of degree project

## MPU 6050
Library from https://github.com/jrowberg/i2cdevlib  
Info from https://naylampmechatronics.com/blog/45_tutorial-mpu6050-acelerometro-y-giroscopio.html

Revisando la sintaxis del switch-case encontré el metodo map, que resulto en un elemto clave para la conversion del rango [90, -90] del sensor a un rango de 12 notas en notación MIDI
[60 - 71]  
https://docs.arduino.cc/built-in-examples/control-structures/SwitchCase  

Tabla de conversion de notas a partir de frecuencias y valores MIDI  
https://www.inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies  

## MIDI BLE
Library used from https://github.com/lathoub/Arduino-BLE-MIDI 
