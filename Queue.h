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
enum RequestID {RD = 0, WR = 1, FET = 2, UDF = 4}; //Read, write, fetch, & undefined.

/*
using namespace std;
^The std namespace uses up a lot of identifiers that we can
no longer include ourselves when we declare it's use like this.
It can be seen as bad to put this in a header file because now
any code that wants to include "Queue.h" now must also deal with
not being able to use that vast amount of identifiers.
TLDR: using namespace std should only be used in .cpp files if necessary.
*/

 /*
Nodes for the CLL
*/
struct DataNode
{
	std::string data;
	DataNode * next;
};

class Queue
{
	public:
		Queue();
		~Queue();
     	void enqueue(Queue * request_queue, std::string &request);//Adding the data into the string.
		void dequeue(Queue * request_queue);//Remove 1 data from the string.
	private:
		DataNode * head;
		DataNode * rear;
};

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



