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

//Node for StringList
struct StringNode
{
    std::string data;
    StringNode *next = nullptr;
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
        bool Add(std::string &Name); //Adds a string to the list. Given string will be unaltered.
        bool GetString(int number, std::string &Buffer); //String reference will hold new string.
        int GetSize(); //Returns size of list.
    private:
        StringNode *Head = nullptr; //List starts empty.
        int Size = 0; //Total amount of strings in the list.

        void AddRecursive(StringNode *current, std::string &Name);
        void ClearRecursive(StringNode *current);
};

#endif // FILEHANDLER_H_INCLUDED
