#ifndef SAIDA_H
#define SAIDA_H
#include "GA_Base.h"


template <class GA>
class SaidaPadrao : public GA
{
public:

    inline void Saida ()
    {
        std::cout << "Iteracao " << GA::iteracoes << ". Melhor individuo:  "
                  << GA::Fitness(GA::Melhor(GA::p)) << ":\n";

        for(const typename GA::Gene& g : GA::p)
        {
            for(int i = 0; i < GA::numeroVariaveis; i++)
                cout << GA::Codificacao(g, i) << "  ";

            cout << "   " << GA::Fitness(g) << endl;
        }

        cout << endl;
    }
};


template <class GA>
struct SaidaSimples : public GA
{
    inline void Saida ()
    {
        if(GA::iteracoes % 100 == 0 && GA::iteracoes > 0)
            cout << GA::iteracoes  << "    " << GA::Fitness(GA::melhor) << endl;
    }
};


#endif // SAIDA_H
