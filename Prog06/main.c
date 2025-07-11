/* Prog06:
- Elabore um programa que faça piscar um led conectado ao pino P2.5. 

- Esta funcionalidade será controlada pelo teclado. 

- No início do processamento, o led deve estar apagado. E o MLCD deve exibir a mensagem 
“Led apagado” na primeira linha.

- O pressionar das teclas de 1 a 9 faz o led piscar na frequência correspondente em Hz.

- A mensagem “Piscando x Hz” deve aparecer na primeira linha do MLCD. Onde “x” corresponde
à tecla pressionada.

- O pressionar da tecla “Anula” faz o led parar de piscar e a mensagem “Led apagado” 
deve aparecer na primeira linha do MLCD.

- Se qualquer outra tecla for pressionada, nada deve ser feito.

Calculos:

Tecla    Freq    T     Delay 
(ASCII)  (Hz)   (ms)    (ms)
 0x31      1   1000,0   500,0
 0x32      2    500,0   250,0
 0x33      3    333,3   166,7
 0x34      4    250,0   125,0
 0x35      5    200,0   100,0
 0x36      6    166,7    83,3
 0x37      7    142,9    71,4
 0x38      8    125,0    62,5
 0x39      9    111,1    55,6
 
 Duas sugestões para implementação:
 1. Um vetor com a lista de delays
    ou
 2. Faz o cálculo: delay = 500/Freq;
 
 Ambas se baseiam em um delay mínimo de 1ms a ser repetido de acordo com a necessidade.
 
*/


#include "io430.h"
#include "Lib_MLCD_v3.h"
#include "kb_lib_joel_cc_ddc.h"


void militempo(float tempo){
  while (tempo){
    __delay_cycles(Fclk*1e-3);
    tempo--;
  }
}


void main( void )
{

  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // Configuração de I/O, display, teclado
  P2SEL &= ~BIT5; // Define P2.5 como I/O
  P2DIR |= BIT5;  // Define P2.5 como saída
  ConfigIO_MLCD();
  Init_MLCD();
  tec_config();
 
  // Variáveis do programa
  char tecla;
  char tecla_anterior;
  char freq;
  float delay = 0;
  
  EnviaString("Led apagado");
  
  while(1){
    tecla = tec_rd();
    
    // Identificar se a tecla está entre 1 e 9
    if ((tecla >= '1')&&(tecla <= '9')){
      
      // Atualiza o dislay somente uma vez
      if (~((tecla_anterior >= '1')&&(tecla_anterior <= '9'))){
        MLCD_LinhaColuna(1, 1);
        EnviaString("Piscando x Hz");
      }
      
      MLCD_LinhaColuna(1, 10);
      EnviaDado(tecla);
      freq = tecla - 0x30;
      delay = 500 / freq;
      
    }
    
    // Se a tecla for anula... 
    else if (tecla == 'A'){
      MLCD_LinhaColuna(1, 1);
      EnviaString("Led apagado  ");
      delay = 0;
    }
    
    // Pisca somente se houver delay
    if (delay){
      P2OUT |= BIT5;
      militempo(delay);
      P2OUT &= ~BIT5;
      militempo(delay);

    }
    else {
      P2OUT &= ~BIT5;
    }
    tecla_anterior = tecla;
  }
}


  /*
  
}  
  
  
  EnviaString("Led apagado");
  while(1){
      tecla = tec_rd();
 
      if ((tecla)&&(tecla != tecla_anterior)){
         tecla_anterior = tecla;
      }
      MLCD_LinhaColuna(1, 1);
      
      switch(tecla_anterior){
        case 'A':
          MLCD_LinhaColuna(1, 1);
          EnviaString("Led apagado  ");
          break;
        case '1':
          MLCD_LinhaColuna(1, 1);
          EnviaString("Piscando 1 Hz");
          P2OUT |= BIT5;
          __delay_cycles(Fclk);
          P2OUT &= ~BIT5;
          __delay_cycles(Fclk);
          break;
        case '2':
          MLCD_LinhaColuna(1, 1);
          EnviaString("Piscando 2 Hz");
          P2OUT |= BIT5;
          __delay_cycles(0.5*Fclk);
          P2OUT &= ~BIT5;
          __delay_cycles(0.5*Fclk);
          break;
        case '3':
          MLCD_LinhaColuna(1, 1);
          EnviaString("Piscando 3 Hz");
          P2OUT |= BIT5;
          __delay_cycles(0.333*Fclk);
          P2OUT &= ~BIT5;
          __delay_cycles(0.333*Fclk);
          break;
        case '4':
          MLCD_LinhaColuna(1, 1);
          EnviaString("Piscando 4 Hz");
          P2OUT |= BIT5;
          __delay_cycles(0.25*Fclk);
          P2OUT &= ~BIT5;
          __delay_cycles(0.25*Fclk);
          break;
        case '5':
          MLCD_LinhaColuna(1, 1);
          EnviaString("Piscando 5 Hz");
          P2OUT |= BIT5;
          __delay_cycles(0.2*Fclk);
          P2OUT &= ~BIT5;
          __delay_cycles(0.2*Fclk);
          break;
        case '6':
          MLCD_LinhaColuna(1, 1);
          EnviaString("Piscando 6 Hz");
          P2OUT |= BIT5;
          __delay_cycles(0.166*Fclk);
          P2OUT &= ~BIT5;
          __delay_cycles(0.166*Fclk);
          break;
        case '7':
          MLCD_LinhaColuna(1, 1);
          EnviaString("Piscando 7 Hz");
          P2OUT |= BIT5;
          __delay_cycles(0.142*Fclk);
          P2OUT &= ~BIT5;
          __delay_cycles(0.142*Fclk);
          break;
        case '8':
          MLCD_LinhaColuna(1, 1);
          EnviaString("Piscando 8 Hz");
          P2OUT |= BIT5;
          __delay_cycles(0.125*Fclk);
          P2OUT &= ~BIT5;
          __delay_cycles(0.125*Fclk);
          break;
        case '9':
          MLCD_LinhaColuna(1, 1);
          EnviaString("Piscando 9 Hz");
          P2OUT |= BIT5;
          __delay_cycles(0.11*Fclk);
          P2OUT &= ~BIT5;
          __delay_cycles(0.11*Fclk);
          break;
        default:
          break;
      }
      
    }
  
  }

void PiscaLed(){
  
}
*/
