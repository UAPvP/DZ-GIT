#pragma once

class Actor;

class ActorComponent
{
    Actor* owner;
    
public:
    ActorComponent(Actor* actor) : owner(actor) {}
    
    Actor* getOwner() const
    {
        return owner;
    }
};