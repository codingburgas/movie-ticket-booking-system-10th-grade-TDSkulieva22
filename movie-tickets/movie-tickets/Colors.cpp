#include "Colors.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Get the standard output handle

void setColor(ConsoleColor text, ConsoleColor background) {
	SetConsoleTextAttribute(hConsole, text + (background << 4));
}

void resetColor() {
	SetConsoleTextAttribute(hConsole, WHITE); //Reset the console text color to white
}
