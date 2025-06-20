#include "pch.h"
#include "DrawText.h"
#include "Menu.h"
#include "Colors.h"

void cinemaCity(){
    setColor(YELLOW, BLACK);
    printCentered("  ______  __  .__   __.  _______ .___  ___.      ___           ______  __  .___________.____    ____ ", 1);
    printCentered(" /      ||  | |  \\ |  | |   ____||   \\/   |     /   \\         /      ||  | |           |\\   \\  /   / ", 2);
    printCentered("|  ,----'|  | |   \\|  | |  |__   |  \\  /  |    /  ^  \\       |  ,----'|  | `---|  |----` \\   \\/   /  ", 3);
    printCentered("|  |     |  | |  . `  | |   __|  |  |\\/|  |   /  /_\\  \\      |  |     |  |     |  |       \\_    _/   ", 4);
    printCentered("|  `----.|  | |  |\\   | |  |____ |  |  |  |  /  _____  \\     |  `----.|  |     |  |         |  |     ", 5);
    printCentered(" \\______||__| |__| \\__| |_______||__|  |__| /__/     \\__\\     \\______||__|     |__|         |__|     ", 6);
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

void loginTitle() {
    setColor(LIGHT_BLUE);
    printCentered("  _     ___   ____ ___ _   _ ", 10);
    printCentered(" | |   / _ \\ / ___|_ _| \\ | |", 11);
    printCentered(" | |  | | | | |  _ | ||  \\| |", 12);
    printCentered(" | |__| |_| | |_| || || |\\  |", 13);
    printCentered(" |_____\\___/ \\____|___|_| \\_|", 14);
    resetColor();
}

void signUpTitle() {
    setColor(LIGHT_BLUE);
    printCentered("  ____ ___ ____ _   _   _   _ ____  ", 10);
    printCentered(" / ___|_ _/ ___| \\ | | | | | |  _ \\ ", 11);
    printCentered(" \\___ \\| | |  _|  \\| | | | | | |_) |", 12);
    printCentered("  ___) | | |_| | |\\  | | |_| |  __/ ", 13);
    printCentered(" |____/___\\____|_| \\_|  \\___/|_|    ", 14);
    resetColor();
}