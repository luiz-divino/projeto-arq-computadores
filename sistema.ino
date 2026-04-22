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
// ==========================================
// 3. UNIDADE DE CONTROLE (UC) - CICLO DE INSTRUÇÃO
// ==========================================

void executarCiclo() {
  if (PC >= 16 || programa[PC] == "") {
    Serial.println("FIM DA MEMORIA DE PROGRAMA.");
    EXECUTANDO = false;
    return;
  }

  // BUSCA E DECODIFICAÇÃO (F02/F03)
  String instrucaoCompleta = programa[PC];
  int espaco = instrucaoCompleta.indexOf(' ');
  int opcodeDec = (espaco == -1) ? instrucaoCompleta.toInt() : instrucaoCompleta.substring(0, espaco).toInt();
  int operando = (espaco == -1) ? 0 : instrucaoCompleta.substring(espaco + 1).toInt();

  IR = (byte)opcodeDec; // Carga no Instruction Register

  // EXECUÇÃO (F03/F04)
  processarInstrucao(IR, operando);

  // APRESENTAÇÃO DE ESTADOS (SEÇÃO 2.2.4)
  exibirEstados();

  PC++; // Atualização do Program Counter
}

void processarInstrucao(byte opcode, int k) {
  switch(opcode) {
    case 0:  Serial.println("NOP"); break;
    case 1:  ACC = lerDistancia(); Serial.println("READ"); break; // F05
    case 2:  ACC = k; Serial.println("LOADK"); break;
    case 3:  ACC += k; Serial.println("ADDK"); break; // F04 (ULA)
    case 4:  ACC -= k; Serial.println("SUBK"); break; // F04 (ULA)
    case 5:  FLAG_Z = (ACC == k); Serial.println("CMPK"); break; // F04 (ULA)
    case 6:  acionarLED(k, HIGH); break; // F06
    case 7:  acionarLED(k, LOW); break;
    case 8:  digitalWrite(PIN_BUZZER, HIGH); break; // F07
    case 9:  digitalWrite(PIN_BUZZER, LOW); break;
    case 10: exibirNoDisplay(ACC); break; // F08
    case 11: executarAlert(); break; // F10
    case 12: Serial.print("BIN: "); Serial.println(opcode, BIN); break;
    case 13: if(k < 16) MEM[k] = ACC; break; // F09
    case 14: if(k < 16) ACC = MEM[k]; break; // F09
    case 15: EXECUTANDO = false; Serial.println("HALT"); break; // F11
  }
}

// ==========================================
// 4. FUNÇÕES DE APOIO
// ==========================================

void loop() {
  char tecla = teclado.getKey();
  if (!tecla) return;

  // Gerenciamento de Modos (Seção 2.2)
  if (tecla == '#') {
    modoLoad = !modoLoad;
    if (modoLoad) {
      ponteiroCarga = 0; PC = 0; EXECUTANDO = false;
      for(int i=0; i<16; i++) programa[i] = "";
      Serial.println(">>> MODO LOAD ATIVADO. INSIRA INSTRUCOES.");
    } else {
      Serial.println(">>> MODO LOAD ENCERRADO.");
    }
  } 
  else if (tecla == 'A' && !modoLoad) { // Comando RUN (Simulado por 'A')
    PC = 0; EXECUTANDO = true;
    Serial.println(">>> MODO RUN INICIADO. PRESSIONE '*' PARA STEP.");
  }
  else if (tecla == '*' && EXECUTANDO) {
    executarCiclo();
  }
  else if (modoLoad) {
    receberInstrucaoTeclado(tecla);
  }
}

void receberInstrucaoTeclado(char t) {
  static String buffer = "";
  if (t >= '0' && t <= '9' || t == 'B') { // 'B' simula o espaço para operando
    if (t == 'B') buffer += " ";
    else buffer += t;
    Serial.print(t);
  } 
  else if (t == 'C') { // 'C' simula o ENTER
    programa[ponteiroCarga] = buffer;
    Serial.println(" -> Salvo em MEM[" + String(ponteiroCarga) + "]");
    ponteiroCarga++;
    buffer = "";
  }
}

void exibirNoDisplay(int v) {
  int idx = v;
  if (v > 9) { idx = 10; Serial.println("OVERFLOW!"); } // 2.5.1
  else if (v < 0) { idx = 11; Serial.println("NEGATIVO!"); } // 2.5.2
  for(int i=0; i<7; i++) digitalWrite(pinDisplay[i], numMap[idx][i]);
}

int lerDistancia() {
  digitalWrite(PIN_TRIG, LOW); delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  return pulseIn(PIN_ECHO, HIGH) * 0.034 / 2;
}

void acionarLED(int l, int st) {
  if(l == 1) digitalWrite(PIN_LED1, st);
  if(l == 2) digitalWrite(PIN_LED2, st);
  if(l == 3) digitalWrite(PIN_LED3, st);
}

void executarAlert() {
  int d = lerDistancia();
  if (d < 10) { digitalWrite(PIN_BUZZER, HIGH); digitalWrite(PIN_LED1, HIGH); }
  else if (d < 20) { digitalWrite(PIN_BUZZER, LOW); digitalWrite(PIN_LED1, HIGH); }
  else { digitalWrite(PIN_BUZZER, LOW); digitalWrite(PIN_LED1, LOW); }
}

void exibirEstados() {
  Serial.print("PC: "); Serial.print(PC);
  Serial.print(" | IR: "); Serial.print(IR, BIN);
  Serial.print(" | ACC: "); Serial.print(ACC);
  Serial.print(" | FLAG_Z: "); Serial.println(FLAG_Z);
}