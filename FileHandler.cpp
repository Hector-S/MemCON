/*
    Created by: Group 21
    Description: For loading files.
*/

#include "FileHandler.h"
#include "General.h"

/*
    CONSTRUCTOR
*/
StringList::StringList()
{

}

/*
    DESTRUCTOR
*/
StringList::~StringList()
{
    Clear(); //Deletes entire list.
}

/*
    Displays the list of strings.
*/
void StringList::Display()
{
    int StrNum = 1;
    StringNode *current = Head;
    while(current)
    {
        cout << StrNum << ". " << current->data << endl;
        ++StrNum;
        current = current->next;
    }
    return;
}

/*
    Recursive function for Clear();
*/
void StringList::ClearRecursive(StringNode *current)
{
    if(current)
    {
        ClearRecursive(current->next);
        delete current;
    }
}

/*
    Clears the list.
*/
void StringList::Clear()
{
    if(Head)
    {
        ClearRecursive(Head);
        Head = nullptr;
    }
    return;
}

/*
    Fills list with names of files in current directory.
    Will clear the list first.
    If TypeCheck is false, it will load all files.
    If TypeCheck is true, it will load only ".txt" files.
*/
void StringList::LoadFromDirectory(bool TypeCheck)
{
    Clear(); //Clear the list.
    DIR *dir;
    struct dirent *ent;
    string FileName;
    if((dir = opendir (".")) != NULL)
    {
        while((ent = readdir (dir)) != NULL)
        {
            FileName = ent->d_name; //Current file name.
            if(FileName.length() >= 4) //If the file is at least 4 characters long.
            {
                if(TypeCheck) //If TypeCheck is true, it will load only ".txt" files.
                {
                    if(FileName.find(".txt", FileName.length() - 4) == (FileName.length() - 4)) //If the file ends with ".txt"
                    {
                        Add(FileName); //Adds file name to list.
                    }
                }
                else
                {
                    Add(FileName);
                }
            }
        }
        closedir (dir);
    }
    else
    {
        cout << "Error: Failed to open directory!" << endl;
    }
    return;
}

/*
    Recursive function for Add().
*/
void StringList::AddRecursive(StringNode *current, string &Name)
{
    if(current->next) //If next node exists
    {
        AddRecursive(current->next, Name);
    }
    else
    {
        current->next = new StringNode;
        current->next->data = Name;
        ++Size;
    }
    return;
}

/*
    Adds a string to the list. Returns true on success.
*/
bool StringList::Add(string &Name)
{
    if(!Name.empty()) //If we were given a non-empty string.
    {
        if(Head)
        {
            AddRecursive(Head, Name);
        }
        else
        { //
            Head = new StringNode;
            Head->data = Name;
            ++Size;
        }
        return true;
    }
    return false;
}

/*
    Obtains nth (number) string. Returns true on success.
    String reference will hold new string.
*/
bool StringList::GetString(int number, string &Buffer)
{
    StringNode *current = Head;
    int StrNum = 1;
    while(current)
    {
        if(StrNum == number) //We got to nth string.
        {
            Buffer = current->data;
            return true; //Success.
        }
        ++StrNum;
        if(StrNum > number) //The list isn't that long.
        {
            break; //Exit while loop.
        }
        current = current->next;
    }
    return false; //Failure: Failed to obtain nth string.
}

int StringList::GetSize()
{
    return Size;
}
/*
Adding the data into the queue at the rear
*/
void StringList::enqueue(queue * request_list, string &Name)
{
	struct DataNode * temp = new DataNode;

	if(request_list -> head == NULL){
		request_list -> head = temp;
	}
	else{
		request_list -> rear -> next = temp;
	}
	request_list -> rear = temp;
	request_list -> rear -> next = request_list -> head;
}
/*
Delete the data at the front of the queue
*/
void StringList::dequeue(queue * request_list)
{
	//if the queue is empty
	if(request_list -> head == NULL){
		cout<<"Nothing is in queue \n"<<endl;
		return;
	}
	//last node need to be delete
	if(request_list -> head == request_list -> rear){
		struct DataNode * temp = request_list -> head;
		delete request_list -> head;
		request_list -> head = NULL;
		request_list -> rear = NULL;
	}
	//more than 1 node is in the queue
	else{
		struct DataNode * temp = request_list -> head;
		request_list -> head = request_list -> head -> next;
		request_list -> rear = request_list -> head;
		delete temp;
	}
	return;
}
/*
display the delete request 
*/
void display_delete_node(DataNode &request){
	cout << request -> data<<endl;
	return;
}







