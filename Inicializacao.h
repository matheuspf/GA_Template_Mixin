#ifndef INICIALIZACAO_H
#define INICIALIZACAO_H
#include "GA_Base.h"


namespace Impl
{

template <class, class>
class InicializacaoPadrao;


template <class GA>
class InicializacaoPadrao<GA, CodificacaoBinaria<>::TipoGene> : public GA
{
public:

    USING(GA);

    void Inicializacao ()
    {
        p = Populacao(GA::tamanhoPopulacao, Gene(GA::tamanhoGene));

        for(Gene& g : p)
            for(auto& c : g)
                c = rand() & 1;
    }
};


template <class GA>
class InicializacaoPadrao<GA, CodificacaoReal<>::TipoGene> : public GA
{
public:

    USING(GA);

    void Inicializacao ()
    {
        static mt19937 gen(time(0));
        static uniform_real_distribution<double> rng(GA::limiteInferior, GA::limiteSuperior);

        p = Populacao(GA::tamanhoPopulacao, Gene(GA::tamanhoGene));

        for(Gene& g : GA::p)
            for(double& d : g)
                d = rng(gen);
    }
};


template <class GA>
class InicializacaoPadrao<GA, CodificacaoInteira<>::TipoGene> : public GA
{
public:

    USING(GA);

    void Inicializacao ()
    {
        static mt19937 gen(time(0));
        static uniform_int_distribution<uint> rng(0, pow(2, GA::tamanhoVariaveis));

        p = Populacao(GA::tamanhoPopulacao, Gene(GA::tamanhoGene));

        for(Gene& g : GA::p)
            for(uint& d : g)
                d = rng(gen);
    }
};

} //Impl


template <class GA>
using InicializacaoPadrao = Impl::InicializacaoPadrao<GA, typename GA::TipoGene>;


#endif // INICIALIZACAO_H
