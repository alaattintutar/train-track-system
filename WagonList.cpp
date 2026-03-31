#include "WagonList.h"

#include <iostream>

WagonList::~WagonList() { clear(); }

void WagonList::clear()
{
    Wagon *curr = front;
    while (curr)
    {
        Wagon *next = curr->getNext();
        delete curr;
        curr = next;
    }
    front = nullptr;
    rear = nullptr;
    totalWeight = 0;
}

WagonList::WagonList(WagonList &&other) noexcept
{
    front = other.front;
    rear = other.rear;
    totalWeight = other.totalWeight;
    other.front = nullptr;
    other.rear = nullptr;
    other.totalWeight = 0;
}

WagonList &WagonList::operator=(WagonList &&other) noexcept
{
    if (this != &other)
    {
        clear();
        front = other.front;
        rear = other.rear;
        totalWeight = other.totalWeight;
        other.front = nullptr;
        other.rear = nullptr;
        other.totalWeight = 0;
    }
    return *this;
}

Wagon *WagonList::findById(int id)
{
    Wagon *curr = front;
    while (curr)
    {
        if (curr->getID() == id)
            return curr;
        curr = curr->getNext();
    }
    return nullptr;
}

void WagonList::addWagonToRear(Wagon *w)
{
    if (!w)
        return;
    w->setNext(nullptr);
    w->setPrev(rear);
    if (rear)
        rear->setNext(w);
    else
        front = w;
    rear = w;
    totalWeight += w->getWeight();
}

int WagonList::getTotalWeight() const { return totalWeight; }

bool WagonList::isEmpty() const
{
    return front == nullptr;
}

void WagonList::insertSorted(Wagon *wagon)
{
    if (!wagon)
        return;

    wagon->setPrev(nullptr);
    wagon->setNext(nullptr);

    if (!front)
    {
        front = rear = wagon;
        totalWeight += wagon->getWeight();
        return;
    }

    if (wagon->getWeight() >= front->getWeight())
    {
        wagon->setNext(front);
        front->setPrev(wagon);
        front = wagon;
        totalWeight += wagon->getWeight();
        return;
    }

    Wagon *curr = front;
    while (curr->getNext() && curr->getNext()->getWeight() >= wagon->getWeight())
        curr = curr->getNext();

    wagon->setNext(curr->getNext());
    if (curr->getNext())
        curr->getNext()->setPrev(wagon);
    else
        rear = wagon;
    wagon->setPrev(curr);
    curr->setNext(wagon);
    totalWeight += wagon->getWeight();
}

void WagonList::appendList(WagonList &&other)
{
   if (other.isEmpty())
       return;

   if (isEmpty())
   {
       front = other.front;
       rear = other.rear;
       totalWeight = other.totalWeight;
   }
   else
   {
       bool otherIsHazard = true;
       for (Wagon *p = other.front; p; p = p->getNext())
       {
           if (p->getCargoType() != CargoType::HAZARDOUS)
           {
               otherIsHazard = false;
               break;
           }
       }

       int thisFront = front ? front->getWeight() : 0;
       int otherFront = other.front ? other.front->getWeight() : 0;

        if (otherIsHazard || thisFront >= otherFront)
        {
            rear->setNext(other.front);
            other.front->setPrev(rear);
            rear = other.rear;
            totalWeight += other.totalWeight;
        }
        else
        {
            other.rear->setNext(front);
            front->setPrev(other.rear);
            front = other.front;
            totalWeight += other.totalWeight;
        }
   }

   // Leave other empty
   other.front = nullptr;
   other.rear = nullptr;
   other.totalWeight = 0;
}

Wagon *WagonList::detachById(int id)
{
    Wagon *toRemove = findById(id);
    if (!toRemove)
        return nullptr;

    if (toRemove->getPrev())
        toRemove->getPrev()->setNext(toRemove->getNext());
    else
        front = toRemove->getNext();

    if (toRemove->getNext())
        toRemove->getNext()->setPrev(toRemove->getPrev());
    else
        rear = toRemove->getPrev();

    toRemove->setPrev(nullptr);
    toRemove->setNext(nullptr);
    totalWeight -= toRemove->getWeight();
    std::cout << "Wagon " << toRemove->getID() << " detached from Wagon List. " << std::endl;
    return toRemove;
}


WagonList WagonList::splitAtById(int id)
{
    WagonList newList; // return-by-value (will be moved automatically)

    Wagon *node = findById(id);
    if (!node)
        return newList;

    if (node == front)
    {
        newList.front = front;
        newList.rear = rear;
        newList.totalWeight = totalWeight;
        front = rear = nullptr;
        totalWeight = 0;
        return newList;
    }

    int tailWeight = 0;
    for (Wagon *p = node; p; p = p->getNext())
        tailWeight += p->getWeight();

    Wagon *prev = node->getPrev();
    if (prev)
        prev->setNext(nullptr);
    node->setPrev(nullptr);

    newList.front = node;
    newList.rear = rear;
    newList.totalWeight = tailWeight;

    rear = prev;
    totalWeight -= tailWeight;

    return newList;
}

// Print is already implemented
void WagonList::print() const
{

    std::cout << *this << std::endl;
    return;
}

// << operator is already implemented
std::ostream &operator<<(std::ostream &os, const WagonList &list)
{
    if (list.isEmpty())
        return os;

    Wagon *current = list.front;

    while (current)
    {
        os << "W" << current->getID() << "(" << current->getWeight() << "ton)";
        if (current->getNext())
            os << " - ";
        current = current->getNext();
    }
    return os;
}
