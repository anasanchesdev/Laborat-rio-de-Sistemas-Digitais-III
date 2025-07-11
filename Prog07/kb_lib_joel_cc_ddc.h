/*
Algumas adi��es e mdofica��es foram feitas nas fun��es originais do professor
Joel.

1. Inclus�o da fun�ao Espera_tecla()
2. Compila��o condicional para ajustar a fun��o correta para os kits 1611 e 149
*/

/* A inten��o � criar um conjunto de fun��es que sejam compiladas de acordo
com o processador selecionado. Para tal vamos utilizar alguns comandos
direcionados ao compilador que permitem implementar o que � conhecido como
compila��o condicional.
*/


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


/* 
Hardware do kitF1611
Linhas  L0 = P3.0 ;  L1 = P3.1;  L2 = P3.2;  L3 = P3.3;
Colunas C0 = P2.0;   C1 = P2.1;  C2 = P2.2  

Vers�o para o kitF149
Linhas  L0 = P6.4 ;  L1 = P6.5;  L2 = P6.6;  L3 = P6.7;
Colunas C0 = P2.0;   C1 = P2.1;  C2 = P2.2  
*/


// Pacote de c�digo a ser compilado se o processador selecionado for 

//------------------------------------------------------------------------------
#if defined (__MSP430F1611__) //Vers�o para o kitF1611

// Configura os pinos de I/O para atender o teclado
void tec_config()
{
  P3SEL &= ~(BIT0+BIT1+BIT2+BIT3);   // Configura P2 e P3 como I/O
  P2SEL &= ~(BIT0+BIT1+BIT2);
  
  P3DIR |=  (BIT0+BIT1+BIT2+BIT3);  // define linhas em P3 como sa�da
  P2DIR &= ~(BIT0+BIT1+BIT2);       //Define P2.0,P2.1 e P2.2 como entrada - colunas
}

// Converte o c�digo de varredura obtido na fun��o tec_rd em ASCII
char ascii_convert(char key_code)
{
  char tab_tec[12] = {"0123456789AE"};
  char tab_key[12] = {0x28,0x41,0x21,0x11,0x42,0x22,0x12,0x44,0x24,0x14,0x48,0x18};
  unsigned int i=0;
  unsigned flag =0; // sinalisador de convers�o de tecla
  
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

// Faz a varredura do teclado e gera como sa�da o c�digo ASCII da tecla presssionada.
// SE n�o houver tecla pressionada retorna 0x00

char tec_rd ()
{
  char col = 0, lin = BIT0, key_code=0, tecla=0;
  
  // Permanece em loop at� encontrar uma coluna ativa
  // ou at� o termino da varredura
  while ((col==0x00) & (lin < BIT4))
  {
    P3OUT = P3OUT &~(BIT0+BIT1+BIT2+BIT3); // Zera todas as sa�das
    P3OUT = P3OUT | lin; //ativa uma linha
    
    // Este tempo de atraso pode ser exclu�do sem preju�zo para o funcionamento
    // da fun��o.
    // Para o caso do uso desta fun��o dentro de uma interrup��o ele gera um
    // extremamente grande, contrariando a principal orienta��o a respeito de
    // fun��es de interrup��o.
    
    //__delay_cycles(20e-3*Fclk); //aguarda estabiliza��o das linhas
    
    col = P2IN &(BIT0+BIT1+BIT2); //atualiza estado das colunas
    
    if (col!=0x00)  // Tem alguma tecla pressionada?
    {
      // Sim, tem tecla pressionada
      col= col << 4;// concatenar linha com coluna
      key_code=lin+col;
      tecla = ascii_convert(key_code); //conveerter em ASCII
    }
    else
      // N�o tem tecla pressionada
      lin = lin << 1; //ativar pr�xima linha
  }
  return (tecla); // retorna ASCII da tecla
}

//Fim da vers�o para o kitF1611
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#elif defined (__MSP430F149__) // Vers�o para o kitF149

/* Vers�o para o kitF149
Linhas  L0 = P6.4 ;  L1 = P6.5;  L2 = P6.6;  L3 = P6.7;
Colunas C0 = P2.0;   C1 = P2.1;  C2 = P2.2  
*/

// Configura os pinos de I/O para atender o teclado
void tec_config()
{
  P6SEL &= ~(BIT4+BIT5+BIT6+BIT7);   // Configura P2 e P6 como I/O
  P2SEL &= ~(BIT0+BIT1+BIT2);
  
  P6DIR |=  (BIT4+BIT5+BIT6+BIT7);  // define linhas em P6 como sa�da
  P2DIR &= ~(BIT0+BIT1+BIT2);       //Define P2.0,P2.1 e P2.2 como entrada - colunas
}

// Converte o c�digo de varredura obtido na fun��o tec_rd em ASCII
char ascii_convert(char key_code)
{
  // Na vers�o adaptada para o kit 149 o keycode teve invers�o na posic�o do 
  // c�dgio da linha com o da coluna
  char tab_key[12] = {0x82,0x14,0x12,0x11,0x24,0x22,0x21,0x44,0x42,0x41,0x84,0x81};
  char tab_tec[12] = {"0123456789AE"}; 
  unsigned int i=0;
  unsigned flag =0; // sinalisador de convers�o de tecla
  
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

//Faz a varredura do teclado e gera como sa�da o c�digo ASCII da tecla presssionada.
//SE n�o houver tecla pressionada retorna 0x00
char tec_rd ()
{
  char col = 0, lin = BIT4, key_code=0, tecla=0, i = 0;
  
  // Permanece em loop at� encontrar uma coluna ativa
  // ou at� o termino da varredura
  while ((col==0x00) & (i < 4))
  {
    P6OUT = P6OUT &~(BIT4+BIT5+BIT6+BIT7); // Zera todas as sa�das
    P6OUT = P6OUT | lin; //ativa uma linha
    
    // Este tempo de atraso pode ser exclu�do sem preju�zo para o funcionamento
    // da fun��o.
    // Para o caso do uso desta fun��o dentro de uma interrup��o ele gera um
    // extremamente grande, contrariando a principal orienta��o a respeito de
    // fun��es de interrup��o.
    
    //__delay_cycles(1e-3*Fclk); //aguarda estabiliza��o das linhas
    
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
      // N�o tem tecla pressionada
      lin = lin << 1; //ativar pr�xima linha
      i++;
    }
  }
  return (tecla); // retorna ASCII da tecla
}

//Fim da vers�o para o kitF149


#else
#error "****** Voce esqueceu de selecionar o processador adequado. *****"
#endif
//------------------------------------------------------------------------------



/*
Fun��o que l� uma uma tecla e s� retorna quando o usu�rio liberar a tecla

Qual o inconveniente desta fun��o?
*/

char Espera_tecla()
{
  char tecla;
  
  tecla = tec_rd();
  
  if(tecla) while(tec_rd());
  
  return tecla;
  
}