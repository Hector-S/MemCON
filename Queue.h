#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

/*
    Created by: Martin Nguyen
    Edited by: Hector Soto
    Description: Defines the queue that the memory controller uses.
*/


#include <fstream> //For files.
#include <string> //For strings.
#include <iostream> //For IO.
#include <iomanip> //For IO stuff.
#include <dirent.h> //For accessing the directory.
#include <queue> //For use as a test queue.

#define QUEUE_SIZE 16 //Queue has a max size of 16.

//IDs for memory requests.
enum RequestID {RD = 0, WR = 1, FET = 2, UDF = 3}; //Read, write, fetch, & undefined.

/*
    Holds a memory request.
    It holds the instruction and address.
*/

struct MemoryRequest
{
    uint8_t Command = UDF; //What type of request. Read, write, instruction fetch.
    //^Would be 2 bits in memory controller.
    uint64_t Address = 0; //Would be 33 bits in memory controller.
};

/*
    Queue used to hold memory requests.
*/
class MemConQueue
{
    public:
        MemConQueue(); //CONSTRUCTOR
        ~MemConQueue(); //DESTRUCTOR
        void Display(); //Displays the queue.
        bool Enqueue(int RequestID, uint64_t NewAddress); //Returns false if failed to add to queue.
        bool Dequeue(uint8_t &Command, uint64_t &Address); //Removes oldest queue member. Returns false if empty.
        bool Empty(); //Returns true if the queue is empty.
    private:
        MemoryRequest Data[QUEUE_SIZE]; //The queue's data.
        uint8_t Front = 0; //Would be 4 bits in memory controller.
        uint8_t Back = 0; //Would be 4 bits in memory controller.
};

/*
    A test queue that utilizes the default queue in C++ std.
    Doesn't return the same exact values as MemConQueue, leading
    to the program giving incorrect output when in debug mode.
    When not in debug mode, program works correctly with this queue.
*/
class TestQueue
{
    public:
        TestQueue(){}; //CONSTRUCTOR
        ~TestQueue(){}; //DESTRUCTOR
        void Display()
        {
            int ListCount = 1; //Keeps track of how many queue items displayed.
            if(!data.empty())
            {
                std::cout << "# | COM | [  Address  ]" << std::endl;
                while(!data.empty())
                {
                    std::cout << std::dec << std::setfill(' ') << std::setw(2) << ListCount << "  ";
                    MemoryRequest temp = data.front();
                    data.pop();
                    switch(temp.Command)
                    {
                        case RD:
                            std::cout << "RD ";
                            break;
                        case WR:
                            std::cout << "WR ";
                            break;
                        case FET:
                            std::cout << "FET";
                            break;
                        case UDF:
                            std::cout << "UDF";
                            break;
                    }
                    std::cout << "   [0x" << std::hex << std::setfill('0') << std::setw(9) << temp.Address << ']' << std::endl;
                    ++ListCount;
                }
                std::cout << std::dec << std::setfill(' '); //Reset cout parameters.
            }
            else
            {
                std::cout << "Queue is empty." << std::endl;
            }
        }
        bool Enqueue(int RequestID, uint64_t NewAddress)
        {
            if(data.size() < QUEUE_SIZE)
            {
                MemoryRequest temp = {(uint8_t) RequestID, NewAddress};
                data.push(temp);
                return true;
            }
            return false; //Queue is full failed to enqueue.
        }
        bool Dequeue(uint8_t &Command, uint64_t &Address)
        {
            if(!data.empty())
            {
                MemoryRequest temp = data.front();
                Command = temp.Command; Address = temp.Address;
                data.pop();
                return true;
            }
            return false; //Queue is empty, can't dequeue.
        }
        bool Empty(){return data.empty();}
    private:
        std::queue<MemoryRequest> data;
};

#endif // QUEUE_H_INCLUDED



