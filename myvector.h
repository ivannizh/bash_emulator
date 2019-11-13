#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <iostream>
#include <vector>
#include <exception>

template <class T>
class MyIter;

template <class T>
class MyVector {
private:
    static const int QUOTA = 10;
    int cnt;
    int cur;
    T* arr;

public:
    MyVector() : cnt(0), cur(0), arr(nullptr) { }

    MyVector(const MyVector &a) :cnt(a.cnt), cur(a.cur), arr(new T[a.cnt]) {
        for (int i = 0; i < cur; ++i)
            arr[i] = a.arr[i];
    }

    MyVector(MyVector &&a) :cnt(a.cnt), cur(a.cur), arr(a.arr) {
        a.arr = nullptr;
    }

    MyVector& operator= (const MyVector &a) {
        if (this != &a) {
            delete[] arr;
            cur = a.cur;
            cnt = a.cnt;
            arr = new T[cnt];
            for (int i = 0; i < cur; ++i)
                arr[i] = a.arr[i];
        }
        return *this;
    }

    MyVector& operator =(MyVector &&a) {
        if (this != &a){
            int tmp = cur;
            cur = a.cur;
            a.cur = tmp;
            tmp = cnt;
            cnt = a.cnt;
            a.cnt = tmp;
            T* tarr = arr;
            arr = a.arr;
            a.arr = tarr;
        }
        return *this;
    }

    ~MyVector() {
        delete[] arr;
    }

    T& operator[] (int index) {
        if(index < 0 || index >= cur)
            throw std::out_of_range("");
        return arr[index];
    }

    const T& operator[](int index) const {
        if(index < 0 || index >= cur)
            throw std::out_of_range("");
        return arr[index];
    }

    size_t size() const { return cur; }

    friend class MyIter<T>;
    typedef MyIter<T> iterator;


    iterator begin() const{
        return iterator(arr);
    }

    iterator end() const {
        return iterator(arr + cur);
    }

    void clear() {
        cur = 0;
        cnt = 0;
        delete[] arr;
        arr = nullptr;
    }

    void push_back(const T &t){
        if(cur + 1 >= cnt){
            T* tmp = new T[cnt + QUOTA];
            for(int i = 0; i < cur; ++i)
                tmp[i] = arr[i];
            cnt += QUOTA;
            delete[] arr;
            arr = tmp;
        }
        arr[cur] = t;
        ++cur;
    }

    T back() {
        return *(iterator(arr + cur - 1));
    }

    MyVector& erase(const iterator &it){
        for(int i = 0; i < cur; i++){
            if(it == &arr[i]){
                T tmp;
                for(int j = i; j < cur -1; j++){
                    tmp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = tmp;
                }
                --cur;
                break;
            }
        }
        return *this;
    }

};

template <class T>
class MyIter{
private:
    T* cur;
public:

    MyIter() :cur(nullptr){}
    MyIter(T *it) :cur(it){}

    MyIter operator-(int index){
        return MyIter(cur - index);
    }

    MyIter operator+(int index){
        return MyIter(cur + index);
    }

    T* operator->() const {
        return cur;
    }

    T& operator* () const {
        if(cur)
            return *cur;
        throw std::runtime_error("illegal value for MyVector Iterator");
    }

    int operator !=(const MyIter& it) const {
        return cur != it.cur;
    }

    int operator == (const T* t) const{
        return t == cur;
    }

    int operator ==(const MyIter& it) const {
        return cur == it.cur;
    }

    MyIter& operator++() {
        ++cur;
        return *this;
    }

    MyIter operator++(int) {
        MyIter res;
        res.cur = cur;
        ++cur;
        return res;
    }

    MyIter & operator--() {
        ++cur;
        return *this;
    }

    MyIter operator--(int) {
        MyIter res;
        res.cur = cur;
        ++cur;
        return res;
    }
};

#endif // MYVECTOR_H
