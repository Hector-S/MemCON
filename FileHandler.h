#ifndef FILEHANDLER_H_INCLUDED
#define FILEHANDLER_H_INCLUDED

/*
    Created by: Group 21
    Description: For loading files.
    More of a directory handler now.
*/

#include <fstream> //For files.
#include <string> //For strings.
#include <iostream> //For IO.
#include <dirent.h> //For accessing the directory.

using namespace std;
//Node for StringList
struct StringNode
{
    string data;
    StringNode *next = nullptr;
};
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
	DataNode * head;
	DataNode * rear;
};

//Linked list of strings.
class StringList
{
    public:
        StringList(); //CONSTRUCTOR
        ~StringList(); //DESTRUCTOR
        void Display(); //Displays the list of strings.
        void Clear(); //Clears the list.
        void LoadFromDirectory(bool TypeCheck); //Fills list with names of files in current directory.
        bool Add(string &Name); //Adds a string to the list. Given string will be unaltered.
        bool GetString(int number, string &Buffer); //String reference will hold new string.
        int GetSize(); //Returns size of list.
    private:
        StringNode *Head = nullptr; //List starts empty.
        int Size = 0; //Total amount of strings in the list.
		void enqueue(queue * request_list, string &request);//Adding the data into the string.
		void dequeue(queue * request_list);//Remove 1 data from the string.
		void display_delete_node(DataNode &request);//display the request that got delete.
        void AddRecursive(StringNode *current, string &Name);
        void ClearRecursive(StringNode *current);
};

#endif // FILEHANDLER_H_INCLUDED
