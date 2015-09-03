#include <iostream>
#include <typeinfo>
#include <array>
#include "GA.h"


using namespace std;



template <class Cod>
using BaseGA = InicializacaoPadrao < SelecaoUniversal < ParadaPadrao < SaidaSimples < GABase < FitnessF3 < Cod >, true > > > > >;


//using GABinario = GASimples < CruzamentoMultiPontos < MutacaoSimples <  BaseGA < CodificacaoBinaria<> > > > >;

using GAInteiro = GASimples < CruzamentoMultiPontos < MutacaoSimples <  BaseGA < CodificacaoInteira<> > > > >;


template <class T>
using CruzamentosReais = CruzamentoMultiplo<T, CruzamentoSimplesV2, CruzamentoAritimeticoUnico, CruzamentoAritimeticoCompleto>;

template <class T>
using MutacoesReais = MutacaoMultipla<T, MutacaoSimplesUnica, MutacaoNaoUniformeUnica, MutacaoNaoUniformeCompleta>;

using GAReal = GASimples < MutacoesReais < CruzamentosReais < BaseGA < CodificacaoReal<> > > > > ;



#define TESTE

int main()
{
    srand(time(0));

    GAReal ga(70, 40000);
    ga.ProbabilidadeCruzamento(0.3);
    ga.ProbabilidadeMutacao(0.2);


//    GAReal ga(60, 20000);
//    ga.ProbabilidadeCruzamento(0.25);
//    ga.ProbabilidadeMutacao(0.045 * ga.numeroVariaveis);

//    GABinario ga(60, 20000);
//    ga.ProbabilidadeCruzamento(0.25);
//    ga.ProbabilidadeMutacao(0.00098);

//    GAInteiro ga(60, 20000);
//    ga.ProbabilidadeCruzamento(0.25);
//    ga.ProbabilidadeMutacao(0.0015);

#ifdef TESTE

    cout << Benchmark ( [&ga](){ ga.Evoluir(); } ) << "\n\n";

    cout << ga.Fitness(ga.melhor) << "\n\n";

    for(auto g : ga.p)
        cout << ga.Fitness(g) << endl;
    cout << endl;

    vector<double> x = ga.Fitness(ga.p);
    double y = accumulate(x.begin(), x.end(), 0.0);

    cout << y / ga.tamanhoPopulacao << endl;

#else

    double r = 0;
    int n = 10;

    for(int i = 0; i < n; i++)
    {
        ga.Evoluir();
        r += ga.Fitness(ga.melhor);
        ga.Iniciar();
    }

    cout << r / n << endl;

#endif

    return 0;
}
