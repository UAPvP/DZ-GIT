#include "../Public/Scene.h"

Scene::Scene()
{
    SIZE = 10;
    count = 0;
    actors = new Actor*[SIZE];
}

Scene::~Scene()
{
    delete[] actors;
}

void Scene::AddActor(Actor* actor)
{
    if (count >= SIZE)
    {
        SIZE *= 2;
        Actor** newArr = new Actor*[SIZE];

        for (int i = 0; i < count; i++)
        {
            newArr[i] = actors[i];
        }

        delete[] actors;
        actors = newArr;
    }
    
    actors[count++] = actor;
}

void Scene::RemoveActor(int index)
{
    for (int i = index; i < count - 1; i++)
    {
        actors[i] = actors[i + 1];
    }
    count--;
}
