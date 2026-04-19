#include <iostream>
using namespace std;

class String
{
    char* ptr;
    int length;

public:
    const char* GetData() const
    {
        return ptr;
    }
    int GetLength() const
    {
        return length;
    }

    String()
    {
        length = 0;
        ptr = new char('\0');
    }

    String(const String& string)
    {
        length = 0;
        Append(string.GetData());
    }

    String(const char* ptr)
    {
        length = 0;
        Append(ptr);
    }

    String(const char* ptr, const int length)
    {
        this->length = 0;
        Append(ptr, length);
    }

    ~String()
    {
        delete[] ptr;
    }

    void Print() const
    {
        cout << ptr << endl;
    }

    void Append(const String& string, int length = -1)
    {
        Append(string.GetData(), length);
    }

    void Append(const char* ptr, int length = -1)
    {
        if (length < 0)
        {
            length = 0;

            while (ptr[length] != '\0')
            {
                length++;
            }
        }

        const int new_length = this->length + length;
        char* new_ptr = new char[new_length + 1];

        for (int i = 0; i < this->length; i++)
        {
            new_ptr[i] = this->ptr[i];
        }
        for (int i = 0; i < length; i++)
        {
            new_ptr[i + this->length] = ptr[i];
        }

        new_ptr[new_length] = '\0';

        delete[] this->ptr;
        this->ptr = new_ptr;
        this->length = new_length;
    }

    void Remove(const char* old_substring, const char* new_substring)
    {
        if (!ptr || !old_substring || !new_substring) return;
        int old_len = 0;
        while (old_substring[old_len] != '\0') old_len++;

        int new_len = 0;
        while (new_substring[new_len] != '\0') new_len++;

        if (old_len == 0) return;

        int i = 0;

        while (i <= length - old_len)
        {
            bool match = true;

            for (int j = 0; j < old_len; j++)
            {
                if (ptr[i + j] != old_substring[j])
                {
                    match = false;
                    break;
                }
            }

            if (match)
            {
                int new_length = length - old_len + new_len;
                char* new_ptr = new char[new_length + 1];
                for (int k = 0; k < i; k++)
                {
                    new_ptr[k] = ptr[k];
                }
                for (int k = 0; k < new_len; k++)
                {
                    new_ptr[i + k] = new_substring[k];
                }
                for (int k = i + old_len; k < length; k++)
                {
                    new_ptr[k - old_len + new_len] = ptr[k];
                }
                new_ptr[new_length] = '\0';
                delete[] ptr;
                ptr = new_ptr;
                length = new_length;
                i += new_len;
            }
            else
            {
                i++;
            }
        }
    }

    void Clear()
    {
        delete[] ptr;

        length = 0;
        ptr = new char('\0');
    }
    bool Contains(const char* substring) const
    {
        int sub_len = 0;
        while (substring[sub_len] != '\0')
        {
            sub_len++;
        }

        if (sub_len == 0) return true;
        if (sub_len > length) return false;

        for (int i = 0; i <= length - sub_len; i++)
        {
            bool match = true;
            for (int j = 0; j < sub_len; j++)
            {
                if (ptr[i + j] != substring[j])
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                return true;
            }
        }
        return false;
    }
    int Count(const char* substring) const
    {
        int sub_len = 0;
        while (substring[sub_len] != '\0')
        {
            sub_len++;
        }
        if (sub_len == 0) return 0;
        int count = 0;
        for (int i = 0; i <= length - sub_len; i++)
        {
            bool match = true;

            for (int j = 0; j < sub_len; j++)
            {
                if (ptr[i + j] != substring[j])
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                count++;
                i += sub_len - 1;
            }
        }
        return count;
    }
    void Insert(int position, const char* substring)
    {
        if (position < 0) position = 0;
        if (position > length) position = length;
        int sub_len = 0;
        while (substring[sub_len] != '\0')
        {
            sub_len++;
        }
        if (sub_len == 0) return;
        int new_length = length + sub_len;
        char* new_ptr = new char[new_length + 1];
        for (int i = 0; i < position; i++)
        {
            new_ptr[i] = ptr[i];
        }
        for (int i = 0; i < sub_len; i++)
        {
            new_ptr[position + i] = substring[i];
        }
        for (int i = position; i < length; i++)
        {
            new_ptr[i + sub_len] = ptr[i];
        }
        new_ptr[new_length] = '\0';
        delete[] ptr;
        ptr = new_ptr;
        length = new_length;
    };

    String operator+(const String& other) const {
        String result(*this);
        result.Append(other);
        return result;
    }
    String& operator+=(const String& other) {
        this->Append(other);
        return (*this);
    }; // додавання до поточного 
    String& operator=(const String& other) {
        if (this == &other)
            return *this;
        delete[] ptr; 
        length = other.length;
        ptr = new char[length + 1];
        for (int i = 0; i < length; i++) {
            ptr[i] = other.ptr[i];
        }
        ptr[length] = '\0';
        return *this;
    } // Копіювання
    bool operator==(const String& other) const {
        if (length != other.length) return false;
        for (int i = 0; i < length; i++) {
            if (ptr[i] != other.ptr[i])
                return false;
        }
        return true;
    }; // Порівняння на рівність
    bool operator!=(const String& other) const {
        return !(*this == other);
    }; // Порівняння на нерівність
    bool operator<(const String& other) const {
        return strcmp(ptr, other.ptr) < 0;
    }// лексикографічно менше
    bool operator>(const String& other) const {
        return other<*this;
    }; // лексикографічно більше
    char& operator[](int index) {
        return this->ptr[index];
    }; // для читання та запису символу
    const char& operator[](int index) const {
        return this->ptr[index];
    }; // для константного об'єкта
    bool operator!() const {
        return length == 0;
    };
    operator const char* () const {
        return ptr;
    }; // дозволяє використовувати String там, де потрібний const char*
};
ostream& operator<<(std::ostream& os, const String& s)
{
    os << s.GetData();
    return os;
}


int main()
{
    String string1("Test TEXT Asd TEXT Asd");
    string1.Print();

    string1.Remove("TEXT", "text123");
    string1.Print();
    cout << string1.Contains("text123") << endl;
    cout << string1.Contains("TEXT") << endl; 

    cout << string1.Count("Asd") << endl;
    cout << string1.Count("text123") << endl;
    string1.Insert(5, "INSERTED ");
    string1.Print();

    string1.Remove("Asd", "");
    string1.Print();

    string1.Clear();
    string1.Print();
    return 0;
}