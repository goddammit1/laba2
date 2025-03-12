#include <iostream>
#include <cstdarg>


class array{
private:
    int* arr;
    int _size;

public:
    array(int size, ...){
        _size = size;
        arr = new int[_size];
        va_list args;
        va_start(args, _size);
        for(int i = 0; i < _size; i++){
            arr[i] = va_arg(args, int);
        }
        va_end(args);
    }

    array(const array& other){
        _size = other._size;
        arr = new int[_size];
        std::copy(other.arr, other.arr + _size, arr);
    }

    array& operator=(const array& other){
        if(this != &other){
            delete[] arr;
            _size = other._size;
            arr = new int[_size];
            std::copy(other.arr, other.arr + _size, arr);
        }
        return *this;
    }

    ~array(){
        delete[] arr;
    }


    int& operator[](int index){
        return arr[index];
    }

    array& operator&=(const array& other){
        for(int i = 0; i < _size; i++){
            if(arr[i] != other.arr[i]){
                arr[i] = arr[_size - 1];
                _size--;
                i--;
            }
        }
        return *this;
    }

    array operator&(const array& other) {
        array res(*this &= other);
        return res;
    }

   friend std::ostream& operator<<(std::ostream& stream, array& obj){
        for(int i = 0; i < obj._size; i++){
            stream << obj.arr[i];
        }
       return stream;
    }
};




int main() {
    array a(3, 1, 2, 3);
    array b(3, 3, 2, 5);
    std::cout << a <<  " " << std::endl;
    std::cout << b << " " << std::endl;
    array c = a & b;
    std::cout << c << " " << std::endl;



    return 0;
}
