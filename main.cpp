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
    string LoadedFile; //To hold file name of loaded file.
    string InputString; //To hold input strings.
    StringList FileNames; //Holds list of files in current directory.
    CMDHandler Simulation; //Simulation (holds timing * commands)
}

enum MenuID {MAIN = 0, LOAD, SIM}; //IDs for the different menus.

void DisplayTitle() //Displays the title.
{
    cout << "---| MemCon V0.92 (";
    if(MainData::TypeCheck)
    {
        cout << "TypeCheckEnabled ";
    }
    if(MainData::DebugMode)
    {
        cout << "DebugMode ";
    }
    if(!MainData::LoadedFile.empty()) //If we've loaded a file.
    {
        cout << ") [Filed Loaded: " << MainData::LoadedFile << "] |---" << endl;
    }
    else
    {
        cout << ") [No Filed Loaded] |---" << endl;
    }
}

/*
    Displays selected menu.
*/
void DisplayMenu(int Menu)
{
    switch(Menu)
    {
        case MAIN:
            DisplayTitle();
            cout << "1. Load File" << endl;
            cout << "2. Simulate" << endl;
            cout << "3. Quit" << endl;
            cout << "Choice: ";
            break;
        case LOAD:
            MainData::FileNames.LoadFromDirectory(MainData::TypeCheck); //Load names in current directory.
            DisplayTitle();
            cout << "Choose file to open." << endl;
            MainData::FileNames.Display();
            cout << "Choice: ";
            break;
        case SIM:
            DisplayTitle();
            cout << "Please enter a save file name." << endl;
            cout << "Choice: ";
            break;
        default:

            break;
    }
    return;
}

/*
    Does the function of the selected menu.
*/
void MenuFunction(int &Menu, int &Choice)
{
    switch(Menu)
    {
        case MAIN: //Main menu function.
            switch(Choice)
            {
                case 1:
                    Menu = LOAD;
                    system(CLEAR_STRING);
                    break;
                case 2:
                    Menu = SIM;
                    system(CLEAR_STRING);
                    break;
                default:
                    if(Choice != MENU_QUIT) //Quitting is a valid option.
                    {
                        cin.clear(); //Clear the invalid input.
                        cin.ignore(IGNORE_LEN, '\n');
                        system(CLEAR_STRING); //Clear the console with system call.
                        cout << "Please enter a valid option." << endl;
                    }
                    break;
            }
            break;
        case LOAD: //Load menu function.
            if(Choice <= MainData::FileNames.GetSize())
            {
                MainData::FileNames.GetString(Choice, MainData::LoadedFile); //Get selected file name.
                Menu = MAIN; //Return to main menu.
                Choice = 0; //Avoid choice of MENU_QUIT quitting program.
                system(CLEAR_STRING);
                MainData::Simulation.LoadFileName = MainData::LoadedFile; //Set simulation load file name.
                if(!MainData::Simulation.LoadTrace()) //Load trace input from file.
                {//If we failed to load the file.
                    MainData::LoadedFile.clear();
                }
            }
            else
            {
                system(CLEAR_STRING);
                cout << "Please choose a valid file." << endl;
            }
            break;
        case SIM: //Simulation menu function.
            { //Curly brackets to restrict existence of SaveFileName.
            string SaveFileName = MainData::InputString;
            cout << "Is the save file name '" << SaveFileName << "' correct? (y/n)" << endl;
            cin >> MainData::InputString;
            cin.ignore(IGNORE_LEN, '\n'); //Clear extra input.
            system(CLEAR_STRING);
            if((MainData::InputString[0] == 'y') || (MainData::InputString[0] == 'Y')) //Can't fail getting a string.
            { //Any input that starts with y/Y is considered a yes.
                MainData::Simulation.SaveFileName = SaveFileName; //Set save file name for simulation.
                if(MainData::Simulation.ExportTrace()) //If file is saved successfully.
                {
                    MainData::Simulation.Simulate();
                    cout << "File was saved as '" << SaveFileName << "'." << endl;
                    if(MainData::DebugMode) //If in debug mode.
                    {
                        MainData::Simulation.Display();
                    }
                }
                else
                {
                    cout << "Failed to save file." << endl;
                }
            }
            else
            {
                cout << "File was not saved." << endl;
            }
            Menu = MAIN; //Return to main menu.
            }
            break;
        default:

            break;
    }
}

int main(int argc, char *argv[])
{
    int Menu = MAIN; //Start in main menu.
    int Choice = 0;
    system(CLEAR_STRING);
    //Load command arguments
    for(int i = 1; i < argc; ++i)
    {
        MainData::InputString = argv[i];
        if(MainData::InputString.compare("-t") == 0) //If the user wants type checking for loading files.
        {
            MainData::TypeCheck = true;
            cout << "Type checking for loadable files is true." << endl;
        }
        else if(MainData::InputString.compare("-d") == 0) //Enable debug, prints out simulation in console.
        {
            MainData::DebugMode = true;
            MainData::Simulation.DebugMode = true;
            cout << "Debug mode enabled." << endl;
        }
        else if(MainData::InputString.compare("-f") == 0) //Load file
        {
            if((i+1) < argc) //If the next argument exists, take it as a file name.
            {
                ++i;
                MainData::LoadedFile = argv[i];
                MainData::Simulation.LoadFileName = MainData::LoadedFile;
                if(!MainData::Simulation.LoadTrace()) //Load trace input from file.
                {//If we failed to load the file.
                    MainData::LoadedFile.clear();
                }
            }
        }
    }
    while(!((Menu == MAIN) && (Choice == MENU_QUIT)))
    {
        DisplayMenu(Menu);
        cin >> Choice;
        if(std::cin) //If the user typed in an integer
        { // ^Returns false if anything but an integer is typed.
            MenuFunction(Menu, Choice);
        }
        else //An integer was not selected.
        {
            if(Menu == SIM) //SIM expects a string.
            {
                cin.clear();
                cin >> MainData::InputString; //Can't fail getting a string.
                cin.ignore(IGNORE_LEN, '\n'); //Clear extra input.
                MenuFunction(Menu, Choice);
            }
            else
            {
                cin.clear(); //Reset error flags.
                cin.ignore(IGNORE_LEN, '\n'); //Clear the invalid input.
                system(CLEAR_STRING);
                cout << "Please enter an integer." << endl;
            }
        }
    }
    return 0;
}
