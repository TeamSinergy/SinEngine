#pragma once
#include "Precompiled.h"
#include "SinSole.h"

unsigned SinSole::BackgroundColor = DEFAULTBACKGROUND;
unsigned SinSole::TextColor = DEFAULTTEXT;
unsigned SinSole::LastBackgroundColor = DEFAULTBACKGROUND;
unsigned SinSole::LastTextColor = DEFAULTTEXT;
HANDLE SinSole::Console = nullptr;
FILE* SinSole::File = nullptr;

void SinSole::CreateConsole(String name)
{
    AllocConsole();
    
    freopen_s(&File, "CONOUT$", "wt", stdout);
    freopen_s(&File, "CONOUT$", "wt", stderr);
    freopen_s(&File, "CONIN$", "rt", stdin);

    SetConsoleTitle(name.c_str());

    /*! getting a handle to the console*/
    Console = GetStdHandle(STD_OUTPUT_HANDLE);
    //GetConsoleColors(_textcolor, _backgroundcolor);

    /*! Saving the original color of the console*/
    //_deftextcolor = _textcolor;
    //_defbackgroundcolor = _backgroundcolor;
    
    SinSole::SetColor(TextColor, BackgroundColor);
}

void SinSole::SetColor(const unsigned textcolor, const unsigned backgroundcolor)
{
    if (PROTECTCOLORS && (textcolor == backgroundcolor))
        return;

    TextColor = textcolor;
    BackgroundColor = backgroundcolor;

    /*! Change _backgroundcolor and _textcolor into wAttributes format*/
    unsigned short wAttributes = ((unsigned int)BackgroundColor << 4) | (unsigned int)TextColor;

    //! Set the text attibute so the the color has now been changed
    SetConsoleTextAttribute(Console, wAttributes);
}

/*!Set the color of the text in the console*/
void SinSole::SetTextColor(const unsigned textcolor)
{
    if (PROTECTCOLORS && (textcolor == BackgroundColor))
        return;
    LastTextColor = TextColor;
    TextColor = textcolor;

    /*! Change _backgroundcolor and _textcolor into wAttributes format*/
    unsigned short wAttributes = ((unsigned int)BackgroundColor << 4) | (unsigned int)TextColor;

    //! Set the text attibute so the the color has now been changed
    SetConsoleTextAttribute(Console, wAttributes);
}

/*!Set the background color of the console*/
void SinSole::SetBackgroundColor(const unsigned backgroundcolor)
{
    if (PROTECTCOLORS && (TextColor == backgroundcolor))
        return;
    LastBackgroundColor = BackgroundColor;
    BackgroundColor = backgroundcolor;

    /*! Change _backgroundcolor and _textcolor into wAttributes format*/
    unsigned short wAttributes = ((unsigned int)BackgroundColor << 4) | (unsigned int)TextColor;

    //! Set the text attibute so the the color has now been changed
    SetConsoleTextAttribute(Console, wAttributes);
}

void SinSole::DestroyConsole()
{
    FreeConsole();
}

std::ostream& operator<<(std::ostream& os, ConsoleColors::ConsoleColors color)
{
    SinSole::SetTextColor(color);
    return os;
}

std::istream& operator>>(std::istream& is, ConsoleColors::ConsoleColors color)
{
    SinSole::SetTextColor(color);
    return is;
}