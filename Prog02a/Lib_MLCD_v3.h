/*
Biblioteca de fun��e para operar com m�dulo LCD 16x2
compat�vel com o chip Hitachi HD44780.

*/

#define Fclk 750e3              // Freq clk kit F1611 ou F149
#define _100us (Fclk*100e-6)    // Delay de 100 micro segundos
#define _5ms   (Fclk*5e-3)      // Delay de 5 mili segundos
#define _2ms   (Fclk*2e-3)      // Delay de 2 mili segundos
#define _250ms (Fclk*250e-3)    // Delay de 250 mili segundos

#define Enable BIT6
#define RS BIT7
#define Disp_sel_Data P5SEL
#define Disp_dir_Data P5DIR
#define Disp_Data P5OUT
#define Disp_sel_Ctl P3SEL
#define Disp_dir_Ctl P3DIR
#define Disp_Ctl P3OUT

/*
Fun��o que gera o pulso de Enable
*/
void Envia(void)
{
Disp_Ctl  &= ~(Enable);   // Gera pulso de Enable
Disp_Ctl  |=  (Enable);
Disp_Ctl  &= ~(Enable);
__delay_cycles(_100us); 
}

/*
Fun��o que envia um comando ou configura��o para o MLCD
*/
void EnviaComando(char Comando)
{
Disp_Ctl  &= ~(RS);       // Faz RS = 0 seleciona op��o para enviar comando
Disp_Data  = Comando;     // Exterioriza o Comando para o barram. de dados do MLCD
Envia();
}

/*
Fun��o que envia um caractere a ser exibido
*/
void EnviaDado (char Dado)
{
Disp_Ctl  |=  (RS);       // Faz RS = 1 seleciona op��o para enviar caractere
Disp_Data  =  Dado;      // Exterioriza o Comando para o barram. de dados do MLCD
Envia();
}

/*
Fun��o que configura os pinos de I/O conectados ao MLCD
Enable = P3.6
RS = P3.7
Barr de dados = P5.0....P5.7
*/
void ConfigIO_MLCD(void)
{
  Disp_sel_Data = 0;      // Config. toda P5 como I/O
  Disp_dir_Data = 0xFF;   // Config. toda P5 como sa�da
  
  Disp_sel_Ctl &= ~(Enable+RS);  // Config Enab e RS como I/O
  Disp_dir_Ctl |=  (Enable+RS);  // Config Enab e RS como sa�da
  Disp_Ctl     &= ~(Enable);     // Enable inicia em 0
}


/*
Fun��o que reposiciona a entrada de caracters no MLCD
Valor Linha deve ser 1 ou 2
Valor Coluna deve estar entre 1 e 16
*/
void MLCD_LinhaColuna(int Linha, int Coluna)
{
EnviaComando( 0x80 +((Linha-1)*0x40) + (Coluna-1));
}

/*
Fun��o que envia a sequencia de comandos respons�veis pela
inicializa��o do MLCD
*/
void Init_MLCD(void)
{
__delay_cycles(_250ms);
EnviaComando(0x38);
__delay_cycles(_5ms);
EnviaComando(0x0E);
EnviaComando(0x06);
EnviaComando(0x01);
__delay_cycles(_2ms);
}

/*
Fun��o que envia uma sequencia de caracteres para o display.
*/
void EnviaString(char *string)
{
  while(*string){  // Enquanto o caracter n�o for NULL...
    EnviaDado(*string);    // Envia o conte�do do ponteiro
    string++;              // Muda o ponteiro para a pr�xima posi��o
  }
}
