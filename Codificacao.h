#ifndef CODIFICACAO_H
#define CODIFICACAO_H
#include "Defines.h"
#include "Representacao.h"


template < class GeneT = GeneBinario,
           class PopulacaoT = PopulacaoBinaria >

class CodificacaoBinaria
{
public:

    using Gene = GeneT;
    using Populacao = PopulacaoT;
    using POW = POW2T<30>::res;

    using TipoGene = typename Gene::value_type;


    double limiteInferior;
    double limiteSuperior;

    int numeroVariaveis;
    int tamanhoVariaveis;
    int tamanhoGene;

    int precisao;


    CodificacaoBinaria (double = -1.0, double = 1.0, int = 1, int = 6);


    double Fitness (const Gene& gene) { return 0.0; }

    double Codificacao (const Gene&, int);
    vector<double> Codificacao (const Gene&);

    inline void Atualizar ();

};


template <class T, class U>
inline CodificacaoBinaria<T, U>::CodificacaoBinaria (double limIferior_, double limSuperior_, int numVariaveis_, int precisao_) :
                    limiteInferior(limIferior_), limiteSuperior(limSuperior_), numeroVariaveis(numVariaveis_), precisao(precisao_)
{
    Atualizar();
}


template <class T, class U>
double CodificacaoBinaria<T, U>::Codificacao (const Gene& g, int pos)
{
    double r = 0.0;
    static double max = (POW::v[tamanhoVariaveis] - 1);
    pos *= tamanhoVariaveis;

    for(int i = pos + tamanhoVariaveis - 1; i >= pos; i--)
        r += g[i] * POW::v[i - pos];

    return (limiteInferior + (r / max) * (limiteSuperior - limiteInferior));
}


template <class T, class U>
vector<double> CodificacaoBinaria<T, U>::Codificacao (const Gene& g)
{
    vector<double> v;

    for(int i = 0; i < numeroVariaveis; ++i)
        v.push_back(Codificacao(g, i));

    return v;
}


template <class T, class U>
inline void CodificacaoBinaria<T, U>::Atualizar ()
{
    tamanhoVariaveis = std::log2((limiteSuperior - limiteInferior) * std::pow(10, precisao) + 1) + 1;
    tamanhoGene = numeroVariaveis * tamanhoVariaveis;
}

//========================================================================================================


template < class GeneT = GeneReal,
           class PopulacaoT = PopulacaoReal >
class CodificacaoReal
{
public:

    using Gene = GeneT;
    using Populacao = PopulacaoT;
    using POW = POW2T<30>::res;

    using TipoGene = typename Gene::value_type;


    double limiteInferior;
    double limiteSuperior;

    int numeroVariaveis;
    int tamanhoGene;


    CodificacaoReal () {}

    CodificacaoReal (double = -1.0, double = 1.0, int = 1);


    double Fitness (const Gene& gene) { return 0.0; }

    inline double Codificacao (const Gene&, int);
    vector<double> Codificacao (Gene);

};




template <class T, class U>
CodificacaoReal<T, U>::CodificacaoReal(double limInferior_, double limSuperior_, int numVariaveis_) :
                                         limiteInferior(limInferior_), limiteSuperior(limSuperior_),
                                         numeroVariaveis(numVariaveis_), tamanhoGene(numVariaveis_)
{
}


template <class T, class U>
inline double CodificacaoReal<T, U>::Codificacao (const Gene& g, int pos)
{
    return g[ pos ];
}

template <class GeneT, class U>
inline vector<double> CodificacaoReal<GeneT, U>::Codificacao (Gene g)
{
    return g;
}


//=========================================================================================

template < class GeneT = GeneInteiro,
           class PopulacaoT = PopulacaoInteira >

class CodificacaoInteira
{
public:

    using Gene = GeneT;
    using Populacao = PopulacaoT;
    using POW = POW2T<30>::res;

    using TipoGene = typename Gene::value_type;


    double limiteInferior;
    double limiteSuperior;

    int numeroVariaveis;
    int precisao;

    int tamanhoVariaveis;
    int tamanhoGene;


    CodificacaoInteira (double = -1.0, double = 1.0, int = 1, int = 6);


    double Fitness (const Gene& gene) { return 0.0; }

    double Codificacao (const Gene&, int);
    vector<double> Codificacao (const Gene&);
};




template <class T, class U>
CodificacaoInteira<T, U>::CodificacaoInteira(double limInferior_, double limSuperior_, int numVariaveis_, int precisao_) :
                                             limiteInferior(limInferior_), limiteSuperior(limSuperior_),
                                             numeroVariaveis(numVariaveis_), precisao(precisao_),
                                             tamanhoVariaveis(std::log2((limiteSuperior - limiteInferior) * std::pow(10, precisao) + 1) + 1),
                                             tamanhoGene(numeroVariaveis * tamanhoVariaveis)
{
}


template <class T, class U>
inline double CodificacaoInteira<T, U>::Codificacao (const Gene& g, int pos)
{
    static const double max = (pow(2, tamanhoVariaveis) - 1);
    static const double dist = (limiteSuperior - limiteInferior);

    return limiteInferior + (double(g[pos]) / max) * dist;
}

template <class T, class U>
inline vector<double> CodificacaoInteira<T, U>::Codificacao (const Gene& g)
{
    vector<double> r(numeroVariaveis);

    for(int i = 0; i < numeroVariaveis; ++i)
        r[i] = Codificacao(g, i);

    return r;
}



//=================================================================================================

/*template < class GeneT = vector<char>,
           class TipoPopulacao = vector<GeneT> >

class CodificacaoBinaria
{
public:

    typedef double (*TipoFitness) (const GeneT&);
    //typedef std::function<double(GeneT)> TipoFitness;

    using Gene = GeneT;
    using Populacao = TipoPopulacao;


    double limiteInferior = -1.0;
    double limiteSuperior = 1.0;
    int precisao = 8;

    int numeroVariaveis = 1;
    int tamanhoVariaveis;
    int tamanhoGene;

    TipoFitness Fitness;

    CodificacaoBinaria ()
    {
        Atualizar();
    }

    void SetFitness (TipoFitness f)
    {
        Fitness = f;
    }

    CodificacaoBinaria (TipoFitness fitness_)
    {
        Fitness = &fitness_;
        Atualizar();
    }

    inline void ParametrosCodificacao (double, double, int = 8, int = 1);

    double Codificacao (const GeneT&, int = 0);


private:

    inline void Atualizar ();

};


template <class T, class U>
inline void CodificacaoBinaria<T, U>::ParametrosCodificacao (double linferior_, double lsuperior_, int precisao_, int numvariaveis_)
{
    limiteInferior = linferior_;
    limiteSuperior = lsuperior_;
    precisao = precisao_;
    numeroVariaveis = numvariaveis_;
    Atualizar();
}


template <class T, class U>
double CodificacaoBinaria<T, U>::Codificacao (const Gene& g, int pos)
{
    double r = 0.0;
    double max = (pow(2, tamanhoVariaveis) - 1);

    for(int i = pos + tamanhoGene - 1; i >= pos; i--)
        r += g[i] * pow(2, i - pos);

    return (limiteInferior + (r / max) * (limiteSuperior - limiteInferior));
}


template <class T, class U>
inline void CodificacaoBinaria<T, U>::Atualizar ()
{
    tamanhoVariaveis = std::log2((limiteSuperior - limiteInferior) * std::pow(10, precisao) + 1) + 1;
    tamanhoGene = numeroVariaveis * tamanhoVariaveis;
}*/



//=============================================================================================================


#endif // CODIFICACAO_H
