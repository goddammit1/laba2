
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


    unsigned int *splitDigits(size_t &newSize) const {
        newSize = _size * 2;
        unsigned int *result = new unsigned int[newSize];
        size_t j = 0;

        for (size_t i = 0; i < _size; i++) {
            result[j++] = _digits[i] & 0xFFFF; // младшая половинка
            result[j++] = (_digits[i] >> 16) & 0xFFFF; // старшая половинка
        }

        return result;
    }

    void print() {
        if (_digits == nullptr) {
            std::cout << "0" << std::endl;
            return;
        }

        if (isNegative()) {
            changeSign();
            std::cout << "-";
        }


        std::cout << _sign << ", ";

        // Выводим младшие разряды
        for (size_t i = 0; i < _size - 1; i++) {
            std::cout << _digits[i] << ", ";
        }

        std::cout << _digits[_size - 1] << std::endl;
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
        if (isZero() && other.isZero()) {
            return *this;
        }

        if (isZero()) {
            return *this = other;
        }

        if (other.isZero()) {
            return *this;
        }

        size_t newSize1, newSize2;
        unsigned int *digits1 = splitDigits(newSize1);
        unsigned int *digits2 = other.splitDigits(newSize2);

        size_t maxSize = std::max(newSize1, newSize2);
        unsigned int *result = new unsigned int[maxSize + 1];
        std::fill(result, result + maxSize + 1, 0);

        unsigned int carry = 0;
        for (size_t i = 0; i < maxSize; i++) {
            unsigned int sum = (i < newSize1 ? digits1[i] : 0) + (i < newSize2 ? digits2[i] : 0) + carry;
            result[i] = sum & 0xFFFF;
            carry = sum >> 16;
        }

        _sign = _sign + other._sign + carry; // Учтем возможные переносы

        delete[] _digits;
        _digits = result;
        _size = maxSize + 1;

        delete[] digits1;
        delete[] digits2;

        return *this;
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
    int dig[] = {111123, 122111};
    int dig2[] = {123111, 113233};
    longInt a(dig,2), b(dig2, 2);
    a.print();
    a += b;
    a.print();

}


