#include <iostream>
#include <functional>
#include <string>
#include <sstream>
#include <random>
#include <time.h>
#include <cmath>
#include <type_traits>
#include <cstdlib>
#include <ctime>
template <typename T>
struct node
{
    T data;
    node<T>* next;
    node<T>* prev;

    node(T data) : data(data), next(nullptr), prev(nullptr) {}
};

template <typename T>
struct list
{
    node<T>* head;
    node<T>* tail;
    int size;
    list() : head(nullptr), tail(nullptr), size(0) {}
    
    void addAtTail(T data)
    {
        node<T>* temp = new node<T>(data);

        if (size > 0)
        {
            tail->next = temp;
            temp->prev = tail;
            tail = temp;
        }
        else
        {
            head = temp;
            tail = temp;
        }
        size++;
    }
    
    void addAtHead(T data)
    {
        node<T>* temp = new node<T>(data);

        if (size > 0)
        {
            head->prev = temp;
            temp->next = head;
            head = temp;
        }
        else
        {
            head = temp;
            tail = temp;
        }
        size++;
    }
    
    void deleteAtTail()
    {
        if (size > 0)
        {
            node<T>* temp = tail;
            if (size > 1)
            {
                tail = tail->prev;
                tail->next = nullptr;
            }
            else
            {
                head = nullptr;
                tail = nullptr;
            }
            if constexpr (std::is_pointer<T>::value)
                delete temp->data;
            delete temp;
            size--;
        }
        else
        {
            return;
        }
    }
    
    void deleteAtHead()
    {
        if (size > 0)
        {
            node<T>* temp = head;
            if (size > 1)
            {
                head = head->next;
                head->prev = nullptr;
            }
            else
            {
                head = nullptr;
                tail = nullptr;
            }
            if constexpr (std::is_pointer<T>::value)
                delete temp->data;
            delete temp;
            size--;
        }
        else
        {
            return;
        }
    }

    
    void deleteAll()
    {
        node<T>* temp = head;
        while (temp != nullptr)
        {
            node<T>* next = temp->next;
            if constexpr (std::is_pointer<T>::value)
                delete temp->data;
            delete temp;
            temp = next;
        }
        head = tail = nullptr;
        size = 0;
    }

    
    void printList() const
    {
        node<T>* temp = head;
        if (size <= 10)
        {
            while (temp != nullptr)
            {
                if constexpr (std::is_pointer<T>::value) {
                    std::cout << "field_1: " << temp->data->getField1()
                        << ", field_2: " << temp->data->getField2() << std::endl;
                }
                else {
                    std::cout << "field_1: " << temp->data.getField1()
                        << ", field_2: " << temp->data.getField2() << std::endl;
                }
                temp = temp->next;
            }
        }
        else
        {
            for (int i = 0; i < 5; i++)
            {
                if constexpr (std::is_pointer<T>::value) {
                    std::cout << "field_1: " << temp->data->getField1()
                        << ", field_2: " << temp->data->getField2() << std::endl;
                }
                else {
                    std::cout << "field_1: " << temp->data.getField1()
                        << ", field_2: " << temp->data.getField2() << std::endl;
                }
                temp = temp->next;
            }
            std::cout << "\n...\n";
            temp = tail;
            for (int i = 0; i < 4; i++)
            {
                temp = temp->prev;
            }
            for (int i = 0; i < 5; i++)
            {
                if constexpr (std::is_pointer<T>::value) {
                    std::cout << "field_1: " << temp->data->getField1()
                        << ", field_2: " << temp->data->getField2() << std::endl;
                }
                else {
                    std::cout << "field_1: " << temp->data.getField1()
                        << ", field_2: " << temp->data.getField2() << std::endl;
                }
                temp = temp->next;
            }
        }

    }

    
    T findByIndex(int index)
    {
        if (index >= size || index < 0)
        {
            return;
        }
        node<T>* temp = nullptr;
        if (index <= size / 2)
        {
            temp = head;
            int ctr = 0;
            while (temp != nullptr && ctr < index)
            {
                temp = temp->next;
                ctr++;
            }
        }
        else
        {
            temp = tail;
            int ctr = size - 1;
            while (temp != nullptr && ctr > index)
            {
                temp = temp->prev;
                ctr--;
            }
        }
        if (temp != nullptr)
        {
            return temp->data;
        }
    }

    bool replaceByIndex(int index, T newData)
    {
        if (index >= size || index < 0)
        {
            return false;
        }
        node<T>* temp = nullptr;
        if (index < size / 2)
        {
            temp = head;
            int ctr = 0;
            while (temp != nullptr && ctr < index)
            {
                temp = temp->next;
                ctr++;
            }
        }
        else
        {
            temp = tail;
            int ctr = size - 1;
            while (temp != nullptr && ctr > index)
            {
                temp = temp->prev;
                ctr--;
            }
        }
        if constexpr (std::is_pointer<T>::value)
            delete temp->data;

        temp->data = newData;
        return true;
    }

