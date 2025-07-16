import os, json, subprocess, sys
import streamlit as st
import pandas as pd
import plotly.express as px

# --------------------------------------------------------------------------------------
# Config inicial
# --------------------------------------------------------------------------------------
st.set_page_config(
    page_title="Simulador de Escalonamento de Processos",
    layout="wide",
    page_icon="⚙️",
)

# Caminho do executável C
EXE = "build/simulador.exe" if sys.platform.startswith("win") else "./build/simulador"

RESULTS_DIR = "results"
DADOS_PATH  = os.path.join(RESULTS_DIR, "dados_originais.json")

os.makedirs(RESULTS_DIR, exist_ok=True)  # garante diretório


# --------------------------------------------------------------------------------------
# Limpeza de arquivos
# --------------------------------------------------------------------------------------
def limpar_tudo():
    """Remove TODOS os JSONs (inclusive dados_originais)."""
    for f in os.listdir(RESULTS_DIR):
        if f.endswith(".json"):
            os.remove(os.path.join(RESULTS_DIR, f))

def limpar_resultados():
    """Remove APENAS os resultados de algoritmos; preserva dados_originais."""
    for f in os.listdir(RESULTS_DIR):
        if f.endswith(".json") and f != "dados_originais.json":
            os.remove(os.path.join(RESULTS_DIR, f))


# --------------------------------------------------------------------------------------
# Estado compartilhado
# --------------------------------------------------------------------------------------
if "processos_gerados" not in st.session_state:
    st.session_state.processos_gerados = False

if "manual_df" not in st.session_state:
    # dataframe default de 3 processos
    st.session_state.manual_df = pd.DataFrame(
        [
            {"pid": 1, "chegada": 0, "execucao": 1, "prioridade": 1},
            {"pid": 2, "chegada": 1, "execucao": 2, "prioridade": 1},
            {"pid": 3, "chegada": 2, "execucao": 1, "prioridade": 1},
        ]
    )


# --------------------------------------------------------------------------------------
# Sidebar
# --------------------------------------------------------------------------------------
st.sidebar.title("Configurações")

modo = st.sidebar.radio(
    "Modo de geração de processos:",
    options=["Nenhum", "Fixos", "Aleatórios", "Manual"],
    index=0,
)

# --------------------------------------------------------------------------------------
# Modo Nenhum
# --------------------------------------------------------------------------------------
if modo == "Nenhum":
    limpar_tudo()
    st.session_state.processos_gerados = False
    st.sidebar.info("Selecione uma opção para gerar processos.")


# --------------------------------------------------------------------------------------
# Modo Fixos
# --------------------------------------------------------------------------------------
elif modo == "Fixos":
    if st.sidebar.button("Gerar processos fixos", use_container_width=True):
        limpar_resultados()
        subprocess.run([EXE, "--fixed"], check=True)
        st.sidebar.success("Processos fixos gerados!")
        st.session_state.processos_gerados = True
    elif os.path.exists(DADOS_PATH):
        st.session_state.processos_gerados = True


# --------------------------------------------------------------------------------------
# Modo Aleatórios
# --------------------------------------------------------------------------------------
elif modo == "Aleatórios":
    if st.sidebar.button("Gerar novos aleatórios", use_container_width=True):
        limpar_resultados()
        subprocess.run([EXE, "--random"], check=True)
        st.sidebar.success("Novos processos aleatórios gerados!")
        st.session_state.processos_gerados = True
    elif os.path.exists(DADOS_PATH):
        st.session_state.processos_gerados = True


