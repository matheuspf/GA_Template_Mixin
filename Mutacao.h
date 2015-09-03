#ifndef MUTACAO_H
#define MUTACAO_H
#include "GA_Base.h"


namespace Impl
{

template <class, bool>
struct FuncaoMutacao;

template <class MutacaoTipo>
struct FuncaoMutacao<MutacaoTipo, true> : public MutacaoTipo
{
    inline void Funcao (typename MutacaoTipo::Gene& g)
    {
        for(int i = 0; i < MutacaoTipo::numeroVariaveis; ++i)
            MutacaoTipo::Mutacao(g[i]);
    }
};

template <class MutacaoTipo>
struct FuncaoMutacao<MutacaoTipo, false> : public MutacaoTipo
{
    inline void Funcao (typename MutacaoTipo::Gene& g)
    {
        MutacaoTipo::Mutacao(g[RAND(MutacaoTipo::numeroVariaveis)]);
    }
};

}

template <class GA, class MutacaoTipo, bool GeneCompleto>
class MutacaoBase : public Impl::FuncaoMutacao<MutacaoTipo, GeneCompleto>
{
public:

    USING(GA);

    using Impl::FuncaoMutacao<MutacaoTipo, GeneCompleto>::Funcao;


    inline void Mutacao ()
    {
        for(Gene& g : p) if(PROBABILIDADE(GA::probabilidadeMutacao))
        {
            Funcao(g);
        }
    }
};

//==================================================================================


namespace Impl
{

template <class, class>
class MutacaoSimples;


/*template <class GA>
class MutacaoSimples<GA, CodificacaoBinaria<>::TipoGene> : public GA
{
public:

    void Mutacao ()
    {
        for(typename GA::Gene& g : GA::p)
        for(int i = 0; i < GA::tamanhoGene; ++i)
        if(PROBABILIDADE(GA::probabilidadeMutacao))
            g[i] = !g[i];
    }
};*/


template <class GA>
class MutacaoSimples<GA, CodificacaoReal<>::TipoGene> : public GA
{
public:

    std::mt19937 gen;
    std::uniform_real_distribution<double> rng;

    MutacaoSimples () : gen(time(0)), rng(GA::limiteInferior, GA::limiteSuperior) {}


    //template <class T>
    inline void Mutacao (double& g)
    {
        g = rng(gen);
    }
};


template <class GA>
class MutacaoSimples<GA, CodificacaoInteira<>::TipoGene> : public GA
{
public:

    std::mt19937 gen;
    std::uniform_int_distribution<uint> rng;

    MutacaoSimples () : gen(time(0)), rng(0, pow(2, GA::tamanhoVariaveis) - 1) {}


    //template <class T>
    inline void Mutacao (uint& g)
    {
        /*for(typename GA::Gene& g : GA::p)
        for(int i = 0; i < GA::tamanhoGene; ++i)
        if(PROBABILIDADE(GA::probabilidadeMutacao))
            g[i / GA::tamanhoVariaveis] ^= 1 << (i % GA::tamanhoVariaveis);*/

        g = rng(gen);
    }

};

} //Impl

template <class GA, bool GeneCompleto = false>
using MutacaoSimples = MutacaoBase<GA, Impl::MutacaoSimples<GA, typename GA::TipoGene>, GeneCompleto>;

template <class GA>
using MutacaoSimplesCompleta = MutacaoSimples<GA, true>;

template <class GA>
using MutacaoSimplesUnica = MutacaoSimples<GA, false>;

//=================================================================


