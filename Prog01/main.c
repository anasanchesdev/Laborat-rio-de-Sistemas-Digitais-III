/* Cabeçalho:
Nome do programa: Prog01
Descrição do programa: Motores que são alimentados por um sistema trifásico só devem funcionar ser as
3 fases estiverem fornecendo energia. A falta de uma fase compromete severamente
o funcionamento do motor.

Elabore um sistema de monitoramento das 3 fases para que o funcionamento do 
motor só seja habilitado se houver a alimentação das 3 fases.

Considere que as seguintes entradas já tiveram os seus sinais condicionados
e que o nível 1 indica a presença da alimentação da fase correspondente.
P1.0 = Fase A
P1.1 = Fase B
P1.2 – Fase C

Utilize a saída P1.7 como a saída que vai fornecer o nível 1 para habilitar
o funcionamento do motor.
*/

#include "io430.h"

void main( void )
{
    // Definição de variáveis locais
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    // Configurações de periféricos
    P1SEL = BIT0;                       // Configura P1 como entrada/saída
    P2SEL = BIT0;                       // Configura P2 como entrada/saída
    P1DIR &= ~(BIT0 + BIT1 + BIT2);     // Configura P1.0, P1.1 e P1.2 como entradas
    P1DIR |= BIT7;                      // Configura P1.7 como saída

    // Loop infinito
    while(1)
    {     
        if ((P1IN&BIT0) & (P1IN&BIT1) & (P1IN&BIT2))
        {
            P1OUT |= BIT7; // Define 1 (ligado) para a saída de P1.7 se todas as fases estiverem funcionando em nível 1
        }

        else
        {
            P1OUT &= ~BIT7; // Define 0 (desligado) para a saída de P1.7 se pelo menos uma fase não estiver funcionando em nível 1
        }
    }
}
