#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

/*class MyException : exception
{
    const string message;

public:

    MyException(const string& mess) : message(mess) {}

    string GetMessage() { return message; }

    /*const char* what() const throw() { return message.c_str(); }#1#
};*/

template<typename T>
void Print(const vector<T>& arr, int index, int size)
{
    if (index < 0 || index + size > arr.size()) throw out_of_range("Out of range from \'Print()\'");

    for (int i = index; i < index + size; i++)
    {
        cout << arr[i] << " ";
    }
}

template<typename T>
bool PrintVectors(const vector<vector<T>*>& arrs, int index, int size)
{
    if (index < 0 || index + size > arrs.size()) throw out_of_range("Out of range from \'PrintVectors()\'");

    for (int i = index; i < index + size; i++)
    {
        Print(*arrs[i], 0, (*arrs[i]).size());
        cout << '\n';
    }

    return true;
}

double Sum(double a, double b) noexcept
{
    return a + b;
}
double Sub(double a, double b) noexcept
{
    return a - b;
}
double Mul(double a, double b) noexcept
{
    return a * b;
}
double Div(double a, double b)
{
    if (b == 0) throw invalid_argument("Division by zero");
    return a / b;
}
double Mod(double a, double b)
{
    if (b == 0) throw invalid_argument("Modulo by zero");
    return fmod(a, b);
}

double Pow(double a, double b)
{
    return pow(a, b);
}

class MathParser
{
    string expr;
    int pos = 0;

    void SkipSpaces()
    {
        while (pos < expr.size() && expr[pos] == ' ')
        {
            pos++;
        }
    }
    // саме число
    double ParseNum()
    {
        SkipSpaces();
        int start = pos;
        while (pos < expr.size() && (isdigit(expr[pos]) || expr[pos] == '.'))
        {
            pos++;
        }
        //якщо позиція не зрушилася то там нема числа
        if (start == pos) {
            throw invalid_argument("Expected a number");
        }
        return stod(expr.substr(start, pos - start));
    }
    //перші скобочки 
    double ParsePart()
    {
        SkipSpaces();
        //якщо є скобочка
        if (pos < expr.size())

        {
            if (expr[pos] == '(') {
                pos++;
                double val = ParseSumSub();
                SkipSpaces();
                if (pos >= expr.size() || expr[pos] != ')')
                    //помилка якщо скобочка не закрита
                    throw invalid_argument("No closing ')'");
                pos++;
                return val;
            }
        }

        return ParseNum();
    }
    //другі ступені
    double ParsePower()
    {
        double left = ParsePart();
        SkipSpaces();

        if (pos < expr.size() && expr[pos] == '^')
        {
            pos++;
            double right = ParseUnary(); //для скобочок (перевірка у символу у ступені)
            left = Pow(left, right);
        }

        return left;
    }

    double ParseUnary() //треті парсуються унарні вирази (-х, +х)
    {
        SkipSpaces();

        if (pos < expr.size() && expr[pos] == '+')
        {
            pos++;
            return ParseUnary();
        }

        if (pos < expr.size() && expr[pos] == '-')
        {
            pos++;
            return -ParseUnary();
        }

        return ParsePower();
    }
    //четверті множення ділення та остаток
    double ParseMulDiv()
    {
        double left = ParseUnary();

        while (true)
        {
            SkipSpaces();

            if (pos >= expr.size()) break;

            char op = expr[pos];

            if (op != '*' && op != '/' && op != '%') break;

            pos++;

            double right = ParseUnary();

            if (op == '*')
                left = Mul(left, right);
            else if (op == '/')
                left = Div(left, right);
            else
                left = Mod(left, right);
        }

        return left;
    }
    //п'яті вже сума та різниця
    double ParseSumSub()
    {
        double left = ParseMulDiv();
        while (true)
        {
            SkipSpaces();
            if (pos >= expr.size()) break;
            char op = expr[pos];

            if (op != '+' && op != '-') break;  //якщо нема операторів
            //якщо є оператор 
            pos++;
            double right = ParseMulDiv();
            if (op == '+') {
                left = Sum(left, right);
            }
            else {
                left = Sub(left, right);
            }
        }
        return left;
    }

public:

    double Evaluate(const string& expression)
    {
        expr = expression;
        pos = 0;
        try
        {
            double result = ParseSumSub();
            SkipSpaces();
            if (pos != expr.size()) {
                throw invalid_argument("Somehow went out of bounds (Unexpected symbols at the end)");
            }
            return result;
        }
        catch (const exception& e)
        {
            throw runtime_error(string("Parse error: ") + e.what());
        }
    }
};

int main()
{
    // --- Тест парсера ---
    try
    {
        MathParser parser;

        vector<string> expressions = {
            "5+2",
            "5-2*3",
            "(5-2)*3",
            "10/2+7",
            "3.5+2.1",
            "-5+2",
            "+7-3",
            "--5",
            "2^3",
            "2^3^2",
            "2^-3",
            "-2^2",
            "(-2)^2",
            "10%3",
            "15%4+1",
            "5+2abc",
            "10/0",
            "10%0",
            "(2+3",
            "abc+2"
        };

        for (const string& expr : expressions)
        {
            cout << "Expression: " << setw(16) << left << expr << " -> ";

            try
            {
                double result = parser.Evaluate(expr);
                cout << result << endl;
            }
            catch (const exception& e)
            {
                cout << "error: " << e.what() << endl;
            }
        }
    }
    catch (const exception& e)
    {
        cout << "Parser error: " << e.what() << endl;
    }

    cout << "\n--- Vector test ---\n";

    vector<vector<int>*> arrs;
    arrs.push_back(new vector<int>{ 1, 2, 3 });
    arrs.push_back(new vector<int>{ 4, 5, 6 });
    arrs.push_back(new vector<int>{ 7, 8, 9 });

    try
    {
        PrintVectors(arrs, 0, 3);

        cout << "\nTrying out-of-range:\n";
        PrintVectors(arrs, 2, 2); 
    }
    catch (const out_of_range& e)
    {
        cout << "Out of range error: " << e.what() << endl;
    }
    catch (const exception& e)
    {
        cout << "Other error: " << e.what() << endl;
    }

    for (vector<int>* arr : arrs)
    {
        delete arr;
    }

    return 0;
}
