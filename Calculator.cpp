#include "Calculator.h"

bool Check_brackets(const char* str, size_t len) { // Ïðîâåðêà ïðàâèëüíîñòè ðàññòàíîâêè ñêîáîê
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

std::string parse_whitespace(std::string& input) {  // Èçâáàâëÿåìñÿ îò ïðîáåëîâ â íà÷àëå ñòðîêè
                                          // ÷òîáû òî÷íî îïðåäåëåíèòü âîçìîæíûé óíàðíûé ìèíóñ
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
    if (token == "+") return 1; // ñëîæåíèå
    if (token == "-") return 1; // ðàçíîñòü
    if (token == "*") return 2; // óìíîæåíèå
    if (token == "/") return 2; // äåëåíèå
    if (token == "%") return 2; // îñòàòîê îò äåëåíèÿ
    if (token == "^") return 3; // ñòåïåíü
    // Äàëåå ïðèîðèòåò ñàìûé âûñîêèé, òê íàì íóæíî âûòàùèòü ïðåôèêñíûå îïåðàöèè ñðàçó ïîñëå çàêðûòèÿ ñêîáîê ñ àðãóìåíòîì 
    if (token == "sin") return 4;
    if (token == "cos") return 4;
    if (token == "log") return 4;
    if (token == "ln") return 4;
    if (token == "tg") return 4;
    if (token == "ctg") return 4;
    if (token == "exp") return 4;
    if (token == "abs") return 4;
    if (token == "sqrt") return 4;
    return 0; // Âîçâðàùàåì 0 åñëè òîêåí - ýòî íå áèíàðíàÿ îïåðàöèÿ (íàïðèìåð ")")
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

std::string Calculator::GetExpression() {
   return expr;
}

void Calculator::Parse() { // Ïàðñåð èñõîäíîãî âûðàæåíèÿ, êîòîðûå ïðåîáðàçóåò åãî â âûðàæåíèå ïîëüñêîé íîòàöèè
    size_t i = 0;
    Stack<std::string> oper;
    std::string ss;

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

            ss += number + " ";
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
                    ss += tmp_res + " ";
                }
                else {
                    double_const << exp;
                    double_const >> tmp_res;
                    ss += tmp_res + " ";
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
                    if (oper.Top() == "(") { // ñäåëàíî äëÿ îïåðàöèé ïî òèïó sin(const)...
                        oper.Pop_Front();
                        ss += oper.Pop() + " ";
                        break;
                    }
                    while ((oper.GetSize() > 0) && (oper.Top() != "(")) {
                        ss += oper.Pop() + " ";
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
                        ss += "0 ";
                    }
                    if (!oper.IsEmpty() && get_priority(oper.Top()) >= get_priority(t)) {
                        do {
                            ss += oper.Pop() + " ";
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
            ss += oper.Pop() + " ";
        }
    }
    this->expr = ss;
}


double Calculator::Calculate() { // Âû÷èñëåíèå âûðàæåíèÿ
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

            double oper2 = strtod(values.Pop().c_str(), nullptr);
            double oper1 = strtod(values.Pop().c_str(), nullptr);
            oper1 -= oper2;
            std::stringstream ss;
            ss << oper1;
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
            double oper2 = strtod(values.Pop().c_str(), nullptr);
            if (oper2 == 0 || fabs(oper2) < 1e-5) {
                throw std::invalid_argument("Division by zero!");
            }
            double oper1 = strtod(values.Pop().c_str(), nullptr);
            oper1 /= oper2;
            std::stringstream ss;
            ss << oper1;
            ss >> res_in_stack;
            values.Push(res_in_stack);
            res_in_stack = "";
            continue;
        }
        if (expr[i] == '^') {
            i++;
            double oper2 = strtod(values.Pop().c_str(), nullptr);
            double oper1 = strtod(values.Pop().c_str(), nullptr);
            oper1 = pow(oper1, oper2);
            std::stringstream ss;
            ss << oper1;
            ss >> res_in_stack;
            values.Push(res_in_stack);
            res_in_stack = "";
            continue;
        }

        for (std::string& t : tokens) {
            if (expr.find(t.c_str(), i) == i) {
                i += t.size();

                if (t == "sin") {
                    double oper = strtod(values.Pop().c_str(), nullptr);
                    oper = sin(oper);
                    std::stringstream ss;
                    ss << oper;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "cos") {
                    double oper = strtod(values.Pop().c_str(), nullptr);
                    oper = cos(oper);
                    std::stringstream ss;
                    ss << oper;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "tg") {
                    double oper = strtod(values.Pop().c_str(), nullptr);
                    oper = tan(oper);
                    std::stringstream ss;
                    ss << oper;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "ctg") {
                    double oper = strtod(values.Pop().c_str(), nullptr);
                    if (tan(oper) == 0 || fabs(tan(oper)) < 1e-5) {
                        throw std::invalid_argument("Division by zero!");
                    }
                    oper = 1.0 / tan(oper);
                    std::stringstream ss;
                    ss << oper;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "log") {
                    double oper = strtod(values.Pop().c_str(), nullptr);
                    if (oper <= 0) {
                        throw std::invalid_argument("Invalid arg in log");
                    }
                    oper = log10(oper);
                    std::stringstream ss;
                    ss << oper;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "ln") {
                    double oper = strtod(values.Pop().c_str(), nullptr);
                    if (oper <= 0) {
                        throw std::invalid_argument("Invalid arg in ln");
                    }
                    oper = log(oper);                   
                    std::stringstream ss;
                    ss << oper;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "abs") {
                    double oper = strtod(values.Pop().c_str(), nullptr);
                    oper = fabs(oper);
                    std::stringstream ss;
                    ss << oper;
                    ss >> res_in_stack;
                    values.Push(res_in_stack);
                    res_in_stack = "";
                }

                if (t == "sqrt") {
                    double oper = strtod(values.Pop().c_str(), nullptr);
                    if (oper < 0) {
                        throw std::invalid_argument("Invalid arg in sqrt");
                    }
                    oper = sqrt(oper);
                    std::stringstream ss;
                    ss << oper;
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
    if (expr.length() == 0) {
        throw std::invalid_argument("Expression in empty");
    }
}
