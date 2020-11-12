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

int write_atuador();

// {C,B,A,E1,E2,E3,E4,PINOUT}
const BYTE atuadores[33][8]={
   {0,0,0 ,1,1,1,1,PIN_C3},
   
   {0,0,0 ,0,1,1,1,PIN_B7},
   {0,0,1 ,0,1,1,1,PIN_B7},
   {0,1,0 ,0,1,1,1,PIN_B7},
   {0,1,1 ,0,1,1,1,PIN_B7},
   {1,0,0 ,0,1,1,1,PIN_B7},
   {1,0,1 ,0,1,1,1,PIN_B7},
   {1,1,0 ,0,1,1,1,PIN_B7},
   {1,1,1 ,0,1,1,1,PIN_B7},
   
   {0,0,0 ,1,0,1,1,PIN_C0},
   {0,0,1 ,1,0,1,1,PIN_C0},
   {0,1,0 ,1,0,1,1,PIN_C0},
   {0,1,1 ,1,0,1,1,PIN_C0},
   {1,0,0 ,1,0,1,1,PIN_C0},
   {1,0,1 ,1,0,1,1,PIN_C0},
   {1,1,0 ,1,0,1,1,PIN_C0},
   {1,1,1 ,1,0,1,1,PIN_C0},
   
   {0,0,0 ,1,1,0,1,PIN_C1},
   {0,0,1 ,1,1,0,1,PIN_C1},
   {0,1,0 ,1,1,0,1,PIN_C1},
   {0,1,1 ,1,1,0,1,PIN_C1},
   {1,0,0 ,1,1,0,1,PIN_C1},
   {1,0,1 ,1,1,0,1,PIN_C1},
   {1,1,0 ,1,1,0,1,PIN_C1},
   {1,1,1 ,1,1,0,1,PIN_C1},
  
   {0,0,0 ,1,1,1,0,PIN_C2},
   {0,0,1 ,1,1,1,0,PIN_C2},
   {0,1,0 ,1,1,1,0,PIN_C2},
   {0,1,1 ,1,1,1,0,PIN_C2},
   {1,0,0 ,1,1,1,0,PIN_C2},
   {1,0,1 ,1,1,1,0,PIN_C2},
   {1,1,0 ,1,1,1,0,PIN_C2},
   {1,1,1 ,1,1,1,0,PIN_C2}
};
BYTE incoming, state; 
BYTE response, atuador,estado;
#INT_SSP
void ssp_interupt ()
{
   state = i2c_isr_state();
   
   if(state == 1){
      incoming = i2c_read();
      atuador = incoming;
   }else if(state == 2){
      incoming = i2c_read();
      estado = incoming;
      write_atuador();
   }else if(state >= 0x80){
      response=atuador;
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
    output_high(PIN_B5);
   while (1)
      ;
   return 0;
 }   

void write_atuador(){
   output_bit(PINO_C, atuadores[atuador][0]);
   output_bit(PINO_B, atuadores[atuador][1]);
   output_bit(PINO_A, atuadores[atuador][2]);
   output_bit(PIN_A3, atuadores[atuador][3]);
   output_bit(PIN_A4, atuadores[atuador][4]);
   output_bit(PIN_A5, atuadores[atuador][5]);
   output_bit(PIN_B5, atuadores[atuador][6]);
   output_bit(atuadores[atuador][7],estado );
   return ;
}