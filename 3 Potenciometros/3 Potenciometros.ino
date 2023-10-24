#include <MD_MAX72xx.h>
#include <math.h>

const uint16_t samples = 64; // Muestras para la precisión.

double vReal[samples]; //Guardar valores de la fórmula
double vImag[samples];

double t; // Tiempo
double A; // Amplitud
double f = 1; // Frecuencia

#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES  4

// Definir pines puerto SPI
#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS


// Interfaz SPI Hardware
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

//Esperamos cierto tiempo en cada actualización de leds.
#define DELAYTIME  100  // En milisegundos

void setup() {
    Serial.begin(115200);
    mx.begin();
    mx.clear();
}

void loop() {

    for (int i = 0; i < samples; i++) {
        A = analogRead(A0)/5; // Leer entrada A0 y guardar para la amplitud
        f = (double)analogRead(A2)/500; // Leer entrada A1 y guardar para la frecuencia

        if (f > 1) // Regula la frecuencia para que no sobrepase 1 (evitar que los leds demasiado aleatorio)
        {
          f = 1; 
        }

        vReal[i] = (A * sin(2 * PI * f * t)); // Formula para generar una onda senoidal

        t += (double)analogRead(A1)/10000; // Incrementa el tiempo según la entrada de A1;

        vImag[i] = 0; // Ignorar

        Serial.print(" ");
        Serial.println(t); // Imprimir tiempo
    }


    //------- ENCENDER LEDS ---------
    LedColumn(1, 24);
    LedColumn(9, 16);
    LedColumn(17, 8);
    LedColumn(25, 0);

    delay(75);  // Añade un retardo para controlar la velocidad de la animación
}

// *************** FUNCIÓN **********************

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


