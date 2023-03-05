//
// Created by Des Caldnd on 2/19/2023.
//

#pragma once
#include <string>
#include <vector>
#include "Variable.h"
#include "Operation.h"

class LogicCalculator {

public:
    static std::string calculate_expression(std::string);

private:

    static bool check_string_for_symbols(const std::string&);

    static bool check_string_for_brackets(const std::string&);

    static bool check_string_for_operators(const std::string&);

    static bool check_string_for_end(const std::string&);

    static std::vector<ExpressionSymbol*> expr_to_postfix(const std::string&);

    static void evaluate_expression(std::vector<ExpressionSymbol*>, std::string);

    static bool hasVar(char);

    static unsigned long long power_of_2(unsigned int);

    static std::vector<ExpressionSymbol*> change_var_to_value(std::vector<ExpressionSymbol*>&);

    static bool value_of_var(char);

    static Variable calc_value(Variable, Variable, char);

    static std::vector<Variable> variables;

    static std::vector<std::vector<bool>> fAnswer;

    static std::vector<std::string> labels;

    enum SymType{
        Var, Oper, OpenBracket, CloseBracket, Constant
    };

    static SymType symType(char);

    static int operCount;

};

