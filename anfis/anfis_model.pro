QT -= gui

CONFIG += c++14 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    input_layer.cpp \
    membership_function_triangular.cpp \
    rule_layer.cpp \
    membership_function.cpp \
    takagi_sugeno_one.cpp \
    takagi_sugeno_zero.cpp \
    anfis.cpp \
    input_rule_link.cpp \
    dataset.cpp \
    takagi_input_rule_link.cpp

HEADERS += \
    input_layer.h \
    membership_function.h \
    membership_function_triangular.h \
    rule_layer.h \
    takagi_sugeno_one.h \
    takagi_sugeno_zero.h \
    anfis.h \
    input_rule_link.h \
    dataset.h \
    takagi_input_rule_link.h
