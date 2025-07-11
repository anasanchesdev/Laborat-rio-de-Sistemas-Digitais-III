/* Cabeçalho:
Nome do programa: Prog2a
Descrição do programa: Envie dois caracteres para o MLCD.
*/

#include "io430.h"
#include "Lib_MLCD_v3.h"

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  char nome[] = "Oii gente! <3";
  
  ConfigIO_MLCD();
  Init_MLCD();
  EnviaString(nome);
}
