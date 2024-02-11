#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
class MyContainer {
public:
    MyContainer() { size_ = 0; }
    MyContainer(vector<T> data_, int size_) { 
        this->data_ = data_;
        this->size_ = size_;
    }
    MyContainer(const MyContainer& mc) {
        this->data_ = mc.data_;
        this->size_ = mc.size_;
    }
    ~MyContainer(){}

    void Add(const T& item) {
        data_.push_back(item);
        size_++;
    }

    T Get(int index) const {
        if (index < 0 || index >= size_) {
            throw out_of_range("Index out of range");
        }
        return data_[index];
    }

    int Size() const {
        return size_;
    }

   T operator[](int i) {
        return data_[i];
    }

private:
    vector<T> data_;
    int size_;
};

template <typename T>
class list {
private:
    struct node {
        T value;
        node* next;
    };
    node* head = nullptr;
    int size = 0;
public:
    list()
    {
        head = nullptr;
        size = 0;
    }
    list(int size)
    {
        head = nullptr;
        this->size = size;
    }
    list(const list& other)
    {
        head = other.head;
        size = other.size;
    }
    ~list()
    {
        node* current = head;
        while (current != nullptr)
        {
            node* next = current->next;
            delete current;
            current = next;
        }
    }
    void push(T x)
    {
        node* new_node = new node;
        new_node->value = x;
        new_node->next = head;
        head = new_node;
        size++;
    }
    void pop()
    {
        node* temp = head;
        head = head->next;
        delete temp;
        size--;
    }
    T front()
    {
        return head->value;
    }
    bool is_empty()
    {
        return size == 0;
    }
    void print()
    {
        node* temp = head;
        while (temp != nullptr)
        {
            cout << temp->value;
            temp = temp->next;
        }
        cout << endl;
    }
    T operator[](int i)
    {
        node* temp = head;
        for (int j = 0; j < i; j++)
        {
            temp = temp->next;
        }
        return temp->value;
    }

};

template <typename T>
class stack
{
private:
    vector <T> array;
    int head = -1;
public:
    stack()
    {
        array.resize(0);
    }
    stack(int size)
    {
        array.resize(size);
    }
    stack(const stack& other)
    {
        array = other.array;
        head = other.head;
    }
    ~stack() {}
    void push(T x)
    {
        head++;
        array.push_back(x);
    }
    void pop()
    {
        head--;
        array.pop_back();
    }
    T front()
    {
        return array[head];
    }
    bool is_empty()
    {
        return head == -1;
    }
    void print()
    {
        int l = array.size();
        for (int i = 0; i < l; i++)
        {
            cout << array[i];
        }
        cout << endl;
    }
    T operator[](int i)
    {
        return array[array.size() - i - 1];
    }
};

int main() {
    MyContainer<int> container;
    container.Add(1);
    container.Add(2);
    container.Add(3);
    cout << "size: " << container.Size() << endl << container.Get(1) << " " << container[2] << endl;

    list <char> l;
    l.push('a');
    l.push('b');
    l.push('c');
    l.print();
    l.pop();
    l.print();
    cout << l.front() << endl;
    cout << l[1] << endl;
    cout << l.is_empty() << endl;

    stack <int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.print();
    s.pop();
    s.print();
    cout << s.front() << endl;
    cout << s[1] << endl;
    cout << s.is_empty() << endl;
}