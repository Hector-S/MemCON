/*
    Created by: Group 21
*/

#include <iostream> //For IO.
#include <string> //For strings.

#include "General.h" //For general stuff.
#include "FileHandler.h" //For loading files.
#include "CMDHandler.h" //For handling commands.

#define IGNORE_LEN 1000 //Ignore length for incorrect input.
#define MENU_QUIT 3

using namespace std; //So we don't have to reference each time. E.g. std::cout or std::endl

namespace MainData //Holds important data for main.cpp
{
    bool TypeCheck = false; //Only ".txt" files are loadable.
    bool DebugMode = false; //Shows debug info in console.
    StringList FileNames; //Holds list of files in current directory.
    CMDHandler Simulation; //Simulation (holds timing * commands)
}

void DisplayTitle() //Displays the title.
{
    cout << "---| MemCon V0.1a ( ";
    if(MainData::TypeCheck)
    {
        cout << "FileTypeCheck ";
    }
    if(MainData::DebugMode)
    {
        cout << "Debug ";
    }
    if(!MainData::Simulation.LoadFileName.empty()) //If we've loaded a file.
    {
        cout << ") [Filed Selected: " << MainData::Simulation.LoadFileName << "] |---" << endl;
    }
    else
    {
        cout << ") [No Filed Selected] |---" << endl;
    }
    return;
}

void SelectFileMenu() //User selects a file.
{
    int Choice = 0;
    MainData::FileNames.LoadFromDirectory(MainData::TypeCheck); //Load names in current directory.
    CLEAR_SCREEN;
    while(true)
    {
        DisplayTitle();
        cout << "Select a file." << endl;
        MainData::FileNames.Display();
        cout << "Choice: ";
        cin >> Choice;
        if(MainData::FileNames.GetSize() == 0)
        {
            cout << "No loadable files in current directory." << endl;
            return;
        }
        if((Choice <= MainData::FileNames.GetSize()) && (Choice >= 1))
        {// If the choice was a possible number in the list.
            MainData::FileNames.GetString(Choice, MainData::Simulation.LoadFileName); //Get selected file name.
            CLEAR_SCREEN;
            break; //Exit the loop.
        }
        else
        {
            CLEAR_SCREEN;
            cout << "Please choose a valid file." << endl;
        }
    }
    return;
}

void SimulateMenu() //Simulate the selected file.
{
    string Choice;
    string temp;
    CLEAR_SCREEN;
    if(!MainData::Simulation.LoadFileName.empty()) //If there's a load file name.
    {
        while(true)
        {
            DisplayTitle();
            cout << "Enter save file name: ";
            cin >> Choice; //Can't fail getting a string.
            cin.ignore(IGNORE_LEN, '\n'); //Clear extra input.
            cout << "Is the save file name '" << Choice << "' correct? (y/n)" << endl;
            cin >> temp;
            if((temp[0] == 'y') || (temp[0] == 'Y')) //Can't fail getting a string.
            {//If yes.
                MainData::Simulation.SaveFileName = Choice;
                CLEAR_SCREEN;
                MainData::Simulation.Simulate(); //Simulate the selected file.
                cout << "Output trace file saved as '" << Choice << "'." << endl;
                break; //Exit the loop.
            }
            else
            {
                CLEAR_SCREEN;
                cout << "Please select a file name." << endl;
            }
        }
    }
    else
    {
        cout << "No selected file to simulate." << endl;
    }
}

int main(int argc, char *argv[])
{
    int Choice = 0;
    string ArgHolder; //To hold arguments.
    CLEAR_SCREEN; //Clear the screen.
    //Load command arguments
    for(int i = 1; i < argc; ++i)
    {
        ArgHolder = argv[i];
        if(ArgHolder.compare("-t") == 0) //If the user wants type checking for loading files.
        {
            MainData::TypeCheck = true;
            cout << "Type checking for loadable files is true." << endl;
        }
        else if(ArgHolder.compare("-d") == 0) //Enable debug, prints out simulation in console.
        {
            MainData::DebugMode = true;
            MainData::Simulation.DebugMode = true;
            cout << "Debug mode enabled." << endl;
        }
        else if(ArgHolder.compare("-f") == 0) //Load file
        {
            if((i+1) < argc) //If the next argument exists, take it as a file name.
            {
                ++i;
                MainData::Simulation.LoadFileName = argv[i];
            }
        }
    }
    while(Choice != 5) //While we haven't selected to quit.
    {
        DisplayTitle(); //Display title.
        cout << "1. Select file" << endl;
        cout << "2. Simulate file" << endl;
        cout << "3. Change debug mode" << endl;
        cout << "4. Change file type checking" << endl;
        cout << "5. Quit" << endl;
        cout << "Choice: ";
        cin >> Choice; //Get input for choice.
        if(std::cin) //If we got an integer.
        {
            cin.clear();
            cin.ignore(IGNORE_LEN, '\n'); //Clear extra input.
            switch(Choice)
            {
                case 1: //Select file.
                    SelectFileMenu();
                    CLEAR_SCREEN;
                    break;
                case 2: //Simulate file.
                    SimulateMenu();
                    break;
                case 3: //Change debug mode.
                    MainData::DebugMode = !MainData::DebugMode;
                    MainData::Simulation.DebugMode = !MainData::Simulation.DebugMode;
                    CLEAR_SCREEN;
                    break;
                case 4: //Change file type checking.
                    MainData::TypeCheck = !MainData::TypeCheck;
                    CLEAR_SCREEN;
                    break;
                case 5: //Quit the program.
                    CLEAR_SCREEN;
                    break;
                default: //Invalid input given.
                    CLEAR_SCREEN;
                    cout << "Please select a valid option." << endl;
                    break;
            }
        }
        else
        {
            cin.clear(); //Reset error flags.
            cin.ignore(IGNORE_LEN, '\n'); //Clear the invalid input.
            CLEAR_SCREEN;
            cout << "Please enter an integer." << endl;
        }
    }
    cout << "Program quitted." << endl;
    return 0;
}
