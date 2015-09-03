#ifndef COMPARACAO_H
#define COMPARACAO_H
#include "Defines.h"


template <bool minimizacao_>
class Comparacao
{
public:

    enum { maximizacao = false, minimizacao = true };

    inline bool Melhor (double a, double b)
    {
        return a < b;
    }

    template <class T, class F>
    inline T Melhor (const T& g, const T& h, F f)
    {
        return (f(g) < f(h)) ? g : h;
    }

    inline auto Melhor (const std::vector<double>& v) -> decltype(std::min_element(v.begin(), v.end()))
    {
        return std::min_element(v.begin(), v.end());
    }

    template <class T, class F>
    inline auto MelhorRange (T inicio, T fim, F comp) -> decltype(*std::min_element(inicio, fim, comp))
    {
        return *std::min_element(inicio, fim, comp);
    }
};


template <>
class Comparacao<false>
{
public:

    enum { maximizacao = true, minimizacao = false };

    inline bool Melhor (double a, double b)
    {
        return a > b;
    }

    template <class T, class F>
    inline T Melhor (const T& g, const T& h, F f)
    {
        return (f(g) > f(h)) ? g : h;
    }

    inline auto Melhor (const std::vector<double>& v) -> decltype(std::max_element(v.begin(), v.end()))
    {
        return std::max_element(v.begin(), v.end());
    }

    template <class T, class F>
    inline auto MelhorRange (T inicio, T fim, F comp) -> decltype(*std::max_element(inicio, fim, comp))
    {
        return *std::max_element(inicio, fim, comp);
    }
};

#endif // COMPARACAO_H
