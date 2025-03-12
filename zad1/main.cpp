
#include <iostream>
#include <iomanip>
#include <limits>
#include <cstring>



class longInt {
private:
    int _sign;
    size_t _size;
    unsigned int *_digits;


public:

    longInt(int const *digs, size_t size) : _digits(nullptr), _size(0) {
        if (digs == nullptr) throw std::logic_error("pointer cant be nullptr");
        if (size <= 0) throw std::logic_error("size cant be GT 0");
        _sign = digs[size - 1];
        if (size != 1) {
            _digits = new unsigned int[size - 1];
            memcpy(_digits, digs, sizeof(unsigned int) * (size - 1));
            _size = size - 1;
        }


    }

    longInt(longInt const &other) : _size(other._size), _sign(other._sign), _digits(nullptr) {
        if (other._digits != nullptr) {
            _digits = new unsigned int[_size];
            memcpy(_digits, other._digits, sizeof(unsigned int) * _size);
        }

    }
    ~longInt() {
        delete[] _digits;
    }


    unsigned int getDigit(int position) const noexcept
    {
        if (_digits == nullptr)
        {
            return position == 0
                   ? _sign
                   : 0;
        }

        size_t const digits_count = _size;
        if (position < digits_count){
            return _digits[position];
        }

        if (position == digits_count){
            return _sign;
        }

        return 0;
    }

    bool isNegative() const {
        return (*reinterpret_cast<unsigned int const *>(&_sign) >> ((sizeof(int) << 3) - 1)) == 1;
    }

    longInt &changeSign() {
        _sign ^= (1 << ((sizeof(int) << 3) - 1));
        return *this;
    }

    int getAbsSign() const {
        return isNegative() ? _sign ^ (1 << ((sizeof(int) << 3) - 1)) : _sign;
    }

    bool isZero() const {
        return _sign == 0 && _digits == nullptr;
    }


    longInt &operator=(longInt const &other) {
        if (this != &other) {
            delete[] _digits;
            _sign = other._sign;
            _size = other._size;
            _digits = nullptr;
            if (other._digits != nullptr) {
                _digits = new unsigned int[_size];
                memcpy(_digits, other._digits, sizeof(unsigned int) * _size);
            }
        }
        return *this;
    }


    longInt &operator+=(longInt const &other) {
        if(this == &other){
            if (other.isZero())
            {
                return *this;
            }

            if (isZero())
            {
                return *this = other;
            }

            if (isNegative())
            {
                changeSign();
                *this += -other;
                return changeSign();
            }

            if (other.isNegative())
            {
                return *this -= -other;
            }
        auto const first_value_digits_count = _size + 1;
        auto const second_value_digits_count = other._size + 1;
        auto const digits_count = std::max(first_value_digits_count, second_value_digits_count);

        unsigned int operation_result = 0;

        constexpr int shift = sizeof(unsigned int) << 2;
        constexpr int mask = (1 << shift) - 1;

        unsigned int *result_digits = new unsigned int[digits_count+1];
        for (int i = 0; i < digits_count; ++i)
        {
            unsigned int first_value_digit = getDigit(i);
            unsigned int second_value_digit = other.getDigit(i);
            result_digits[i] = 0;

            for (int j = 0; j < 2; ++j)
            {
                operation_result += (first_value_digit & mask) + (second_value_digit & mask);
                first_value_digit >>= shift;
                second_value_digit >>= shift;
                *reinterpret_cast<unsigned int *>(&result_digits[i]) |= ((operation_result & mask) << shift * j);
                operation_result >>= shift;
            }
        }

        auto result_digits_count = result_digits.size();

        if (operation_result == 1)
        {
            if ((*reinterpret_cast<unsigned int *>(&result_digits[digits_count - 1]) >> ((sizeof(unsigned int) << 3) - 1)) == 0)
            {
                *reinterpret_cast<unsigned int *>(&result_digits[digits_count - 1]) |= (1u << ((sizeof(unsigned int) << 3) - 1));
            }
            else
            {
                result_digits.back() = 1;
            }
        }
        else if ((*reinterpret_cast<unsigned int *>(&result_digits[digits_count - 1]) >> ((sizeof(unsigned int) << 3) - 1)) == 0)
        {
            --result_digits_count;
        }

        return *this = longInt(normalDigit(result_digits));
    }


}









    longInt operator-() const{
        return longInt(*this).changeSign();
    }


    bool operator >(longInt const &other) const {
        if(isNegative() != other.isNegative()){
            return !isNegative();
        }

        if(_size != other._size){
            return _size > other._size;
        }

        if(getAbsSign() != other.getAbsSign()){
            return getAbsSign() > other.getAbsSign();
        }

        for(size_t i = _size; i > 0; i--){
            if(_digits[i-1] != other._digits[i-1]){
                return _digits[i-1] > other._digits[i-1];
            }
        }

        return false;
    }




    bool operator>=(longInt const &other) const{
        if(*this > other || *this == other) return true;
        return false;
    }


    bool operator<(longInt const &other) const{
        if(!(*this > other) && !(*this == other)) return true;
        return false;
    }


    bool operator<=(longInt const &other) const{
        if(*this < other || *this == other) return true;
        return false;
    }



    bool operator==(longInt const &other) const {
        if(_size != other._size || _sign != other._sign) {
            return false;
        }

        for(size_t i = 0; i < _size; i++) {
            if(_digits[i] != other._digits[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(longInt const &other) const{
        if(!(*this == other)) return true;

        return false;
    }

    longInt &operator-=(longInt const &other){

    }
};


int main(){
    int dig[] = {100000000};
    int dig2[] = {9};
    longInt a(dig,1), b(dig2, 1);
    for(int i = 0; i < 10; i++){
        a += a;
        a.print();
    }
    a.print();



}



