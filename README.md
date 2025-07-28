# Simulador de Escalonamento de Processos

Projeto acadêmico de Sistemas Operacionais I  
Simulação dos algoritmos: FIFO, SJF, Prioridade e Lottery (Escalonamento por Sorteio),  
com visualização dos resultados em Python via interface gráfica interativa.

---

## **Estrutura do Projeto**

```

trabalhoSO/
├── src/              # Códigos-fonte em C (implementação dos algoritmos, simulação, exportação)
│   ├── main.c                # Função principal, ponto de entrada do simulador
│   ├── escalonador.c/h       # Lógica do escalonamento
│   ├── algoritmos.c/h        # Implementação dos algoritmos de escalonamento
│   ├── modulo1.c/h           # Geração e utilidades dos processos
│   ├── exporta\_json.c/h      # Funções para exportar os resultados em JSON
│   └── processo.h            # Definição da estrutura de um processo
├── build/           # Binários compilados (.exe, .out)
│   └── simulador            # Executável do simulador (gerado via gcc)
├── results/         # Resultados das simulações (arquivos JSON)
│   └── dados\_originais.json    # Processos gerados na última execução (fixos ou aleatórios)
│   └── \[algoritmo].json        # Saídas de cada algoritmo (gerados sob demanda)
├── gui/             # Scripts Python para visualização dos resultados
│   ├── run.py               # Compila e executa a interface
│   ├── interface.py         # Interface gráfica interativa (Streamlit)
│   └── config.toml          # Configurações de tema para a interface
├── .gitignore       # Arquivos e pastas ignorados pelo Git
└── README.md        # Este arquivo de documentação

````

---

## **Dependências**

**C:**
- [gcc](https://gcc.gnu.org/) (para compilação dos códigos C)

**Python:**
- Python 3.8 ou superior recomendado
- Bibliotecas:
  - `streamlit`
  - `pandas`
  - `plotly`

Instale tudo facilmente usando o pip:

```bash
pip install streamlit pandas plotly
````

### Ambiente recomendado

Crie e ative um ambiente virtual para evitar conflitos de dependências:

```bash
python3 -m venv venv
source venv/bin/activate      # No Linux/Mac
venv\Scripts\activate         # No Windows
pip install streamlit pandas plotly
```

---

## **Como compilar e executar**

1. **Compile o projeto em C:**

   O script Python já faz a compilação, mas se quiser manualmente:

   ```bash
   gcc -Isrc src/main.c src/escalonador.c src/algoritmos.c src/modulo1.c src/exporta_json.c -o build/simulador
   ```

2. **Execute a interface gráfica:**

   ```bash
   python gui/run.py
   ```

   Ou execute direto a interface (caso o binário já esteja compilado):

   ```bash
   streamlit run gui/interface.py
   ```

---

## **Notas sobre a interface**

* **interface.py:** Permite escolher geração fixa ou aleatória dos processos e selecionar quais algoritmos simular. Resultados aparecem na própria interface.
* **config.toml:** Permite customizar cores e layout do Streamlit.
* Os arquivos `.json` de resultados são criados/atualizados sob demanda na pasta `results/`.
* O modo "aleatório" agora requer clicar no botão "Gerar Novos Aleatórios" para mudar os dados; assim, você pode testar vários algoritmos sobre os mesmos processos.

---

## **Observações**

* O projeto cria automaticamente as pastas `build/` e `results/` se não existirem.
* Apenas os arquivos de código-fonte, scripts e resultados são versionados no Git; binários e arquivos temporários são ignorados.
* Para evitar poluição, os arquivos `.json` de resultados antigos são removidos sempre que um novo conjunto de processos é gerado.

---

## **Autores**
* Anderson Moura Costa do Nascimento (555856)
* João Rodrigo Ferreira do Nascimento (553663)
* Lucas de Oliveira Sobral (556944)
* Osvaldo Medeiros Cavalcante Neto (554693)
---
