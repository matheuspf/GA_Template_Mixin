TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
CONFIG += c++14
QMAKE_CXXFLAGS_RELEASE += -g

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

QMAKE_CXXFLAGS_RELEASE *= -O3

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Defines.h \
    GA.h \
    Selecao.h \
    Cruzamento.h \
    Mutacao.h \
    Representacao.h \
    Codificacao.h \
    GA_Base.h \
    Scale.h \
    Inicializacao.h \
    Parada.h \
    Saida.h \
    Comparacao.h \
    Fitness.h

