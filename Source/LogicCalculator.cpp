//
// Created by Des Caldnd on 2/19/2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "../Headers/LogicCalculator.h"
#include <stack>
#include <iostream>
#include <sstream>

int LogicCalculator::operCount = 0;
std::vector<Variable> LogicCalculator::variables{};
std::vector<std::vector<bool>> LogicCalculator::fAnswer{};
std::vector<std::string> LogicCalculator::labels{};

std::string LogicCalculator::calculate_expression(std::string str) {

    std::ostringstream response;

    response << "{ \"status\":";
    if (!check_string_for_symbols(str)){
        response << "\"Wrong symbols, use A-B, a-z, 1, 0, (, ), -, &, |, <, /, ~, +\"}";
        return response.str();
    }
    if (str.length() == 0){
        response << "\"Expression is empty\"}";
        return response.str();
    }
    if (!check_string_for_brackets(str)){
        response << "\"Brackets error\"}";
        return response.str();
    }
    if (!check_string_for_operators(str)){
        response << "\"More than one operator in a row\"}";
        return response.str();
    }
    if (!check_string_for_end(str)){
        response << "\"This expression is not completed\"}";
        return response.str();
    }
    auto expression = expr_to_postfix(str);

    if (variables.size() > 63){
        response << "\"Too many variables (max=63)\"}";
        return response.str();
    }
    evaluate_expression(expression, str);

    response << "\"OK\", \"matrix\": [ [";
    bool isFirst = true;
//    TODO: Matrix
    for (std::string label : labels){
        if(!isFirst) {
            response << ", ";
        } else
            isFirst = false;
        response << "\"" << label << "\"";
    }
    response << "], ";

    isFirst = true;

    for (std::vector<bool> row : fAnswer){
        if(!isFirst)
            response << ", ";
        else
            isFirst = false;
        response << "[";
        bool isSecond = true;
        for (bool data : row) {
            if(!isSecond)
                response << ", ";
            else
                isSecond = false;
            response << "\"" << data << "\"";
        }
        response << "]";
    }

    response << "], \"answer\": \"";

    bool isAllTrue = true, isAllFalse = true;
    for (std::vector<bool> vector_data : fAnswer){
        bool x = vector_data[vector_data.size()-1];
        if(!x)
            isAllTrue = false;
        else
            isAllFalse = false;
        if (!(isAllTrue || isAllFalse))
            break;
    }
    if (isAllTrue){
        response << "This expression is absolute true";
    } else if(isAllFalse){
        response << "This expression is absolute false";
    } else{
        response << "This expression is false at (";
        isFirst = true;
        for (Variable data : variables){
            if (!isFirst) {
                response << ", ";
            }
            response << data.getSymbol();
            isFirst = false;
        }
        response << "):";
        isFirst = true;
        for(std::vector<bool> vector_data : fAnswer){
            if (!vector_data[vector_data.size()-1]){
                if(!isFirst) {
                    response << ",)";
                }
                response << " (";
                for(int j = 0; j < variables.size(); j++){
                    if(j != 0)
                        response << ", ";
                    response << vector_data[j];
                }
                if(vector_data.size() == variables.size())
                    response << "0";
                response << ")";
                isFirst = false;
            }
        }
        response << "\"}";
    }
    return response.str();
}

bool LogicCalculator::check_string_for_brackets(const std::string &str) {
    int brackets = 0;
    for (char sym : str){
        if (sym == '(')
            brackets++;
        else if (sym == ')')
            brackets--;
        if (brackets < 0)
            return false;
    }
    return brackets == 0;
}

