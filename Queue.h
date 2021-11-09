#include <fstream> //For files.
#include <string> //For strings.
#include <iostream> //For IO.
#include <dirent.h> //For accessing the directory.

using namespace std;

 /*
Nodes for the CLL
*/
struct DataNode
{
	string data;
	DataNode * next;
};
struct queue
{
	public:
		queue();
		~queue();
     	void enqueue(queue * request_queue, string &request);//Adding the data into the string.
		void dequeue(queue * request_queue);//Remove 1 data from the string.
	private:
		DataNode * head;
		DataNode * rear;
};
 




