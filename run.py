import subprocess
import sys
import os

# Defina o nome do executável conforme sistema operacional
exe = 'simulador.exe' if sys.platform.startswith('win') else './simulador'

# Comando para compilar
compile_cmd = [
    'gcc',
    'main.c',
    'escalonador.c',
    'algoritmos.c',
    'modulo1.c',
    '-o', 'simulador'
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
