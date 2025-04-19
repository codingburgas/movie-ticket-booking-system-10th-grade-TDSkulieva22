#include "DrawText.h"
#include "Menu.h"
#include "Color.h"

void cinemaCity()
{
    setColor(YELLOW, BLACK);
    printCentered("  ______  __  .__   __.  _______ .___  ___.      ___           ______  __  .___________.____    ____ ", 2);
    printCentered(" /      ||  | |  \\ |  | |   ____||   \\/   |     /   \\         /      ||  | |           |\\   \\  /   / ", 3);
    printCentered("|  ,----'|  | |   \\|  | |  |__   |  \\  /  |    /  ^  \\       |  ,----'|  | `---|  |----` \\   \\/   /  ", 4);
    printCentered("|  |     |  | |  . `  | |   __|  |  |\\/|  |   /  /_\\  \\      |  |     |  |     |  |       \\_    _/   ", 5);
    printCentered("|  `----.|  | |  |\\   | |  |____ |  |  |  |  /  _____  \\     |  `----.|  |     |  |         |  |     ", 6);
    printCentered(" \\______||__| |__| \\__| |_______||__|  |__| /__/     \\__\\     \\______||__|     |__|         |__|     ", 7);
    resetColor();
}