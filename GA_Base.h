#ifndef GA_BASE_H
#define GA_BASE_H
#include "Defines.h"
#include "Codificacao.h"
#include "Comparacao.h"
#include "Scale.h"


template < class Codificacao,
           bool minimizacao = true,
           class Scaling = NoScaling >

class GABase : public Codificacao,
               public Comparacao<minimizacao>,
               public Scaling
{
public:

    using Gene = typename Codificacao::Gene;
    using Populacao = typename Codificacao::Populacao;


    inline double ProbabilidadeCruzamento (double);
    inline double ProbabilidadeMutacao (double);
    inline void ParametrosGA (int, int);

    inline double Fitness (const Gene&);
    std::vector<double> Fitness (const Populacao&);

    inline bool Melhor (double, double);
    inline Gene Melhor (const Gene&, const Gene&);
    inline double Melhor (const vector<double>&);
    inline Gene Melhor (const Populacao&);


public:


    //GABase () : gen(time(0)), rng(0.0, 1.0) {}


    Populacao p;
    Gene melhor;

    double probabilidadeCruzamento = 0.25;
    double probabilidadeMutacao = 0.05;

    int tamanhoPopulacao;
    int maxIteracoes;
    int iteracoes = 0;

//    std::mt19937 gen;
//    std::uniform_real_distribution<double> rng;

    //Rng r;

};

//template <class T, bool B, class U>
//std::mt19937 GABase<T, B, U>::gen = std::mt19937(time(0));

//template <class T, bool B, class U>
//std::uniform_real_distribution<double> GABase<T, B, U>::rng = std::uniform_real_distribution<double>(0.0, 1.0);


template <class T, bool B, class U>
inline double GABase<T, B, U>::ProbabilidadeCruzamento (double probcruzamento_)
{
    return probabilidadeCruzamento = probcruzamento_;
}

template <class T, bool B, class U>
inline double GABase<T, B, U>::ProbabilidadeMutacao (double probmutacao_)
{
    return probabilidadeMutacao = probmutacao_;
}


template <class Codificacao, bool B, class Scaling>
inline double GABase<Codificacao, B, Scaling>::Fitness (const Gene& g)
{
    return Codificacao::Fitness(g);
}

template <class Codificacao, bool B, class Scaling>
std::vector<double> GABase<Codificacao, B, Scaling>::Fitness (const Populacao& p)
{
    std::vector<double> r;

    for(const Gene& g : p)
        r.push_back(Codificacao::Fitness(g));

    Scaling::Scale(r);

    return r;
}


template <class T, bool minimizacao, class U>
inline bool GABase<T, minimizacao, U>::Melhor (double a, double b)
{
    return Comparacao<minimizacao>::Melhor(a, b);
}

template <class Codificacao, bool minimizacao, class U>
inline typename GABase<Codificacao, minimizacao, U>::Gene GABase<Codificacao, minimizacao, U>::Melhor (const Gene& g, const Gene& h)
{
    return Comparacao<minimizacao>::Melhor(g, h, [&](const typename Codificacao::Gene& g) -> double
                                                     { return Codificacao::Fitness(g); });
}

template <class Codificacao, bool minimizacao, class U>
inline double GABase<Codificacao, minimizacao, U>::Melhor (const vector<double>& v)
{
    return *Comparacao<minimizacao>::Melhor(v);
}

template <class Codificacao, bool minimizacao, class U>
inline typename GABase<Codificacao, minimizacao, U>::Gene GABase<Codificacao, minimizacao, U>::Melhor (const Populacao& p)
{
    return Comparacao<minimizacao>::MelhorRange(p.begin(), p.end(), [&](const Gene& g, const Gene& h) -> bool
                                       { return Codificacao::Fitness(g) < Codificacao::Fitness(h); });
}


template <class T, bool B, class U>
inline void GABase<T, B, U>::ParametrosGA (int tampopulacao_, int maxiteracoes_)
{
    tamanhoPopulacao = tampopulacao_;
    maxIteracoes = maxiteracoes_;
}

//===========================================================================================



/*template < class Codificacao,
           bool minimizacao = false,
           class Scaling = NoScaling >

class GABasico : public GABase< Codificacao, minimizacao, Scaling>
{
public:

    using Gene = typename Codificacao::Gene;
    using Populacao = typename Codificacao::Populacao;


    int tamanhoPopulacao = 40;

    int maxIteracoes = 1000;
    int iteracoes = 0;


    inline void ParametrosGA (int, int);

//    void Inicializacao () {}
//    void Selecao () {}
//    void Cruzamento () {}
//    void Mutacao () {}
//    void Parada () {}
//    void Saida () {}


    Populacao p;

    Gene melhor;

};


template <class T, bool B, class U>
inline void GABasico<T, B, U>::ParametrosGA (int tampopulacao_, int maxiteracoes_)
{
    tamanhoPopulacao = tampopulacao_;
    maxIteracoes = maxiteracoes_;
}*/


#endif // GA_BASE_H
