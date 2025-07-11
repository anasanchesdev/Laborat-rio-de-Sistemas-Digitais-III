/*
Prog04: Anunciador visual

 Elabore um programa para o kit MSP430F1611 que implemente a função de anunciador visual
 de paradas do metrô, para atender aos deficientes auditivos.
Durante as viagens do metrô, o condutor anuncia pelos autofalantes o nome da próxima parada.
Mas os deficientes auditivos precisam desta informação de forma visual, em um display 
por exemplo.  
Na primeira linha do display será exibida a frase “Proxima parada:” e na segunda linha, 
o nome da estação.

	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
Linha 1:P r o x i m a   p a  r  a  d  a  :	
Linha 2:E s t   V i l a r i  n  h  o			

O início de uma viagem acontece na primeira estação listada abaixo. O condutor é responsável 
por gerar um pulso de nível alto em P1.0 logo que ocorrer a partida de uma estação, para 
que a mensagem no display exiba o nome da próxima estação. 
Lembre-se que ao chegar a um dos extremos da linha, a composição retorna, passando pelas
estações, agora em sentido inverso. 
A cada novo anúncio de estação, o bit P1.7, que comandará uma lâmpada, 
que deverá acender por 3 segundos, fazendo o reforço com a sinalização visual.

Lista de estações:
"Est VILARINHO   "
"Est FLORAMAR    "
"Est WALDO LOBO  "
"Est 1o. DE MAIO "
"Est SAO GABRIEL "
"Est MINAS SHOPP "
"Est J. CANDIDO  "  
"Est SANTA INES	 "
"Est HORTO	 "
"Est STA TEREZA	 "
"Est STA EFIGENIA"
"Est CENTRAL	 "
"Est LAGOINHA    "
'Est C. PRATES	 "
'Est CALAFATE	 "
'Est GAMELEIRA	 "
'Est VILA OESTE	 "
'Est ELDORADO	 "

Sugestão: criar uma matriz de caracteres
char Estacoes[18][17] = {"Est VILARINHO   ",
                         "Est FLORAMAR    ",
                     assim por diante       };
*/

#include "io430.h"
#include "Lib_MLCD_v3.h"

void main( void )
{
  // Configurações de periféricos
  P1SEL &= BIT0;     // Configura P1 como entrada/saída
  P1DIR &= ~BIT0;    // Configura P1.0 como entrada
  P1DIR |= BIT7;     // Configura P1.7 como saída

  
  // Variáveis do programa
  char estacoes[18][17] = {"VILARINHO   ",
                           "FLORAMAR    ",   
                           "WALDO LOBO  ",
                           "1o. DE MAIO ",
                           "SAO GABRIEL ",
                           "MINAS SHOPP ",
                           "J. CANDIDO  ", 
                           "SANTA INES  ",
                           "HORTO       ",
                           "STA TEREZA  ",
                           "STA EFIGENIA",
                           "CENTRAL     ",
                           "LAGOINHA    ",
                           "C. PRATES   ",
                           "CALAFATE    ",
                           "GAMELEIRA   ",
                           "VILA OESTE  ",
                           "ELDORADO    "};
  
  int direcao;               // 1 ou -1
  int estacao_atual = 0;     // indice do vetor "estacoes"
  int anuncio = 0;           // indica se o anuncio já foi feito
  char nova_estacao = 0x00;  // recebe o pulso do botão

  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // Configuração de I/O, display e interface
  ConfigIO_MLCD();
  Init_MLCD();
  EnviaString("Proxima parada:");
  MLCD_LinhaColuna(2, 1);
  EnviaString("Est");
  
  // Mostra a estação inicial (Vilarinho)
  MLCD_LinhaColuna(2, 5);
  EnviaString(estacoes[estacao_atual]);

  // Loop infinito
  while(1)
  {
    nova_estacao = P1IN&BIT0;
    
    // Ao chegar na estação final muda de direção
    if (estacao_atual == 17){
      direcao = -1;
    }
    
    // Ao chegar na primeira estação muda de direção
    else if (estacao_atual == 0){
      direcao = 1;
    }
      
    // Se o pulso voltar a zero, significa que o anúncio ainda não foi feito
    if (nova_estacao == 0x00){
      anuncio = 0;
    }
    
    // Ao chegar em uma nova estação...
    // Somente ao chegar em uma nova estação e se o anúncio não foi feito...
    if ((nova_estacao) & (anuncio == 0)){
      estacao_atual += direcao;  // Próxima estação
      MLCD_LinhaColuna(2, 5);
      EnviaString(estacoes[estacao_atual]);
      P1OUT |= BIT7;                // Acende Led
      __delay_cycles(_250ms * 4);   // Tempo de 1 segundo
      P1OUT &= ~BIT7;               // Apaga led
      anuncio = 1;                  // Anúncio feito
    }
  }
}
