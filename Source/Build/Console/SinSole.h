#pragma once
#include "wincon.h"
using namespace Zilch;

#define PROTECTCOLORS true
#define DEFAULTTEXT ConsoleColors::Red;
#define DEFAULTBACKGROUND ConsoleColors::Black

namespace ConsoleColors
{
	enum ConsoleColors
	{
		Black = 0,                                                                         //! 0
		DarkBlue = FOREGROUND_BLUE,                                                        //! 1
		DarkGreen = FOREGROUND_GREEN,                                                      //! 2  
		DarkCyan = FOREGROUND_GREEN | FOREGROUND_BLUE,                                     //! 3
		DarkRed = FOREGROUND_RED,                                                          //! 4
		DarkMagenta = FOREGROUND_RED | FOREGROUND_BLUE,                                    //! 5
		DarkYellow = FOREGROUND_RED | FOREGROUND_GREEN,                                    //! 6
		DarkGray = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,                    //! 7
		Gray = FOREGROUND_INTENSITY,                                                       //! 8
		Glue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,                                     //! 9
		Green = FOREGROUND_INTENSITY | FOREGROUND_GREEN,                                   //! 10
		Cyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,                  //! 11
		Red = FOREGROUND_INTENSITY | FOREGROUND_RED,                                       //! 12
		Magenta = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,                 //! 13
		Yellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,                 //! 14
		White = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,//! 15
	};
};


class SinSole
{
public:

	static unsigned TextColor;
	static unsigned BackgroundColor;

	/*!Set the text and background color*/
	static void SetColor(const unsigned textcolor, const unsigned backgroundcolor);
	/*!set the text color of the console*/
	static void SetTextColor(const unsigned textcolor);
	/*!Set the background color of the console*/
	static void SetBackgroundColor(const unsigned backgroundcolor);

	//Inherited functions
	static void CreateConsole(String name);
	static void DestroyConsole();

	//Handle for console;
	static HANDLE Console;
	//For Writing;
	static FILE* File;
	
private:

};

std::ostream& operator<<(std::ostream& os, ConsoleColors::ConsoleColors color);

std::istream& operator>>(std::istream& is, ConsoleColors::ConsoleColors color);