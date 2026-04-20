#include <iostream>
#include <iomanip>
using namespace std;

template <typename T>
class SmartArray
{
private:

    T* data;                 // Вказівник на динамічний масив елементів
    size_t size;             // Поточна кількість елементів у масиві
    size_t capacity;         // Максимальна кількість елементів, яку можна зберігати без перевиділення пам'яті

    static size_t objectCount; // Лічильник створених об'єктів цього типу (окремо для кожного T)

    // Приватний метод для зміни розміру внутрішнього буфера
    // Повинен перевиділити пам'ять і скопіювати існуючі елементи
    void ResizeInternal(size_t newCapacity) {
        T* newData = new T[newCapacity];

        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    // Приватний метод для явного переміщення значення
    static T&& Move(T& value) {
        return static_cast<T&&>(value);
    }

public:

    // =========================
    // RULE OF FIVE
    // =========================

    // Конструктор за замовчуванням
    // Повинен створити порожній масив
    SmartArray() : data(nullptr), size(0), capacity(0) {
        objectCount++;
    };

    // Конструктор із заданим розміром
    // Повинен створити масив із size елементів
    explicit SmartArray(size_t size) : data(new T[size]), size(size), capacity(size) {
        objectCount++;
    };

    // Конструктор із розміром та значенням
    // Повинен заповнити всі елементи значенням value
    SmartArray(size_t size, const T& value) : data(new T[size]), size(size), capacity(size){
        for (size_t i = 0;i < capacity;i++) {
            data[i] = value;
        };
        objectCount++;
    };

    // Конструктор копіювання
    // Повинен створити повну копію іншого масиву
    SmartArray(const SmartArray& other) : size(other.size), capacity(other.capacity) {
        data = new T[size];
        for (size_t i = 0; i < capacity; i++) {
            data[i] = other.data[i];
        }
        objectCount++;

    };

    // Move-конструктор
    // Повинен "перемістити" ресурси з іншого об'єкта
    SmartArray(SmartArray&& other) : size(other.size), data(other.data), capacity(other.capacity) noexcept {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;

        objectCount++;
    };

    // Деструктор
    // Повинен звільнити всю виділену пам'ять
    ~SmartArray() {
        delete[] data;
        data = nullptr;
        objectCount--;
    };

    // Оператор копіювання
    // Повинен правильно обробляти самоприсвоєння
    SmartArray& operator=(const SmartArray& other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }

        return *this;
    }

    // Move-оператор присвоювання
    // Повинен передати ресурси без копіювання
    SmartArray& operator=(SmartArray&& other) noexcept {
        if (this == &other) return *this;
        delete[] data;
        data = other.data;
        size = other.size;
        capacity = other.capacity;
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;

        return *this;
    }

    // =========================
    // ACCESS
    // =========================

    // Оператор доступу до елементу за індексом без перевірки меж
    T& operator[](size_t index) {
        return data[index];
    };

    // Константна версія оператора []
    const T& operator[](size_t index) const {
        return data[index];
    };


    // Повертає перший елемент масиву
    T& Front() {
        return data[0];
    };

    // Повертає останній елемент масиву
    T& Back() {
        return data[size-1];
    };

    // Константна версія Front
    const T& Front() const {
        return data[0];
    };

    // Константна версія Back
    const T& Back() const {
        return data[size-1];
    };

    // Повертає сирий вказівник на масив
    T* Data() {
        return data;
    };

    // Константна версія Data
    const T* Data() const {
        return data;
    };

    // =========================
    // SIZE
    // =========================

    // Повертає кількість елементів
    size_t Size() const {
        return size;
    };

    // Повертає поточну місткість масиву
    size_t Capacity() const {
        return capacity;
    };

    // Перевіряє, чи масив порожній
    bool Empty() const {
        return size == 0;
    };

    // Очищає масив (розмір стає 0, але пам'ять може залишатися)
    void Clear() {
        for (size_t i = 0; i < size;i++) {
            data[i] = 0;
        }
    };

