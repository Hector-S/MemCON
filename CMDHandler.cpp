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
bool CMDHandler::LoadTrace(std::string &FileName)
{
    //Load file.
    std::ifstream File;
    int Request = 0; //What request "line" we were loading.
    int Time = 0; //Time of last loaded CMD.
    bool Reason = false; //Set to true for specific read fail reason.
    std::string temp; //To hold stuff temporarily.
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
            std::cout << "Invalid operation '" << Head->operation << "' on Request " << Request << std::endl;
            Reason = true; goto TRACE_FAIL;
        }
        if(!(File >> temp)){goto TRACE_FAIL;}
        try
        {
            Head->address = std::stoull(temp, nullptr, 16); //Convert hex string to int.
        }
        catch(std::invalid_argument &) //Failed to convert string to int.
        {
            std::cout << "Invalid address string '" << temp << "' on Request " << Request << std::endl;
            Reason = true; goto TRACE_FAIL;
        }
        catch(std::out_of_range &) //Converted int is too big.
        {
            std::cout << "Invalid address conversion '" << Head->address << "' on Request " << Request << std::endl;
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
                std::cout << "Invalid timing. Last CMD time: " << Time << " | This CMD time: " << current->next->time << ", on Request " << Request << std::endl;
                Reason = true; goto TRACE_FAIL;
            }
            else
            {
                Time = current->next->time;
            }
            if(!(File >> current->next->operation)){goto TRACE_FAIL;}
            if((current->next->operation > 2) || (current->next->operation < 0)) //Invalid operation read.
            {
                std::cout << "Invalid operation '" << current->next->operation << "' on Request " << Request << std::endl;
                Reason = true; goto TRACE_FAIL;
            }
            if(!(File >> temp)){goto TRACE_FAIL;}
            try
            {
                current->next->address = std::stoull(temp, nullptr, 16); //Convert hex string to int.
            }
            catch(std::invalid_argument &) //Failed to convert string to int.
            {
                std::cout << "Invalid address string '" << temp << "' on Request " << Request << std::endl;
                Reason = true; goto TRACE_FAIL;
            }
            catch(std::out_of_range &) //Converted int is too big.
            {
                std::cout << "Invalid address conversion '" << current->next->address << "' on Request " << Request << std::endl;
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
        std::cout << "Invalid data on Request " << Request << std::endl;
    }
    std::cout << "Failed to load file, reason is above." << std::endl;
    Clear(Head); //Clear list.
    Head = nullptr;
    return false;
}

#define BIN_16(B3,B2,B1,B0) ((B3 << 12) + (B2 << 8) + (B1 << 4) + B0) //To create 16-bit binary literals easier.

/*
    Exports a visual trace file. Not loadable.
    Returns true on success.
*/
bool CMDHandler::ExportTrace(std::string &FileName)
{
    std::ofstream File;
    CMDData *current = Head;
    uint16_t BitHolder = 0;
    std::string temp; //For holding data temporarily.
    File.open(FileName);
    if(File.is_open())
    {
        File << "Cycle | CMD | Bank Group | Bank | Column" << std::endl;
        while(current)
        {
            File << std::dec << std::setfill(' ') << std::setw(6) << std::left << std::to_string(current->time);
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
            File << "0x" << std::hex << std::setw(9) << BitHolder << "  ";
            BitHolder = ((current->address >> 8) & 0b11); //Bank
            File << "0x" << std::setw(3) << BitHolder << "  ";
            BitHolder = ((current->address >> 3) & 0b111) | (((current->address >> 10) & 0b1111111) << 3);
            File << "0x" << std::setw(5) << BitHolder << std::endl;
            current = current->next;
        }
        File.close();
        std::cout << std::dec;
        std::cout << std::setfill(' ');
        return true;
    }
    std::cout << std::dec;
    std::cout << std::setfill(' ');
    return false;
}

/*
    Displays the list
*/
void CMDHandler::Display()
{
    int FixedWidth = 5;
    int RequestWidth = std::to_string(Size).length();
    int Request = 0;
    std::string temp; //For holding data temporarily.
    CMDData *current = Head;
    while(current)
    {
        std::cout << "Request " << std::dec << std::setfill(' ') << std::setw(RequestWidth) << Request << ':';
        temp = std::to_string(current->time);
        while((int) temp.length() <= FixedWidth)
        {//Add spaces until we reach the desired width.
            temp += ' ';
        }
        std::cout << temp;
        switch(current->operation)
        {
            case 0:
                std::cout << "RD ";
                break;
            case 1:
                std::cout << "WR ";
                break;
            case 2:
                std::cout << "FET";
                break;
        }
        std::cout << " [" << "0x" << std::hex << std::setfill('0') << std::setw(9) << current->address << ']' << std::endl;
        current = current->next;
        ++Request;
    }
    std::cout << std::dec;
    std::cout << std::setfill(' ');
    return;
}



