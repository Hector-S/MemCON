#ifndef CMDHANDLER_H_INCLUDED
#define CMDHANDLER_H_INCLUDED

/*
    Created by: Group 21
    Description: For handling commands and simulation.
*/

#include <fstream> //For files.
#include <string> //For strings.
#include <iostream> //For IO.
#include <iomanip> //For IO stuff.
#include <cmath> //For math stuff.

#include "Queue.h" //For the memory controller's queue & memory request IDs.

/*
    Holds simulation data for memory request read from file.
*/
struct CMDData
{
    int time = -1;
    int operation = UDF;
    uint64_t address = -1;
};

/*
    Handles commands that are stored in a linked list.
    Also handles simulation.
*/
class CMDHandler
{
    public:
        CMDHandler(); //INSTRUCTOR
        ~CMDHandler(); //DESTRUCTOR
        bool Simulate(); //Simulates the memory controller.
        bool DebugMode = false; //True if in debug mode.
        std::string SaveFileName;
        std::string LoadFileName;
    private:
        int CPU_Time = 0; //Time in CPU clock cycles.
        int MEM_Time = 0; //Time in memory clock cycles. MEM_Time = CPU_Time * 2.
        MemConQueue Queue; //The memory controller's queue.

        //Memory Controller Functions
        int ProcessRequest(uint8_t Command, uint64_t Address, bool NewRequest); //Returns # of memory clock cycles until next request can be processed.
};

#endif // CMDHANDLER_H_INCLUDED
