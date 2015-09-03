#ifndef SELECAO_H
#define SELECAO_H
#include "GA_Base.h"


template <class GA>
class SelecaoPadrao : public GA
{
public:

    using Gene = typename GA::Gene;
    using GA::p;

    void Selecao ()
    {
        sort(p.begin(), p.end(), [&](const Gene& g, const Gene& h) { return GA::Fitness(g) < GA::Fitness(h); });

        p.erase(p.begin() + GA::tamanhoPopulacao, p.end());
    }
};

//======================================================================

class SelecaoProporcional
{
public:

    void Positivo (vector<double>& fitness)
    {
        double menor = *std::min_element(fitness.begin(), fitness.end());

        for(double& n : fitness)
            n = n - menor;
    }

    void Proporcional (vector<double>& fitness)
    {
        Positivo(fitness);
        double t = std::accumulate(fitness.begin(), fitness.end(), 0.0);

        for(double& n : fitness)
            n = n / t;
    }

    template <bool>
    vector<double> Roleta (vector<double>);
};


template <bool minimizacao>
vector<double> SelecaoProporcional::Roleta (vector<double> roleta)
{
    for(double& n : roleta)
        n = 1.0 / n;


    double t = std::accumulate(roleta.begin(), roleta.end(), 0.0);

    for(double& n : roleta)
        n = n / t;

    for(unsigned int i = 1; i < roleta.size(); ++i)
        roleta[i] += roleta[i - 1];

    return roleta;
}

template <>
vector<double> SelecaoProporcional::Roleta<false> (vector<double> roleta)
{
    Proporcional(roleta);

    for(unsigned int i = 1; i < roleta.size(); ++i)
        roleta[i] += roleta[i - 1];

    return roleta;
}


template <class GA>
class SelecaoUniversal : public SelecaoProporcional, public GA
{
public:

    using Gene = typename GA::Gene;
    using Populacao = typename GA::Populacao;
    using GA::p;

    void Selecao ()
    {
        Populacao nova_populacao;
        vector<double> roleta = Roleta<GA::minimizacao>(GA::Fitness(p));

        double passo = 1.0 / GA::tamanhoPopulacao;
        //int i = 0;

        for(double n = RANDF(0.0, passo); n < 1.0; n += passo)
        {
            for(uint i = 0; i < roleta.size(); ++i)
                if(roleta[i] >= n)
                {
                    nova_populacao.push_back(p[i]);
                    break;
                }
        }

        p = std::move(nova_populacao);
    }
};



#endif // SELECAO_H
