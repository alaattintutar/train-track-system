#include "Train.h"
#include <iostream>

Train::Train() : name(""), destination(Destination::OTHERS), totalWeight(0), nextLocomotive(nullptr) {}
Train::Train(const std::string &_name, Destination _dest) : name(_name), destination(_dest), totalWeight(0), nextLocomotive(nullptr) {}
Train::~Train() { clear(); }

// This function is given to you ready
void Train::appendWagonList(WagonList &wl)
{
    // Makes appendList use move semantics
    wagons.appendList(std::move(wl));
    totalWeight = wagons.getTotalWeight();
}

// This function is given to you ready
void Train::addWagonToRear(Wagon *w)
{
    wagons.addWagonToRear(w);
    totalWeight = wagons.getTotalWeight();
}

void Train::clear()
{
    wagons.clear();
    totalWeight = 0;
}

// This function is given to you ready
void Train::print() const
{
    std::cout << "Train " << name << " (" << totalWeight << " tons): ";
    std::cout << wagons << std::endl;
}

Train *Train::verifyCouplersAndSplit(int splitCounter)
{
    int pulled = 0;
    Wagon *curr = wagons.getRear();
    while (curr)
    {
        if (pulled > curr->getMaxCouplerLoad())
        {
            Wagon *next = curr->getNext();
            if (!next)
                break;
            int splitId = next->getID();
            WagonList tail = wagons.splitAtById(splitId);
            Train *newTrain = new Train(name + std::string("_split_") + std::to_string(splitCounter), destination);
            newTrain->appendWagonList(tail);
            totalWeight = wagons.getTotalWeight();
            std::cout << "Train " << name << " split due to coupler overload before Wagon " << splitId << std::endl;
            std::cout << newTrain->wagons << std::endl;
            return newTrain;
        }
        pulled += curr->getWeight();
        curr = curr->getPrev();
    }

    return nullptr;
}
