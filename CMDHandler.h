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

//Timing Parameters in memory cycles.
#define T_RC    76
#define T_RAS   52
#define T_RRD_L 6
#define T_RRD_S 4
#define T_RP    24
#define T_RFC   560
#define T_CWD   20
#define T_CAS   24
#define T_RCD   24
#define T_WR    20
#define T_RTP   12
#define T_CCD_L 8
#define T_CCD_S 4
#define T_BURST 4
#define T_WRT_L 12
#define T_WTR_S 4

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
        bool LabelOutput = false; //True if output labels data columns.
        bool SimpleOutput = false; //True if outputting requests with constant cycles of 10, instead of DRAM commands.
        std::string SaveFileName;
        std::string LoadFileName;
    private:
        int CPU_Time = 0; //Time in CPU clock cycles.
        int MEM_Time = 0; //Time in memory clock cycles. MEM_Time = CPU_Time * 2.
        MemConQueue Queue; //The memory controller's queue.

        //Memory Controller Stuff
        uint16_t LastBankGroup = -1, LastBank = -1, LastColumn = -1, LastRow = -1; //2 bits, 2 bits, 11 bits, 15 bits.
        int TimeLastACT = -10000; //Last time an activate command was given.
        int WriteCommand(std::ofstream &File, uint8_t Command, uint64_t Address, int MemTime, bool DebugMode);
        int ProcessRequest(uint8_t Command, uint64_t Address, bool NewRequest); //Returns # of memory clock cycles until next request can be processed.
};

#endif // CMDHANDLER_H_INCLUDED
