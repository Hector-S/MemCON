/*
    Created by: Group 21
    Description: For handling commands.
*/

#include "CMDHandler.h"

using namespace std;

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
    Removes oldest request from list and returns pointer to it.
    You must free the returned data node to avoid a memory leak.
    Returns nullptr if we reached end of requests.
*/
CMDData *CMDHandler::GetRequest()
{
    CMDData *temp = Head;
    if(Head)
    {
        Head = Head->next;
        --Size;
        return temp; //Return old head.
    }
    return nullptr;
}

/*
    Loads a trace file. Returns true on success.
*/
bool CMDHandler::LoadTrace()
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

    File.open(LoadFileName);
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
bool CMDHandler::ExportTrace()
{
    ofstream File;
    CMDData *current = Head;
    uint16_t BitHolder = 0;
    string temp; //For holding data temporarily.
    File.open(SaveFileName);
    if(File.is_open())
    {
        File << "Cycle | CMD | Bank Group | Bank | Column" << endl;
        while(current)
        {
            File << dec << setfill(' ') << setw(6) << left << to_string(current->time);
            switch(current->operation)
            {
                case RD:
                    File << "  RD    ";
                    break;
                case WR:
                    File << "  WR    ";
                    break;
                case FET:
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
    cout << "---| Requests Read |---" << endl;
    while(current)
    {
        cout << "Request " << dec << setfill(' ') << setw(RequestWidth) << Request << ": ";
        temp = to_string(current->time);
        while((int) temp.length() <= FixedWidth)
        {//Add spaces until we reach the desired width.
            temp += ' ';
        }
        cout << temp;
        switch(current->operation)
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
        }
        cout << " [" << "0x" << hex << setfill('0') << setw(9) << current->address << ']' << endl;
        current = current->next;
        ++Request;
    }
    cout << dec;
    cout << setfill(' ');
    return;
}

/*
    Simulates the memory controller.
*/
void CMDHandler::Simulate()
{
    int TimeForNextRequest = 0; //How much time until next request can be processed in CPU cycles.
    int TimeUntilNextRequest = 0; //How much time until next CPU memory request.
    CMDData *TempRequestData = nullptr;
    ofstream File;
    File.open(SaveFileName);
    if(File.is_open()) //Create simulation's output file.
    {
        File << "Cycle | CMD | Bank Group | Bank | Column" << endl;
        File.close();
    }
    cout << "---| Simulation Messages |---" << endl;
    if(DebugMode)
    {
        cout << "Debug Mode: Output timing is in memory cycles." << endl;
    }
    TempRequestData = GetRequest(); //Gets next memory request.
    if(TempRequestData) //In case file doesn't start with clock cycle at 0.
    {
        CPU_Time = TempRequestData->time; //Set CPU time to time of latest CPU request.
        MEM_Time = round(CPU_Time / 2.0); //On odd # CPU cycles, we skip a memory cycle.
    }

    while(TempRequestData) //While there are still CPU memory requests to be issued.
    { //Process those requests.
        int temp;
        if(TimeUntilNextRequest >= 0)
        {
            temp = ProcessRequest(TempRequestData->operation, TempRequestData->address, true);
            MEM_Time += temp;
            CPU_Time = MEM_Time * 2;
            //Process request
            delete TempRequestData; //Free data node we extracted from list.
            TempRequestData = GetRequest(); //Gets next memory request.
        }
        else
        {
            while(TimeUntilNextRequest < 0)
            {
                if(!Queue.Enqueue(TempRequestData->operation, TempRequestData->address))
                {//Adds request to queue, or if queue is full does...
                    cout << "Queue is full. Request [";
                    switch(TempRequestData->operation)
                    {
                        case RD:
                            cout << "RD  ";
                            break;
                        case WR:
                            cout << "WR  ";
                            break;
                        case FET:
                            cout << "FET ";
                            break;
                        case UDF:
                            cout << "UDF ";
                            break;
                    }
                    cout << hex << setfill('0') << setw(9) << TempRequestData->address << " @ " << dec << TempRequestData->time << "] was skipped." << endl;
                    cout << setfill(' ');
                    if(DebugMode) //Display current queue in debug mode.
                    {
                        Queue.Display();
                    }
                } //We enqueued the request, move on to next one.
                delete TempRequestData; //Free data node we extracted from list.
                TempRequestData = GetRequest(); //Gets next memory request.
                if(TempRequestData)
                {
                    TimeUntilNextRequest = TempRequestData->time - CPU_Time;
                }
                else
                {
                    break;
                }
            }
        }
        if(TempRequestData)
        {
            TimeUntilNextRequest = TempRequestData->time - CPU_Time;
            if(TimeUntilNextRequest > 0) //If we have time until next request.
            {//Process requests in queue.
                if(!Queue.Empty()) //If queue is not empty.
                {
                    while(TimeUntilNextRequest > 0)
                    { //Process items in queue until we get next CPU memory request.
                        temp = ProcessRequest(TempRequestData->operation, TempRequestData->address, false);
                        MEM_Time += temp;
                        TimeForNextRequest -= temp * 2;
                        if(temp == 0) //If queue is empty.
                        { //Jump to next CPU memory request time.
                            CPU_Time = TempRequestData->time;
                            MEM_Time = round(CPU_Time / 2.0);
                            TimeUntilNextRequest = 0;
                            break;
                        }
                    }
                    if(TimeUntilNextRequest < 0) //Enqueue CPU_Request.
                    { //CPU memory request came while processing a request.
                        CPU_Time = MEM_Time * 2;
                        CPU_Time -= TimeUntilNextRequest; //Advance CPU_Time until request is done.
                        MEM_Time = round(CPU_Time / 2.0);
                        TimeUntilNextRequest = 0;
                        if(!Queue.Enqueue(TempRequestData->operation, TempRequestData->address))
                        {//Adds request to queue, or if queue is full does...
                            cout << "Queue is full. Request [";
                            switch(TempRequestData->operation)
                            {
                                case RD:
                                    cout << "RD  ";
                                    break;
                                case WR:
                                    cout << "WR  ";
                                    break;
                                case FET:
                                    cout << "FET ";
                                    break;
                                case UDF:
                                    cout << "UDF ";
                                    break;
                            }
                            cout << hex << setfill('0') << setw(9) << TempRequestData->address << " @ " << dec << TempRequestData->time << "] was skipped." << endl;
                            cout << setfill(' ');
                            if(DebugMode) //Display current queue in debug mode.
                            {
                                Queue.Display();
                            }
                        } //We enqueued the request, move on to next one.
                        delete TempRequestData; //Free data node we extracted from list.
                        TempRequestData = GetRequest(); //Gets next memory request.
                    }
                }
                else //If queue is empty.
                {//Jump to latest request time.
                    CPU_Time = TempRequestData->time;
                    MEM_Time = round(CPU_Time / 2.0);
                    TimeUntilNextRequest = 0;
                }
            }
            else if(TimeUntilNextRequest == 0)
            {//Jump to latest request time.
                CPU_Time = TempRequestData->time; //Advance time.
                MEM_Time = round(CPU_Time / 2.0);
            }
        }
    }
    /*
        Only memory requests left are in the queue. So we process requests until queue is empty.
    */
    if(DebugMode) //Display current queue in debug mode.
    {
        cout << "---| Last Memory Requests in Queue |---" << endl;
        Queue.Display();
    }
    uint8_t temp;
    uint64_t temp2;
    //NewRequest = false;
    while(!Queue.Empty())
    {
        MEM_Time += ProcessRequest(temp, temp2, false);
    }
    CPU_Time = MEM_Time * 2;
    cout << "---| Simulation Time: CPU = " << CPU_Time << " Memory = " << MEM_Time << " |---" << endl;
    //Clear simulation data.
    CPU_Time = 0;
    MEM_Time = 0;
    while(Queue.Dequeue(temp, temp2)) //Clear Queue.
    {

    }
    LoadTrace(); //Re-read memory requests from file.
    return;
}

/*
    Writes given command to simulate output file.
*/
void WriteCommand(ofstream &File, uint8_t Command, uint64_t Address, int MemTime, bool DebugMode)
{
    if(!DebugMode) //Output timing as memory cycles if not in debug mode.
    {
        MemTime *= 2;
    }
    uint16_t BitHolder = 0;
    File << left << setw(6) << to_string(MemTime);
    switch(Command)
    {
        case RD:
            File << "  RD    ";
            break;
        case WR:
            File << "  WR    ";
            break;
        case FET:
            File << "  FET   ";
            break;
        case UDF:
            File << "  UDF   ";
            break;
    }
    BitHolder = ((Address >> 6) & 0b11); //Bank Group.
    File << "0x" << hex << setw(9) << BitHolder << "  ";
    BitHolder = ((Address >> 8) & 0b11); //Bank
    File << "0x" << setw(3) << BitHolder << "  ";
    BitHolder = ((Address >> 3) & 0b111) | (((Address >> 10) & 0b11111111) << 3);
    File << "0x" << setw(5) << BitHolder << endl;
    cout << dec << setfill(' ');
    return;
}


/*
    Processes request given or will enqueue it
    if the queue is not empty.
*/
int CMDHandler::ProcessRequest(uint8_t Command, uint64_t Address, bool NewRequest)
{
    int ClockCycles = 0; //Number of cycles request will take.
    ofstream File;
    File.open(SaveFileName, ios_base::app);
    if(File.is_open())
    {
        if(NewRequest)
        {
            WriteCommand(File, Command, Address, MEM_Time, DebugMode);
            ClockCycles = 5; //Set fake time to do request until we code it.
        }
        else
        {//Process request in queue instead.
            if(Queue.Dequeue(Command, Address))
            { //If something was dequeued, process it.
                WriteCommand(File, Command, Address, MEM_Time, DebugMode);
                ClockCycles = 5; //Set fake time to do request until we code it.
            }
        }
        File.close();
    }
    return ClockCycles;
}

