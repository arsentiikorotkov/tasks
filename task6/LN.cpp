
#include "LN.h"
#include <string_view>
#include <string>
#include <cstdlib>
#include <climits>

bool LN::get_sign() {return this->sign_;}
bool LN::isNaN() {return !this->isNum_;}
int LN::get_size() {return this->size_;}
int LN::get_i_digit(int num) {return this->number_[num];}

LN::LN(const char *num) {
    if (num[0] == '-') {
        this->sign_ = false;
    } else {
        this->sign_ = true;
    }
    this->isNum_ = true;
    int num_size = 0;
    std::string str;
    while (num[num_size] != 0) {
        str += num[num_size];
        num_size++;
    }
    int i;
    if (this->sign_) {
        this->size_ = num_size;
        i = 0;
    } else {
        this->size_ = num_size - 1;
        i = 1;
    }
    this->number_ = (int *)malloc(num_size * sizeof(int));
    if (this->number_ == nullptr) {
        throw 2;
    }
    for (int j = i; j < num_size; j++) {
        this->number_[j] = (str[num_size - 1 - j] % 48);
    }
}

LN::LN(std::string_view num) {
    int i;
    if (num[0] == '-') {
        this->sign_ = false;
        this->size_ = num.size() - 1;
        i = 1;
    } else {
        this->sign_ = true;
        this->size_ = num.size();
        i = 0;
    }
    this->isNum_ = true;
    this->number_ = (int *)malloc(num.size() * sizeof(int));
    if (this->number_ == nullptr) {
        throw 2;
    }
    for (int j = i; j < num.size(); j++) {
        this->number_[j] = num[num.size() - 1 - j] % 48;
    }
}

LN::LN(long long num) {
    if (num == 0) {
        this->sign_ = true;
        this->isNum_ = true;
        this->size_ = 1;
        this->number_ = (int *)malloc(sizeof(int));
        if (this->number_ == nullptr) {
            throw 2;
        }
        this->number_[0] = 0;
    }
    if (num < 0) {
        this->sign_ = false;
        num *= -1;
    } else {
        this->sign_ = true;
    }
    this->isNum_ = true;
    long long c = num;
    int size_num = 0;
    while (c != 0) {
        c /= 10;
        size_num++;
    }
    this->size_ = size_num;
    this->number_ = (int *)malloc(size_num * sizeof(int));
    if (this->number_ == nullptr) {
        throw 2;
    }
    for (int i = 0; i < size_num; i++){
        this->number_[i] = num % 10;
        num /= 10;
    }
}

LN operator ""_ln(const char *s) {
    return LN(s);
}

LN::LN(const LN &ln) {
    this->sign_ = ln.sign_;
    this->isNum_ = ln.isNum_;
    this->size_ = ln.size_;
    this->number_ = (int *)malloc(ln.size_ * sizeof(int));
    if (this->number_ == nullptr) {
        throw 2;
    }
    for (int i = 0; i < ln.size_; i++) {
        this->number_[i] = ln.number_[i];
    }
}

LN::LN(LN &&ln) noexcept {
    if (this != &ln) {
        this->sign_ = ln.sign_;
        this->isNum_ = ln.isNum_;
        this->size_ = ln.size_;
        this->number_ = ln.number_;
        ln.number_ = nullptr;
        ln.size_ = 0;
    }
}

LN& LN::operator=(const LN &ln) {
    if (this == &ln) {
        return *this;
    }
    this->sign_ = ln.sign_;
    this->isNum_ = ln.isNum_;
    this->size_ = ln.size_;
    free(number_);
    this->number_ = (int *)malloc(ln.size_ * sizeof(int));
    if (this->number_ == nullptr) {
        throw 2;
    }
    for (int i = 0; i < ln.size_; i++) {
        this->number_[i] = ln.number_[i];
    }
    return *this;
}

LN& LN::operator=(LN &&ln) noexcept {
    if (this != &ln) {
        this->sign_ = ln.sign_;
        this->isNum_ = ln.isNum_;
        this->size_ = ln.size_;
        this->number_ = ln.number_;
        ln.number_ = nullptr;
        ln.size_ = 0;
        return *this;
    }
}

LN::~LN() {
    free(this->number_);
}

int LN::compare (const LN &a, const LN &b) {
    if (a.sign_ && b.sign_) {
        if (a.size_ > b.size_) {
            return 1;
        } else if (a.size_ < b.size_) {
            return -1;
        } else if (a.size_ == b.size_) {
            for (int i = 0; i < a.size_; i++) {
                if (a.number_[i] > b.number_[i]) {
                    return 1;
                } else if (a.number_[i] < b.number_[i]) {
                    return -1;
                }
            }
            return 0;
        }
    } else if (a.sign_ && !b.sign_) {
        return 1;
    } else if (!a.sign_ && b.sign_) {
        return -1;
    } else {
        if (a.size_ > b.size_) {
            return -1;
        } else if (a.size_ < b.size_) {
            return 1;
        } else if (a.size_ == b.size_) {
            for (int i = 0; i < a.size_; i++) {
                if (a.number_[i] > b.number_[i]) {
                    return -1;
                } else if (a.number_[i] < b.number_[i]) {
                    return 1;
                }
            }
            return 0;
        }
    }
}

