---

# Simulador de Escalonamento de Processos

Projeto acadêmico de Sistemas Operacionais I  
Simulação dos algoritmos: FIFO, SJF, Prioridade e Lottery (Escalonamento por Sorteio),  
com visualização dos resultados via Python.

---

## **Estrutura do Projeto**

```

trabalhoSO/
├── src/              # Códigos-fonte em C (implementação dos algoritmos, simulação, exportação)
│   ├── main.c                # Função principal, ponto de entrada do simulador
│   ├── escalonador.c/h       # Lógica do escalonamento, função principal da simulação
│   ├── algoritmos.c/h        # Implementação dos algoritmos de escalonamento
│   ├── modulo1.c/h           # Funções auxiliares e geração de processos
│   ├── exporta\_json.c/h      # Funções para exportar os resultados em formato JSON
│   └── processo.h            # Definição da estrutura de um processo
├── build/           # Binários compilados (.exe, .out)
│   └── simulador            # Executável do simulador
├── results/         # Resultados das simulações (arquivos JSON)
│   ├── fifo.json           # Saída da simulação FIFO
│   ├── sjf.json            # Saída da simulação SJF
│   ├── prioridade.json     # Saída da simulação por Prioridade
│   └── lottery.json        # Saída da simulação Lottery
├── gui/             # Scripts Python para visualização dos resultados
│   └── run.py               # Compila, executa e mostra os resultados das simulações
├── .gitignore       # Arquivos e pastas ignorados pelo Git
└── README.md        # Este arquivo de documentação

```

---

## **Como compilar e executar**

1. **Compile e rode o projeto usando o script Python:**

   ```bash
   python gui/run.py
   ```

   **ou**

   ```bash
   python3 gui/run.py
   ```

O script compila todos os arquivos, executa o simulador e exibe os resultados em formato tabular.

2. **Resultados das simulações**

   - Os arquivos `.json` são gerados em `results/` após cada execução.
   - Os dados podem ser usados para gráficos, análises ou integração com outras GUIs.

---

## **Observações**

- O projeto cria automaticamente as pastas `build/` e `results/` se não existirem.
- Apenas os arquivos de código-fonte, scripts e resultados são versionados no Git; binários e arquivos temporários são ignorados.

---

## **Autores**

---
