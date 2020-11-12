#include <16F677.h>

#device ADC=10
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

#use standard_io (B)


#define PINO_A PIN_A0
#define PINO_B PIN_A1
#define PINO_C PIN_A2
#define PINO_EN1 PIN_A3
#define PINO_EN2 PIN_A4
#define PINO_EN3 PIN_A5
#define PINO_EN4 PIN_B5

int read_sensor();

// {C,B,A,E1,E2,E3,E4,PINOUT}
const BYTE sensores[34][8]={
   {0,0,0 ,1,1,1,1,PIN_C6},
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
   {1,1,1 ,1,1,1,0,PIN_C2},
   {0,0,0 ,1,1,1,1,PIN_C3} 
};
BYTE incoming, state; // I2C vars
BYTE response,sensor;
#INT_SSP
void ssp_interupt ()
{
   state = i2c_isr_state();
   
   if(state == 1){
      incoming = i2c_read();
      sensor = incoming;
   }else if(state >= 0x80){
      response=read_sensor();  
      delay_ms(10);
      i2c_write(response);
   }else     
      i2c_read();
}


int main (void)
 { 
   set_tris_a(0);
   set_tris_b(0b11011111);
   set_tris_c(0xFF);

   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   setup_vref(FALSE);
   setup_oscillator(OSC_8MHZ);
   setup_comparator(NC_NC_NC_NC);
   setup_adc(ADC_CLOCK_DIV_8);
   setup_adc_ports(sAN8,VSS_VDD);

    
   set_adc_channel(8); 
   delay_us(10);
   
   output_high(PIN_A3);
   output_high(PIN_A4);
   output_high(PIN_A5);
   output_high(PIN_B5);
   while (1)
      ;
   return 0;
 }   

int read_sensor(){
   int leitura;
   long leituraAdc;
   if(sensor==0){
      leituraAdc = read_adc();
      if(leituraAdc < 125)
	 leitura = 1;
      else if(leituraAdc >= 125 && leituraAdc < 300)
	 leitura = 2;
      else if(leituraAdc >= 300 && leituraAdc < 500)
	 leitura = 3;
      else if(leituraAdc >= 500 && leituraAdc < 700)
	 leitura = 4;
      else if(leituraAdc >= 700)
	 leitura = 5;    
      } else
      {
	 output_bit(PINO_C, sensores[sensor][0]);
	 output_bit(PINO_B, sensores[sensor][1]);
	 output_bit(PINO_A, sensores[sensor][2]);
	 output_bit(PIN_A3, sensores[sensor][3]);
	 output_bit(PIN_A4, sensores[sensor][4]);
	 output_bit(PIN_A5, sensores[sensor][5]);
	 output_bit(PIN_B5, sensores[sensor][6]);
	 delay_ms(10);
	 leitura = input(sensores[sensor][7]);
	 delay_ms(10); 
      }
   return leitura;
}