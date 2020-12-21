#include "Calculator.h"

bool Check_brackets(const char* str, size_t len) { // Проверка правильности расстановки скобок
    size_t count = 0, i = 0;
    Stack<char> brackets;
    while (i < len) {
        i++;
        if (*str == '(') {
            brackets.Push(*str);
        }
        else if (*str == ')') {
            brackets.Pop_Front();
        }
        str++;
    }
    return !brackets.IsEmpty();
}

std::string parse_whitespace(std::string& input) {  // Извбавляемся от пробелов в начале строки
                                          // чтобы точно определенить возможный унарный минус
    size_t i = 0;
    while (input[i] == ' ') {
        i++;
    }
    std::string new_input;
    for (i; i < input.size(); i++) {
        new_input += input[i];
    }
    return new_input;
}

int get_priority(const std::string& token) {
    if (token == "(") return 0;
    if (token == "+") return 1; // сложение
    if (token == "-") return 1; // разность
    if (token == "*") return 2; // умножение
    if (token == "/") return 2; // деление
    if (token == "%") return 2; // остаток от деления
    if (token == "^") return 3; // степень 
    return 4; // Возвращаем 4 если токен -  перфиксная операция
}

bool Check_unary_minus(std::string& expr, size_t index) {
    if (index == 0) {
        return true;
    }
    while (index > 0) {
        if (isspace(expr[index - 1])) {
            index--;
            continue;
        }
        if (expr[index - 1] != '(') return false;
        if (expr[index - 1] == '(') return true;
    }
}

void Calculator::PrintExpression() {
   std:: cout << expr << std::endl;
}

void Calculator::Parse() { // Парсер исходного выражения, которые преобразует его в выражение польской нотации
    size_t i = 0;
    Stack<std::string> oper;
    std::string new_expr;

    while (i < expr.size()) {
        if (isspace(expr[i])) {
            i++;
        }

        if (isdigit(expr[i])) {
            std::string number;
            while (isdigit(expr[i]) || expr[i] == '.') {
                number.push_back(expr[i]);
                i++;
            }

            new_expr += number + " ";
            continue;
        }


        bool flag = true;
        for (std::string& c : Const) {
            if (expr.find(c.c_str(), i) == i) {
                flag = false;
                i += c.size();
                std::stringstream double_const;
                std::string tmp_res;
                if (c == "pi") {
                    double_const << pi;
                    double_const >> tmp_res;
                    new_expr += tmp_res + " ";
                }
                else {
                    double_const << exp;
                    double_const >> tmp_res;
                    new_expr += tmp_res + " ";
                }
                break;
            }
        }

        for (std::string& t : tokens) {
            if (expr.find(t.c_str(), i) == i) {
                //  expr += t.size();
                i += t.size();
                flag = false;

                if (t == ")") {
                    if (oper.Top() == "(") { // сделано для операций по типу sin(const)...
                        oper.Pop_Front();
                        new_expr += oper.Pop() + " ";
                        break;
                    }
                    while ((oper.GetSize() > 0) && (oper.Top() != "(")) {
                        new_expr += oper.Pop() + " ";
                    }
                    oper.Pop_Front();
                    break;
                }

                else if (t == "(") {
                    oper.Push(t);
                    break;
                }

                else {
                    if (t == "-" && Check_unary_minus(expr, i - t.size())) {
                        new_expr += "0 ";
                    }
                    if (!oper.IsEmpty() && get_priority(oper.Top()) >= get_priority(t)) {
                        do {
                            new_expr += oper.Pop() + " ";
                        } while (!oper.IsEmpty() && get_priority(oper.Top()) >= get_priority(t));
                        oper.Push(t);
                    }
                    else {
                        oper.Push(t);
                    }
                    break;
                }

            }

        }
        if (flag) {
            throw std::invalid_argument("Unknown token!\n");
        }
    }
    while (!oper.IsEmpty()) {
        if (oper.Top() == "(") {
            oper.Pop_Front();
        }
        else {
            new_expr += oper.Pop() + " ";
        }
    }
    this->expr = new_expr;
}


