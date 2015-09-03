#ifndef DEFINES_H
#define DEFINES_H
#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <chrono>
#include <random>
#include <thread>
#include <tuple>
#include <type_traits>
#define RAND(x) (rand() % (x))
//#define RAND0() ((double)rand() / RAND_MAX)
#define RAND0() (Rng::rng(Rng::gen))
#define RANDI(a, b) ((a) + (rand() % ((b) - (a))))
#define RANDF(a, b) ((a) + RAND0() * ((b) - (a)))
//#define PROBABILIDADE(X) (RAND0() < (X))
#define PROBABILIDADE(x) (Rng::rng(Rng::gen) < (x))
//#define PROBABILIDADE(x) (GA::r.Rand() < (x))
#define DB(X) cout << X << endl;
#define POW2(x) (x) * (x)
#define SIGN(x) ((x) < 0 ? -1.0 : 1.0)

#define USING(X)     using Gene = typename X::Gene;    \
                     using Populacao = typename X::Populacao;  \
                     using X::p

using namespace std;

struct Rng
{
    static std::mt19937 gen;
    static std::uniform_real_distribution<double> rng;
};

std::mt19937 Rng::gen = std::mt19937(time(0));
std::uniform_real_distribution<double> Rng::rng = std::uniform_real_distribution<double>(0.0, 1.0);

//class Rng
//{
//public:

//    std::mt19937 gen;
//    std::uniform_real_distribution<double> rng;

//    static const int M = 200000;
//    static constexpr double Thresh = 0.95 * M;

//    double tabela[M];
//    int pos = 0;


//    Rng () : gen(time(0)), rng(0.0, 1.0)
//    {
//        Gerar(M);
//    }

//    void Gerar (int t)
//    {
//        std::vector<std::thread> v;

//        int qt = 10, passo = t / qt, fim = t - passo;

//        for(int i = 0; i <= fim; i += passo)
//            v.push_back(thread([&](int de, int para){

//                for(int i = de; i < para; ++i)
//                    tabela[i] = rng(gen);

//            }, i, i + passo));

//        for(thread& t : v)
//            t.join();
//    }


//    double Rand ()
//    {
//        static bool b = true;

//        if(pos > Thresh && b)
//        {
//            int x = Thresh;
//            thread([&](int t){ Gerar(t); }, x).detach();
//            b = false;
//            //cout << "AAAAA" << endl;
//        }

//        if(!pos) b = true;

//        double r = tabela[pos];

//        pos = (pos + 1) % M;

//        return r;
//    }

//};

//==========================================================

namespace Impl
{

struct NullType {};

template <class, template <class> class...>
struct Montagem;

template <class GA, template <class> class T, template <class> class ...Args>
struct Montagem<GA, T, Args...>
{
    using Res = T<typename Montagem<GA, Args...>::Res>;
};

template <class GA>
struct Montagem<GA>
{
    using Res = GA;
};


template <class T, class P>
struct Link
{
    using Tipo = T;
    using TipoProx = P;

    P* prox;
};


template <class, template <class> class...>
struct Linker;

template <class GA>
struct Linker <GA>
{
    using Res = NullType;
};

template <class GA, template <class> class T, template <class> class ...Args>
struct Linker <GA, T, Args...>
{
    using Res = Link<T<typename Montagem<GA, Args...>::Res>, typename Linker<GA, Args...>::Res>;
};


//--------------------------------------------------------------------

template <class, template <class> class...>
class OperadorMultiplo;

template <class GA, template <class> class T, template <class> class ... Operadores>
class OperadorMultiplo<GA, T, Operadores...> : public Montagem<GA, T,  Operadores...>::Res
{
public:

    using Tipo = typename Montagem<GA, T,  Operadores...>::Res;

    Impl::Link<Tipo, typename Linker<GA,  Operadores...>::Res> link;


    OperadorMultiplo ()
    {
        Construir(&link);
    }

    template <class U>
    void Construir (U* u)
    {
        u->prox = new typename U::TipoProx;
        Construir(u->prox);
    }

    void Construir (NullType*) {}

};

} //Impl

//==================================================================================


template <int T>
struct Quadrado
{
    static const long long val = 2 * Quadrado<T-1>::val;
};

template<>
struct Quadrado<0>
{
    static const long long val = 1;
};


template <int ...Args>
struct Array
{
    static const long long v[sizeof...(Args)];
};

template <int ...Args>
const long long Array<Args...>::v[sizeof...(Args)] = { Args... };


template <int T, template <int> class Funcao, long long ...Args>
struct Gerador
{
    using res = typename Gerador<T-1, Funcao, Funcao<T>::val, Args...>::res;
};

template <template <int> class Funcao, long long ...Args>
struct Gerador<-1, Funcao, Args...>
{
    using res = Array<Args...>;
};


template <int T>
struct POW2T
{
    using res = typename Gerador<T, Quadrado>::res;
};


//Testa tempo de execução de uma função qualquer
template <typename Func, typename... Args>
double Benchmark (Func f, Args... args)
{
    return Benchmark(f, args..., std::chrono::milliseconds());
}

template <typename Func, typename... Args, typename u, typename v>
double Benchmark (Func f, Args... args, std::chrono::duration<u,v> e)
{
     std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();

    f(args...);     //Chama função com a lista de parâmetros

    return  std::chrono::duration_cast< decltype(e) >(  std::chrono::high_resolution_clock::now() - t ).count();
}



#endif // DEFINES_H
