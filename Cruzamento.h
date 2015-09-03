#ifndef CRUZAMENTO_H
#define CRUZAMENTO_H
#include "GA_Base.h"
#define MASK(x, n) (x & ((1 << n) - 1))

namespace Impl
{

template <class, class, bool>
struct FuncaoCruzamento;

template <class GA, class CruzamentoTipo>
struct FuncaoCruzamento<GA, CruzamentoTipo, true> : public CruzamentoTipo
{
    using Gene = typename GA::Gene;

    inline void Funcao (const Gene& g, const Gene& h, Gene& fg, Gene& fh)
    {
        for(int i = 0; i < GA::numeroVariaveis; ++i)
            CruzamentoTipo::Cruzamento(g[i], h[i], fg[i], fh[i]);
    }
};

template <class GA, class CruzamentoTipo>
struct FuncaoCruzamento<GA, CruzamentoTipo, false> : public CruzamentoTipo
{
    using Gene = typename GA::Gene;

    inline void Funcao (const Gene& g, const Gene& h, Gene& fg, Gene& fh)
    {
        int i = RAND(GA::numeroVariaveis);

        CruzamentoTipo::Cruzamento(g[i], h[i], fg[i], fh[i]);
    }
};

}

template <class GA, class CruzamentoTipo, bool GeneCompleto>
class CruzamentoBase : public Impl::FuncaoCruzamento<GA, CruzamentoTipo, GeneCompleto>
{
public:

    USING(GA);

    using Impl::FuncaoCruzamento<GA, CruzamentoTipo, GeneCompleto>::Funcao;


    void Cruzamento ()
    {
        Populacao cruzamento;

        for(const Gene& g : p) if(PROBABILIDADE(GA::probabilidadeCruzamento))
        {
            const Gene& h = p[RAND(p.size())];
            Gene fg(g), fh(h);

            Funcao(g, h, fg, fh);

            cruzamento.push_back(std::move(fg));
            cruzamento.push_back(std::move(fh));
        }

        std::move(cruzamento.begin(), cruzamento.end(), std::inserter(p, p.end()));
    }
};


//==========================================================================================
namespace Impl
{

template <class, class>
class CruzamentoSimples;

//template <class GA, bool GeneCompleto>
//class CruzamentoSimples<GA, GeneCompleto, CodificacaoBinaria<>::TipoGene> : public GA
//{
//public:

//    USING(GA);

//    void Cruzamento ()
//    {
//        Populacao cruzamento;

//        for(const Gene& g : p) if(PROBABILIDADE(GA::probabilidadeCruzamento))
//        {
//            const Gene& h = p[RAND(p.size())];

//            Gene fg(g), fh(h);

//            for(int i = 0; i < GA::tamanhoGene; i += GA::tamanhoVariaveis)
//            {
//                int pos = RANDI(i, i + GA::tamanhoVariaveis);

//                for(int j = i; j < pos; ++j)
//                {
//                    fg[j] = h[j];
//                    fh[j] = g[j];
//                }
//            }

//            cruzamento.push_back(std::move(fg));
//            cruzamento.push_back(std::move(fh));
//        }

//        p.insert(p.end(), cruzamento.begin(), cruzamento.end());
//    }
//};


template <class GA>
class CruzamentoSimples<GA, CodificacaoReal<>::TipoGene> : public GA
{
public:

    using T = typename GA::TipoGene;


    static const int precisaoMinima = -3;

    void Cruzamento (const T& g, const T& h, T& fg, T& fh)
    {
        int pos = int(log10(max(abs(g), abs(h))));
        pos = (pos <= precisaoMinima) ? precisaoMinima : RANDI(precisaoMinima, pos + 1);
        if(!pos) --pos;

        fg = abs(fg);
        fh = abs(fh);

        double div = pow(10, pos);
        double auxg = fmod(fg, div);
        double auxh = fmod(fh, div);

        fg = (fg - auxg) + auxh;
        fh = (fh - auxh) + auxg;

        fg *= SIGN(g);
        fh *= SIGN(h);
    }
};


template <class GA>
class CruzamentoSimples<GA, CodificacaoInteira<>::TipoGene> : public GA
{
public:

