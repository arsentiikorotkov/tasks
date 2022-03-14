
#ifndef LAB4_LN_H
#define LAB4_LN_H

#include <string_view>
#include <stdexcept>
#include <string>
#include <cstdlib>

class LN {
public:

    bool get_sign();
    bool isNaN();
    int get_size();
    int get_i_digit(int num);

    explicit LN(const char *num);
    explicit LN(std::string_view num);
    explicit LN(long long num = 0);

    friend LN operator ""_ln(const char *s);

    LN(const LN& ln);
    LN(LN&& ln) noexcept ;
    LN &operator=(const LN& ln);
    LN &operator=(LN&& ln) noexcept ;

    ~LN();

    friend bool operator==(const LN& a, const LN& b);
    friend bool operator!=(const LN& a, const LN& b);
    friend bool operator>(const LN& a, const LN& b);
    friend bool operator<=(const LN& a, const LN& b);
    friend bool operator<(const LN& a, const LN& b);
    friend bool operator>=(const LN& a, const LN& b);

    explicit operator long long() const;
    explicit operator bool() const;

    friend LN operator-(const LN& a);
    friend LN operator+(const LN& a, const LN& b);
    friend LN operator-(const LN& a, const LN& b);
    friend LN operator*(const LN& a, const LN& b);
    friend LN operator/(const LN& a, const LN& b);
    friend LN operator%(const LN& a, const LN& b);
    friend LN operator~(const LN& a);

    LN &operator+=(const LN& a);
    LN &operator-=(const LN& a);
    LN &operator*=(const LN& a);
    LN &operator/=(const LN& a);
    LN &operator%=(const LN& a);

    static int compare(const LN &a, const LN &b);

private:
    int *number_;
    int size_;
    bool isNum_;
    //true - number, false - NaN
    bool sign_;
    //true - plus, false - minus

    static int compareAbs(const LN &a, const LN &b);

    static LN sum(const LN &a, const LN &b);

    static LN sub(const LN &a, const LN &b);
};
#endif 