# --------------------------------------------------------------------------------------
# Modo Manual  (USANDO st.data_editor)
# --------------------------------------------------------------------------------------
elif modo == "Manual":
    st.sidebar.info("Edite a tabela e clique em **Salvar dados manuais**.")

    st.markdown("### Processos (edição manual)")
    st.write(
        "Edite os campos. Para adicionar/remover linhas, use os controles abaixo da tabela "
        "(num_rows='dynamic'). PID será renumerado automaticamente ao salvar."
    )

    # Permite edição dinâmica. O usuário pode adicionar ou remover linhas.
    edited_df = st.data_editor(
        st.session_state.manual_df,
        num_rows="dynamic",
        use_container_width=True,
        hide_index=True,
        column_config={
            "pid": st.column_config.NumberColumn("PID", disabled=True, help="Gerado automaticamente."),
            "chegada": st.column_config.NumberColumn("Chegada", min_value=0, step=1),
            "execucao": st.column_config.NumberColumn("Execução", min_value=1, step=1),
            "prioridade": st.column_config.NumberColumn("Prioridade", min_value=1, step=1),
        },
        key="data_editor_manual",
    )

    # Botão salvar
    if st.button("Salvar dados manuais"):
        # Normaliza: remove linhas completamente vazias
        df = edited_df.copy().dropna(how="all")

        # Preenche defaults para campos faltando
        for col, default in (("chegada", 0), ("execucao", 1), ("prioridade", 1)):
            df[col] = df[col].fillna(default)
        # Converte e corrige limites
        df["chegada"]   = df["chegada"].astype(int).clip(lower=0)
        df["execucao"]  = df["execucao"].astype(int).clip(lower=1)
        df["prioridade"]= df["prioridade"].astype(int).clip(lower=1)

        # Renumera pid sequencialmente
        df["pid"] = range(1, len(df) + 1)

        # Atualiza session_state
        st.session_state.manual_df = df

        # Salva JSON
        procs = df.to_dict(orient="records")
        with open(DADOS_PATH, "w") as f:
            json.dump({"processos": procs}, f, indent=2)

        # Limpa resultados de algos (assim forçamos novo cálculo)
        limpar_resultados()
        st.session_state.processos_gerados = True
        st.success("Dados manuais salvos!")


# --------------------------------------------------------------------------------------
# Título principal
# --------------------------------------------------------------------------------------
st.title("Simulador de Escalonamento de Processos")


# --------------------------------------------------------------------------------------
# Exibir processos iniciais (se houver)
# --------------------------------------------------------------------------------------
if st.session_state.processos_gerados and os.path.exists(DADOS_PATH):
    with open(DADOS_PATH) as f:
        orig_data = json.load(f)
    df_iniciais = pd.DataFrame(orig_data["processos"]).astype(
        {"pid": int, "chegada": int, "execucao": int, "prioridade": int}
    )
    st.subheader("Processos Gerados (Iniciais)")
    st.dataframe(
        df_iniciais.set_index("pid")[["chegada", "execucao", "prioridade"]],
        use_container_width=True,
    )
else:
    st.info("Gere os processos para visualizar a tabela inicial.")
    df_iniciais = None  # usado mais tarde


# --------------------------------------------------------------------------------------
# Seleção de algoritmos
# --------------------------------------------------------------------------------------
algos = st.sidebar.multiselect(
    "Selecione um ou mais algoritmos:",
    options=["FIFO", "SJF", "PRIORIDADE", "LOTTERY", "RR", "SRTN"],
)

# Paleta cores por PID (para Gantt)
COLOR_MAP = {
    1: "#3366cc", 2: "#dc3912", 3: "#ff9900", 4: "#109618",
    5: "#990099", 6: "#0099c6", 7: "#dd4477", 8: "#66aa00",
    9: "#b82e2e", 10: "#316395",
}

# Paleta cores por Algoritmo (para comparação de turnaround)
ALGO_COLORS = {
    "FIFO": "#1f77b4",
    "SJF": "#ff7f0e",
    "PRIORIDADE": "#2ca02c",
    "LOTTERY": "#d62728",
}


# --------------------------------------------------------------------------------------
# Rodar algoritmos e exibir resultados
# --------------------------------------------------------------------------------------
comparativos = []  # <- vamos acumular turnaround aqui

