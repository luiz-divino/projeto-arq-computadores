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
