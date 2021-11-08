#ifndef CMDHANDLER_H_INCLUDED
#define CMDHANDLER_H_INCLUDED

/*
    Created by: Group 21
    Description: For handling commands.
*/

#include <fstream> //For files.
#include <string> //For strings.
#include <iostream> //For IO.
#include <iomanip> //For IO stuff.

using namespace std;
enum CMDID {RD = 0, WR = 1, FET = 2}; //Read, Write, Fetch

struct CMDData
{
    int time = -1;
    int operation = -1;
    long long address = -1;
    CMDData *next = nullptr;
};

/*
    Handles commands that are stored in a linked list.
*/
class CMDHandler
{
    public:
        CMDHandler(); //INSTRUCTOR
        ~CMDHandler(); //DESTRUCTOR
        bool ExportTrace(std::string &FileName); //Exports a visual trace file. Not loadable.
        bool LoadTrace(std::string &FileName); //Loads a trace file.
        void Display(); //Displays the list
    private:
        void Clear(CMDData *current); //Clears list.
        CMDData *Head = nullptr; //First command.
        int Size = 0; //Size of list.
};

#endif // CMDHANDLER_H_INCLUDED
