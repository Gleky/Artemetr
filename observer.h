#pragma once

#include <string>
#include <list>
using namespace std;

class Subscriber
{
public:
    virtual ~Subscriber();
    virtual void update() = 0;
};

class Publisher
{
public:
    void subscribe(Subscriber *sub)
    {
        _subscribers.push_back(sub);
    }
    void unsubscribe(Subscriber *sub)
    {
        _subscribers.remove(sub);
    }

protected:
    void notifySubscribers()
    {
        for (auto sub : _subscribers)
            sub->update();
    }

private:
    list<Subscriber *> _subscribers;
};
