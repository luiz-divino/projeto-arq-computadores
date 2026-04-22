#  Interpretador de Instruções - Arquitetura de Computadores
**Disciplina:** Arquitetura de Computadores  
**Hardware:** Arduino Mega 2560

---

##  Sobre o Projeto
Este sistema simula o funcionamento de um processador real. Ele não apenas executa comandos, mas implementa o conceito de **Programa Armazenado** (Arquitetura de Von Neumann). O usuário "carrega" um software via teclado matricial, e a Unidade de Controle processa cada instrução seguindo o ciclo de Fetch-Decode-Execute.

---

##  Mapeamento da arquitetura (Software-Hardware)
Para atender aos requisitos técnicos, o sistema foi estruturado com as seguintes abstrações:

* **Unidade de Controle (UC):** Gerencia o fluxo através do `PC` e a decodificação no `IR`.
* **ULA:** Realiza operações aritméticas e atualiza a `FLAG_Z`.
* **Memória de Programa:** Vetor `programa[16]` que armazena as instruções no modo **LOAD**.
* **Memória de Dados:** Vetor `MEM[16]` para armazenamento volátil (instruções `STORE`/`LOADM`).
* **Registradores:** Implementação física das variáveis `PC`, `IR`, `ACC` e `FLAG_Z`.

---

##  Instruções de Uso
1. **Modo LOAD (#):** Pressione `#` para iniciar. Digite o Opcode e o Operando (ex: `2 5` para LOADK 5). Use a tecla `C` para confirmar cada linha. Pressione `#` novamente para salvar.
2. **Modo RUN:** Pressione `A` para inicializar o Program Counter (`PC=0`).
3. **Step (*):** Cada pressão na tecla `*` executa exatamente UMA instrução, permitindo acompanhar o estado dos registradores no Serial Monitor.

---

##  Pinagem Obrigatória
O projeto utiliza barramentos organizados para simular a comunicação de dados de um processador:
* **Display 7 Seg:** Pinos 22-28 (Barramento Paralelo)
* **Teclado:** Pinos 30-37 (Interface de Entrada)
* **Sensores/Atuadores:** Pinos 40-45

---

##  Conjunto de Instruções (ISA)
O sistema utiliza uma arquitetura de instruções de 4 bits para o Opcode, permitindo até 16 comandos distintos.

| Decimal | Opcode | Mnemônico | Descrição |
| :--- | :--- | :--- | :--- |
| 0 | 0000 | NOP | Nenhuma operação. |
| 1 | 0001 | READ | Lê o sensor ultrassônico e carrega o valor no `ACC`. |
| 2 | 0010 | LOADK | Carrega uma constante `K` no `ACC`. |
| 3 | 0011 | ADDK | Soma uma constante `K` ao `ACC` (Operação da ULA). |
| 4 | 0100 | SUBK | Subtrai uma constante `K` do `ACC` (Operação da ULA). |
| 5 | 0101 | CMPK | Compara `ACC` com `K` e atualiza a `FLAG_Z`. |
| 6 | 0110 | LEDON | Liga um LED (especificado pelo operando). |
| 7 | 0111 | LEDOFF| Desliga um LED (especificado pelo operando). |
| 8 | 1000 | BUZON | Ativa o alerta sonoro (Buzzer). |
| 9 | 1001 | BUZOFF| Desativa o alerta sonoro (Buzzer). |
| 10 | 1010 | DISP | Exibe o conteúdo do `ACC` no Display de 7 Segmentos. |
| 11 | 1011 | ALERT | Executa rotina automática de segurança baseada na distância. |
| 12 | 1100 | BINC | Exibe o Opcode binário da instrução atual no Serial. |
| 13 | 1101 | STORE | Armazena o valor do `ACC` na posição `MEM[X]`. |
| 14 | 1110 | LOADM | Carrega em `ACC` o valor contido em `MEM[X]`. |
| 15 | 1111 | HALT | Interrompe o Ciclo de Instrução e encerra a execução. |

---

##  Tratamento de Resultados e Exceções
Para garantir a integridade dos dados e a correta interface com o usuário, o sistema trata limites representáveis:

* **Overflow (> 9):** Caso o resultado de uma operação na ULA ou um valor carregado no `ACC` seja maior que a capacidade de um dígito do display, o sistema exibe o caractere `E` (Error) e emite um alerta no Monitor Serial.
* **Resultado Negativo (< 0):** Operações que resultem em valores negativos são sinalizadas no display através do caractere `-`, indicando um valor não representável em formato decimal simples.