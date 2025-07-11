/*
Prog01b: Rel� de falta de fase
Altere a vers�o �a� para que o led pisque na frequ�ncia de 1Hz ao contr�rio de permanecer aceso. 
Utilize a fun��o __delay_cycles.
Fclk = 750kHz tanto para o kit F1611 como para o kit F149.
*/

/* Cabe�alho:
Nome do programa: Prog01
Descri��o do programa: Motores que s�o alimentados por um sistema trif�sico devem funcionar se as
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
*/

#include "io430.h"
#define fclk 750e3 // 750 pulsos de clock

void main(void)
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    // Configura��es de perif�ricos
    P1SEL &= BIT0;                      // Configura P1 como entrada/sa�da
    P1DIR &= ~(BIT0 + BIT1 + BIT2);     // Configura P1.0, P1.1 e P1.2 como entradas
    P1DIR |= BIT7;                      // Configura P1.7 como sa�da

    // Loop infinito
    while(1)
    {       
        // Se todas as fases estiverem funcionando...
        if ((P1IN&BIT0) && (P1IN&BIT1) && (P1IN&BIT2))
        {
            // Led alterna entre ligado/desligado com intervalo de 0.5 segundo
            P1OUT |= BIT7;               // Define 1 (ligado) para a sa�da de P1.7 
            __delay_cycles(fclk * 0.5);  // Delay de 0.5 segundo
            P1OUT &= ~BIT7;              // Define 0 (desligado) para a sa�da de P1.7
            __delay_cycles(fclk * 0.5);  // Delay de 0.5 segundo

        }
        
        // Se pelo menos uma fase n�o estiver funcionando em n�vel 1...
        else
        {
            P1OUT &= ~BIT7; // Define 0 (desligado) para a sa�da de P1.7 
        }
    }
}