namespace Impl
{

template <class, class>
class MutacaoNaoUniforme;

/*template <class GA>
class MutacaoNaoUniforme<GA, CodificacaoBinaria<>::TipoGene> : public GA
{
public:

    void Mutacao ()
    {
        for(typename GA::Gene& g : GA::p)
        for(int i = 0; i < GA::tamanhoGene; i += GA::tamanhoVariaveis)
        for(int j = 0; j < GA::tamanhoVariaveis; ++j)
        if(PROBABILIDADE(GA::probabilidadeMutacao))
        {
            int pos = Funcao(GA::tamanhoVariaveis) + i;

            if(rand() & 1 && pos < GA::tamanhoGene)
                ++pos;

            g[pos] = !g[pos];
        }
    }

    inline double Funcao (double x)
    {
        static const double r = GA::rng(GA::gen);
        static const double b = 1.5;

        return x * (1.0 - pow(r, pow(1.0 - double(GA::iteracoes) / double(GA::maxIteracoes), b)));
    }
};*/


template <class GA>
class MutacaoNaoUniforme<GA, CodificacaoReal<>::TipoGene> : public GA
{
public:

    //template <class T>
    inline void Mutacao (double& g)
    {
        /*for(typename GA::Gene& g : GA::p)
        for(int i = 0; i < GA::tamanhoGene; ++i)
        if(PROBABILIDADE(GA::probabilidadeMutacao))
        {
            g[i] = (rand() & 1) ? g[i] + Funcao(GA::limiteSuperior - g[i]) :
                                  g[i] - Funcao(g[i] - GA::limiteInferior);
        }*/

        g = (rand() & 1) ? g + Funcao(GA::limiteSuperior - g) :
                           g - Funcao(g - GA::limiteInferior);
    }

    inline double Funcao (double x)
    {
        static const double r = RAND0();
        static const double b = 2.0;

        return x * (1.0 - pow(r, pow(1.0 - double(GA::iteracoes) / double(GA::maxIteracoes), b)));
    }
};


template <class GA>
class MutacaoNaoUniforme<GA, CodificacaoInteira<>::TipoGene> : public GA
{
public:

    template <class T>
    inline void Mutacao (T& g)
    {
        /*for(typename GA::Gene& g : GA::p)
        for(int i = 0; i < GA::tamanhoGene; ++i)
        //for(int j = 0; j < GA::tamanhoVariaveis; ++j)
        if(PROBABILIDADE(GA::probabilidadeMutacao))
        {
            int pos = Funcao(GA::tamanhoVariaveis);

            if(rand() & 1 && pos < GA::tamanhoVariaveis) ++pos;

            g[i / GA::tamanhoVariaveis] ^= 1 << pos;
        }*/

        int pos = Funcao(GA::tamanhoVariaveis);

        if(rand() & 1 && pos < GA::tamanhoVariaveis) ++pos;

        g ^= 1 << pos;
    }

    inline double Funcao (double x)
    {
        static const double r = RAND0();
        static const double b = 1.5;

        return x * (1.0 - pow(r, pow(1.0 - double(GA::iteracoes) / double(GA::maxIteracoes), b)));
    }
};

} //Impl


template <class GA, bool GeneCompleto = false>
using MutacaoNaoUniforme = MutacaoBase<GA, Impl::MutacaoNaoUniforme<GA, typename GA::TipoGene>, GeneCompleto>;

template <class GA>
using MutacaoNaoUniformeCompleta = MutacaoNaoUniforme<GA, true>;

template <class GA>
using MutacaoNaoUniformeUnica = MutacaoNaoUniforme<GA, false>;


////===========================================================================================

template <class GA, template <class> class ...Mutacoes>
class MutacaoMultipla : public Impl::OperadorMultiplo<GA, Mutacoes...>
{
public:

    using Impl::OperadorMultiplo<GA, Mutacoes...>::link;


    inline void Mutacao ()
    {
        Iter(&link, RAND(sizeof...(Mutacoes<GA>) + 1));
    }


    template <class U>
    void Iter (U* u, int pos)
    {
        if(pos) Iter(u->prox, pos-1);

        else U::Tipo::Mutacao();
    }

    void Iter (Impl::NullType*, int) {}
};

#endif // MUTACAO_H
