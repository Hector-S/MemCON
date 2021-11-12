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
    CMDData *next = nullptr;
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
        bool ExportTrace(); //Exports a visual trace file. Not loadable.
        bool LoadTrace(); //Loads a trace file.
        void Display(); //Displays the list
        void Simulate(); //Simulates the memory controller.
        bool DebugMode = false; //True if in debug mode.
        std::string SaveFileName;
        std::string LoadFileName;
    private:
        int Size = 0; //Size of list.
        int CPU_Time = 0; //Time in CPU clock cycles.
        int MEM_Time = 0; //Time in memory clock cycles. MEM_Time = CPU_Time * 2.
        CMDData *Head = nullptr; //First command.
        MemConQueue Queue; //The memory controller's queue.
        void Clear(CMDData *current); //Clears list.
        CMDData *GetRequest(); //Returns null if we reached end of requests.

        //Memory Controller Functions
        int ProcessRequest(uint8_t Command, uint64_t Address, bool NewRequest); //Returns # of memory clock cycles until next request can be processed.
};

#endif // CMDHANDLER_H_INCLUDED