int LN::compareAbs (const LN &a, const LN &b) {
    if (a.sign_ && b.sign_) {
        return LN::compare(a, b);
    } else if (!a.sign_ && b.sign_) {
        return LN::compare(-a, b);
    } else if (a.sign_ && !b.sign_) {
        return LN::compare(a, -b);
    } else {
        return LN::compare(-a, -b);
    }
}

bool operator==(const LN &a, const LN &b) {
    if (!a.isNum_ || !b.isNum_) {
        return false;
    }
    if (LN::compare(a, b) == 0) {
        return true;
    } else {
        return false;
    }
}

bool operator!=(const LN &a, const LN &b) {
    if (!a.isNum_ && !b.isNum_) {
        return true;
    } else if (!a.isNum_ || !b.isNum_) {
        return false;
    }
    return !(a == b);
}

bool operator>(const LN &a, const LN &b) {
    if (!a.isNum_ || !b.isNum_) {
        return false;
    }
    if (LN::compare(a, b) == 1) {
        return true;
    } else {
        return false;
    }
}

bool operator<=(const LN &a, const LN &b) {
    if (!a.isNum_ || !b.isNum_) {
        return false;
    }
    return !(a > b);
}

bool operator<(const LN &a, const LN &b) {
    if (!a.isNum_ || !b.isNum_) {
        return false;
    }
    if (LN::compare(a, b) == -1) {
        return true;
    } else {
        return false;
    }
}

bool operator>=(const LN &a, const LN &b) {
    if (!a.isNum_ || !b.isNum_) {
        return false;
    }
    return !(a < b);
}

LN::operator bool() const {
    if (((this->size_ == 1) && (this->number_[0] == 0)) || !(this->isNum_)) {
        return false;
    } else {
        return true;
    }
}

LN::operator long long() const {
    if (!this->isNum_) {
        throw 1;
    } else {
        if (this->sign_) {
            if (*this <= LN(LLONG_MAX)) {
                long long res = 0;
                for (int i = this->size_ - 1; i >= 0; i--) {
                    res = (res * 10) + this->number_[i];
                }
                return res;
            } else {
                throw 1;
            }
        } else {
            if (*this >= LN(LLONG_MIN)) {
                long long res = 0;
                for (int i = this->size_ - 1; i >= 0; i--) {
                    res = (res * 10) + this->number_[i];
                }
                res *= -1;
                return res;
            } else {
                throw 1;
            }
        }
    }
}


LN operator-(const LN &a) {
    LN res = a;
    res.sign_ = !res.sign_;
    return res;
}

LN LN::sum(const LN &a, const LN &b) {
    int ost = 0;
    for (int j = 0; j < std::max(a.size_, b.size_); j++) {
        ost /= 10;
        ost = ost + a.number_[j] + b.number_[j];
    }
    LN res;
    if (ost > 9) {
        res.size_ = std::max(a.size_, b.size_) + 1;
    } else {
        res.size_ = std::max(a.size_, b.size_);
    }
    res.sign_ = true;
    if (!a.isNum_ || !b.isNum_) {
        res.isNum_ = false;
    }
    free(res.number_);
    res.number_ = (int *)malloc(res.size_ * sizeof(int));
    if (res.number_ == nullptr) {
        throw 2;
    }
    ost = 0;
    for (int j = 0; j < res.size_ - 1; j++) {
        res.number_[j] = (ost + a.number_[j] + b.number_[j]) % 10;
        ost = (ost + a.number_[j] + b.number_[j]) / 10;
    }
    res.number_[res.size_ - 1] = ost;
    return res;
}

LN LN::sub (const LN &a, const LN &b) {
    LN a1 = a;
    if (a > b) {
        int c = 0;
        for (int j = 0; j < b.size_; j++) {
            if (a1.number_[j] < b.number_[j]) {
                int k = j + 1;
                while (a1.number_[k] == 0) {
                    k++;
                }
                a1.number_[k]--;
                for (int y = k - 1; y > j; y--) {
                    a1.number_[y] = 9;
                }
                a1.number_[j] = 10 + a1.number_[j] - b.number_[j];
            } else {
                a1.number_[j] = a1.number_[j] - b.number_[j];
            }
        }
        int d = a1.size_ - 1;
        while (a1.number_[d] == 0) {
            d--;
        }
        LN res;
        res.size_ = d + 1;
        res.sign_ = true;
        if (!a.isNum_ || !b.isNum_) {
            res.isNum_ = false;
        }
        free(res.number_);
        res.number_ = (int *)malloc(res.size_ * sizeof(int));
        if (res.number_ == nullptr) {
            throw 2;
        }
        for (int j = 0; j < res.size_; j++) {
            res.number_[j] = a1.number_[j];
        }
        return res;
    } else if (a < b) {
        return -sub(b, a);
    } else {
        return 0_ln;
    }
}

