/*
Biblioteca de fun��es para operar com m�dulo LCD 16x2
compat�vel com o chip Hitachi HD44780.
*/


#define Fclk 750e3              // Freq clk kit F1611 ou F149
#define _100us (Fclk*100e-6)    // Delay de 100 micro segundos
#define _2ms (Fclk*2e-3)        // Delay de 2 mili segundos
#define Enable BIT6
#define RS BIT7

/*
Fun��o que configura os pinos de I/O conectados ao MLCD
Enable = P3.6
RS = P3.7
Barr de dados = P5.0....P5.7
*/
void ConfigIO_MLCD(void)
{
  P5SEL = 0;      // Config. toda P5 como I/O
  P5DIR = 0xFF;   // Config. toda P5 como sa�da
  
  P3SEL &= ~(Enable+RS);  // Config Enab e RS como I/O
  P3DIR |=  (Enable+RS);  // Config Enab e RS como sa�da
  P3OUT &= ~(Enable);     // Enable inicia em 0
}

/*
Fun��o que envia um comando ou configura��o para o MLCD
*/
void EnviaComando(char Comando)
{
  P3OUT &= ~(RS);       // Faz RS = 0 seleciona op��o para enviar comando
  P5OUT  = Comando;     // Exterioriza o Comando via P5OUT
  Envia();   
}

/*
Fun��o que envia um caractere para ser exibido no MLCD
*/
void Enviadado(char Dado)
{
  P3OUT |= RS;          // Faz RS = 0 seleciona op��o para enviar comando
  P5OUT  = Dado;        // Exterioriza o Dado via P5OUT
  Envia(); 
}

/*
Fun��o que gera o pulso de Enable
*/
void Envia(void)
{
P3OUT |= Enable;      // Gera pulso de Enable
  P3OUT &= ~(Enable);
  __delay_Cycles(_100us); 
}
