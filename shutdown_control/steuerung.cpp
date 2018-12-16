#include "../rc-switch/RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
 
int main(int argc, char *argv[]) {
    int PIN = 0; 					// wiring Pi pin
    int codeSocketDon = 6124556;	// enter your own code
    int codeSocketDoff = 6124547;	// enter your own code
 
    if (wiringPiSetup() == -1) return 1;
 
    RCSwitch mySwitch = RCSwitch();
    mySwitch.enableTransmit(PIN);
 
    if (atoi(argv[1]) == 1) {
        mySwitch.send(codeSocketDon, 24);
    } else {
        mySwitch.send(codeSocketDoff, 24);
    }
    return 0;
}
