/* Prog07 - Cronometro:
Faca um programa e codificacao de um cronometro. A saida do cronometro sera mos
trada no LCD conforme formato da figura. Utilize dois bytes para as horas, dois
bytes para os minutos e dois bytes para os segundos.
____________
| HH:MM:SS |

Utilize o teclado do sistema para inserir as seguintes funcionalidades ao 
cronometro:

TECLA   | FUNCAO
------------------------------
0       | Zera o cronometro
1       | Dispara a contagem
2       | Trava a contagem
3       | Continua a contagem

Considere que o intervalo de tempo de 1s, para atualizacao do cronometro, seja
realizada por software (funcao de temporizacao)
*/

#include "io430.h"
#include "kb_lib_joel_cc_ddc.h"
#include "Lib_MLCD_v3.h"

// Prototipos de funcao
void militempo(float tempo);
void atualizaCronometro(int coluna, char tempo);

// Programa principal
void main(void)
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // Configuracao de I/O, display e teclado
  ConfigIO_MLCD();
  Init_MLCD();
  tec_config();
  
  // Variaveis do programa
  char dezenas_segundos = 0x30; // 0x30 = '0'
  char dezenas_minutos = 0x30;
  char dezenas_horas = 0x30;
  char unidades_segundos = 0x30;
  char unidades_minutos = 0x30;
  char unidades_horas = 0x30;
  char tecla_anterior;
  char tecla_atual;
  
  // Configuracao inicial do display HH:MM:SS
  EnviaString("00:00:00");
  
  // Loop infinito
  while(1){
    militempo(1000);  // tempo de atualizacao do cronometro
    tecla_atual = tec_rd();  // Le a tecla atual
      
    // Incrementa unidades dos segundos
    unidades_segundos++;
    // Atualiza MLCD
    atualizaCronometro(8, unidades_segundos);
    
    // Incrementa dezenas dos segundos apos 00:00:09
    if (unidades_segundos > '9'){
      dezenas_segundos++;
      unidades_segundos = 0x30;  // Reseta unidade dos segundos
      // Atualiza MLCD
      atualizaCronometro(7, dezenas_segundos);
      atualizaCronometro(8, unidades_segundos);
    }
    
    // Incrementa unidade dos minutos apos 00:00:59
    if (dezenas_segundos > '5'){
      unidades_minutos++;
      dezenas_segundos = 0x30;  // Reseta dezena dos segundos
      // Atualiza MLCD
      atualizaCronometro(5, unidades_minutos);
      atualizaCronometro(7, dezenas_segundos);
    }
    
    // Incrementa dezenas dos minutos apos 00:09:59
    if (unidades_minutos > '9'){
      dezenas_minutos++;
      unidades_minutos = 0x30;  // Reseta unidade dos minutos
      // Atualiza MLCD
      atualizaCronometro(4, dezenas_minutos);
      atualizaCronometro(5, unidades_minutos);
    }
    
    // Incrementa unidades das horas apos 00:59:59
    if (dezenas_minutos > '5'){
      unidades_horas++;
      dezenas_minutos = 0x30;  // Reseta dezena dos minutos
      // Atualiza MLCD
      atualizaCronometro(2, unidades_horas);
      atualizaCronometro(4, dezenas_minutos);
    }
    
    // Reseta cronometro           
    if (((dezenas_horas == '2') && (unidades_horas > '3')) || (tecla_atual == '0') ){
      // Zera todas as variaveis
      dezenas_segundos = 0x30; 
      dezenas_minutos = 0x30;
      dezenas_horas = 0x30;
      unidades_segundos = 0x30;
      unidades_minutos = 0x30;
      unidades_horas = 0x30;
      // Atualiza MLCD
      atualizaCronometro(1, dezenas_horas);
      atualizaCronometro(2, unidades_horas);
      atualizaCronometro(4, dezenas_minutos);
      atualizaCronometro(5, unidades_minutos);
      atualizaCronometro(7, dezenas_segundos);
      atualizaCronometro(8, unidades_segundos);
    }
                
    // Incrementa dezenas das horas apos 09h59m59s
    else if (unidades_horas > '9'){
      dezenas_horas++;
      unidades_horas = 0x30;  // Reseta unidade das horas
      // Atualiza MLCD
      atualizaCronometro(1, dezenas_horas);
      atualizaCronometro(2, unidades_horas);
    }           
    
    tecla_anterior = tecla_atual;
      
    /*
    segundos ++
    
    // Incrementa minutos
    if (segundos > '59'){
      minutos++;
      segundos = 0x30;
    }
    
    // Incrementa horas
    if (minutos > '59'){
      minutos = 0x30;
      horas++;
    }
    
    // Zera o cronometro ao atingir o limite
    if (horas > '23'){
      segundos = 0x30;
      minutos = 0x30;
      horas = 0x30;*/
    
    }
  
 
}

// Funcoes do programa
void militempo(float tempo){
  while (tempo){
    __delay_cycles(Fclk*1e-3);
    tempo--;
  }
}

/*
Atualiza coluna da linha 1 com 1 byte
Args:
- int coluna: numero da coluna
- char dado: dado a ser escrito
*/

void atualizaCronometro(int coluna, char dado){
  MLCD_LinhaColuna(1, coluna);
  EnviaDado(dado);
}

