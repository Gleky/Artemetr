#pragma once



class Subscriber
{
public:
    virtual ~Subscriber() {}
    virtual void updateSub() = 0;
};


class Publisher
{
public:
    void subscribe(Subscriber *sub)
    {
        for (auto &curSub : _subscribers)
            if (curSub == nullptr) {
                curSub = sub;
                break;
            }
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
                sub->updateSub();
    }

private:
    Subscriber *_subscribers[3] = {0};
};
