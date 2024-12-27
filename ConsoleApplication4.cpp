#include <iostream>
#include <string>
#include <algorithm>

class BigInt {
public:
    BigInt() : number("0"), sign(false) {}
    BigInt(int num);
    BigInt(long num);
    BigInt(long long num);
    BigInt(const std::string& str);

    BigInt& operator=(int num);
    BigInt& operator=(long num);
    BigInt& operator=(long long num);
    BigInt& operator=(const std::string& str);

    BigInt operator-() const;

    friend BigInt operator+(const BigInt& left, const BigInt& right);
    friend BigInt operator-(const BigInt& left, const BigInt& right);
    friend BigInt operator*(const BigInt& left, const BigInt& right);
    friend bool operator==(const BigInt& left, const BigInt& right);
    friend bool operator!=(const BigInt& left, const BigInt& right);
    friend bool operator<(const BigInt& left, const BigInt& right);
    friend bool operator<=(const BigInt& left, const BigInt& right);
    friend bool operator>(const BigInt& left, const BigInt& right);
    friend bool operator>=(const BigInt& left, const BigInt& right);
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);

private:
    std::string number;
    bool sign;

    void trim();
};

BigInt::BigInt(int num) {
    if (num >= 0) {
        sign = false;
        number = std::to_string(num);
    }
    else {
        sign = true;
        number = std::to_string(-num);
    }
}

BigInt::BigInt(long num) {
    if (num >= 0) {
        sign = false;
        number = std::to_string(num);
    }
    else {
        sign = true;
        number = std::to_string(-num);
    }
}

BigInt::BigInt(long long num) {
    if (num >= 0) {
        sign = false;
        number = std::to_string(num);
    }
    else {
        sign = true;
        number = std::to_string(-num);
    }
}

BigInt::BigInt(const std::string& str) {
    if (str.length() == 0) {
        number = "0";
        sign = false;
    }
    else if (str.length() == 1 && (str[0] == '+' || str[0] == '-')) {
        number = "0";
        sign = false;
    }
    else {
        if (str[0] == '-') {
            sign = true;
            number = str.substr(1);
        }
        else {
            sign = false;
            number = str;
        }
        trim();
    }
}

BigInt& BigInt::operator=(int num) {
    if (num >= 0) {
        sign = false;
        number = std::to_string(num);
    }
    else {
        sign = true;
        number = std::to_string(-num);
    }
    return *this;
}

BigInt& BigInt::operator=(long num) {
    if (num >= 0) {
        sign = false;
        number = std::to_string(num);
    }
    else {
        sign = true;
        number = std::to_string(-num);
    }
    return *this;
}

BigInt& BigInt::operator=(long long num) {
    if (num >= 0) {
        sign = false;
        number = std::to_string(num);
    }
    else {
        sign = true;
        number = std::to_string(-num);
    }
    return *this;
}

BigInt& BigInt::operator=(const std::string& str) {
    if (str.length() == 0) {
        number = "0";
        sign = false;
    }
    else if (str.length() == 1 && (str[0] == '+' || str[0] == '-')) {
        number = "0";
        sign = false;
    }
    else {
        if (str[0] == '-') {
            sign = true;
            number = str.substr(1);
        }
        else {
            sign = false;
            number = str;
        }
        trim();
    }
    return *this;
}

BigInt BigInt::operator-() const {
    BigInt result = *this;
    result.sign = !sign;
    return result;
}

void BigInt::trim() {
    while (number.size() > 1 && number[0] == '0') {
        number.erase(0, 1);
    }
    if (number == "0") {
        sign = false;
    }
}

