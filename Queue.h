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

#endif // QUEUE_H_INCLUDED



