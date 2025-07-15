import subprocess
import sys
import os
import json

# Criando diretórios automaticamente
os.makedirs('build', exist_ok=True)
os.makedirs('results', exist_ok=True)

# executável, de acordo com o sistema operacional
exe = 'build/simulador.exe' if sys.platform.startswith('win') else './build/simulador'


# Comando para compilar
compile_cmd = [
    'gcc',
    '-Isrc',
    'src/main.c',
    'src/escalonador.c',
    'src/algoritmos.c',
    'src/modulo1.c',
    'src/exporta_json.c',
    '-o', 'build/simulador'
]

print('Compilando...')
comp = subprocess.run(compile_cmd)
if comp.returncode != 0:
    print('Erro na compilação!')
    sys.exit(1)

print('Executando...')
run = subprocess.run([exe])

if run.returncode == 0:
    print('\nExecução finalizada com sucesso!')
else:
    print('\nErro ao executar o programa!')

# Visualizar os JSON gerados
json_files = ['results/fifo.json', 'results/sjf.json', 'results/prioridade.json', 'results/lottery.json']

for filename in json_files:
    if not os.path.exists(filename):
        print(f'Arquivo {filename} não encontrado!')
        continue

    print(f'\n===== Resultados: {filename} =====')
    with open(filename) as f:
        data = json.load(f)

    print(f"Algoritmo: {data['algoritmo']}")
    print("PID | Espera | Turnaround | Estado final", end='')
    show_tickets = data['algoritmo'].lower() == 'lottery'
    # Apenas se for Lottery!
    if show_tickets:
        print(" | Tickets")
        tickets = data['tickets']
    else:
        print()
    for i, proc in enumerate(data['processos']):
        print(f"{proc['pid']:>2} | {proc['espera']:>6} | {proc['turnaround']:>9} | {proc['estado']:<12}", end='')
        if show_tickets:
            print(f" | {tickets[i]}")
        else:
            print()