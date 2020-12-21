#pragma once
#include <cmath>
#include <iostream>
#include <sstream>
#include "stack.h"
#include <stdlib.h>

#define pi 3.14159265358979323846
#define Exp 2.71828182845904523536

std::string tokens[] = { "+", "-","(", ")","*", "/","sqrt","sin","cos","abs","exp","log","ln","tg","ctg", "^", "%" };

std::string Const[] = { "pi", "e" };

class Calculator {
private:
    std::string expr;
    Stack<std::string> values;
    Stack<std::string> opers;
public:
    Calculator() {
        this->expr = "";
    }
    Calculator(std::string expr) {
        if (expr.size() == 0) {
            throw std::invalid_argument("Expression is empty");
        }
        this->expr = expr;
    }
    void ReadExpr();
    void Parse();
    std::string GetExpression();
    double Calculate();
};