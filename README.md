# 💾 Mini Banco de Dados em C

Um sistema de gerenciamento de dados leve e eficiente construído inteiramente na linguagem C. Este projeto demonstra os fundamentos de persistência de dados, alocação dinâmica de memória e manipulação de arquivos binários/texto.

## 🚀 Funcionalidades
* **CRUD Completo:** Criação, leitura, atualização e exclusão de registros.
* **Persistência em Disco:** Os dados são salvos localmente em arquivos `.txt`.
* **Gerenciamento de Memória:** Uso otimizado de ponteiros e `structs` para evitar vazamento de memória (memory leaks).

## 🛠️ Tecnologias Utilizadas
* **C (Standard Library)**
* **GCC (Compilador)**

## ⚙️ Como Executar na Sua Máquina
*pré-requisito: instalação do compilado (GCC) no terminal do seu Sistema Operacional (SO).

1. Clone este repositório:
   git clone https://github.com/DaniloODev/Mini-Banco-de-Dados.git
2. Navegue até a pasta.
3. Compile o código usanod o GCC:
   gcc MiniBD.c -o minidb
4. Execute o programa:
   -No linux/mac: ./minidb
   -No windows: minidb.exe