    using T = typename GA::TipoGene;


    void Cruzamento (const T& g, const T& h, T& fg, T& fh)
    {
        int p = 1 + RAND(int(log2(max(abs(g), abs(h)))));

        int auxg = MASK(g, p);
        int auxh = MASK(h, p);

        fg = (fg - auxg) + auxh;
        fh = (fh - auxh) + auxg;
    }
};

} //Impl


template <class GA, bool GeneCompleto = true>
using CruzamentoSimples = CruzamentoBase<GA, Impl::CruzamentoSimples<GA, typename GA::TipoGene>, GeneCompleto>;

template <class GA>
using CruzamentoSimplesCompleto = CruzamentoSimples<GA, true>;

template <class GA>
using CruzamentoSimplesUnico = CruzamentoSimples<GA, false>;



//=========================================================================================

namespace Impl
{

template <class, class>
class CruzamentoMultiPontos;


//template <class GA>
//class CruzamentoMultiPontos<GA, CodificacaoBinaria<>::TipoGene> : public GA
//{
//public:

//    USING(GA);

//    const double probPonto = 0.2;


//    void Cruzamento ()
//    {
//        Populacao cruzamento;

//        for(const Gene& g : p) if(PROBABILIDADE(GA::probabilidadeCruzamento))
//        {
//            const Gene& h = p[RAND(p.size())];
//            Gene fg(g), fh(h);
//            std::vector<int> pontos(1);

//            for(int i = 1; i < GA::tamanhoGene; ++i) if(PROBABILIDADE(probPonto))
//                pontos.push_back(i);

//            for(int i = 0; i < pontos.size() - 1; i += 2)
//            {
//                for(int j = pontos[i], f = pontos[i+1]; j < f; ++j)
//                {
//                    fg[j] = h[j];
//                    fh[j] = g[j];
//                }
//            }

//            cruzamento.push_back(std::move(fg));
//            cruzamento.push_back(std::move(fh));
//        }

//        std::move(cruzamento.begin(), cruzamento.end(), std::inserter(p, p.end()));
//    }
//};


template <class GA>
class CruzamentoMultiPontos<GA, CodificacaoReal<>::TipoGene> : public GA
{
public:

    using T = typename GA::TipoGene;


    const double probabilidadePonto = 0.5;
    static const int maxPrecisao = 10;

    long double auxpow[maxPrecisao * 2];
    long double* pow10;


    CruzamentoMultiPontos ()
    {
        pow10 = auxpow + maxPrecisao;

        for(int i = -maxPrecisao; i < maxPrecisao; ++i)
            pow10[i] = pow(10, i);
    }


    template <class T>
    void Cruzamento (const T& g, const T& h, T& fg, T& fh)
    {
        vector<int> pts = {int(max(log10(abs(g)), log10(abs(h))) + 1)};

        for(int i = pts[0]; i > -maxPrecisao; --i)
            if(PROBABILIDADE(probabilidadePonto) && i)
                pts.push_back(i);


        double v[pts.size()], u[pts.size()];

        for(int i = 0; i < pts.size(); i++)
        {
            v[i] = fmod(g, pow10[pts[i]]);
            u[i] = fmod(h, pow10[pts[i]]);
        }

        for(int i = 0; i < pts.size()-1; i++)
        {
            v[i] -= v[i+1];
            u[i] -= u[i+1];
        }


        int i = 0;
        fg = 0, fh = 0;

        if(pts.size() & 1)
        {
            fg += v[0];
            fh += u[0];
            i = 1;
        }

        for(; i < pts.size(); i+=2)
        {
            fg += (u[i] + v[i+1]);
            fh += (v[i] + u[i+1]);
        }
    }
};


template <class GA>
class CruzamentoMultiPontos<GA, CodificacaoInteira<>::TipoGene> : public GA
{
public:

