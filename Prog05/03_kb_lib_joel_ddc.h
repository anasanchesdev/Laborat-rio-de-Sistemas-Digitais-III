/* T�tulo: bllbioteca de func�es do teclado kit MSP430F1611
Autor: Joel A. Santos
Data: abril/2019
Ambiente: IAR Embeded Workbench IDE; ver.: 5.5.2.1
�ltima rev: 04/2019
------------------------------------------------------------------------------*/
/* T�tulo: Leitura de teclado matricial por polling
Autor: Joel A. Santos
Data: agosto/2020
Ambiente: IAR Embeded Workbench IDE; ver.: 5.60.2
�ltima rev: 06/2021
------------------------------------------------------------------------------*/

//-----------------------Declara��o de Fun��es---------------------------------

//--------------------------Fun��es do Teclado--------------------------------



// Configura o I/O correspondente ao teclado
void tec_config()
{
  P3SEL &= ~(BIT0+BIT1+BIT2+BIT3);   // Configura P2 e P3 como I/O
  P2SEL &= ~(BIT0+BIT1+BIT2);

  P3DIR |=  (BIT0+BIT1+BIT2+BIT3);  // define linhas em P3 como sa�da -> valores a serem alterados
  P2DIR &= ~(BIT0+BIT1+BIT2);       // define P2.0,P2.1 e P2.2 como entrada - colunas -> teclas do teclado
}

// Converte o c�digo de varredura obtido na fun��o tec_rd em ASCII
char ascii_convert(char key_code)
// key_code -> valor hexadecimal da tecla obtido atrav�s de linha(hex) + coluna (hex)
{
  unsigned char tab_tec[12] = {"0123456789AE"};  // Teclas convertidas 
  unsigned char tab_key[12] = {0x28,0x41,0x21,0x11,0x42,0x22,0x12,0x44,0x24,0x14,0x48,0x18};
  unsigned int i=0;
  unsigned flag =0; // sinalizador de convers�o de tecla

  // Roda enquanto n�o houver correspod�ncia entre a tecla (key_code) com os valores de tab_key (itera��o)
  while (flag==0x00)
  {
    if ( key_code != tab_key[i])
    {
      i++;
    }
    else
      flag=0xFF;  
  }
  return tab_tec[i]; 
}


// (*) -> duvida
// Faz a varredura do teclado e gera como sa�da o c�digo ASCII da tecla presssionada.
// SE n�o houver tecla pressionada retorna 0x00
char tec_rd ()
{
  char col = 0, lin = BIT0, key_code=0, tecla=0;

  // Permanece em loop at� encontrar uma coluna ativa (tecla pressionada)
  // ou at� o termino da varredura
  while ((col==0x00) & (lin < BIT4)) // lin < 0x04
  {
    P3OUT = P3OUT &~(BIT0+BIT1+BIT2+BIT3); // Zera todas as sa�das (linhas)
    P3OUT = P3OUT | lin; // ativa uma linha (*)

    // Este tempo de atraso pode ser exclu�do sem preju�zo para o funcionamento
    // da fun��o.
    // Para o caso do uso desta fun��o dentro de uma interrup��o ele gera um
    // extremamente grande, contrariando a principal orienta��o a respeito de
    // fun��es de interrup��o.

    __delay_cycles(20e-3*Fclk); //aguarda estabiliza��o das linhas

    col = P2IN &(BIT0+BIT1+BIT2); // atualiza estado das colunas -> l� o teclado

    if (col!=0x00)  // Tem alguma tecla pressionada? Coluna diferente de zero?
    {
      // Sim, tem tecla pressionada
      col= col << 4;// concatenar linha com coluna (*)
      key_code=lin+col; // 0xCL (0xcolunalinha)
      tecla = ascii_convert(key_code); //converter em ASCII
    }
    else
      // N�o tem tecla pressionada
      lin = lin << 1; //ativar pr�xima linha -> desloca para o pr�ximo bit
  }
  return (tecla); // retorna ASCII da tecla
}


// Fun��o que l� uma uma tecla e s� retorna quando o usu�rio
// liberar a tecla

// Qual o problema desta fun��o? -> delay?

char espera_tecla()
{
  char tecla;

  tecla = tec_rd(); // tecla -> recebe tecla em ASCII

  if(tecla) while(tec_rd()); // se tecla for "0"? while(tec_rd) sempre vai retornar algum valor

  return tecla;

}