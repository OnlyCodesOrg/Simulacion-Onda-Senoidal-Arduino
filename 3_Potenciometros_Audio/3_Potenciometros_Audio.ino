#include <MD_MAX72xx.h>
#include "arduinoFFT.h"
#include <math.h>

const uint16_t samples = 64; //Muestras 
uint16_t k = 0;
double vReal[samples];
double vImag[samples];
double t = 0; //Variable de tiempo 
double ganancia;
double A; //= 100.0; // Amplitud
double f = 1;  // Frecuencia en Hz

#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES  4

// Definir pines puerto SPI
#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS

arduinoFFT FFT = arduinoFFT();

// SPI hardware interface
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Esperar cierto tiempo cada vez que se despligue en los leds
#define DELAYTIME  100  // En milisegundos

void setup() {

    Serial.begin(115200);
    mx.begin();
    mx.clear();

}

void loop() {

    for (int i = 0; i < samples; i++) 
    {
        A = analogRead(A0)/5; //Obtener amplitud
        f = (double)analogRead(A2)/500; //Obtener Frecuencia
        ganancia = (double)analogRead(A4) / 2; //Obtener ganancia del audio 

        if (f > 1) //Mantener la frecuencia debajo de 1
        {
          f = 1;
        }

        if (ganancia < 15) //Mantener ganancia menor a 15 para que cuando no haya audio, se 
        {
          vReal[i] = 0;
        }
        else
        {
          vReal[i] = (A * sin(2 * PI * f * t)) * ganancia; // Formula para generar una onda senoidal
        }

        t += (double)analogRead(A1)/10000; // Incrementa el tiempo en función de la frecuencia
        
        vImag[i] = 0;
        Serial.print(" ");
        Serial.println(ganancia); 
    }


//------- ENCENDER LEDS -------------
    LedColumn(1, 24);
    LedColumn(9, 16);
    LedColumn(17, 8);
    LedColumn(25, 0);

    delay(75);  // Añade un retardo para controlar la velocidad de la animación
}

// *************** función **********************

void LedColumn(int Fi, int Ci){
  
    for (int i=Fi; i < Fi+8 ; i ++){

     if (vReal[i] > 89){
     mx.setColumn(Ci, B00000011);
     } else {
        if (vReal[i] > 75){
           mx.setColumn(Ci, B00000110);
        } else {
          if (vReal[i] > 60) {
            mx.setColumn(Ci, B00001100);
          } else {
            if (vReal[i] > 50) {
              mx.setColumn(Ci, B00011000);
            } else {
                if (vReal[i] > -15) {
                  mx.setColumn(Ci, B00011000);
                } else {
                     if (vReal[i] > -50){
                        mx.setColumn(Ci, B01100000);
                     } else {
                       if (vReal[i] > -75) {
                          mx.setColumn(Ci, B01100000);
                       } else {
                          if (vReal[i] > -100) {
                            mx.setColumn(Ci, B11000000);
                          } else {
                              mx.setColumn(Ci, B110000);
                          }
                       }
                     }
                }
            }
            
          }
        }
     }
     Ci++;
    }
}  


