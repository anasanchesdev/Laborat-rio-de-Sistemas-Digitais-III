/* Prog05a:
Fa�a a leitura de uma tecla e envie o c�digo correspondente para o MLCD.
Ao final da linha1 reposicionar a entrada para o in�cio da linha seguinte. 
E ao final da linha2, reposicionar a entrada para o in�cio da primeira linha. 
N�o � necess�rio apagar o conte�do anterior das linhas ap�s o preenchimento da linha2.
*/

 
#include "io430.h"
#include "Lib_MLCD_v3.h"
#include "03_kb_lib_joel_ddc.h"

void main( void )
{

  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // Configura��o de I/O, display, teclado
  ConfigIO_MLCD();
  Init_MLCD();
  tec_config();
  
  // Vari�veis do programa
  char tecla;
  int linha = 1; 
  int coluna = 1;

  // Loop infinito
  while(1)
  {
   tecla = espera_tecla();  // tecla atual
   
   // Ao chegar na �ltima coluna da �ltima linha...
   if ((coluna > 16) && (linha == 2)){
      linha = 1; 
      coluna = 1;
      MLCD_LinhaColuna(linha, coluna);
   }
   // Ao chegar na �ltima coluna da primeira linha...
   else if ((coluna > 16) && (linha == 1)){
      coluna = 1;
      linha++;
      MLCD_LinhaColuna(linha, coluna);
   }
   
   // Mostra a tecla pressionada e pula para a pr�xima posi��o do display
   if (tecla){
      MLCD_LinhaColuna(linha, coluna);
      EnviaDado(tecla);
      coluna++;
   }
  }
}
