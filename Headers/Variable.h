//
// Created by ivanl on 21.02.2023.
//
#pragma once
#include "ExpressionSymbol.h"

class Variable : public ExpressionSymbol{
    Type type = ExpressionSymbol::Type::Var;

public:
    void setSymbol(char) override;

    void setIndex(int) override;

    int getIndex();

    char getSymbol() override;

    Variable(char);

    Variable(char, int);

    ExpressionSymbol::Type getType() override;

};