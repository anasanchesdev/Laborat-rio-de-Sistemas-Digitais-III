/* Cabeçalho:
 Nome do programa: REV01
 Descrição do programa: Elaborar um fragmento de programa que configure 
 toda a porta P1 como entrada e toda a porta P2 como saída.
*/

#include "io430.h"

// Definição de variáveis globais (VG)
// Só crie variáveis globais se não houver outra solução mais elegante.
// VGs não justificadas não serão bem vistas.


// Apresentação de protótipo das funções que foram definidas após a função maim
// Definição de funções antes do main


void main( void )
{

    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    // Configurações de periféricos

    /*Elaborar um fragmento de programa que configure toda a porta P1 
    como entrada e toda a porta P2 como saída. */

    P1SEL = BIT0;        // Configura P1 como entrada ou saída
    P2SEL = BIT0;        // Configura P2 como entrada ou saída
    P1DIR &= BIT0;       // Configura P1 como entrada
    P2DIR |= ~BIT0;       // Configura P1 como saída

    /* Elaborar um fragmento de program que configure a porta P1 da seguinte forma: 
    P1.0... P1.5 como entrada e P1.6 e P1.7 como saída.
    */
    P1SEL = BIT0;        // Configura P1 como entrada ou saída
    P1DIR |= BIT7+BIT6;   // Configura P1.6 e P1.7 como saída

    // Loop infinito
    // A grande maioria dos programas possui uma lista repetitiva de tarefas.

}


// Definição de funções depois do main
