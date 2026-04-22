/*
12345 Seu Nome - TA
Antonio Damasceno
Luis F
Giovanna Salles
Guilherme
Matheus
*/

#include <Keypad.h>

// ==========================================
// 1. ELEMENTOS ARQUITETURAIS (OBRIGATÓRIOS)
// ==========================================
int MEM[16];           // Memória de dados simulada (F09)
int PC = 0;            // Program Counter (Contador de Programa)
byte IR = 0;           // Instruction Register (Registrador de Instrução - Opcode)
int ACC = 0;           // Acumulador (ULA e operandos)
bool FLAG_Z = false;   // Flag de Zero (Resultado de comparações)
bool EXECUTANDO = false; // Controle de execução (HALT)

// Memória de Programa (Armazena as instruções no modo LOAD)
String programa[16];   
int ponteiroCarga = 0; 
bool modoLoad = false;

// ==========================================
// 2. PINAGEM OBRIGATÓRIA (SEÇÃO 3.3)
// ==========================================
const byte pinDisplay[] = {22, 23, 24, 25, 26, 27, 28}; // a, b, c, d, e, f, g
const byte pinTeclado[] = {30, 31, 32, 33, 34, 35, 36, 37}; // L1-L4, C1-C4
#define PIN_TRIG 40
#define PIN_ECHO 41
#define PIN_LED1 42
#define PIN_LED2 43
#define PIN_LED3 44
#define PIN_BUZZER 45

// Configuração do Teclado
char keys[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
byte rowPins[4] = {30, 31, 32, 33}; 
byte colPins[4] = {34, 35, 36, 37}; 
Keypad teclado = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

// Matriz do Display (Cátodo Comum)
byte numMap[12][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}, // 9
  {1,0,0,1,1,1,1}, // E (Overflow)
  {0,0,0,0,0,0,1}  // - (Negativo)
};

void setup() {
  Serial.begin(9600);
  for(int i=0; i<7; i++) pinMode(pinDisplay[i], OUTPUT);
  pinMode(PIN_TRIG, OUTPUT); pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_LED1, OUTPUT); pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_LED3, OUTPUT); pinMode(PIN_BUZZER, OUTPUT);
  Serial.println("SISTEMA PRONTO. PRESSIONE '#' PARA MODO LOAD.");
}
