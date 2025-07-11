/*
Prog01:
Motores que s�o alimentados por um sistema trif�sico devem funcionar se as
3 fases estiverem fornecendo energia. A falta de uma fase compromete severamente
o funcionamento do motor.

Elabore um sistema de monitoramento das 3 fases para que o funcionamento do 
motor s� seja habilitado se houver a alimenta��o das 3 fases.

Considere que as seguintes entradas j� tiveram os seus sinais condicionados
e que o n�vel 1 indica a presen�a da alimenta��o da fase correspondente.
P1.0 = Fase A
P1.1 = Fase B
P1.2 = Fase C

Utilize a sa�da P1.7 como a sa�da que vai fornecer o n�vel 1 para habilitar
o funcionamento do motor.

Prog03: Rel� de falta de fase com interface alfanum�rica
Elaborar uma nova vers�o do programa Prog01 que, agora, contar� com a exibi��o,
no MLCD do status de cada fase, assim como da sa�da para a habilita��o do motor.


Utilize a seguinte formata��o no MLCD:

	Colunas
         1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
Linha 1: F  a  s  e  s  :  A  B  C    M  o  t  o  r	
Linha 2:                   1  1  0    0		

23/05/25: Otimiza��o para impedir atualiza��es constantes quando n�o h� mudan�a
de estado nas portas.
*/

#include "io430.h"
#include "Lib_MLCD_v3.h"

void main( void )
{
  // Configura��es de perif�ricos
  P1SEL &= BIT0;                    // Configura P1 como entrada/sa�da
  P1DIR &= ~(BIT0 + BIT1 + BIT2);   // Configura P1.0, P1.1 e P1.2 como entradas
  P1DIR |= BIT7;                    // Configura P1.7 como sa�da

  
  // Configura��o da interface
  char interface[] = "Fases:ABC Motor";
  
  
  // Vari�veis de controle do estado das fases (otimiza��o)
  char estadoA = 0xFF;
  char estadoB = 0xFF;
  char estadoC = 0xFF;
  char estadoM = 0xFF;
  char fase;
  char motor;

  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // Configura��o de I/O e display
  ConfigIO_MLCD();
  Init_MLCD();
  EnviaString(interface);
  
  // Loop infinito
  while(1)
  {     
      // Se todas as fases estiverem funcionando...
      if ((P1IN&BIT0) && (P1IN&BIT1) && (P1IN&BIT2))
      {
          P1OUT |= BIT7; // Define 1 (ligado) para a sa�da de P1.7 
          
          // Exibe valor na coluna do motor
          motor = P1OUT&BIT7;
          if (estadoM != motor){
            MLCD_LinhaColuna(2, 11);
            EnviaDado('1');
            estadoM = motor;
          }
      }  
     
      // Se pelo menos uma fase n�o estiver funcionando em n�vel 1...
      else
      {
          P1OUT &= ~BIT7; // Define 0 (desligado) para a sa�da de P1.7
          
          // Exibe valor na coluna do motor
          motor = P1OUT&BIT7;
          if (estadoM != motor){
            MLCD_LinhaColuna(2, 11);
            EnviaDado('0');
            estadoM = motor;
          }
      }
      
      // Configura��o do display do estado das fases
      // Ocorrer� altera��o somente se o estado for diferente do anterior
      
      // Verifica a fase A (CH0)
      fase = (P1IN&BIT0);          // Valor atual da fase A
      if (estadoA != fase){      // Se o valor atual for dif do anterior...
        if (fase){
          MLCD_LinhaColuna(2, 7);
          EnviaDado('1');
          }
        else {
          MLCD_LinhaColuna(2, 7);
          EnviaDado('0');
        }
        estadoA = fase;    // Atualiza o estado anterior para o atual
      }
      
      // Verifica a fase B (CH1)
      fase = (P1IN&BIT1);
      if (estadoB != fase){
        if (fase){
          MLCD_LinhaColuna(2, 8);
          EnviaDado('1');
        }
        else{
          MLCD_LinhaColuna(2, 8);
          EnviaDado('0');
        }
        estadoB = fase;
      }
      
      // Verifica a fase C (CH2)
      fase = (P1IN&BIT2);
      if (estadoC != fase){
        if (fase){
          MLCD_LinhaColuna(2, 9);
          EnviaDado('1');
        }
        else{
          MLCD_LinhaColuna(2, 9);
          EnviaDado('0');
        }
        estadoC = fase;
      }  
 }
}
