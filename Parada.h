#ifndef PARADA_H
#define PARADA_H
#include "GA_Base.h"


template <class GA>
class ParadaPadrao : public GA
{
public:

    inline bool Parada ()
    {
        return GA::iteracoes >= GA::maxIteracoes;
    }
};


//template <class GA>
//class ParadaConvergencia : public GA
//{
//public:

//    const double convergenciaMaxima = 0.9;

//    inline bool Parada ()
//    {
//        vector<double> fitness = GA::Fitness(GA::p);
//        vector<int> v;
//        int aux = 0;

//        sort(fitness.begin(), fitness.end());

//        for(int i = 0; i < fitness.size() - 1; ++i)
//        {
//            if(IGUAL(fitness[i], fitness[i+1]))
//                aux++;

//            else
//            {
//                v.push_back(aux);
//                aux = 0;
//            }
//        }

//        v.push_back(aux);

//        return (double(*std::max_element(v.begin(), v.end())) / double(GA::tamanhoPopulacao) > convergenciaMaxima);
//    }
//};

#endif // PARADA_H
