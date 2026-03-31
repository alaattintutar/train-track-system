#include "ClassificationYard.h"
#include <iostream>

ClassificationYard::ClassificationYard() {}
ClassificationYard::~ClassificationYard() { clear(); }

WagonList &ClassificationYard::getBlockTrain(int destination, int cargoType)
{
    return blockTrains[destination][cargoType];
}

WagonList *ClassificationYard::getBlocksFor(Destination dest)
{
    return blockTrains[static_cast<int>(dest)];
}

// Inserts vagon to the correct place at the yard
void ClassificationYard::insertWagon(Wagon *w)
{
    if (!w)
        return;
    int dest = static_cast<int>(w->getDestination());
    int cargo = static_cast<int>(w->getCargoType());
    blockTrains[dest][cargo].insertSorted(w);
}

// Merges multiple blocks into a train while keeping blocks grouped
Train *ClassificationYard::assembleTrain(Destination dest, const std::string &trainName)
{
    // Gather all non-empty non-hazardous blocks and sort by front wagon weight desc
    int destIdx = static_cast<int>(dest);
    int idxs[NUM_CARGOTYPES_INT];
    int weights[NUM_CARGOTYPES_INT];
    int count = 0;
    for (int j = 0; j < NUM_CARGOTYPES_INT; ++j)
    {
        if (j == static_cast<int>(CargoType::HAZARDOUS))
            continue;
        if (!blockTrains[destIdx][j].isEmpty())
        {
            Wagon *front = blockTrains[destIdx][j].getFront();
            idxs[count] = j;
            weights[count] = front ? front->getWeight() : 0;
            ++count;
        }
    }

    bool hasHazard = !blockTrains[destIdx][static_cast<int>(CargoType::HAZARDOUS)].isEmpty();
    if (count == 0 && !hasHazard)
        return nullptr;

    for (int i = 0; i < count; ++i)
    {
        int maxIdx = i;
        for (int k = i + 1; k < count; ++k)
            if (weights[k] > weights[maxIdx])
                maxIdx = k;
        if (maxIdx != i)
        {
            int tw = weights[i]; weights[i] = weights[maxIdx]; weights[maxIdx] = tw;
            int ti = idxs[i]; idxs[i] = idxs[maxIdx]; idxs[maxIdx] = ti;
        }
    }

    Train *train = new Train(trainName, dest);
    for (int i = 0; i < count; ++i)
    {
        int cj = idxs[i];
        WagonList wl = std::move(blockTrains[destIdx][cj]);
        train->appendWagonList(wl);
    }

    if (hasHazard)
    {
        int hzIdx = static_cast<int>(CargoType::HAZARDOUS);
        Wagon *hzFront = blockTrains[destIdx][hzIdx].getFront();
        if (hzFront)
        {
            Wagon *hz = blockTrains[destIdx][hzIdx].detachById(hzFront->getID());
            train->addWagonToRear(hz);
        }
    }

    return train;
}

bool ClassificationYard::isEmpty() const
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
        for (int j = 0; j < NUM_CARGOTYPES_INT; ++j)
            if (!blockTrains[i][j].isEmpty())
                return false;
    return true;
}

void ClassificationYard::clear()
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
        for (int j = 0; j < NUM_CARGOTYPES_INT; ++j)
            blockTrains[i][j].clear();
}

// Print function is already implemented to keep output uniform
void ClassificationYard::print() const
{
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        auto dest = destinationToString(static_cast<Destination>(i));
        std::cout << "Destination " << dest << ":\n";
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j)
        {
            if (!blockTrains[i][j].isEmpty())
            {
                auto type = cargoTypeToString(static_cast<CargoType>(j));
                std::cout << "  CargoType " << type << ": ";
                blockTrains[i][j].print();
            }
        }
    }
}
