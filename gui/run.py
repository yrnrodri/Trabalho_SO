import subprocess
import sys
import os

# Criar diretórios automaticamente
os.makedirs('build', exist_ok=True)
os.makedirs('results', exist_ok=True)

# Comando para compilar o projeto C
compile_cmd = [
    'gcc',
    '-Isrc',
    'src/main.c',
    'src/escalonador.c',
    'src/algoritmos.c',
    'src/modulo1.c',
    'src/exporta_json.c',
    'src/FilaCircular.c',
    '-o', 'build/simulador'
]

# Compilando
print('Compilando...')
comp = subprocess.run(compile_cmd)
if comp.returncode != 0:
    print('Erro na compilação!')
    sys.exit(1)

# Não executa o binário C, apenas inicia a interface gráfica
print('Abrindo interface gráfica...')
subprocess.run(['streamlit', 'run', 'gui/interface.py'])