std::vector<ExpressionSymbol*> LogicCalculator::expr_to_postfix(const std::string &str) {
    variables.clear();
    operCount = 0;

    std::vector<ExpressionSymbol*> answer;
    std::stack<Operation> operationStack;

    int i = 0;
    for (char sym : str){
        switch (symType(sym)) {
            case Var: {
                answer.push_back(new Variable(sym, i));
                if (!hasVar(sym))
                    variables.push_back(Variable{sym});
                break;
            }
            case Oper: {
                operCount++;
                int order = Operation::order(sym);
                while (operationStack.size() != 0) {
                    if (operationStack.top().getOrder() > order) {
                        answer.push_back(new Operation(operationStack.top()));
                        operationStack.pop();
                    } else
                        break;
                }
                operationStack.push(Operation{sym, i});
                break;
            }
            case OpenBracket: {
                operationStack.push(Operation{sym});
                break;
            }
            case CloseBracket: {
                while (operationStack.top().getSymbol() != '(') {
                    answer.push_back(new Operation(operationStack.top()));
                    operationStack.pop();
                }
                operationStack.pop();
                break;
            }
            case Constant: {
                answer.push_back(new Variable(sym, i));
                answer[answer.size()-1]->value = (sym == '1');
                break;
            }
        }
        i++;
    }

    while (operationStack.size() != 0) {
        answer.push_back(new Operation(operationStack.top()));
        operationStack.pop();
    }

    return answer;
}

