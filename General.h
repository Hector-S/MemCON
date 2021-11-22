#ifndef GENERAL_H_INCLUDED
#define GENERAL_H_INCLUDED

/*
    Created by: Group 21
    Description: A header file to hold general stuff.
*/

//The following is for easy screen clearing on Windows or Linux.
#ifdef _WIN32
    #define CLEAR_SCREEN system("cls")
#elif defined __unix__
    #define CLEAR_SCREEN system("clear")
#endif

#endif // GENERAL_H_INCLUDED

