#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include "LN.h"

int main(int argc, char *argv[])
{

    if (argc != 3) {
        std::cerr << "Specify only the file name, the name of the input and output files as arguments" << std::endl;
        return 1;
    }

    std::ifstream in;
    in.open(argv[1]);
    std::ofstream out;
    out.open(argv[2]);
    if (!in.is_open() || !out.is_open()) {
        std::cerr << "The file cannot be opened" << std::endl;
        return 1;
    }

    std::string s;
    std::stack<LN> stck;
    try {
        while (in >> s) {
            if (s == "==") {
                LN b = stck.top();
                stck.pop();
                LN a = stck.top();
                stck.pop();
                stck.push(LN((long long) (a == b)));
            } else if (s == "!=") {
                LN b = stck.top();
                stck.pop();
                LN a = stck.top();
                stck.pop();
                stck.push(LN((long long) (a != b)));
            } else if (s == "<") {
                LN b = stck.top();
                stck.pop();
                LN a = stck.top();
                stck.pop();
                stck.push(LN((long long) (a < b)));
            } else if (s == "<=") {
                LN b = stck.top();
                stck.pop();
                LN a = stck.top();
                stck.pop();
                stck.push(LN((long long) (a <= b)));
            } else if (s == ">") {
                LN b = stck.top();
                stck.pop();
                LN a = stck.top();
                stck.pop();
                stck.push(LN((long long) (a > b)));
            } else if (s == ">=") {
                LN b = stck.top();
                stck.pop();
                LN a = stck.top();
                stck.pop();
                stck.push(LN((long long) (a >= b)));
            }  else if (s == "+") {
                LN b = stck.top();
                stck.pop();
                LN a = stck.top();
                stck.pop();
                stck.push(a + b);
            } else if (s == "-") {
                LN b = stck.top();
                stck.pop();
                LN a = stck.top();
                stck.pop();
                stck.push(a - b);
            } else if (s == "*") {
                LN b = stck.top();
                stck.pop();
                LN a = stck.top();
                stck.pop();
                stck.push(a * b);
            } else if (s == "/") {
                LN b = stck.top();
                stck.pop();
                LN a = stck.top();
                stck.pop();
                stck.push(a / b);
            } else if (s == "%") {
                LN b = stck.top();
                stck.pop();
                LN a = stck.top();
                stck.pop();
                stck.push(a % b);
            } else if (s == "~") {
                LN a = stck.top();
                stck.pop();
                stck.push(~a);
            } else if (s == "_") {
                LN a = stck.top();
                stck.pop();
                stck.push(-a);
            } else {
                stck.push(LN(s));
            }
        }
    } catch (int e) {
        std::cerr << "Memory allocation error" << std::endl;
        in.close();
        while (!stck.empty()) {
            LN v = stck.top();
            if (v.isNaN()) {
                out << "NaN" << std::endl;
            } else {
                if (!v.get_sign()) {
                    out << '-';
                }
                for (int i = 0; i < v.get_size(); i++) {
                    out << v.get_i_digit(v.get_size() - i - 1);
                }
                out << std::endl;
                stck.pop();
            }
        }
        out.close();
        return e;
    }

    in.close();
    while (!stck.empty()) {
        LN v = stck.top();
        if (v.isNaN()) {
            out << "NaN" << std::endl;
        } else {
            if (!v.get_sign()) {
                out << '-';
            }
            for (int i = 0; i < v.get_size(); i++) {
                out << v.get_i_digit(v.get_size() - i - 1);
            }
            out << std::endl;
            stck.pop();
        }
    }
    out.close();

    return 0;
}
