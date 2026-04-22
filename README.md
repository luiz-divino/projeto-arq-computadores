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

## 🎮 Instruções de Uso
1. **Modo LOAD (#):** Pressione `#` para iniciar. Digite o Opcode e o Operando (ex: `2 5` para LOADK 5). Use a tecla `C` para confirmar cada linha. Pressione `#` novamente para salvar.
2. **Modo RUN:** Pressione `A` para inicializar o Program Counter (`PC=0`).
3. **Step (*):** Cada pressão na tecla `*` executa exatamente UMA instrução, permitindo acompanhar o estado dos registradores no Serial Monitor.

---