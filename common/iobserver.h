#pragma once
#include <stdlib.h>


using namespace std;

class Subscriber
{
public:
    virtual ~Subscriber() {}
    virtual void update() = 0;
};

class Publisher
{
public:
    void subscribe(Subscriber *sub)
    {
        for (auto &curSub : _subscribers)
            if (curSub == nullptr)
                curSub = sub;
    }

    void unsubscribe(Subscriber *sub)
    {
        for (auto &curSub : _subscribers)
            if (curSub == sub)
                curSub = nullptr;
    }

protected:
    void notifySubscribers() const
    {
        for (auto sub : _subscribers)
            if (sub != nullptr)
                sub->update();
    }

private:
    Subscriber *_subscribers[3];
};