    using T = typename GA::TipoGene;


    const double probabilidadePonto = 0.3;
    static const int maxPrecisao = 10;
    double pow10[maxPrecisao];


    CruzamentoMultiPontos ()
    {
        for(int i = 0; i < maxPrecisao; ++i)
            pow10[i] = pow(10, i);
    }

    template <class T>
    void Cruzamento (const T& g, const T& h, T& fg, T& fh)
    {
        vector<int> pts = {int(max(log2(g), log2(h))) + 1};

        for(int i = pts.front(); i >= 1; --i)
            if(PROBABILIDADE(probabilidadePonto))
                pts.push_back(i);

        int v[pts.size()], u[pts.size()];

        for(int i = 0; i < pts.size(); ++i)
        {
            v[i] = MASK(g, pts[i]);
            u[i] = MASK(h, pts[i]);
        }

        for(int i = 0; i < pts.size() - 1; ++i)
        {
            u[i] -= u[i+1];
            v[i] -= v[i+1];
        }


        int i = 0;
        fg = 0, fh = 0;

        if(pts.size() & 1)
        {
            fg = v[0];
            fh = u[0];
            i = 1;
        }

        for(; i < pts.size(); i += 2)
        {
            fg += (v[i] + u[i+1]);
            fh += (u[i] + v[i+1]);
        }
    }
};

} //Impl


template <class GA, bool GeneCompleto = true>
using CruzamentoMultiPontos = CruzamentoBase<GA, Impl::CruzamentoMultiPontos<GA, typename GA::TipoGene>, GeneCompleto>;

template <class GA>
using CruzamentoMultiPontosCompleto = CruzamentoMultiPontos<GA, true>;

template <class GA>
using CruzamentoMultiPontosUnico = CruzamentoMultiPontos<GA, false>;




//===============================================================================

namespace Impl
{

template <class GA>
class CruzamentoAritimetico : public GA
{
public:

    using T = typename GA::TipoGene;


    void Cruzamento (const T& g, const T& h, T& fg, T& fh)
    {
        double a = RAND0();

        fg = a * g + (1.0 - a) * h;
        fh = a * h + (1.0 - a) * g;
    }
};

}

template <class GA, bool GeneCompleto = true>
using CruzamentoAritimetico = CruzamentoBase<GA, Impl::CruzamentoAritimetico<GA>, GeneCompleto>;

template <class GA>
using CruzamentoAritimeticoCompleto = CruzamentoAritimetico<GA, true>;

template <class GA>
using CruzamentoAritimeticoUnico = CruzamentoAritimetico<GA, false>;


//=============================================================================================


template <class GA>
class CruzamentoSimplesV2 : public GA
{
public:

    USING(GA);

    void Cruzamento ()
    {
        Populacao cruzamento;

        for(const Gene& g : p) if(PROBABILIDADE(GA::probabilidadeCruzamento))
        {
            int pos = RANDI(1, GA::numeroVariaveis - 1);

            Gene fg(Gene(g.begin(), g.begin() + pos));

            fg.insert(fg.end(), g.begin() + pos, g.end());
        }

        std::move(cruzamento.begin(), cruzamento.end(), std::inserter(p, p.end()));
    }
};



//=============================================================================================

template <class GA, template <class> class ...Cruzamentos>
class CruzamentoMultiplo : public Impl::OperadorMultiplo<GA, Cruzamentos...>
{
public:

    using Impl::OperadorMultiplo<GA, Cruzamentos...>::link;


    inline void Cruzamento ()
    {
        Iter(&link, RAND(sizeof...(Cruzamentos<GA>) + 1));
    }

    template <class U>
    void Iter (U* u, int pos)
    {
        if(pos) Iter(u->prox, pos-1);

        else U::Tipo::Cruzamento();
    }

    void Iter (Impl::NullType*, int) {}
};


#endif // CRUZAMENTO_H
