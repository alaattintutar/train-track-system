<div align="center">
  
# 🚂 Train Track Classification Yard

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Data Structures](https://img.shields.io/badge/Custom-Linked_Lists-purple?style=for-the-badge)
![Memory](https://img.shields.io/badge/Dynamic-Memory_Management-green?style=for-the-badge)

*An algorithmic routing and assembly system for heavy locomotive logistics.*

</div>

## 📌 Overview
The **Train Track System** is a sophisticated C++ application that simulates a real-life railway classification yard. It algorithmically routes inbound wagons carrying different cargo types, groups them into block trains, and assembles outbound trains based on origin, destination, and payload capacities. 

## ✨ Core Features
- 🛤️ **Dynamic Classification Yard:** Dynamically builds and maintains isolated train tracks for specific operations (`ClassificationYard.cpp`).
- 🔗 **Algorithmic Wagon Linking:** Uses custom-built node-based pointer architectures (`WagonList`, `Wagon`) replacing standard libraries for raw maximum performance.
- 📦 **Cargo Logic:** Smartly handles diverse cargo categories (including isolation mechanics for `HAZARDOUS` materials).
- 🔀 **Rail Marshal Control:** A central `RailMarshal` orchestrates complex maneuvers like detaching, swapping, and assembling multi-block trains efficiently.

## ⚙️ Technical Architecture
- **Language:** C++11 or higher
- **Data Structures:** Custom Singly/Doubly linked lists representing sequential physical train wagons.
- **Memory Handling:** Deeply optimized with explicit destructor cleanup (`delete`, `clear()`) preventing any memory leaks during massive transit simulations.

## 🚀 Getting Started
To compile and simulate the yard maneuvers:

```bash
# Compile the components
g++ -std=c++11 *.cpp -o TrainTrackSystem

# Run the simulation
./TrainTrackSystem
```
