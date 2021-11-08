/*
    Created by: Group 21
    Description: For handling commands.
*/

#include "CMDHandler.h"

/*
    CONSTRUCTOR
*/
CMDHandler::CMDHandler()
{

}

/*
    DESTRUCTOR
*/
CMDHandler::~CMDHandler()
{
    Clear(Head);
}

/*
    Deletes CMDData list.
*/
void CMDHandler::Clear(CMDData *current)
{
    if(current)
    {
        Clear(current->next);
        delete current;
    }
    if(Size != 0)
    {
        Size = 0;
    }
}

/*
    Loads a trace file. Returns true on success.
*/
bool CMDHandler::LoadTrace(string &FileName)
{
    //Load file.
    ifstream File;
    int Request = 0; //What request "line" we were loading.
    int Time = 0; //Time of last loaded CMD.
    bool Reason = false; //Set to true for specific read fail reason.
    string temp; //To hold stuff temporarily.
    CMDData *current = nullptr;

    Clear(Head); //Clear list.
    Head = nullptr;

    File.open(FileName);
    if(File.is_open())
    {
        Head = new CMDData; //Created head.
        if(!(File >> Head->time)){goto TRACE_FAIL;} //Get time.
        Time = Head->time;
        if(!(File >> Head->operation)){goto TRACE_FAIL;}
        if((Head->operation > 2) || (Head->operation < 0)) //Invalid operation read.
        {
            cout << "Invalid operation '" << Head->operation << "' on Request " << Request << endl;
            Reason = true; goto TRACE_FAIL;
        }
        if(!(File >> temp)){goto TRACE_FAIL;}
        try
        {
            Head->address = stoull(temp, nullptr, 16); //Convert hex string to int.
        }
        catch(invalid_argument &) //Failed to convert string to int.
        {
            cout << "Invalid address string '" << temp << "' on Request " << Request << endl;
            Reason = true; goto TRACE_FAIL;
        }
        catch(out_of_range &) //Converted int is too big.
        {
            cout << "Invalid address conversion '" << Head->address << "' on Request " << Request << endl;
            Reason = true; goto TRACE_FAIL;
        }
        Reason = false;
        current = Head;
        ++Request;
        ++Size;
        while(File.peek() != EOF) //Load rest of commands until end of file. File must end with EOF we will return false and an error message.
        {
            current->next = new CMDData; //Create next CMD request.
            if(!(File >> current->next->time)){goto TRACE_FAIL;}
            if(Time > current->next->time) //If this request is from the past.
            {
                cout << "Invalid timing. Last CMD time: " << Time << " | This CMD time: " << current->next->time << ", on Request " << Request << endl;
                Reason = true; goto TRACE_FAIL;
            }
            else
            {
                Time = current->next->time;
            }
            if(!(File >> current->next->operation)){goto TRACE_FAIL;}
            if((current->next->operation > 2) || (current->next->operation < 0)) //Invalid operation read.
            {
                cout << "Invalid operation '" << current->next->operation << "' on Request " << Request << endl;
                Reason = true; goto TRACE_FAIL;
            }
            if(!(File >> temp)){goto TRACE_FAIL;}
            try
            {
                current->next->address = stoull(temp, nullptr, 16); //Convert hex string to int.
            }
            catch(invalid_argument &) //Failed to convert string to int.
            {
                cout << "Invalid address string '" << temp << "' on Request " << Request << endl;
                Reason = true; goto TRACE_FAIL;
            }
            catch(out_of_range &) //Converted int is too big.
            {
                cout << "Invalid address conversion '" << current->next->address << "' on Request " << Request << endl;
                Reason = true; goto TRACE_FAIL;
            }
            current = current->next; //Move to next current.
            ++Request;
            ++Size;
        }
        File.close();
        return true; //Success
    }
TRACE_FAIL: //Failed to read a request.
    if(!Reason)
    {
        cout << "Invalid data on Request " << Request << endl;
    }
    cout << "Failed to load file, reason is above." << endl;
    Clear(Head); //Clear list.
    Head = nullptr;
    return false;
}

/*
    Exports a visual trace file. Not loadable.
    Returns true on success.
*/
bool CMDHandler::ExportTrace(string &FileName)
{
    ofstream File;
    CMDData *current = Head;
    uint16_t BitHolder = 0;
    string temp; //For holding data temporarily.
    File.open(FileName);
    if(File.is_open())
    {
        File << "Cycle | CMD | Bank Group | Bank | Column" << endl;
        while(current)
        {
            File << dec << setfill(' ') << setw(6) << left << to_string(current->time);
            switch(current->operation)
            {
                case 0:
                    File << "  RD    ";
                    break;
                case 1:
                    File << "  WR    ";
                    break;
                case 2:
                    File << "  FET   ";
                    break;
            }
            BitHolder = ((current->address >> 6) & 0b11); //Bank Group.
            File << "0x" << hex << setw(9) << BitHolder << "  ";
            BitHolder = ((current->address >> 8) & 0b11); //Bank
            File << "0x" << setw(3) << BitHolder << "  ";
            BitHolder = ((current->address >> 3) & 0b111) | (((current->address >> 10) & 0b11111111) << 3);
            File << "0x" << setw(5) << BitHolder << endl;
            current = current->next;
        }
        File.close();
        cout << dec;
        cout << setfill(' ');
        return true;
    }
    cout << dec;
    cout << setfill(' ');
    return false;
}

/*
    Displays the list
*/
void CMDHandler::Display()
{
    int FixedWidth = 5;
    int RequestWidth = to_string(Size).length();
    int Request = 0;
    string temp; //For holding data temporarily.
    CMDData *current = Head;
    while(current)
    {
        cout << "Request " << dec << setfill(' ') << setw(RequestWidth) << Request << ':';
        temp = to_string(current->time);
        while((int) temp.length() <= FixedWidth)
        {//Add spaces until we reach the desired width.
            temp += ' ';
        }
        cout << temp;
        switch(current->operation)
        {
            case 0:
                cout << "RD ";
                break;
            case 1:
                cout << "WR ";
                break;
            case 2:
                cout << "FET";
                break;
        }
        cout << " [" << "0x" << hex << setfill('0') << setw(9) << current->address << ']' << endl;
        current = current->next;
        ++Request;
    }
    cout << dec;
    cout << setfill(' ');
    return;
}



