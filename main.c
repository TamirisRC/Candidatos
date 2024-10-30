#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef int **Preferencias;

bool verificaPreferenciaSetor(int n, Preferencias preferenciasCandidatos, int candidato, int novoSetor, int setorAtual)
{
    for (int i = 0; i < n; i++)
    {
        if (preferenciasCandidatos[candidato][i] == novoSetor)
        {
            return true;
        }
        if (preferenciasCandidatos[candidato][i] == setorAtual)
        {
            return false;
        }
    }
    return false;
}

bool verificaPreferenciaCandidato(int n, Preferencias preferenciasSetores, int setor, int novoCandidato, int candidatoAtual)
{
    for (int i = 0; i < n; i++)
    {
        if (preferenciasSetores[setor][i] == novoCandidato) 
        {
            return true;
        }
        if (preferenciasSetores[setor][i] == candidatoAtual)
        {
            return false;
        }
    }
    return false;
}

int *algoritmoGaleShapleyCandidatos(int n, Preferencias preferenciasCandidatos, Preferencias preferenciasSetores)
{
    int *emparelhamentoSetores = (int *)malloc(n * sizeof(int));
    int *proximoSetor = (int *)malloc(n * sizeof(int));
    bool *emparelhadoCandidato = (bool *)malloc(n * sizeof(bool));

    for (int i = 0; i < n; i++)
    {
        emparelhamentoSetores[i] = -1;
        emparelhadoCandidato[i] = false;
        proximoSetor[i] = 0;
    }

    int candidatosSemSetor = n;

    while (candidatosSemSetor > 0)
    {
        int candidatoLivre;
        for (candidatoLivre = 0; candidatoLivre < n; candidatoLivre++)
        {
            if (!emparelhadoCandidato[candidatoLivre])
            {
                break;
            }
        }

        int setorPreferido = preferenciasCandidatos[candidatoLivre][proximoSetor[candidatoLivre]];

        if (emparelhamentoSetores[setorPreferido] == -1)
        {
            emparelhamentoSetores[setorPreferido] = candidatoLivre;
            emparelhadoCandidato[candidatoLivre] = true;
            candidatosSemSetor--;
        }
        else
        {
            int candidatoAtual = emparelhamentoSetores[setorPreferido];
            if (verificaPreferenciaCandidato(n, preferenciasSetores, setorPreferido, candidatoLivre, candidatoAtual))
            {
                emparelhadoCandidato[candidatoAtual] = false;
                emparelhamentoSetores[setorPreferido] = candidatoLivre;
                emparelhadoCandidato[candidatoLivre] = true;
            }
        }
        proximoSetor[candidatoLivre]++;
    }

    free(emparelhadoCandidato);
    free(proximoSetor);
    return emparelhamentoSetores;
}

int *algoritmoGaleShapleySetores(int n, Preferencias preferenciasCandidatos, Preferencias preferenciasSetores)
{
    int *emparelhamentoCandidatos = (int *)malloc(n * sizeof(int));
    int *proximoCandidato = (int *)malloc(n * sizeof(int));
    bool *emparelhadoSetor = (bool *)malloc(n * sizeof(bool));

    for (int i = 0; i < n; i++)
    {
        emparelhamentoCandidatos[i] = -1;
        emparelhadoSetor[i] = false;
        proximoCandidato[i] = 0;
    }

    int setoresSemCandidato = n;

    while (setoresSemCandidato > 0)
    {
        int setorLivre;
        for (setorLivre = 0; setorLivre < n; setorLivre++)
        {
            if (!emparelhadoSetor[setorLivre])
            {
                break;
            }
        }

        int candidatoPreferido = preferenciasSetores[setorLivre][proximoCandidato[setorLivre]];

        if (emparelhamentoCandidatos[candidatoPreferido] == -1)
        {
            emparelhamentoCandidatos[candidatoPreferido] = setorLivre;
            emparelhadoSetor[setorLivre] = true;
            setoresSemCandidato--;
        }
        else
        {
            int setorAtual = emparelhamentoCandidatos[candidatoPreferido];
            if (verificaPreferenciaSetor(n, preferenciasCandidatos, candidatoPreferido, setorLivre, setorAtual))
            {
                emparelhadoSetor[setorAtual] = false;
                emparelhamentoCandidatos[candidatoPreferido] = setorLivre;
                emparelhadoSetor[setorLivre] = true;
            }
        }
        proximoCandidato[setorLivre]++;
    }
    free(emparelhadoSetor);
    free(proximoCandidato);
    return emparelhamentoCandidatos;
}

Preferencias criaMatriz(int n)
{
    Preferencias matriz = (Preferencias)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        matriz[i] = (int *)malloc(n * sizeof(int));
    }
    return matriz;
}

int main()
{
    FILE *entrada = fopen("entrada.txt", "r");
    FILE *saida = fopen("saida.txt", "w");

    if (entrada == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int tamanho;
    fscanf(entrada, "%d\n", &tamanho);

    Preferencias preferenciasCandidatos = criaMatriz(tamanho);
    Preferencias preferenciasSetores = criaMatriz(tamanho);

    for (int i = 0; i < tamanho; i++)
    {
        for (int j = 0; j < tamanho; j++)
        {
            fscanf(entrada, "%d", &preferenciasCandidatos[i][j]);
        }
    }

    for (int i = 0; i < tamanho; i++)
    {
        for (int j = 0; j < tamanho; j++)
        {
            fscanf(entrada, "%d", &preferenciasSetores[i][j]);
        }
    }

    int *resultadoSetores = algoritmoGaleShapleySetores(tamanho, preferenciasCandidatos, preferenciasSetores);

    fprintf(saida, "Emparelhamento ótimo para os setores:\n");
    for (int i = 0; i < tamanho; i++)
    {
        fprintf(saida, "%d %d\n", i + 1, resultadoSetores[i] + 1);
    }
    fprintf(saida, "\n");

    int *resultadoCandidatos = algoritmoGaleShapleyCandidatos(tamanho, preferenciasCandidatos, preferenciasSetores);

    fprintf(saida, "Emparelhamento ótimo para os candidatos:\n");
    for (int i = 0; i < tamanho; i++)
    {
        fprintf(saida, "%d %d\n", i + 1, resultadoCandidatos[i] + 1);
    }

    fclose(entrada);
    fclose(saida);
    return 0;
}
