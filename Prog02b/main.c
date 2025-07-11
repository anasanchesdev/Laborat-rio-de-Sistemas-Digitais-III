/*
Cabeçalho
Prog2a
Elabore um programa que escreva 3 caracteres no MLCD

Prog2b
Elabore um programa que escreva o seu nome na primeira linha do MLCD
e ano, turma e subturma na segunda linha de forma centralizada.

Prog02c
Em seguida apague o seu último nome na primeira linha

Em tempo: Vamos apagar o cursor do MLCD. 
Altere a linha de comando correspondente na função 
Init_MLCD
*/

#include "io430.h"
#include "Lib_MLCD_v3.h"

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  char nome[] = "Ana Clara";
  char turma[] = "ELT 3C T1";
  
  ConfigIO_MLCD();
  Init_MLCD();
  
  // Prog02b
  MLCD_LinhaColuna(1, 4);
  EnviaString(nome);
  MLCD_LinhaColuna(2, 4);
  EnviaString(turma);
  MLCD_LinhaColuna(1, 8);
  
  // Prog02c
  EnviaString("     ");
}
