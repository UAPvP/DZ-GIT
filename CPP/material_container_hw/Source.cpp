#include <iostream>
#include <stdexcept>

using namespace std;

class Texture
{
  char* buffer;
  int size;

public:

  Texture(int size)
  {
    this->size = size;
    buffer = new char[size];

    cout << "Texture constructor" << endl;
  }

  Texture(const Texture& other)
  {
    size = other.size;
    buffer = new char[size];

    for (int i = 0; i < size; i++)
    {
      buffer[i] = other.buffer[i];
    }

    cout << "Texture copy constructor" << endl;
  }

  Texture(Texture&& other)
  {
    buffer = other.buffer;
    size = other.size;
    other.buffer = nullptr;
    other.size = 0;
    cout << "Texture move constructor" << endl;
  }
  int getBufferSize() const {
      return size;
  }

  Texture& operator=(const Texture& other)
  {
    if (this == &other) { return *this; }

    delete[] buffer;

    size = other.size;
    buffer = new char[size];

    for (int i = 0; i < size; i++)
    {
      buffer[i] = other.buffer[i];
    }

    return *this;
  }

  Texture& operator=(Texture&& other)
  {
    if (this == &other) { return *this; }

    delete[] buffer;

    size = other.size;
    buffer = other.buffer;

    other.size = 0;
    other.buffer = nullptr;


    return *this;
  }

  ~Texture()
  {
    delete[] buffer;
  }
};

class Material
{
  Texture texture;
  string name;

public:

  Material() : texture(16), name("default")
  {
    cout << "Material default constructor" << endl;
  }
  
  Material(int textureSize, const string& name) : texture(textureSize), name(name)
  {
    cout << "Material constructor" << endl;
  }

  Material(const Material& other) : texture(other.texture), name(other.name)
  {
    cout << "Material copy constructor" << endl;
  }

  Material(Material&& other) : texture(move(other.texture)), name(move(other.name))
  {
    cout << "Material move constructor" << endl;
  }
  int getBufferSize() const {
      return texture.getBufferSize();

  }

  Material& operator=(const Material& other)
  {
    if (this == &other) { return *this; }

    texture = other.texture;
    name = other.name;

    return *this;
  }

  Material& operator=(Material&& other) noexcept
  {
    if (this == &other) { return *this; }

    texture = move(other.texture);
    name = move(other.name);

    return *this;
  }
  bool operator==(const Material& other) const {
      return name == other.name;
  }
};

class MaterialContainer
{
    Material** arr;
    int count;
    int bufferSize;


public:
    MaterialContainer() : count(0), bufferSize(0)
    {
        
        arr = new Material * [count+1];
    }

    MaterialContainer(const MaterialContainer& other) : bufferSize(other.bufferSize), count(other.count)
    {
        arr = new Material * [count+1];
        for (int i = 0; i < count; i++) {
            arr[i] = new Material(*other.arr[i]);
        }
    }

    int size() const {return count;}
    bool empty() const {return count == 0;}
    int getBufferSize() const {return bufferSize;}

    MaterialContainer& operator=(const MaterialContainer& other)
    {
        if (this == &other) return *this;
        for (int i = 0; i < count; i++) {
            delete arr[i];
        }
        delete[] arr;
        bufferSize = other.bufferSize;
        count = other.count;
        arr = new Material * [count+1];
        for (int i = 0; i < count; i++) {
            arr[i] = new Material(*other.arr[i]);
        }
        return *this;
    }

    Material& operator[](int index)
    {
        if (index < 0 || index >= count) {
            throw out_of_range("MaterialContainer index out of range");
        }
        return *arr[index];
    }

    const Material& operator[](int index) const
    {
        if (index < 0 || index >= count) {
            throw out_of_range("MaterialContainer index out of range");
        }
        return *arr[index];
    }

    MaterialContainer(MaterialContainer&& other) noexcept : arr(other.arr), count(other.count), bufferSize(other.bufferSize)
    {
        other.arr = nullptr;
        other.count = 0;
        other.bufferSize = 0;
    }

    MaterialContainer& operator=(MaterialContainer&& other) noexcept
    {
        if (this == &other) return *this;
        for (int i = 0; i < count; i++) {
            delete arr[i];
        }
        delete[] arr;
        arr = other.arr;
        count = other.count;
        bufferSize = other.bufferSize;
        other.arr = nullptr;
        other.count = 0;
        other.bufferSize = 0;
        return *this;
    }

    void addMaterial(const Material& material)
    {
        Material** newArr = new Material * [count + 1];

        for (int i = 0; i < count; i++) {
            newArr[i] = arr[i];
        }

        newArr[count] = new Material(material);
        bufferSize += material.getBufferSize();

        delete[] arr;
        arr = newArr;
        count++;
    }

    void RemoveMaterial(const Material& material)
    {
        for (int i = 0; i < count; i++) {
            if (*arr[i] == material) {
                bufferSize -= arr[i]->getBufferSize();
                delete arr[i];

                for (int j = i; j < count - 1; j++) {
                    arr[j] = arr[j + 1];
                }

                count--;
                arr[count] = nullptr;
                return;
            }
        }
    }

    ~MaterialContainer()
    {
        for (int i = 0; i < count; i++) {
            delete arr[i];
        }
        delete[] arr;
    }
};
int main()
{
    cout << "=== Create Materials ===" << endl;

    Material m1(32, "Wood");
    Material m2(64, "Stone");
    Material m3(16, "Metal");

    cout << "\n=== Create Container ===" << endl;
    MaterialContainer container;

    cout << "\n=== Add Materials ===" << endl;
    container.addMaterial(m1);
    container.addMaterial(m2);
    container.addMaterial(m3);

    cout << "Container size: " << container.size() << endl;

    cout << "\n=== Access Elements ===" << endl;
    cout << container[0].operator==(m1) << endl;
    cout << container[1].operator==(m2) << endl;

    cout << "\n=== Remove Material (Stone) ===" << endl;
    container.RemoveMaterial(m2);

    cout << "Container size after remove: " << container.size() << endl;

    cout << "\n=== Copy Container ===" << endl;
    MaterialContainer copyContainer = container;

    cout << "Copy size: " << copyContainer.size() << endl;

    cout << "\n=== Move Container ===" << endl;
    MaterialContainer movedContainer = move(container);

    cout << "Moved size: " << movedContainer.size() << endl;
    cout << "Original size after move: " << container.size() << endl;

    cout << "\n=== End ===" << endl;

    return 0;
}