    // Змінює розмір масиву
    // Якщо новий розмір більший — додає елементи за замовчуванням
    void Resize(size_t newSize) {
        if (newSize > capacity) {
            size_t newCapacity = (capacity == 0) ? 1 : capacity;
            while (newCapacity < newSize) {
                newCapacity *= 2;
            }
            ResizeInternal(newCapacity);
        }

        if (newSize > size) {
            for (size_t i = size; i < newSize; i++) {
                data[i] = T();
            }
        }

        size = newSize;
    }


    // Резервує пам'ять під newCapacity елементів
    void Reserve(size_t newCapacity) {
        if (newCapacity <= capacity) {
            return;
        }

        ResizeInternal(newCapacity);
    }

    // Зменшує capacity до size
    void ShrinkToFit() {
        ResizeInternal(size);
    };

    // =========================
    // MODIFICATION
    // =========================

    // Додає елемент у кінець масиву (копіювання)
    void PushBack(const T& value) {
        if (size == capacity) {
            Reserve(capacity == 0 ? 1 : capacity * 2);
        }

        data[size] = value;
        size++;
    };

    // Додає елемент у кінець масиву (переміщення)
    void PushBack(T&& value) {
        if (size == capacity) {
            Reserve(capacity == 0 ? 1 : capacity * 2);
        }

        data[size] = Move(value);
        size++;
    };

    // Видаляє останній елемент масиву
    void PopBack() {
        if (size == 0) {
            return;
        }

        --size;
        data[size] = T();
    };

