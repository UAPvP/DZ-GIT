#pragma once

class Actor;

class Scene
{
    Actor** actors;
    int count;
    int SIZE;
  
public:
    Scene();
    ~Scene();

    void AddActor(Actor* actor);
    void RemoveActor(int index);

    Actor** getActors()
    {
        return actors;
    }

    int getActorsCount()
    {
        return count;
    }

};
