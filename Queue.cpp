/*
    Created by: Martin Nguyen
    Edited by: Hector Soto
    Description: Defines the queue that the memory controller uses.
*/

#include "Queue.h"

using namespace std;

//constructor of the Queue class that will set everything to NULL
Queue::Queue(){
	head = NULL;
	rear = NULL;
}
//deconstructor of the Queue class that will delete the queue when the program is finish
Queue::~Queue(){
}
/*
Adding the data into the queue at the rear
*/
void Queue::enqueue(Queue * request_queue, string &Name)
{
	struct DataNode * temp = new DataNode;
	temp -> data = Name;
	if(request_queue -> head == NULL){
		request_queue -> head = temp;
	}
	else{
		request_queue -> rear -> next = temp;
	}
	request_queue -> rear = temp;
	request_queue -> rear -> next = request_queue -> head;
}
/*
Delete the data at the front of the queue
*/
void Queue::dequeue(Queue * request_queue)
{
	//if the queue is empty
	if(request_queue -> head == NULL){
		cout<<"Nothing is in queue \n"<<endl;
		return;
	}
	//last node need to be delete
	if(request_queue -> head == request_queue -> rear){
		//struct DataNode * temp = request_queue -> head; //Commented out because not used.
		//^ Remove if not used or utilize if needed.
		delete request_queue -> head;
		request_queue -> head = NULL;
		request_queue -> rear = NULL;
	}
	//more than 1 node is in the queue
	else{
		struct DataNode * temp = request_queue -> head;
		request_queue -> head = request_queue -> head -> next;
		request_queue -> rear = request_queue -> head;
		delete temp;
	}
	return;
}
/*
display the delete request
*/
void queuedisplay_delete_node(DataNode &request){
	cout << request.data<<endl;
	return;
}

//START OF MemConQueue

/*
    CONSTRUCTOR
*/
MemConQueue::MemConQueue()
{

}

/*
    DESTRUCTOR
*/
MemConQueue::~MemConQueue()
{

}

/*
    Increments a variable based off queue size.
*/
void INCR_VAR(uint8_t &x)
{
    if(x == (QUEUE_SIZE - 1))
    {
        x = 0;
    }
    else
    {
        ++x;
    }
    return;
}

/*
    Displays the queue.
*/
void MemConQueue::Display()
{
    int ListCount = 1; //Keeps track of how many queue items displayed.
    if(Data[Front].Command != UDF) //If the queue is not empty.
    {
        cout << "# | COM | [  Address  ]" << endl;
        uint8_t i = Front;
        do
        {//Display whole queue.
            cout << dec << setfill(' ') << setw(2) << ListCount << "  ";
            switch(Data[i].Command)
            {
                case RD:
                    cout << "RD ";
                    break;
                case WR:
                    cout << "WR ";
                    break;
                case FET:
                    cout << "FET";
                    break;
                case UDF:
                    cout << "UDF";
                    break;
            }
            cout << "   [0x" << hex << setfill('0') << setw(9) << Data[i].Address << ']' << endl;
            ++ListCount;
            INCR_VAR(i);
        }while(i != Back);//(((i != (Back - 1)) && (Back != 0)) || ((Back != (QUEUE_SIZE)) && (Back != 0)));
    }
    else
    {
        cout << "Queue is empty." << endl;
    }
    cout << dec << setfill(' '); //Reset cout parameters.
    return;
}

/*
    Adds memory request to queue.
    Returns false if failed to add to queue.
*/
bool MemConQueue::Enqueue(int RequestID, uint64_t NewAddress)
{
    if((Front != Back) || ((Front == Back) && (Data[Front].Command == UDF)))
    {//If the queue is not full.
        if(Data[Back].Command == UDF) //If current position is empty.
        {
            Data[Back].Command = RequestID;
            Data[Back].Address = NewAddress;
            INCR_VAR(Back);
        }
        else
        { //Current position is full.
            INCR_VAR(Back);
            Data[Back].Command = RequestID;
            Data[Back].Address = NewAddress;
            INCR_VAR(Back);
        }
        return true;
    }
    return false;
};

/*
    Removes oldest queue member.
    Returns false if empty.
*/
bool MemConQueue::Dequeue(uint8_t &Command, uint64_t &Address)
{
    if(!Empty()) //If the queue is not empty.
    {
        Command = Data[Front].Command;
        Data[Front].Command = UDF; //Indicate it's empty.
        Address = Data[Front].Address;
        INCR_VAR(Front);
        return true;
    }
    return false;
};

/*
    Returns true if the queue is empty.
*/
bool MemConQueue::Empty()
{
    return (Data[Front].Command == UDF);
}
