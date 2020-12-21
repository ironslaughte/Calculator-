#include "pch.h"
#include "Calculator.cpp"

Calculator calc;

TEST(TestCalc, Parse) {
	std::string expr_true = "0 1 - ";
	calc = Calculator("-1");
	calc.Parse();
	ASSERT_EQ(calc.GetExpression(), expr_true);
	expr_true = "1 2 - 3 * ";
	calc = Calculator("(1 - 2)*3");
	calc.Parse();
	ASSERT_EQ(calc.GetExpression(), expr_true);
}

TEST(TestCalc, Exceptions) {
	ASSERT_ANY_THROW(calc.ReadExpr()); // Если выражение пустое
	calc = Calculator("2 * cas(pi)");
	ASSERT_ANY_THROW(calc.Parse()); // Ввели неизвестный токен
	calc = Calculator("log(-2)");
	calc.Parse();
	ASSERT_ANY_THROW(calc.Calculate()); // поделили на ноль или в логарифме аргумент <= нуля
}

TEST(TestCalc, Calculate) {
	std::string expr = "(2*3 - 10)*(-4) / 16";
	calc = Calculator(expr);
	calc.Parse();
	ASSERT_EQ(calc.Calculate(), 1.0);
 
	expr = "(cos(0) + sin(0)) / 2";
	calc = Calculator(expr);
	calc.Parse();
	ASSERT_EQ(calc.Calculate(), 0.5);

	expr = "(abs(-2) + 3)^2 - 25";
	calc = Calculator(expr);
	calc.Parse();
	ASSERT_EQ(calc.Calculate(),0);

	expr = "(sqrt(4) - (-2)) * 5 + cos(pi)/2";
	calc = Calculator(expr);
	calc.Parse();
	ASSERT_EQ(calc.Calculate(), 19.5);

	expr = "(abs(cos(pi))*sqrt(9))^2";
	calc = Calculator(expr);
	calc.Parse();
	ASSERT_EQ(calc.Calculate(), 9);

	expr = "(log(100)/ln(e)) + (2*(5 - (-2))";
	calc = Calculator(expr);
	calc.Parse();
	ASSERT_EQ(calc.Calculate(), 16);

	expr = "(exp(0)*exp(1))/e";
	calc = Calculator(expr);
	calc.Parse();
	ASSERT_EQ(calc.Calculate(), 1);

	expr = " 2 + (-2) + 12 - 10 + 15.5";
	calc = Calculator(expr);
	calc.Parse();
	ASSERT_EQ(calc.Calculate(), 17.5);

	expr = "((125 / 5) * 4)/2 *50";
	calc = Calculator(expr);
	calc.Parse();
	ASSERT_EQ(calc.Calculate(), 2500);

	expr = "sin(2)*sin(2) + cos(2)*cos(2)";//  Получается погрешность, результат 0.999999. Проблемы вещественного типа
	calc = Calculator(expr);
	calc.Parse();
	ASSERT_EQ(calc.Calculate(), 1);


	expr = "sqrt(2) / cos(pi/4)"; // Получается погрешность, результат 1.999999
	calc = Calculator(expr);
	calc.Parse();
	ASSERT_EQ(calc.Calculate(), 2);


}