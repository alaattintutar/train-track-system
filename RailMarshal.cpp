#include "RailMarshal.h"
#include <iostream>
#include <sstream>

RailMarshal::RailMarshal()
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        departureYard[i] = TrainTrack(static_cast<Destination>(i));
    }
}

RailMarshal::~RailMarshal()
{
    // Cleanup handled in TrainTrack and ClassificationYard
}

// Getter (ready)
ClassificationYard &RailMarshal::getClassificationYard()
{
    return classificationYard;
}

// Getter (ready)
TrainTrack &RailMarshal::getDepartureYard(Destination dest)
{
    int idx = static_cast<int>(dest);
    return departureYard[idx];
}

void RailMarshal::processCommand(const std::string &line)
{
    std::istringstream iss(line);
    std::string cmd;
    if (!(iss >> cmd)) return;
    
    if (cmd == "ADD_WAGON")
    {
        int id, weight, maxLoad;
        std::string cargoStr, destStr;
        if (!(iss >> id >> cargoStr >> destStr >> weight >> maxLoad) || (iss >> std::ws, !iss.eof()))
        {
            std::cout << "Error: Invalid ADD_WAGON parameters.\n";
            return;
        }
        CargoType cargo = parseCargo(cargoStr);
        Destination dest = parseDestination(destStr);
        Wagon *w = new Wagon(id, cargo, dest, weight, maxLoad);
        classificationYard.insertWagon(w);
        std::cout << "Wagon " << *w << " added to yard." << std::endl;
        return;
    }
    else if (cmd == "REMOVE_WAGON")
    {
        int id;
        if (!(iss >> id) || (iss >> std::ws, !iss.eof()))
        {
            std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
            return;
        }
        Wagon *removed = nullptr;
        for (int i = 0; i < NUM_DESTINATIONS_INT && !removed; ++i)
        {
            for (int j = 0; j < NUM_CARGOTYPES_INT && !removed; ++j)
            {
                removed = classificationYard.getBlockTrain(i, j).detachById(id);
            }
        }
        if (removed)
        {
            std::cout << "Wagon " << id << " removed." << std::endl;
            delete removed;
        }
        else
        {
            std::cout << "Error: Wagon " << id << " not found." << std::endl;
        }
        return;
    }
    else if (cmd == "ASSEMBLE_TRAIN")
    {
        std::string destStr;
        if (!(iss >> destStr) || (iss >> std::ws, !iss.eof()))
        {
            std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
            return;
        }
        Destination dest = parseDestination(destStr);
        TrainTrack &track = getDepartureYard(dest);
        std::string trainName = track.generateTrainName();
        Train *t = classificationYard.assembleTrain(dest, trainName);
        if (!t)
        {
            std::cout << "No wagons to assemble for " << destStr << std::endl;
            return;
        }
        int splitCounter = 1;
        while (true)
        {
            Train *split = t->verifyCouplersAndSplit(splitCounter);
            if (!split)
                break;
            ++splitCounter;
            std::cout << "Train " << split->getName() << " assembled after split with " << split->getWagons() << " wagons." << std::endl;
            track.addTrain(split);
        }
        std::cout << "Train " << t->getName() << " assembled with " << t->getWagons() << " wagons." << std::endl;
        track.addTrain(t);
        return;
    }
    else if (cmd == "DISPATCH_TRAIN")
    {
        std::string destStr;
        if (!(iss >> destStr) || (iss >> std::ws, !iss.eof()))
        {
            std::cout << "Error: Invalid DISPATCH parameters.\n";
            return;
        }
        Destination dest = parseDestination(destStr);
        dispatchFromTrack(dest);
        return;
    }
    else if (cmd == "PRINT_YARD")
    {
        std::cout << "--- classification Yard ---\n";
        classificationYard.print();
        return;
    }
    else if (cmd == "PRINT_TRACK")
    {
        std::string destStr;
        if (!(iss >> destStr) || (iss >> std::ws, !iss.eof()))
        {
            std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
            return;
        }
        Destination dest = parseDestination(destStr);
        getDepartureYard(dest).printTrack();
        return;
    }
    else if (cmd == "AUTO_DISPATCH")
    {
        std::string val;
        if (!(iss >> val) || (iss >> std::ws, !iss.eof()))
        {
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
            return;
        }
        std::string up = toUpper(val);
        if (up == "ON")
        {
            TrainTrack::autoDispatch = true;
            std::cout << "Auto dispatch enabled" << std::endl;
        }
        else if (up == "OFF")
        {
            TrainTrack::autoDispatch = false;
            std::cout << "Auto dispatch disabled" << std::endl;
        }
        else
        {
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
        }
        return;
    }
    else if (cmd == "CLEAR")
    {
        classificationYard.clear();
        for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
        {
            TrainTrack &tr = departureYard[i];
            while (true)
            {
                Train *t = tr.departTrain();
                if (!t) break;
                delete t;
            }
        }
        std::cout << "System cleared." << std::endl;
        return;
    }
    else
    {
        std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
    }
}

void RailMarshal::dispatchFromTrack(Destination track)
{
    TrainTrack &tr = getDepartureYard(track);
    if (tr.isEmpty())
    {
        std::cout << "Error: No trains to dispatch from track " << destinationToString(track) << ".\n";
        return;
    }
    Train *t = tr.departTrain();
    if (t)
    {
        std::cout << "Dispatching " << t->getName() << " (" << t->getTotalWeight() << " tons)." << std::endl;
        delete t;
    }
}

void RailMarshal::printDepartureYard() const
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        std::cout << "Track " << i << " ("
                  << destinationToString(static_cast<Destination>(i)) << "):\n";
        departureYard[i].printTrack();
    }
}

// Debug helper functions
void RailMarshal::printStatus() const
{
    std::cout << "--- classification Yard ---\n";
    classificationYard.print();

    std::cout << "--- Departure Yard ---\n";
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        departureYard[i].printTrack();
    }
}
