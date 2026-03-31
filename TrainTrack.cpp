#include "TrainTrack.h"
#include <iostream>

bool TrainTrack::autoDispatch = false;

TrainTrack::TrainTrack()
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(Destination::OTHERS),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::TrainTrack(Destination _dest)
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(_dest),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::~TrainTrack()
{
    while (firstLocomotive)
    {
        Train *t = departTrain();
        if (t)
            delete t;
    }
}

// Given to you ready
std::string TrainTrack::generateTrainName()
{
    ++trainCounter;
    return "Train_" + destinationToString(destination) + "_" + std::to_string(trainCounter);
}

void TrainTrack::addTrain(Train *train)
{
    if (!train)
        return;

    train->setNext(nullptr);
    if (!firstLocomotive)
    {
        firstLocomotive = lastLocomotive = train;
    }
    else
    {
        lastLocomotive->setNext(train);
        lastLocomotive = train;
    }
    totalWeight += train->getTotalWeight();

    if (autoDispatch)
    {
        while (firstLocomotive && totalWeight > AUTO_DISPATCH_LIMIT)
        {
            Train *departed = departTrain();
            if (!departed)
                break;
            std::cout << "Auto-dispatch: departing " << departed->getName() << " to make room.\n";
            delete departed;
        }
    }
}

Train *TrainTrack::departTrain()
{
    if (!firstLocomotive)
        return nullptr;
    Train *removed = firstLocomotive;
    firstLocomotive = removed->getNext();
    if (!firstLocomotive)
        lastLocomotive = nullptr;
    removed->setNext(nullptr);
    totalWeight -= removed->getTotalWeight();
    std::cout << "Train " << removed->name << " departed from Track " << destinationToString(destination) << "." << std::endl;
    return removed;
}

bool TrainTrack::isEmpty() const
{
    return firstLocomotive == nullptr;
}


Train *TrainTrack::findTrain(const std::string &name) const
{
    Train *curr = firstLocomotive;
    while (curr)
    {
        if (curr->name == name)
            return curr;
        curr = curr->getNext();
    }
    return nullptr;
}

// Given to you ready
void TrainTrack::printTrack() const
{

    if (isEmpty())
        return;

    Train *current = firstLocomotive;

    std::cout << "[Track " << static_cast<int>(firstLocomotive->destination) << "] ";
    while (current)
    {
        std::cout << current->getName() << "(" << current->getTotalWeight() << "ton)-" << current->wagons << " -> ";
        current = current->getNext();
    }
    std::cout << std::endl;
    return;
}
