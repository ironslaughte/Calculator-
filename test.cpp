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
	ASSERT_ANY_THROW(calc.ReadExpr()); // ���� ��������� ������
	calc = Calculator("2 * cas(pi)");
	ASSERT_ANY_THROW(calc.Parse()); // ����� ����������� �����
	calc = Calculator("log(-2)");
	calc.Parse();
	ASSERT_ANY_THROW(calc.Calculate()); // �������� �� ���� ��� � ��������� �������� <= ����
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
}