    node<T>* search(const T& target, std::function<bool(const T&, const T&)> comparator)
    {
        node<T>* temp = head;
        while (temp != nullptr)
        {
            if (comparator(temp->data, target))
            {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    bool searchAndDelete(const T& target, std::function<bool(const T&, const T&)> comparator)
    {
        node<T>* temp = head;
        while (temp != nullptr)
        {
            if (comparator(temp->data, target))
            {
                if (temp == head)
                {
                    deleteAtHead();
                }
                else if (temp == tail)
                {
                    deleteAtTail();
                }
                else
                {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                    if constexpr (std::is_pointer<T>::value)
                        delete temp->data;
                    delete temp;
                    size--;
                }
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
    
    void addAtDetermined(T newData, std::function<bool(const T&, const T&)> comparator)
    {
        node<T>* temp = head;

        while (temp != nullptr)
        {
            if (comparator(newData, temp->data))
            {
                node<T>* toAdd = new node<T>(newData);
                toAdd->next = temp->next;
                toAdd->prev = temp;
                if (temp->next == nullptr)
                {
                    tail = toAdd;
                }
                else
                {
                    temp->next->prev = toAdd;
                }
                temp->next = toAdd;
                size++;
                return;
            }
            temp = temp->next;
        }
        addAtHead(newData);
    }

    std::string toString()
    {
        node<T>* temp = head;
        std::ostringstream output;
        while (temp != nullptr)
        {
            if constexpr (std::is_pointer<T>::value) {
                output << "(" << temp->data->getField1() << " , " << temp->data->getField2() << ") ";
            }
            else {
                output << "(" << temp->data.getField1() << " , " << temp->data.getField2() << ") ";
            }
            
            temp = temp->next;

        }
        return output.str();
    }

    ~list() {
        deleteAll();
    }
};

struct someObject
{
private:
    int field_1;
    char field_2;
public:
    someObject(int f1, char f2) : field_1(f1), field_2(f2) {}
    someObject() : field_1(10), field_2('z') {}

    int getField1() const
    {
        return field_1;
    }
    char getField2() const
    {
        return field_2;
    }
    void setField1(int field)
    {
        field_1 = field;
    }
    void setField2(char field)
    {
        field_2 = field;
    }
};

int main()
{
    std::function<bool(someObject*, someObject*)> comparator = [](someObject* obj1, someObject* target)
        {
            return obj1->getField1() == target->getField1();
        };

    std::function<bool(const someObject&, const someObject&)> ocomparator = [](const someObject& obj1, const someObject& target)
        {
            return obj1.getField1() == target.getField1();
        };

    std::function<bool(someObject*, someObject*)> anotherComparator = [](someObject* obj1, someObject* target)
        {
            return obj1->getField1() >= target->getField1();
        };

    std::function<bool(const someObject&, const someObject&)> oanotherComparator = [](const someObject& obj1, const someObject& target)
        {
            return obj1.getField1() >= target.getField1();
        };
    srand(time(0));
    clock_t sum1 = clock();
    list<someObject*>* l1 = new list<someObject*>();
    const int MAX_ORDER = 6;
    char letters[] = "abcdefghijklmnopqrstuvwxyz";
    double addingTime = 0.0;
    double deletingTime = 0.0;
    clock_t sum2 = clock();
    double total = (sum2 - sum1) / (double)CLOCKS_PER_SEC;
    std::cout << std::endl << "Total time: " << total << std::endl;
    std::cout << std::endl << "Total adding time: " << addingTime << std::endl;
    std::cout << std::endl << "Total deleting time: " << deletingTime << std::endl;

   
    list<someObject>* l2 = new list<someObject>();
   
    l2->addAtDetermined(someObject(16, 'f'), oanotherComparator);
    l2->printList();
    std::cout << "---------------------------------------" << std::endl;
    l2->addAtDetermined(someObject(12, 'f'), oanotherComparator);
    l2->printList();
    std::cout << "---------------------------------------" << std::endl;

    l2->addAtDetermined(someObject(118, 'f'), oanotherComparator);
    l2->printList();
    std::cout << "---------------------------------------" << std::endl;

    l2->addAtDetermined(someObject(9, 'f'), oanotherComparator);
    l2->printList();
    l2->search(someObject(13, 'p'), ocomparator);

    delete l1;
    delete l2;
    return 0;
}