if st.session_state.processos_gerados and df_iniciais is not None and algos:
    for algo in algos:
        res_path = os.path.join(RESULTS_DIR, f"{algo.lower()}.json")

        # Gerar se ainda não existir
        if not os.path.exists(res_path):
            cmd = None
            if modo == "Fixos":
                cmd = [EXE, "--fixed", algo]
            elif modo == "Aleatórios":
                cmd = [EXE, "--random", algo]
            elif modo == "Manual":
                cmd = [EXE, "--load", algo]

            if cmd:
                try:
                    subprocess.run(cmd, check=True)
                except subprocess.CalledProcessError as e:
                    st.error(f"Erro ao executar {algo}: {e}")
                    continue

        # Conferir se agora existe
        if not os.path.exists(res_path):
            st.error(f"Arquivo não encontrado: {res_path}")
            continue

        # Carregar resultado
        with open(res_path) as f:
            data = json.load(f)
        df_res = pd.DataFrame(data["processos"])

        # Acumula comparativo (turnaround)
        # garante colunas esperadas
        if {"pid", "turnaround"}.issubset(df_res.columns):
            for _, row in df_res.iterrows():
                comparativos.append(
                    {"pid": int(row["pid"]), "algoritmo": algo, "turnaround": float(row["turnaround"])}
                )

        st.subheader(f"Resultado: {algo}")
        st.dataframe(df_res.set_index("pid"), use_container_width=True)

        # --- Gantt ---
        timeline = data.get("timeline", [])
        if not timeline:
            st.info("Nenhuma linha do tempo disponível para este algoritmo.")
            continue

        # Converte timeline (lista de PIDs a cada tick) em segmentos start/end
        segments = []
        current = timeline[0]
        start = 0
        for i, pid in enumerate(timeline):
            if pid != current:
                segments.append({"pid": current, "start": start, "end": i})
                current = pid
                start = i
        segments.append({"pid": current, "start": start, "end": len(timeline)})

        gantt_df = pd.DataFrame(segments)
        gantt_df["Processo"] = gantt_df["pid"].astype(str)
        gantt_df["duration"] = gantt_df["end"] - gantt_df["start"]

        # Ordenação consistente no eixo Y (1..N) com base nos processos iniciais
        todos_pids = df_iniciais["pid"].astype(str).tolist()
        todos_pids.sort(key=int)

        fig = px.bar(
            gantt_df,
            x="duration",
            y="Processo",
            base="start",
            orientation="h",
            color="Processo",
            color_discrete_map={str(pid): COLOR_MAP.get(pid, "#888888") for pid in gantt_df["pid"].unique()},
            title=f"Gantt: {algo}",
            category_orders={"Processo": todos_pids},
        )
        fig.update_yaxes(autorange="reversed", title="Processo")
        fig.update_xaxes(title="Tempo (ticks)", range=[0, gantt_df["end"].max()])
        fig.update_layout(showlegend=False, height=320)
        st.plotly_chart(fig, use_container_width=True)


# --------------------------------------------------------------------------------------
# Comparação de TURNAROUND por PID (aparece no fim da página)
# --------------------------------------------------------------------------------------
if comparativos:
    st.markdown("---")
    st.header("Comparação de Turnaround por Processo")

    comp_df = pd.DataFrame(comparativos)

    # tabela pivô: linhas = pid, colunas = algoritmos
    pivot_df = (
        comp_df
        .pivot_table(index="pid", columns="algoritmo", values="turnaround", aggfunc="first")
        .reindex(sorted(comp_df["pid"].unique()))  # ordena pid crescente
        .reindex(columns=algos)  # mantém ordem selecionada na UI
    )

    st.subheader("Tabela comparativa (Turnaround)")
    st.dataframe(pivot_df, use_container_width=True)

    # média por algoritmo
    medias_df = comp_df.groupby("algoritmo", as_index=False)["turnaround"].mean()
    medias_df = medias_df[medias_df["algoritmo"].isin(algos)]  # garante ordem
    medias_df["turnaround"] = medias_df["turnaround"].round(2)
    st.subheader("Média de Turnaround por Algoritmo")
    st.dataframe(medias_df.set_index("algoritmo"), use_container_width=True)

    # gráfico barras agrupadas (turnaround por pid x algoritmo)
    comp_df["pid_str"] = comp_df["pid"].astype(str)
    fig_comp = px.bar(
        comp_df,
        x="pid_str",
        y="turnaround",
        color="algoritmo",
        barmode="group",
        category_orders={"pid_str": sorted(comp_df["pid_str"].unique(), key=int),
                         "algoritmo": algos},
        color_discrete_map=ALGO_COLORS,
        title="Turnaround por PID (Comparativo)",
        labels={"pid_str": "PID", "turnaround": "Turnaround (ticks)", "algoritmo": "Algoritmo"},
    )
    fig_comp.update_layout(height=400)
    st.plotly_chart(fig_comp, use_container_width=True)
