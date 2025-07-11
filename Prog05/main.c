/* Prog05a:
Faça a leitura de uma tecla e envie o código correspondente para o MLCD.
Ao final da linha1 reposicionar a entrada para o início da linha seguinte. 
E ao final da linha2, reposicionar a entrada para o início da primeira linha. 
Não é necessário apagar o conteúdo anterior das linhas após o preenchimento da linha2.
*/

 
#include "io430.h"
#include "Lib_MLCD_v3.h"
#include "03_kb_lib_joel_ddc.h"

void main( void )
{

  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // Configuração de I/O, display, teclado
  ConfigIO_MLCD();
  Init_MLCD();
  tec_config();
  
  // Variáveis do programa
  char tecla;
  int linha = 1; 
  int coluna = 1;

  // Loop infinito
  while(1)
  {
   tecla = espera_tecla();  // tecla atual
   
   // Ao chegar na última coluna da última linha...
   if ((coluna > 16) && (linha == 2)){
      linha = 1; 
      coluna = 1;
      MLCD_LinhaColuna(linha, coluna);
   }
   // Ao chegar na última coluna da primeira linha...
   else if ((coluna > 16) && (linha == 1)){
      coluna = 1;
      linha++;
      MLCD_LinhaColuna(linha, coluna);
   }
   
   // Mostra a tecla pressionada e pula para a próxima posição do display
   if (tecla){
      MLCD_LinhaColuna(linha, coluna);
      EnviaDado(tecla);
      coluna++;
   }
  }
}
