/* Título: bllbioteca de funcões do teclado kit MSP430F1611
Autor: Joel A. Santos
Data: abril/2019
Ambiente: IAR Embeded Workbench IDE; ver.: 5.5.2.1
última rev: 04/2019
------------------------------------------------------------------------------*/
/* Título: Leitura de teclado matricial por polling
Autor: Joel A. Santos
Data: agosto/2020
Ambiente: IAR Embeded Workbench IDE; ver.: 5.60.2
última rev: 06/2021
------------------------------------------------------------------------------*/

//-----------------------Declaração de Funções---------------------------------

//--------------------------Funções do Teclado--------------------------------



// Configura o I/O correspondente ao teclado
void tec_config()
{
  P3SEL &= ~(BIT0+BIT1+BIT2+BIT3);   // Configura P2 e P3 como I/O
  P2SEL &= ~(BIT0+BIT1+BIT2);

  P3DIR |=  (BIT0+BIT1+BIT2+BIT3);  // define linhas em P3 como saída -> valores a serem alterados
  P2DIR &= ~(BIT0+BIT1+BIT2);       // define P2.0,P2.1 e P2.2 como entrada - colunas -> teclas do teclado
}

// Converte o código de varredura obtido na função tec_rd em ASCII
char ascii_convert(char key_code)
// key_code -> valor hexadecimal da tecla obtido através de linha(hex) + coluna (hex)
{
  unsigned char tab_tec[12] = {"0123456789AE"};  // Teclas convertidas 
  unsigned char tab_key[12] = {0x28,0x41,0x21,0x11,0x42,0x22,0x12,0x44,0x24,0x14,0x48,0x18};
  unsigned int i=0;
  unsigned flag =0; // sinalizador de conversão de tecla

  // Roda enquanto não houver correspodência entre a tecla (key_code) com os valores de tab_key (iteração)
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
// Faz a varredura do teclado e gera como saída o código ASCII da tecla presssionada.
// SE não houver tecla pressionada retorna 0x00
char tec_rd ()
{
  char col = 0, lin = BIT0, key_code=0, tecla=0;

  // Permanece em loop até encontrar uma coluna ativa (tecla pressionada)
  // ou até o termino da varredura
  while ((col==0x00) & (lin < BIT4)) // lin < 0x04
  {
    P3OUT = P3OUT &~(BIT0+BIT1+BIT2+BIT3); // Zera todas as saídas (linhas)
    P3OUT = P3OUT | lin; // ativa uma linha (*)

    // Este tempo de atraso pode ser excluído sem prejuízo para o funcionamento
    // da função.
    // Para o caso do uso desta função dentro de uma interrupção ele gera um
    // extremamente grande, contrariando a principal orientação a respeito de
    // funções de interrupção.

    __delay_cycles(20e-3*Fclk); //aguarda estabilização das linhas

    col = P2IN &(BIT0+BIT1+BIT2); // atualiza estado das colunas -> lê o teclado

    if (col!=0x00)  // Tem alguma tecla pressionada? Coluna diferente de zero?
    {
      // Sim, tem tecla pressionada
      col= col << 4;// concatenar linha com coluna (*)
      key_code=lin+col; // 0xCL (0xcolunalinha)
      tecla = ascii_convert(key_code); //converter em ASCII
    }
    else
      // Não tem tecla pressionada
      lin = lin << 1; //ativar próxima linha -> desloca para o próximo bit
  }
  return (tecla); // retorna ASCII da tecla
}


// Função que lê uma uma tecla e só retorna quando o usuário
// liberar a tecla

// Qual o problema desta função? -> delay?

char espera_tecla()
{
  char tecla;

  tecla = tec_rd(); // tecla -> recebe tecla em ASCII

  if(tecla) while(tec_rd()); // se tecla for "0"? while(tec_rd) sempre vai retornar algum valor

  return tecla;

}