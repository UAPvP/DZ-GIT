#pragma once

#include <string>

class Actor
{
protected:
    std::string name;
    std::string type;

public:
    Actor(std::string type = "Actor", std::string name = "Actor") : type(type), name(name) {}
  
    std::string getType()
    {
        return type;
    }
    
    std::string getName()
    {
        return name;
    }
};
