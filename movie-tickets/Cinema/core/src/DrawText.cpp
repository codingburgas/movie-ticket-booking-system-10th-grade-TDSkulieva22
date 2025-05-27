#include "pch.h"
#include "DrawText.h"
#include "Menu.h"
#include "Colors.h"

void cinemaCity(){
    setColor(YELLOW, BLACK);
    printCentered("  ______  __  .__   __.  _______ .___  ___.      ___           ______  __  .___________.____    ____ ", 2);
    printCentered(" /      ||  | |  \\ |  | |   ____||   \\/   |     /   \\         /      ||  | |           |\\   \\  /   / ", 3);
    printCentered("|  ,----'|  | |   \\|  | |  |__   |  \\  /  |    /  ^  \\       |  ,----'|  | `---|  |----` \\   \\/   /  ", 4);
    printCentered("|  |     |  | |  . `  | |   __|  |  |\\/|  |   /  /_\\  \\      |  |     |  |     |  |       \\_    _/   ", 5);
    printCentered("|  `----.|  | |  |\\   | |  |____ |  |  |  |  /  _____  \\     |  `----.|  |     |  |         |  |     ", 6);
    printCentered(" \\______||__| |__| \\__| |_______||__|  |__| /__/     \\__\\     \\______||__|     |__|         |__|     ", 7);
    resetColor();
}

void adminTitle() {
    setColor(LIGHT_BLUE);
    printCentered("     _        _             _         _       _                _               ", 2);
    printCentered("    / \\    __| | _ __ ___  (_) _ __  (_) ___ | |_  _ __  __ _ | |_  ___   _ __ ", 3);
    printCentered("   / _ \\  / _` || '_ ` _ \\ | || '_ \\ | |/ __|| __|| '__|/ _` || __|/ _ \\ | '__|", 4);
    printCentered("  / ___ \\| (_| || | | | | || || | | || |\\__ \\| |_ | |  | (_| || |_| (_) || |   ", 5);
    printCentered(" /_/   \\_\\\\__,_||_| |_| |_||_||_| |_||_||___/ \\__||_|   \\__,_| \\__|\\___/ |_|   ", 6);
    printCentered("                                                                               ", 7);
    resetColor();
}