    // Вставляє елемент у задану позицію
    // Повинен зсунути інші елементи вправо
    void Insert(size_t index, const T& value) {
        if (index > size) {
            return;
        }
        if (size == capacity) {
            Reserve(capacity == 0 ? 1 : capacity * 2);
        }
        for (size_t i = size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        size++;
    };

    // Видаляє елемент за індексом
    // Повинен зсунути інші елементи вліво
    void RemoveAt(size_t index) {
        if (index >= size) {
            return;
        }
        for (size_t i = index; i + 1 < size; ++i) {
            data[i] = data[i + 1];
        }
        --size;
        data[size] = T();
    };

    // Видаляє перше входження заданого значення
    void Remove(const T& value) {
        for (size_t i = 0; i < size; ++i) {
            if (data[i] == value) {
                RemoveAt(i);
                return;
            }
        }
    };

    // Обмінює вміст двох масивів
    void Swap(SmartArray& other) {
        T* tempData = data;
        data = other.data;
        other.data = tempData;

        size_t tempSize = size;
        size = other.size;
        other.size = tempSize;

        size_t tempCapacity = capacity;
        capacity = other.capacity;
        other.capacity = tempCapacity;
    };

    // Заповнює всі елементи заданим значенням
    void Fill(const T& value) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = value;
        }
    };

    // =========================
    // SEARCH
    // =========================

    // Повертає індекс першого входження значення
    // Якщо не знайдено — повертає -1
    int Find(const T& value) const {
        for (size_t i = 0;i < size;i++) {
            if (data[i] == value) {
                return i;
            }
        }
        return -1;
    };

    // Перевіряє, чи містить масив задане значення
    bool Contains(const T& value) const {
        for (size_t i = 0;i < size;i++) {
            if (data[i] == value) {
                return true;
            }
        }
        return false;
    };

    // Повертає кількість входжень заданого значення
    int Count(const T& value) const {
        int count = 0;
        for (size_t i = 0;i < size;i++) {
            if (data[i] == value) {
                count++;
            }
        }
        return count;
    };

    // =========================
    // ITERATORS
    // =========================

    // Повертає ітератор на початок масиву
    T* begin() {
        return data;
    };
    // Повертає ітератор на кінець масиву
    T* end() {
        return data + size;
    };

    // Константна версія begin
    const T* begin() const {
        return data;
    };

    // Константна версія end
    const T* end() const {
        return data + size;
    };

    // =========================
    // OPERATORS
    // =========================

    // Повертає новий масив як результат додавання двох масивів
    SmartArray operator+(const SmartArray& other) const {
        SmartArray result(size + other.size);
        for (size_t i = 0; i < size; ++i) {
            result.data[i] = data[i];
        }
        for (size_t i = 0; i < other.size; ++i) {
            result.data[size + i] = other.data[i];
        }
        return result;
    };

    // Повертає новий масив як результат віднімання
    SmartArray operator-(const SmartArray& other) const {
        SmartArray result;
        for (size_t i = 0; i < size; ++i) {
            if (!other.Contains(data[i])) {
                result.PushBack(data[i]);
            }
        }
        return result;
    };

    // Множить кожен елемент на значення value
    SmartArray operator*(const T& value) const {
        SmartArray result(size);
        for (size_t i = 0; i < size; ++i) {
            result.data[i] = data[i] * value;
        }
        return result;
    };

    // Додає інший масив до поточного
    SmartArray& operator+=(const SmartArray& other) {
        *this = *this + other;
        return *this;

    };

    // Віднімає інший масив від поточного
    SmartArray& operator-=(const SmartArray& other) {
        *this = *this - other;
        return *this;
    };
    // Перевіряє рівність двох масивів
    bool operator==(const SmartArray & other) const {
         if (size != other.size) return false;
         for (int i = 0;i < size;i++) {
             if (data[i] != other.data[i]) {
                 return false;
             }
         }
         return true;
    };

    // Перевіряє нерівність
    bool operator!=(const SmartArray& other) const {
        return !(*this == other);
    };

    // Порівняння масивів (наприклад за сумою елементів)
    bool operator<(const SmartArray& other) const {
        T sum = 0;
        T sum_2 = 0;
        for (int i = 0; i < size;i++) {
            sum += data[i];
        }
        for (int i = 0;i < other.size;i++) {
            sum_2 += other.data[i];
        }
        return sum < sum_2;
    };

    // Порівняння масивів
    bool operator>(const SmartArray& other) const {
        return other < *this;
    };

    // Порівняння масивів
    bool operator<=(const SmartArray& other) const {
        return *this < other || *this == other;
    };

    // Порівняння масивів
    bool operator>=(const SmartArray& other) const {
        return *this > other || *this == other;
    };

    // Оператор логічного заперечення
    // Повертає true, якщо масив порожній
    bool operator!() const {
        return Empty();
    };

    // =========================
    // STATIC
    // =========================

    // Повертає кількість створених об'єктів цього типу
    static size_t GetObjectCount() {
        return objectCount;
    };

    // =========================
    // TEMPLATE METHODS
    // =========================

    // Перетворює масив у масив іншого типу
    // Наприклад SmartArray<int> -> SmartArray<double>
    template <typename U>
    void Transform(SmartArray<U>& out) const {
        out.Resize(size);

        for (size_t i = 0; i < size; ++i) {
            out[i] = static_cast<U>(data[i]);
        }
    };

    // =========================
    // FRIEND OPERATORS
    // =========================

    // Виведення масиву у потік (cout)
    template <typename U>
    friend ostream& operator<<(ostream& out, const SmartArray<U>& arr);

    // Введення масиву з потоку (cin)
    template <typename U>
    friend istream& operator>>(istream& in, SmartArray<U>& arr);

    
};



template <typename U>
ostream& operator<<(ostream& out, const SmartArray<U>& arr) {
    out << "[";
    for (size_t i = 0; i < arr.size; ++i) {
        out << arr.data[i];
        if (i + 1 < arr.size) {
            out << ", ";
        }
    }
    out << "]";
    return out;
}
template <typename U>
istream& operator>>(istream& in, SmartArray<U>& arr) {
    for (size_t i = 0; i < arr.size; ++i) {
        in >> arr.data[i];
    };
    return in;
}


template <typename T>
size_t SmartArray<T>::objectCount = 0;