BigInt operator+(const BigInt& left, const BigInt& right) {
    BigInt result;
    if (left.sign == right.sign) {
        result.sign = left.sign;
        int carry = 0;
        int i = left.number.size() - 1;
        int j = right.number.size() - 1;
        while (i >= 0 || j >= 0 || carry != 0) {
            int sum = carry;
            if (i >= 0) {
                sum += left.number[i] - '0';
                i--;
            }
            if (j >= 0) {
                sum += right.number[j] - '0';
                j--;
            }
            result.number.insert(0, std::to_string(sum % 10));
            carry = sum / 10;
        }
    }
    else {
        if (left.sign) {
            result = right - (-left);
        }
        else {
            result = left - (-right);
        }
    }
    return result;
}

BigInt operator-(const BigInt& left, const BigInt& right) {
    BigInt result;
    if (left.sign == right.sign) {
        if (left < right) {
            result = -(right - left);
        }
        else {
            result.sign = left.sign;
            int borrow = 0;
            int i = left.number.size() - 1;
            int j = right.number.size() - 1;
            while (i >= 0 || j >= 0) {
                int diff = borrow;
                if (i >= 0) {
                    diff += left.number[i] - '0';
                    i--;
                }
                if (j >= 0) {
                    diff -= right.number[j] - '0';
                    j--;
                }
                borrow = 0;
                if (diff < 0) {
                    borrow = 1;
                    diff += 10;
                }
                result.number.insert(0, std::to_string(diff));
            }
            result.trim();
        }
    }
    else {
        if (left.sign) {
            result = -(-left + right);
        }
        else {
            result = left + (-right);
        }
    }
    return result;
}
BigInt operator*(const BigInt& left, const BigInt& right) {
    BigInt result;
    if (left.number == "0" || right.number == "0") {
        return result;
    }
    result.number.resize(left.number.size() + right.number.size(), '0');
    for (int i = left.number.size() - 1; i >= 0; i--) {
        int carry = 0;
        for (int j = right.number.size() - 1; j >= 0; j--) {
            int product = (left.number[i] - '0') * (right.number[j] - '0') + carry + (result.number[i + j + 1] - '0');
            result.number[i + j + 1] = product % 10 + '0';
            carry = product / 10;
        }
        result.number[i] += carry;
    }
    result.trim();
    if (left.sign != right.sign) {
        result.sign = true;
    }
    return result;
}

bool operator==(const BigInt& left, const BigInt& right) {
    return (left.sign == right.sign) && (left.number == right.number);
}

bool operator!=(const BigInt& left, const BigInt& right) {
    return !(left == right);
}

bool operator<(const BigInt& left, const BigInt& right) {
    if (left.sign != right.sign) {
        return left.sign;
    }
    if (left.number.size() != right.number.size()) {
        return (left.number.size() < right.number.size()) ^ left.sign;
    }
    for (size_t i = 0; i < left.number.size(); i++) {
        if (left.number[i] != right.number[i]) {
            return (left.number[i] < right.number[i]) ^ left.sign;
        }
    }
    return false;
}

bool operator<=(const BigInt& left, const BigInt& right) {
    return (left == right) || (left < right);
}

bool operator>(const BigInt& left, const BigInt& right) {
    return !(left <= right);
}

bool operator>=(const BigInt& left, const BigInt& right) {
    return !(left < right);
}

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    if (num.sign) {
        os << '-';
    }
    os << num.number;
    return os;
}


int main() {
    BigInt a, b, c;

    a = "12345670";
    b = "9876576210";
    c = a + b;
    std::cout << a << " + " << b << " = " << c << std::endl;

    a = "1234567012";
    b = "987654323210";
    c = a - b;
    std::cout << a << " - " << b << " = " << c << std::endl;

   
    a = "12345678890";
    b = "987654210";
    c = a * b;
    std::cout << a << " * " << b << " = " << c << std::endl;

   
    a = "123456890";
    b = "12345690";
    std::cout << a << " == " << b << " is " << (a == b) << std::endl;

    
    a = "123452890";
    b = "98765432210";
    std::cout << a << " < " << b << " is " << (a < b) << std::endl;

    a = "1234567867890";
    b = "98765432143210";
    std::cout << a << " > " << b << " is " << (a > b) << std::endl;

    return 0;
}