double Calculator::Calculate() { // Вычисление выражения
    size_t i = 0;
    std::string res_in_stack;
    while (i < expr.size()) {
        if (isspace(expr[i])) {
            i++;
        }

        if (isdigit(expr[i])) {
            std::string number;
            while (isdigit(expr[i]) || expr[i] == '.') {
                number.push_back(expr[i]);
                i++;
            }
            values.Push(number);
            continue;
        }

        if (expr[i] == '+') {
            i++;
            double res = strtod(values.Pop().c_str(), nullptr) + strtod(values.Pop().c_str(), nullptr);
            std::stringstream ss;
            ss << res;
            ss >> res_in_stack;
            values.Push(res_in_stack);
            res_in_stack = "";
            continue;
        }
        if (expr[i] == '-') {
            i++;

            double value2 = strtod(values.Pop().c_str(), nullptr);
            double value1 = strtod(values.Pop().c_str(), nullptr);
            value1 -= value2;
            std::stringstream ss;
            ss << value1;
            ss >> res_in_stack;
            values.Push(res_in_stack);
            res_in_stack = "";
            continue;
        }

        if (expr[i] == '*') {
            i++;
            double res = strtod(values.Pop().c_str(), nullptr) * strtod(values.Pop().c_str(), nullptr);
            std::stringstream ss;
            ss << res;
            ss >> res_in_stack;
            values.Push(res_in_stack);
            res_in_stack = "";
            continue;
        }
        if (expr[i] == '/') {
            i++;
            double value2 = strtod(values.Pop().c_str(), nullptr);
            if (value2 == 0 || fabs(value2) < 1e-5) {
                throw std::invalid_argument("Division by zero!");
            }
            double value1 = strtod(values.Pop().c_str(), nullptr);
            value1 /= value2;
            std::stringstream ss;
            ss << value1;
            ss >> res_in_stack;
            values.Push(res_in_stack);
            res_in_stack = "";
            continue;
        }
        if (expr[i] == '^') {
            i++;
            double value2 = strtod(values.Pop().c_str(), nullptr);
            double value1 = strtod(values.Pop().c_str(), nullptr);
            value1 = pow(value1, value2);
            std::stringstream ss;
            ss << value1;
            ss >> res_in_stack;
            values.Push(res_in_stack);
            res_in_stack = "";
            continue;
        }

        for (std::string& t : tokens) {
            if (expr.find(t.c_str(), i) == i) {
                i += t.size();

                if (t == "sin") {
                    double value = strtod(values.Pop().c_str(), nullptr);
                    value = sin(value);
                    std::stringstream ss;
                    ss << value;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "cos") {
                    double value = strtod(values.Pop().c_str(), nullptr);
                    value = cos(value);
                    std::stringstream ss;
                    ss << value;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "tg") {
                    double value = strtod(values.Pop().c_str(), nullptr);
                    value = tan(value);
                    std::stringstream ss;
                    ss << value;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "ctg") {
                    double value = strtod(values.Pop().c_str(), nullptr);
                    if (tan(value) == 0 || fabs(tan(value)) < 1e-5) {
                        throw std::invalid_argument("Division by zero!");
                    }
                    value = 1.0 / tan(value);
                    std::stringstream ss;
                    ss << value;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "log") {
                    double value = strtod(values.Pop().c_str(), nullptr);
                    if (value <= 0) {
                        throw std::invalid_argument("Invalid arg in log");
                    }
                    value = log10(value);
                    std::stringstream ss;
                    ss << value;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "ln") {
                    double value = strtod(values.Pop().c_str(), nullptr);
                    if (value <= 0) {
                        throw std::invalid_argument("Invalid arg in ln");
                    }
                    value = log(value);
                    std::stringstream ss;
                    ss << value;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "abs") {
                    double value = strtod(values.Pop().c_str(), nullptr);
                    value = fabs(value);
                    std::stringstream ss;
                    ss << value;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "sqrt") {
                    double value = strtod(values.Pop().c_str(), nullptr);
                    if (value < 0) {
                        throw std::invalid_argument("Invalid arg in sqrt");
                    }
                    value = sqrt(value);
                    std::stringstream ss;
                    ss << value;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }
                break;
            }
        }
    }
    return strtod(values.Pop().c_str(), nullptr);
}


void Calculator::ReadExpr() {
    std::getline(std::cin, expr);
    if (Check_brackets(expr.c_str(), expr.length())) {
        throw std::invalid_argument("Syntax error\n");
    }
    expr = parse_whitespace(expr);
    if (expr.size() == 0) {
        throw std::invalid_argument("Expression is empty");
    }
}