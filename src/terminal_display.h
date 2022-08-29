#ifndef TERMINAL_DISPLAY_H
#define TERMINAL_DISPLAY_H

template <typename T>
void _dealWithTerminalScroll(T text, bool newline);

template <typename T>
void td(T text);
template <typename T>
void td(T text, bool displayTime);

template <typename T>
void tdln(T text);
template <typename T>
void tdln(T text, bool displayTime);

// https://stackoverflow.com/a/495056
#include "terminal_display.cpp"

#endif
