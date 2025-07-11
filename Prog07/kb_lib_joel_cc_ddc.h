/*
Algumas adições e mdoficações foram feitas nas funções originais do professor
Joel.

1. Inclusão da funçao Espera_tecla()
2. Compilação condicional para ajustar a função correta para os kits 1611 e 149
*/

/* A intenção é criar um conjunto de funções que sejam compiladas de acordo
com o processador selecionado. Para tal vamos utilizar alguns comandos
direcionados ao compilador que permitem implementar o que é conhecido como
compilação condicional.
*/


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


/* 
Hardware do kitF1611
Linhas  L0 = P3.0 ;  L1 = P3.1;  L2 = P3.2;  L3 = P3.3;
Colunas C0 = P2.0;   C1 = P2.1;  C2 = P2.2  

Versão para o kitF149
Linhas  L0 = P6.4 ;  L1 = P6.5;  L2 = P6.6;  L3 = P6.7;
Colunas C0 = P2.0;   C1 = P2.1;  C2 = P2.2  
*/


// Pacote de código a ser compilado se o processador selecionado for 

//------------------------------------------------------------------------------
#if defined (__MSP430F1611__) //Versão para o kitF1611

// Configura os pinos de I/O para atender o teclado
void tec_config()
{
  P3SEL &= ~(BIT0+BIT1+BIT2+BIT3);   // Configura P2 e P3 como I/O
  P2SEL &= ~(BIT0+BIT1+BIT2);
  
  P3DIR |=  (BIT0+BIT1+BIT2+BIT3);  // define linhas em P3 como saída
  P2DIR &= ~(BIT0+BIT1+BIT2);       //Define P2.0,P2.1 e P2.2 como entrada - colunas
}

// Converte o código de varredura obtido na função tec_rd em ASCII
char ascii_convert(char key_code)
{
  char tab_tec[12] = {"0123456789AE"};
  char tab_key[12] = {0x28,0x41,0x21,0x11,0x42,0x22,0x12,0x44,0x24,0x14,0x48,0x18};
  unsigned int i=0;
  unsigned flag =0; // sinalisador de conversão de tecla
  
  while ( flag==0x00)
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

// Faz a varredura do teclado e gera como saída o código ASCII da tecla presssionada.
// SE não houver tecla pressionada retorna 0x00

char tec_rd ()
{
  char col = 0, lin = BIT0, key_code=0, tecla=0;
  
  // Permanece em loop até encontrar uma coluna ativa
  // ou até o termino da varredura
  while ((col==0x00) & (lin < BIT4))
  {
    P3OUT = P3OUT &~(BIT0+BIT1+BIT2+BIT3); // Zera todas as saídas
    P3OUT = P3OUT | lin; //ativa uma linha
    
    // Este tempo de atraso pode ser excluído sem prejuízo para o funcionamento
    // da função.
    // Para o caso do uso desta função dentro de uma interrupção ele gera um
    // extremamente grande, contrariando a principal orientação a respeito de
    // funções de interrupção.
    
    //__delay_cycles(20e-3*Fclk); //aguarda estabilização das linhas
    
    col = P2IN &(BIT0+BIT1+BIT2); //atualiza estado das colunas
    
    if (col!=0x00)  // Tem alguma tecla pressionada?
    {
      // Sim, tem tecla pressionada
      col= col << 4;// concatenar linha com coluna
      key_code=lin+col;
      tecla = ascii_convert(key_code); //conveerter em ASCII
    }
    else
      // Não tem tecla pressionada
      lin = lin << 1; //ativar próxima linha
  }
  return (tecla); // retorna ASCII da tecla
}

//Fim da versão para o kitF1611
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#elif defined (__MSP430F149__) // Versão para o kitF149

/* Versão para o kitF149
Linhas  L0 = P6.4 ;  L1 = P6.5;  L2 = P6.6;  L3 = P6.7;
Colunas C0 = P2.0;   C1 = P2.1;  C2 = P2.2  
*/

// Configura os pinos de I/O para atender o teclado
void tec_config()
{
  P6SEL &= ~(BIT4+BIT5+BIT6+BIT7);   // Configura P2 e P6 como I/O
  P2SEL &= ~(BIT0+BIT1+BIT2);
  
  P6DIR |=  (BIT4+BIT5+BIT6+BIT7);  // define linhas em P6 como saída
  P2DIR &= ~(BIT0+BIT1+BIT2);       //Define P2.0,P2.1 e P2.2 como entrada - colunas
}

// Converte o código de varredura obtido na função tec_rd em ASCII
char ascii_convert(char key_code)
{
  // Na versão adaptada para o kit 149 o keycode teve inversão na posicão do 
  // códgio da linha com o da coluna
  char tab_key[12] = {0x82,0x14,0x12,0x11,0x24,0x22,0x21,0x44,0x42,0x41,0x84,0x81};
  char tab_tec[12] = {"0123456789AE"}; 
  unsigned int i=0;
  unsigned flag =0; // sinalisador de conversão de tecla
  
  while ( flag==0x00)
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

//Faz a varredura do teclado e gera como saída o código ASCII da tecla presssionada.
//SE não houver tecla pressionada retorna 0x00
char tec_rd ()
{
  char col = 0, lin = BIT4, key_code=0, tecla=0, i = 0;
  
  // Permanece em loop até encontrar uma coluna ativa
  // ou até o termino da varredura
  while ((col==0x00) & (i < 4))
  {
    P6OUT = P6OUT &~(BIT4+BIT5+BIT6+BIT7); // Zera todas as saídas
    P6OUT = P6OUT | lin; //ativa uma linha
    
    // Este tempo de atraso pode ser excluído sem prejuízo para o funcionamento
    // da função.
    // Para o caso do uso desta função dentro de uma interrupção ele gera um
    // extremamente grande, contrariando a principal orientação a respeito de
    // funções de interrupção.
    
    //__delay_cycles(1e-3*Fclk); //aguarda estabilização das linhas
    
    col = P2IN &(BIT0+BIT1+BIT2); //atualiza estado das colunas
    
    if (col!=0x00)  // Tem alguma tecla pressionada?
    {
      // Sim, tem tecla pressionada
      //col= col << 4;
      key_code=lin+col;    // concatenar linha com coluna
      tecla = ascii_convert(key_code); //conveerter em ASCII
    }
    else
    {
      // Não tem tecla pressionada
      lin = lin << 1; //ativar próxima linha
      i++;
    }
  }
  return (tecla); // retorna ASCII da tecla
}

//Fim da versão para o kitF149


#else
#error "****** Voce esqueceu de selecionar o processador adequado. *****"
#endif
//------------------------------------------------------------------------------



/*
Função que lê uma uma tecla e só retorna quando o usuário liberar a tecla

Qual o inconveniente desta função?
*/

char Espera_tecla()
{
  char tecla;
  
  tecla = tec_rd();
  
  if(tecla) while(tec_rd());
  
  return tecla;
  
}