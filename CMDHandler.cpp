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

}

/*
    Simulates the memory controller.
*/
bool CMDHandler::Simulate()
{
    ifstream File; //Filed being loaded.
    ofstream OutputFile; //Output file.
    //File load data.
    bool Reason = false; //Set to true for specific read fail reason.
    int Request = 0; //What request "line" we were loading.
    //int Time = 0; //Time of last loaded CMD.
    string temp; //To hold stuff temporarily.
    CMDData CurReq; //Current request data.
    //Simulation data.
    CPU_Time = 0;
    MEM_Time = 0;
    int BusyTime = 0; //How much time until DIMM is no longer busy.
    int TimeUntil = 0; //Time until current request.

    cout << "---| Simulation Messages |---" << endl;
    if(DebugMode)
    {
        cout << "Debug Mode: Output timing is in memory cycles." << endl;
    }

    File.open(LoadFileName); //Open the selected file for simulation.
    OutputFile.open(SaveFileName); //Open the selected file for output.
    if(File.is_open() && OutputFile.is_open())
    {
        OutputFile << "Cycle | CMD | Bank Group | Bank | Column" << endl;
        while(File.peek() != EOF) //Load rest of commands until end of file.
        {//File must end with EOF or we will return false and an error message.
            if(!(File >> CurReq.time)){cout << "Non-integer time." << endl; Reason = true; goto TRACE_FAIL;} //Get time.
            if(!(File >> CurReq.operation)){cout << "Non-integer operation." << endl; Reason = true; goto TRACE_FAIL;} //Get operation.
            if((CurReq.operation > 2) || (CurReq.operation < 0)) //Invalid operation read.
            {
                cout << "Invalid operation '" << CurReq.operation << "' on Request " << Request << endl;
                Reason = true; goto TRACE_FAIL;
            }
            if(!(File >> temp)){goto TRACE_FAIL;} //Get address.
            try
            {
                CurReq.address = stoull(temp, nullptr, 16); //Convert hex string to int.
            }
            catch(invalid_argument &) //Failed to convert string to int.
            {
                cout << "Invalid address string '" << temp << "' on Request " << Request << endl;
                Reason = true; goto TRACE_FAIL;
            }
            catch(out_of_range &) //Converted int is too big.
            {
                cout << "Invalid address conversion '" << CurReq.address << "' on Request " << Request << endl;
                Reason = true; goto TRACE_FAIL;
            }
            //Current request was loaded.
            TimeUntil = CurReq.time - CPU_Time; //Time until next request.
            if((TimeUntil - BusyTime) == 0) //No time until next request. Process immediately.
            {
                CPU_Time = CurReq.time; //Jump immediately to this request's time.
                MEM_Time = round(CPU_Time / 2.0); //On odd # CPU cycles, we skip a memory cycle.
                BusyTime = ProcessRequest(CurReq.operation, CurReq.address, true) * 2; //1 memory cycle = 2 CPU cycles.
            }
            else if((TimeUntil - BusyTime) > 0) //We have time to process requests.
            {
                do
                { //While we have time until the next request, process requests in queue.
                    TimeUntil -= BusyTime;
                    CPU_Time += BusyTime;
                    MEM_Time += (BusyTime / 2);
                    BusyTime = ProcessRequest(CurReq.operation, CurReq.address, false) * 2;
                    if(BusyTime == 0) //Break loop if nothing in queue.
                    {
                        break;
                    }
                }while((TimeUntil - BusyTime) > 0);
                //Process current request or enqueue if no time left.
                if((TimeUntil - BusyTime) >= 0) //Process request.
                {
                    MEM_Time = round(CurReq.time / 2.0); //Jump immediately to this request's time.
                    CPU_Time = MEM_Time * 2; //Jump immediately to this request's time.
                    BusyTime = ProcessRequest(CurReq.operation, CurReq.address, true) * 2; //1 memory cycle = 2 CPU cycles.
                }
                else //Enqueue request. No time to process.
                {
                    if(!Queue.Enqueue(CurReq.operation, CurReq.address))
                    {//Adds request to queue or if queue is full does..
                        cout << "Queue is full. Request [";
                        switch(CurReq.operation)
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
                        cout << hex << setfill('0') << setw(9) << CurReq.address << " @ " << dec << CurReq.time << "] was skipped." << endl;
                        cout << setfill(' ');
                        if(DebugMode) //Display current queue in debug mode.
                        {
                            Queue.Display();
                        }
                    }
                }
            }
            else if((TimeUntil - BusyTime) < 0) //If we're still busy enqueue request.
            {
                //Advance by time needed to catch up.
                CPU_Time += BusyTime;
                MEM_Time = round(CPU_Time / 2.0); //On odd # CPU cycles, we skip a memory cycle.
                BusyTime = 0;
                if(!Queue.Enqueue(CurReq.operation, CurReq.address))
                {//Adds request to queue or if queue is full does..
                    cout << "Queue is full. Request [";
                    switch(CurReq.operation)
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
                    cout << hex << setfill('0') << setw(9) << CurReq.address << " @ " << dec << CurReq.time << "] was skipped." << endl;
                    cout << setfill(' ');
                    if(DebugMode) //Display current queue in debug mode.
                    {
                        Queue.Display();
                    }
                }
            }
            Reason = false;
            ++Request;
        }
        //Only memory requests left are in the queue. So we process requests until queue is empty.
        if(DebugMode) //Display current queue in debug mode.
        {
            cout << "---| Last Memory Requests in Queue |---" << endl;
            Queue.Display();
        }
        uint8_t temp = 0;
        uint64_t temp2 = 0;
        //NewRequest = false;
        while(!Queue.Empty())
        {
            MEM_Time += ProcessRequest(temp, temp2, false);
        }
        CPU_Time = MEM_Time * 2;
        cout << "---| Simulation Time: CPU = " << CPU_Time << " Memory = " << MEM_Time << " |---" << endl;
        cout << dec << setfill(' ');
        return true; //Success, whole file simulated.
    }
TRACE_FAIL: //Failed to read a request.
    cout << dec << setfill(' ');
    if(Reason) //If a reason was given.
    {
        cout << "Failed to simulate file, reason is above." << endl;
    }
    cout << "Invalid data on Request " << Request << '.' << endl;
    return false;
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
    Processes request given.
    Returns clock cycles until no longer busy.
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

