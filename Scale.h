#ifndef SCALE_H
#define SCALE_H
#include "Defines.h"


class NoScaling
{
public:

    inline void Scale (std::vector<double>&) { }
};


class SigmaScaling
{
public:

    const int c = 2;

    inline void Scale (std::vector<double>& v)
    {
        double media = 0.0, variancia = 0.0;

        for(double n : v)
            media += n;

        media /= v.size();

        for(double n : v)
            variancia += pow(n - media, 2);

        variancia = sqrt(variancia / v.size());

        for(double& n : v)
            n = n + (media - c * variancia);
    }
};




#endif // SCALE_H
