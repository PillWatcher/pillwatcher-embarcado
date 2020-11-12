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


#define IN1 PIN_A0
#define IN2 PIN_A1
#define MS1 PIN_C2
#define MS2 PIN_C3
#define MS3 PIN_C4
#define EN1 PIN_A2
#define EN2 PIN_A3
#define EN3 PIN_A4
#define EN4 PIN_A5
#define EN5 PIN_B5
#define DIR PIN_C0
#define STEP PIN_C1
#define RST PIN_B7

int write_atuador();

// {E1,E2,E3,E4,E5}
const BYTE atuadores[6][5]={
{1,1,1,1,1},
{0,1,1,1,1},
{1,0,1,1,1},
{1,1,0,1,1},
{1,1,1,0,1},
{1,1,1,1,0},
};
BYTE incoming, state; 
BYTE response, atuador,direcao,passos,periodo;
#INT_SSP
void ssp_interupt ()
{
   state = i2c_isr_state();
   
   if(state == 1){
      incoming = i2c_read();
      atuador = incoming;
   }else if(state == 2){
      incoming = i2c_read();
      direcao = incoming; 
   }else if(state == 3){
      incoming = i2c_read();
      passos = incoming;  //passos do motor ou aticação motor dc
   }else if(state == 4){
      incoming = i2c_read();
      periodo = incoming; 
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
   output_bit(EN1, atuadores[atuador][0]);
   output_bit(EN2, atuadores[atuador][1]);
   output_bit(EN3, atuadores[atuador][2]);
   output_bit(EN4, atuadores[atuador][3]);
   output_bit(EN5, atuadores[atuador][4]);
   if(atuador > 0){
      for(int i=1;i<=passos;i++){
	 output_high(STEP);
	 delay_ms(periodo);
	 output_low(STEP);
	 delay_ms(periodo);
	 }
   }else if(direcao == 1){
      output_bit(IN1,passos);
      output_bit(IN2,FALSE);
   }else{
      output_bit(IN1,FALSE);
      output_bit(IN2,passos);
      }
   return ;
}