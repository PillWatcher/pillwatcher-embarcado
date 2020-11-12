#include <16F677.h>

//Configuracao FUSES
#FUSES NOWDT
#FUSES NOHS
#FUSES INTRC_IO
#FUSES NOPROTECT
#FUSES NOBROWNOUT
#FUSES NOMCLR
#FUSES NOCPD
#FUSES NOPUT

#use delay(clock=8MHz)

#use i2c(SLAVE, SDA=PIN_B4, SCL=PIN_B6, address=0xA0, FORCE_HW)



#define PINO_A PIN_A0
#define PINO_B PIN_A1
#define PINO_C PIN_A2
#define PINO_EN1 PIN_A3
#define PINO_EN2 PIN_A4
#define PINO_EN3 PIN_A5

int read_sensor(sensor);

// {C,B,A,E1,E2,E3,PINOUT}
const BYTE sensores[30][7]={
   {0,0,0 ,0,1,1,PIN_B5},
   {0,0,1 ,0,1,1,PIN_B5},
   {0,1,0 ,0,1,1,PIN_B5},
   {0,1,1 ,0,1,1,PIN_B5},
   {1,0,0 ,0,1,1,PIN_B5},
   {1,0,1 ,0,1,1,PIN_B5},
   {1,1,0 ,0,1,1,PIN_B5},
   {1,1,1 ,0,1,1,PIN_B5},
   {0,0,0 ,1,0,1,PIN_B7},
   {0,0,1 ,1,0,1,PIN_B7},
   {0,1,0 ,1,0,1,PIN_B7},
   {0,1,1 ,1,0,1,PIN_B7},
   {1,0,0 ,1,0,1,PIN_B7},
   {1,0,1 ,1,0,1,PIN_B7},
   {1,1,0 ,1,0,1,PIN_B7},
   {1,1,1 ,1,0,1,PIN_B7},
   {0,0,0 ,1,1,0,PIN_C0},
   {0,0,1 ,1,1,0,PIN_C0},
   {0,1,0 ,1,1,0,PIN_C0},
   {0,1,1 ,1,1,0,PIN_C0},
   {1,0,0 ,1,1,0,PIN_C0},
   {1,0,1 ,1,1,0,PIN_C0},
   {1,1,0 ,1,1,0,PIN_C0},
   {1,1,1 ,1,1,0,PIN_C0},
   {0,0,0 ,1,1,1,PIN_C1},
   {0,0,0 ,1,1,1,PIN_C2},
   {0,0,0 ,1,1,1,PIN_C3},
   {0,0,0 ,1,1,1,PIN_C4},
   {0,0,0 ,1,1,1,PIN_C5},
   {0,0,0 ,1,1,1,PIN_C6},
};
BYTE incoming, state; // I2C vars
BYTE response, sensor;
#INT_SSP
void ssp_interupt ()
{
   state = i2c_isr_state();
   
   if(state == 1){
      incoming = i2c_read();
      sensor = incoming;
   }else if(state >= 0x80){
      response=read_sensor(sensor);
      i2c_write(response);
   }else     
      i2c_read();
}


int main (void)
 { 
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
    set_tris_a(0);
    setup_vref(FALSE);
    output_high(PIN_A3);
    output_high(PIN_A4);
    output_high(PIN_A5);
   while (1)
      ;
   return 0;
 }   

int read_sensor(sensor){
   int leitura;
   output_bit(PINO_C, sensores[sensor][0]);
   output_bit(PINO_B, sensores[sensor][1]);
   output_bit(PINO_A, sensores[sensor][2]);
   output_bit(PIN_A3, sensores[sensor][3]);
   output_bit(PIN_A4, sensores[sensor][4]);
   output_bit(PIN_A5, sensores[sensor][5]);
   leitura = input(sensores[sensor][6]);
   return leitura;
}