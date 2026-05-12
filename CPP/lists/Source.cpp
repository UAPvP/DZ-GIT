#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

template<typename T>
class Node
{
    T data;
    Node<T>* next;
    Node<T>* prev;

public:
    Node<T>* GetNext() const { return next; }
    Node<T>* GetPrev() { return prev; }
    void SetNext(Node<T>* next) { this->next = next; }
    void SetPrev(Node<T>* prev) { this->prev = prev; }
    T& GetData() { return data; }
    const T& GetData() const { return data; }
    void SetData(T newData) { data = newData; }
    Node(T data, Node<T>* next = nullptr, Node<T>* prev = nullptr)
    {
        this->data = data;
        this->next = next;
        this->prev = prev;
    }
};

template<typename T>
class List
{
    Node<T>* head;
    Node<T>* tail;
    int size;
public:
    Node<T>* GetHead() const { return head; }
    Node<T>* GetTail() const { return tail; }
    int GetSize() const { return size; }

    Node<T>* At(const int pos) const
    {
        if (pos < 0 || pos >= size) return nullptr;
        if (pos <= size / 2)
        {
            Node<T>* curr = head;
            for (int i = 0; i < pos; i++)
            {
                curr = curr->GetNext();
            }
            return curr;
        }
        Node<T>* curr = tail;
        for (int i = size - 1; i > pos; i--)
        {
            curr = curr->GetPrev();
        }
        return curr;
    }

    T& operator[](const int pos)
    {
        return At(pos)->GetData();
    }

    const T& operator[](const int pos) const
    {
        return At(pos)->GetData();
    }

    void PushBack(T newData)
    {
        Push(newData, size);
    }

    void PushFront(T newData)
    {
        Push(newData, 0);
    }

    void Push(T newData, const int pos)
    {
        if (pos < 0 || pos > size) return;
        Node<T>* newNode = new Node<T>(newData);
        if (size <= 0)
        {
            head = tail = newNode;
        }
        else if (pos == 0)
        {
            newNode->SetNext(head);
            head->SetPrev(newNode);
            head = newNode;
        }
        else if (pos == size)
        {
            tail->SetNext(newNode);
            newNode->SetPrev(tail);
            tail = newNode;
        }
        else
        {
            Node<T>* curr = At(pos);
            Node<T>* prev = curr->GetPrev();
            prev->SetNext(newNode);
            newNode->SetPrev(prev);
            newNode->SetNext(curr);
            curr->SetPrev(newNode);
        }
        size++;
    }

    void PopBack()
    {
        Pop(size - 1);
    }

    void PopFront()
    {
        Pop(0);
    }

    void Pop(const int pos)
    {
        if (pos < 0 || pos >= size) return;
        if (size == 1)
        {
            delete head;
            head = tail = nullptr;
        }
        else if (pos == 0)
        {
            Node<T>* fordel = head;
            head = head->GetNext();
            head->SetPrev(nullptr);
            delete fordel;
        }
        else if (pos == size - 1)
        {
            Node<T>* fordel = tail;
            tail = tail->GetPrev();
            tail->SetNext(nullptr);
            delete fordel;
        }
        else
        {
            Node<T>* fordel = At(pos);
            Node<T>* prev = fordel->GetPrev();
            Node<T>* next = fordel->GetNext();
            prev->SetNext(next);
            next->SetPrev(prev);
            delete fordel;
        }
        size--;
    }

    List() : head(nullptr), tail(nullptr), size(0) {}

    ~List()
    {
        Node<T>* curr = head;

        for (int i = 0; i < size; i++)
        {
            Node<T>* fordel = curr;
            curr = curr->GetNext();

            delete fordel;
        }
    }
};
template <typename T>
class Stack
{
    List<T> list;

    const int max_size;

public:
    Stack() : max_size(-1){};
    Stack(int max_size) : max_size(max_size) {};

    void Push(const T& item) {
        if (max_size==-1 || list.GetSize() < max_size) {
            list.PushFront(item);
        }
        else {
            throw overflow_error("Stack is full!");
        }

    };
    T Pop() {
        if (Empty())
            throw out_of_range("Stack is empty");
        T val = list.GetHead()->GetData();
        list.PopFront();
        return val;
    };
    const T& Top() const {
        if (Empty())
            throw out_of_range("Stack is empty");
        return list.GetHead()->GetData();
    };
    bool Empty() const {
        return list.GetSize() == 0;
    };
    int Size() const {
        return list.GetSize();
    };
};
template<typename T>
class Queue
{
    List<T> list;

    bool isCyclical;

public:
    Queue(bool isCyclical) :isCyclical(isCyclical) {};

    void Push(const T& item) {
        list.PushBack(item);
    };
    T Pop() {
        if (Empty())
            throw out_of_range("Queue is empty");
        T item = list.GetHead()->GetData();
        list.PopFront();
        if (isCyclical) {
            list.PushBack(item);
        }
        return item;
    };
    const T& Front() const {
        if (Empty())
            throw out_of_range("Queue is empty");
        return list.GetTail()->GetData();
    };
    bool Empty() const {
        return list.GetSize() == 0;
    };
    int Size() const {
        return list.GetSize();
    };

};



int main()
{
    Stack<int> stack(5);
    stack.Push(10);
    stack.Push(20);
    stack.Push(30);

    cout << "===== STACK =====" << endl;
    cout << "Top: " << stack.Top() << endl;
    cout << "Size: " << stack.Size() << endl;
    while (!stack.Empty())
    {
        cout << "Pop: " << stack.Pop() << endl;
    }
    cout << endl;
    cout << "===== QUEUE =====" << endl;

    Queue<int> queue(false);
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);
    queue.Push(4);

    cout << "Front: " << queue.Front() << endl;
    cout << "Size: " << queue.Size() << endl;
    while (!queue.Empty())
    {
        cout << "Pop: " << queue.Pop() << endl;
    }
    cout << endl;
    cout << "===== CYCLICAL QUEUE =====" << endl;

    Queue<int> cyclicalQueue(true);
    cyclicalQueue.Push(100);
    cyclicalQueue.Push(200);
    cyclicalQueue.Push(300);

    for (int i = 0; i < 10; i++)
    {
        cout << cyclicalQueue.Pop() << endl;

        this_thread::sleep_for(chrono::milliseconds(500));
    }
    return 0;
}
