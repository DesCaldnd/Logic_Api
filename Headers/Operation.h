//
// Created by Des Caldnd on 2/21/2023.
//
#pragma once
#include <vector>
#include "ExpressionSymbol.h"

class Operation : public ExpressionSymbol{

    void setIndex(int) override;

    static const std::vector<proxy> ORDER_LIST;

    Type type = ExpressionSymbol::Type::Oper;
public:


    void setSymbol(char) override;

    char getSymbol() override;

    ExpressionSymbol::Type getType() override;

    static int order(char);

    int getOrder();

    Operation(char);

    Operation(char, int);
};
