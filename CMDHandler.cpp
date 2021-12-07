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
    //Reset memory controller data.
    LastBankGroup = -1, LastBank = -1, LastColumn = -1, LastRow = -1; TimeLastACT = -10000;

    cout << "---| Simulation Messages |---" << endl;
    if(DebugMode)
    {
        cout << "Debug Mode: Output timing is in memory cycles." << endl;
    }

    File.open(LoadFileName); //Open the selected file for simulation.
    OutputFile.open(SaveFileName); //Open the selected file for output.
    if(File.is_open() && OutputFile.is_open())
    {
        if(LabelOutput) //Label output if true.
        {
            OutputFile << "Cycle | CMD | Bank Group | Bank | Col/Row" << endl;
        }
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
            TimeUntil = CurReq.time - CPU_Time; //Time until this request.
            while(TimeUntil >= 0)
            {//Handle requests in queue until empty or out of time.
                BusyTime = ProcessRequest(0, 0, false) * 2; //Get CPU cycles.
                if(BusyTime != 0)
                {
                    TimeUntil -= BusyTime;
                    CPU_Time += BusyTime;
                    MEM_Time += BusyTime / 2;
                }
                else
                {
                    break; //Exit loop if queue is empty.
                }
            }
            if(TimeUntil >= 0) //If there's still time until this request.
            { //Process the request.
                CPU_Time = CurReq.time; //Jump to this request's time.
                MEM_Time = round(CurReq.time / 2.0);
                BusyTime = ProcessRequest(CurReq.operation, CurReq.address, true);
                CPU_Time += BusyTime * 2;
                MEM_Time += BusyTime;
            }
            else //Enqueue request if no time left.
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
int WriteCommandSimple(ofstream &File, uint8_t Command, uint64_t Address, int MemTime, bool DebugMode)
{
    int PassedCycles = 5; //How many memory cycles have been used.
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
    BitHolder = ((Address >> 3) & 0b111) | (((Address >> 10) & 0b11111111) << 3); //Column
    File << "0x" << setw(5) << BitHolder << endl;
    cout << dec << setfill(' ');
    return PassedCycles;
}

/*
    Write DRAM commands to simulate output file.
*/
int CMDHandler::WriteCommand(ofstream &File, uint8_t Command, uint64_t Address, int MemTime, bool DebugMode)
{
    int PassedCycles = 0; //To track cycles passed.
    int DelayRAS = 0; //Need to account for RAS and delay time until precharge.
    uint16_t BankGroup = 0, Bank = 0, Column = 0, Row = 0;
    BankGroup = ((Address >> 6) & 0b11); //Bank Group.
    Bank = ((Address >> 8) & 0b11); //Bank
    Column = ((Address >> 3) & 0b111) | (((Address >> 10) & 0b11111111) << 3); //Column
    Row = Address >> 18; //Row
    //Get delay for T_RAS if needed.
    DelayRAS = T_RAS - (MEM_Time - TimeLastACT);
    if(DelayRAS < 0)
    {
        DelayRAS = 0;
    }
    if(!DebugMode) //Output timing as memory cycles if not in debug mode.
    {
        MemTime *= 2;
    }
    //Currently there is no bank parallelism.
    if(Row == LastRow) //Page hit or empty.
    {
        //If statement for page hit/empty is incorrect. Need to change.
        if(Bank == LastBank) //Page hit
        {
            if(DebugMode){cout << "Page Hit" << endl;}
        }
        else //Page empty
        {
            if(DebugMode){cout << "Page Empty" << endl;}
            File << left << setw(6) << to_string(MemTime + PassedCycles*(1 + !DebugMode)); //Output current time.
            TimeLastACT = MemTime + PassedCycles;
            File << "  ACT   " ; //Output bank group + bank + row;
            File << "0x" << hex << setw(9) << BankGroup << "  0x" << setw(3) << Bank << "  0x" << setw(5) << Row << endl;
            PassedCycles += T_RCD; //Advance time by RAS to CAS delay.
        }
    }
    else //Page Miss
    {
        if(DebugMode){cout << "Page Miss" << endl;}
        PassedCycles += DelayRAS; //Delay for T_RAS if needed.
        File << left << setw(6) << to_string(MemTime + PassedCycles*(1 + !DebugMode)); //Output current time.
        File << "  PRE   " ; //Output bank group + bank;
        File << "0x" << hex << setw(9) << BankGroup << "  0x" << setw(3) << Bank << "  " << endl;
        PassedCycles += T_RP; //Advance time by row precharge timing.

        File << left << setw(6) << to_string(MemTime + PassedCycles*(1 + !DebugMode)); //Output current time.
        TimeLastACT = MemTime + PassedCycles;
        File << "  ACT   " ; //Output bank group + bank + row;
        File << "0x" << hex << setw(9) << BankGroup << "  0x" << setw(3) << Bank << "  0x" << setw(5) << Row << endl;
        PassedCycles += T_RCD; //Advance time by RAS to CAS delay.
    }
    File << left << setw(6) << to_string(MemTime + PassedCycles*(1 + !DebugMode)); //Output current time.
    //Send command.
    if(Command == RD)
    {
        File << "  RD    " ; //Output bank group + bank + column;
        File << "0x" << hex << setw(9) << BankGroup << "  0x" << setw(3) << Bank << "  0x" << setw(5) << Column << endl;
        PassedCycles += T_CAS + T_BURST; //Advance time by column delay & data burst.
    }
    else if(Command == WR)
    {
        File << "  WR    " ; //Output bank group + bank + column;
        File << "0x" << hex << setw(9) << BankGroup << "  0x" << setw(3) << Bank << "  0x" << setw(5) << Column << endl;
        PassedCycles += T_CAS + T_BURST; //Advance time by column delay.
    }
    else if(Command == FET) //Duplicate of RD for now.
    {
        File << "  RD    " ; //Output bank group + bank + column;
        File << "0x" << hex << setw(9) << BankGroup << "  0x" << setw(3) << Bank << "  0x" << setw(5) << Column << endl;
        PassedCycles += T_CAS + T_BURST; //Advance time by column delay & data burst.
    }
    else
    {
        cout << "Error: Invalid command given." << endl;
    }
    LastBankGroup = BankGroup; LastBank = Bank; LastColumn = Column; LastRow = Row;
    cout << dec << setfill(' '); //Reset output format.
    return PassedCycles;
}


/*
    Processes request given.
    Returns memory clock cycles needed for request.
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
            if(!SimpleOutput)
            {
                ClockCycles = WriteCommand(File, Command, Address, MEM_Time, DebugMode);
            }
            else
            {
                ClockCycles = WriteCommandSimple(File, Command, Address, MEM_Time, DebugMode);
            }
        }
        else
        {//Process request in queue instead.
            if(Queue.Dequeue(Command, Address))
            { //If something was dequeued, process it.
                if(!SimpleOutput)
                {
                    ClockCycles = WriteCommand(File, Command, Address, MEM_Time, DebugMode);
                }
                else
                {
                    ClockCycles = WriteCommandSimple(File, Command, Address, MEM_Time, DebugMode);
                }
            }
        }
        File.close();
    }
    return ClockCycles;
}

