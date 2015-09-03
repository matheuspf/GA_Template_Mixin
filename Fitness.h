#ifndef FITNESS_H
#define FITNESS_H
#include "Codificacao.h"


class FitnessF1 : public CodificacaoReal<>
{
public:

    FitnessF1 () : CodificacaoReal<>(-5.12, 5.12, 3) {}

    inline double Fitness (const Gene& g)
    {
        double r = 0.0;

        for(int i = 0; i < numeroVariaveis; i++)
            r += pow(Codificacao(g, i), 2);

        return r;
    }
};


class FitnessF2 : public CodificacaoReal<>
{
public:

    FitnessF2() : CodificacaoReal<>(-2.048, 2.048, 2) {}

    inline double Fitness (const Gene& g)
    {
        double x1 = Codificacao(g, 0);
        double x2 = Codificacao(g, 1);

        return 100.0 * pow(x1 * x1 - x2, 2) + pow(1 - x1, 2);
    }
};


template <class Cod>
class FitnessF3 : public Cod
{
public:

    using typename Cod::Gene;

    FitnessF3() : Cod(-200.0, 200.0, 45) {}

    double Fitness (const Gene& g)
    {
        double x = 100.0, y = 0.0;
        vector<double> v = Cod::Codificacao(g);

        for(int i = 0; i < Cod::numeroVariaveis; ++i)
        {
            y += (POW2(x) + POW2(v[i]));
            x = x + v[i];
        }

        return y + POW2(x);
    }
};

//=======================================================================================


template <class Cod>
class LinearQuadratic : public Cod
{
public:

    using typename Cod::Gene;

    const double a = 1.0;
    const double b = 1.0;
    const double q = 1.0;
    const double r = 1.0;
    const double s = 1.0;


    LinearQuadratic () : Cod(-100.0, 100.0, 45) {}


    double Fitness (const Gene& g)
    {
        vector<double> v = Cod::Codificacao(g);
        double x = 100.0, aux = 0.0;

        for(double u : v)
        {
            aux += (s * POW2(x) + r * POW2(u));
            x = a * x + b * u;
        }

        return POW2(x) + aux;
    }
};


//template <class Cod>
//class NLP : public Cod
//{
//public:

//    using typename Cod::Gene;

//    NLP() : Cod(-1.0, 1.0, 3) {}


//    double Fitness (const Gene& g)
//    {

//    }



//    double LimiteInferior (const Gene& g, int pos)
//    {

//    }

//    double LimiteSuperior (const Gene& g, int pos)
//    {

//    }

//};




#endif // FITNESS_H
