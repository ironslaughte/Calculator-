#pragma once
template <typename T>
class Stack {
public:
    Stack();
    T Top();
    T Pop();
    void Pop_Front();
    void Clear();
    void Push(const T number);
    size_t GetSize();
    bool IsEmpty();
    T& operator[] (size_t index);

private:


    template <typename T>
    class Elem {
    public:
        Elem* pnext;
        T Num;
        Elem(const T& s = T(), Elem* pNext = nullptr) {
            this->Num = s;
            this->pnext = pNext;
        }
    };
    size_t Size;
    Elem<T> *head;
};


template <typename T>
Stack<T>::Stack() {
    Size = 0;
    head = nullptr;
}

template <typename T>
T Stack<T>::Top() {
    return head->Num;
}
template <typename T>
T Stack<T>::Pop() {
    T data = head->Num;
    Pop_Front();
    return data;
}
template <typename T>
void Stack<T>::Pop_Front()
{    
    Elem<T>* tmp = head;
    head = head->pnext;
    delete tmp;
    Size--;
}

template <typename T>
void Stack<T>::Push(const T number) {
    head = new Elem<T>(number, head);
    Size++;
}

template <typename T>
void Stack<T>::Clear()
{
    while (Size > 0) {
        Pop_Front();
    }
}


template <typename T>
size_t Stack<T>::GetSize()
{
    return Size;
}

template <typename T>
T& Stack<T>::operator[](size_t index)
{
    int count = 0;
    Elem<T>* cur = head;
    while (cur != nullptr) {
        if (count == index) {
            return cur->Num;
        } cur = cur->pnext;
        count++;
    }
}

template <typename T>
bool Stack<T>::IsEmpty() {

    return Size == 0;
}
