# Air Traffic Control System

This project simulates an Air Traffic Control System using POSIX-compliant C programs based on multi-threading, IPC, and synchronization concepts.

## Problem Statement
The project includes processes for planes (passenger and cargo), airports, and an air traffic controller that manages communication using pipes and message queues.

## Features
- Passenger and Cargo planes simulation
- Airports handling multiple planes and assigning runways
- Synchronization with mutexes and semaphores
- Message queues and pipes for IPC

## Technologies Used
- C (POSIX)
- Pthread library
- Message Queues, Pipes
- Mutexes, Semaphores

## Setup Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/Air-Traffic-Control-System.git
   ```
2. Compile the code:
   ```bash
   gcc -o plane src/plane.c -lpthread
   gcc -o airtrafficcontroller src/airtrafficcontroller.c -lpthread
   gcc -o airport src/airport.c -lpthread
   gcc -o cleanup src/cleanup.c -lpthread
   ```
3. Run each program in separate terminals:
   - Run planes:
     ```bash
     ./plane
     ```
   - Run air traffic controller:
     ```bash
     ./airtrafficcontroller
     ```
   - Run airports:
     ```bash
     ./airport
     ```
   - Run cleanup:
     ```bash
     ./cleanup
     ```

## License
This project is licensed under the MIT License.
