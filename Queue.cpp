#include "Queue.h"


//constructor of the Queue class that will set everything to NULL
queue::queue(){
	head = NULL;
	rear = NULL;
}
//deconstructor of the Queue class that will delete the queue when the program is finish
queue::~queue(){
}
/*
Adding the data into the queue at the rear
*/
void queue::enqueue(queue * request_queue, string &Name)
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
void queue::dequeue(queue * request_queue)
{
	//if the queue is empty
	if(request_queue -> head == NULL){
		cout<<"Nothing is in queue \n"<<endl;
		return;
	}
	//last node need to be delete
	if(request_queue -> head == request_queue -> rear){
		struct DataNode * temp = request_queue -> head;
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









