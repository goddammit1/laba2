
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

    void print() {
        if (_digits != nullptr) {
            for (size_t i = 0; i < _size; i++) {
                std::cout << _digits[i] << ' ';
            }
        }
        std::cout << _sign << std::endl;
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
        if (this == &other) {
            *this += longInt(*this);
            return *this;
        }

        if (isNegative() != other.isNegative()) {
            if (isNegative()) {
                longInt tmp = *this;
                tmp.changeSign();
                *this = other;
                *this -= tmp;
            } else {
                longInt tmp = other;
                tmp.changeSign();
                *this -= tmp;
            }
            return *this;
        }

        unsigned int carry = 0;
        size_t new_size = std::max(_size, other._size) + 1;
        unsigned int *new_digits = new unsigned int[new_size];
        memset(new_digits, 0, sizeof(unsigned int) * new_size);

        for (size_t i = 0; i < new_size - 1; i++) {
            unsigned int sum = (i < _size ? _digits[i] : 0) + (i < other._size ? other._digits[i] : 0) + carry;
            new_digits[i] = sum % (1u << (sizeof(unsigned int) * 8));
            carry = sum / (1u << (sizeof(unsigned int) * 8));
        }

        new_digits[new_size - 1] = _sign + other._sign + carry;

        delete[] _digits;
        _sign = new_digits[new_size - 1];
        _size = new_size - 1;
        _digits = new unsigned int[_size];
        for (size_t i = 0; i < _size; i++) {
            _digits[i] = new_digits[i];
        }

        delete[] new_digits;

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
    int dig[] = {11, 22};
    int dig2[] = {23, 32};
    longInt a(dig,2), b(dig2, 2);
    a.print();
    a += b;
    a.print();

}


