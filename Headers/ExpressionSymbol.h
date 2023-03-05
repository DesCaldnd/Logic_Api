//
// Created by ivanl on 21.02.2023.
//
#pragma once

class ExpressionSymbol{

protected:
    struct proxy{
        char symbol = '2';
        int index = -1;
    };

    proxy data;

    virtual void setIndex(int) =0;

public:

    enum Type{
        Oper, Var
    };

    bool value = 0;

    int positionOfStart = 0, positionOfEnd = 0;

    virtual void setSymbol(char) =0;

    virtual char getSymbol() = 0;

    virtual Type getType() =0;
};

