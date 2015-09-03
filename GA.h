#ifndef GA_H
#define GA_H
#include "GA_Base.h"
#include "Inicializacao.h"
#include "Saida.h"
#include "Parada.h"
#include "Comparacao.h"
#include "Selecao.h"
#include "Cruzamento.h"
#include "Mutacao.h"
#include "Fitness.h"



template <class GA>
class GASimples : public GA
{

public:

    USING(GA);

    using GA::melhor;
    using GA::iteracoes;

    using GA::Inicializacao;
    using GA::Selecao;
    using GA::Cruzamento;
    using GA::Mutacao;
    using GA::Parada;
    using GA::Saida;


    GASimples (int tamanhoPopulacao_ = 50, int maxIteracoes_ = 1000)
    {
        GA::tamanhoPopulacao = tamanhoPopulacao_;
        GA::maxIteracoes = maxIteracoes_;

        Iniciar();
    }


    Gene Evoluir ()
    {
        while(!Parada())
        {
            Cruzamento();
            Mutacao();
            Selecao();

            melhor = Melhor();
            iteracoes++;

            Saida();
        }

        //Saida();

        return melhor;
    }


    inline Gene Melhor ()
    {
        return GA::Melhor(melhor, GA::Melhor(p));
    }


    inline void Iniciar ()
    {
        p.clear();
        Inicializacao();

        iteracoes = 0;
        melhor = p[0];
    }

};


//=====================================================================





#endif // GA_H