LogicCalculator::SymType LogicCalculator::symType(char symbol) {
    if((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z'))
        return LogicCalculator::SymType::Var;
    else if (symbol == '(')
        return LogicCalculator::SymType::OpenBracket;
    else if (symbol == ')')
        return LogicCalculator::SymType ::CloseBracket;
    else if (symbol == '1' || symbol == '0')
        return LogicCalculator::SymType::Constant;
    return LogicCalculator::SymType::Oper;
}

bool LogicCalculator::hasVar(char var) {
    for(Variable data : variables){
        if (data.getSymbol() == var)
            return true;
    }
    return false;
}

bool LogicCalculator::check_string_for_operators(const std::string &string) {
    bool isOperator = false, isMinus = false;
    for (char sym : string){
        if (sym == '&' || sym == '|' || sym == '/' || sym == '+' || sym == '<' || sym == '~'){
            if (isOperator)
                return false;
            isOperator = true;
        } else if(sym != '-') {
            isOperator = false;
            isMinus = false;
        } else{
            if (isMinus)
                return false;
            isMinus = true;
        }
    }
    return !(isMinus || isOperator);
}

bool LogicCalculator::check_string_for_end(const std::string &string) {
    bool hasFirstOperand = false, isOper = false;
    for (char sym : string){
        switch (symType(sym)) {
            case Oper:{
                if (sym != '-'){
                    if (!hasFirstOperand){
                        return false;
                    } else {
                        hasFirstOperand = false;
                    }
                }
                isOper = true;
                break;
            }
            case OpenBracket:{
                isOper = false;
                hasFirstOperand = false;
                break;
            }
            case CloseBracket:{
                if(isOper){
                    return false;
                }
                hasFirstOperand = true;
                break;
            }
            default:{
                if (hasFirstOperand)
                    return false;
                hasFirstOperand = true;
                isOper = false;
                break;
            }
        }
    }
    return true;
}

void LogicCalculator::evaluate_expression(std::vector<ExpressionSymbol*> expression, std::string string) {
    fAnswer.clear();
    unsigned long long vars_2 = power_of_2(variables.size());
    labels.clear();
    bool isFirst = true;
    for(unsigned long long i = vars_2; i > 0; i--){
        fAnswer.push_back(std::vector<bool>());
        int opers = 0;
        for(int j = 0; j < variables.size(); j++){
            variables[j].value = (1ULL << variables.size() - (j + 1)) & (i - 1);
            if (isFirst)
                labels.push_back(std::string(1, variables[j].getSymbol()));
            fAnswer[vars_2 - i].push_back(variables[j].value);
        }
        auto var_expression = change_var_to_value(expression);
        std::stack<ExpressionSymbol*> exprStack;
        for (ExpressionSymbol *var_expr : var_expression){
            if(var_expr->getType() == ExpressionSymbol::Type::Var){
                exprStack.push(var_expr);
            } else{
                Variable result('A');
                if(var_expr->getSymbol() == '-'){
                    result = *dynamic_cast<Variable*>(exprStack.top());
                    result.positionOfStart--;
                    exprStack.pop();
                    result.value = !result.value;
                } else {
                    auto second_val = exprStack.top();
                    exprStack.pop();
                    auto first_val = exprStack.top();
                    exprStack.pop();
                    result = calc_value(*dynamic_cast<Variable *>(first_val), *dynamic_cast<Variable *>(second_val),
                                        var_expr->getSymbol());
                }
                    if (isFirst){
                        while (result.positionOfStart > 0 && result.positionOfEnd < string.length() - 1){
                            if (string[result.positionOfStart - 1] == '(' && string[result.positionOfEnd + 1] == ')'){
                                result.positionOfStart--;
                                result.positionOfEnd++;
                            } else
                                break;
                        }
                        labels.push_back(string.substr(result.positionOfStart, result.positionOfEnd - result.positionOfStart + 1));
                    }
                    exprStack.push(new Variable(result));
                fAnswer[vars_2 - i].push_back(result.value);
                opers++;
            }
        }
        isFirst = false;
    }
}

unsigned long long LogicCalculator::power_of_2(unsigned int pow) {
    unsigned long long result = 1;
    for (int i = 0; i < pow; i++){
        result *= 2u;
    }
    return result;
}

std::vector<ExpressionSymbol*> LogicCalculator::change_var_to_value(std::vector<ExpressionSymbol*> &expression) {
    for(ExpressionSymbol *expr : expression){
        if(symType(expr->getSymbol()) == LogicCalculator::SymType::Var){
            expr->value = value_of_var(expr->getSymbol());
        }
    }
    return expression;
}

bool LogicCalculator::value_of_var(char var) {
    for (Variable data : variables){
        if (var == data.getSymbol())
            return data.value;
    }
    return false;
}

Variable LogicCalculator::calc_value(Variable first_var, Variable second_var, char oper) {
    Variable result{'1', first_var.positionOfStart};
    result.positionOfEnd = second_var.positionOfEnd;
    bool f = first_var.value, s = second_var.value;
    switch (oper){
        case '&':{
            f = f && s;
            break;
        }
        case '|':{
            f = f || s;
            break;
        }
        case '<':{
            f = f <= s;
            break;
        }
        case '~':{
            f = f == s;
            break;
        }
        case '/':{
            f = f && !s;
            break;
        }
        case '+':{
            f = f != s;
            break;
        }
    }
    result.value = f;
    return result;
}

bool LogicCalculator::check_string_for_symbols(const std::string &str) {
    for(char sym : str){
        if(!((sym >= 'A' && sym <= 'Z') || (sym >= 'a' && sym <= 'z') || sym == '0' || sym == '1' || sym == '(' || sym == ')' ||
                sym == '-' || sym == '&' || sym == '|' || sym == '<' || sym == '~' || sym == '+' || sym == '/'))
            return false;
    }
    return true;
}

/*
void LogicCalculator::action_file() {
    if(ui->table->rowCount() == 0 || ui->table->columnCount() == 0) {
        errorMessageBox.setText("Table is empty");
        errorMessageBox.exec();
        return;
    }
    std::string path = QFileDialog::getSaveFileName(this,  tr("Save Table"), "/downloads/untitled.xlsx",
                                                tr("Exel new table (*.xlsx);;Exel simple table (*.csv)"));

    QFileInfo info(path);

    if (info.suffix() == "csv") {
        std::ofstream outData(path.toStdString(), std::ios::out | std::ios::trunc);
        if (!outData.is_open()) {
            errorMessageBox.setText("Error during file open");
            errorMessageBox.exec();
            return;
        }
        outData << "Row index;";
        for (std::string data: labels) {
            outData << data.toStdString() << ';';
        }
        outData << std::endl;
        int i = 1;
        for (std::vector<bool> iter: fAnswer) {
            outData << i << ';';
            for (bool data: iter) {
                outData << data << ';';
            }
            outData << std::endl;
            i++;
        }
        outData.close();
    } else {
        QXlsx::Document outData;
        QVariant writeValue = std::string("Row index");
        outData.write(1, 1, writeValue);

        int i = 2;
        for (std::string data: labels) {
            outData.write(1, i, data);
            i++;
        }
        i = 2;
        for (std::vector<bool> iter: fAnswer) {
            int j = 2;
            outData.write(i, 1, i - 1);
            for (bool data: iter) {
                outData.write(i, j, static_cast<int>(data));
                j++;
            }
            i++;
        }
        outData.saveAs(path);
    }
}
*/