LN mul(const LN &a, int k) {
    LN res = 0_ln;
    for (int j = 0; j < k; j++) {
        res = res + res;
    }
    return res;
}

LN operator+(const LN &a, const LN &b) {
    if (a.sign_ && b.sign_) {
        return LN::sum(a, b);
    } else if (!a.sign_ && !b.sign_) {
        return -LN::sum(a, b);
    } else if (a.sign_ && !b.sign_) {
        return LN::sub(a, b);
    } else {
        return LN::sub(b, a);
    }
}

LN operator-(const LN &a, const LN &b) {
    return ((-b) + a);
}

LN operator*(const LN &a, const LN &b) {
    LN c = 0_ln;
    c.sign_ = !(a.sign_ ^ b.sign_);
    c.size_ = (a.size_ + 1) * (b.size_ + 1);
    if (!a.isNum_ || !b.isNum_) {
        c.isNum_ = false;
    }
    free(c.number_);
    c.number_ = (int *)malloc(c.size_ * sizeof(int));
    if (c.number_ == nullptr) {
        throw 2;
    }
    for (int j = 0; j < c.size_; j++) {
        c.number_[j] = 0;
    }
    for (int j = 0; j < b.size_; j++) {
        LN e = mul(a, b.number_[j]);
        int ost = 0;
        for (int y = j; y < j + e.size_; y++) {
            c.number_[y] = (c.number_[y] + e.number_[y - j] + ost) % 10;
            ost = (c.number_[y] + e.number_[y - j] + ost) / 10;
        }
    }
    int d = c.size_ - 1;
    while (c.number_[d] == 0) {
        d--;
    }
    LN res;
    res.size_ = d + 1;
    res.sign_ = c.sign_;
    res.isNum_ = c.isNum_;
    free(res.number_);
    res.number_ = (int *)malloc(res.size_ * sizeof(int));
    if (res.number_ == nullptr) {
        throw 2;
    }
    for (int j = 0; j < res.size_; j++) {
        res.number_[j] = c.number_[j];
    }
    return res;
}

LN operator/(const LN &a, const LN &b) {
    LN c;
    c.sign_ = (!(a.sign_ ^ b.sign_));
    if (!a.isNum_ || !b.isNum_ || b == 0_ln) {
        c.isNum_ = false;
        return c;
    } else if (b == 1_ln) {
        return a;
    } else if (b == -1_ln) {
        return -a;
    } else if (LN::compareAbs(a, b) == -1) {
        return 0_ln;
    } else if (LN::compareAbs(a, b) == 0) {
        if (!(a.sign_ ^ b.sign_)) {
            return 1_ln;
        } else {
            return -1_ln;
        }
    }

    free(c.number_);
    c.size_ = a.size_;
    c.number_ = (int *)malloc(c.size_ * sizeof(int));
    if (c.number_ == nullptr) {
        throw 2;
    }

    LN ost = 0_ln;
    for (int j = 0; j < a.size_; j++) {
        int k = a.size_ - j - 1;
        ost += LN((long long) (a.number_[k]));
        c.number_[k] = 0;
        while (LN::compareAbs(ost, b) != -1) {
            if (!b.sign_) {
                ost += c;
            } else {
                ost += -c;
            }
            c.number_[k]++;
        }
    }
    int d = c.size_ - 1;
    while (c.number_[d] == 0) {
        d--;
    }
    LN res;
    res.size_ = d + 1;
    res.sign_ = c.sign_;
    res.isNum_ = c.isNum_;
    free(res.number_);
    res.number_ = (int *)malloc(res.size_ * sizeof(int));
    if (res.number_ == nullptr) {
        throw 2;
    }
    for (int j = 0; j < res.size_; j++) {
        res.number_[j] = c.number_[j];
    }
    return res;
}

LN operator%(const LN &a, const LN &b) {
    if (!a.isNum_ || !b.isNum_ || b == 0_ln) {
        LN res;
        res.isNum_ = false;
        return res;
    }
    return a - a / b * b;
}

LN operator~(const LN &a) {
    LN res;
    if (!res.isNum_ || !res.sign_) {
        res.isNum_ = false;
        return res;
    }
    if (a == 0_ln) {
        return 0_ln;
    }
    LN l = 0_ln;
    LN r = a + 1_ln;
    while (r - l > 1_ln) {
        LN m = (l + r) / 2_ln;
        if (m * m <= a) {
            l = m;
        } else {
            r = m;
        }
    }
    return l;
}

LN &LN::operator+=(const LN &a) {
    LN c = *this + a;
    *this = c;
    return *this;
}

LN &LN::operator-=(const LN &a) {
    LN c = *this - a;
    *this = c;
    return *this;
}

LN &LN::operator*=(const LN &a) {
    LN c = *this * a;
    *this = c;
    return *this;
}

LN &LN::operator/=(const LN &a) {
    LN c = *this / a;
    *this = c;
    return *this;
}

LN &LN::operator%=(const LN &a) {
    LN c = *this % a;
    *this = c;
    return *this;
}


