#pragma once
template <class T>
class Elem {
public:
    Elem* pnext;
    T Num;
    Elem(const T& s = T(), Elem* pNext = nullptr) {
        Num = s;
        pnext = pNext;
    }
};