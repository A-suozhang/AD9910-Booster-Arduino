# include "AD9910.h"
# include <SPI.h>

# define cs 5
# define rst 16
# define update 17
# define sdio 23
# define sclk 18

# define CLOCKSPEED 1000000

AD9910 DDS(cs, rst,update,sdio,sclk);
double freq = 1000000.0;
double amp = 100.0;

void setup()
{
    pinMode(25,OUTPUT);
    // Serial
    Serial.begin(115200);
    // SPI
    SPI.begin();
    // DDS
    DDS.begin();
}

void loop(){
    // LIt The Light To Denote Code IN Effect
    digitalWrite(25, HIGH);

    Serial.println("Working..");

    /* 
    // ----------------- Frequency Sweep ------------------------
    for (int i = 0;i<40;i++){
        DDS.set_freq(freq*i,0);
        Serial.print("Setting Frequency: ");
        Serial.println(1000000*(i+1));
        delay(50);
    }
    for (int j=40;j>0;j--){
        DDS.set_freq(freq*j,0);
        Serial.print("Setting Frequency: ");
        Serial.println(1000000*(j+1));
        delay(50);
    }
    */
   
   // ------------------- Amp Sweep --------------------------
   for (int i = 0;i<10;i++){
       DDS.set_Amp(amp*i);
       delay(50);
   }
   for (int j = 10; j>0;j--){
       DDS.set_Amp(amp*j);
       delay(50);
   }
   

//  DDS.set_freq(freq);
//  DDS.set_Amp(amp);



}