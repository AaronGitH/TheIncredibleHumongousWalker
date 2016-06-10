
#include <stdio.h>

int main( int argc, const char* argv[] ){
	int motor_ports[8] = {1, 2, 3, 4, 5, 6, 7, 8}
	
	Robot robot = new Robot(motor_ports);
	
	AI ai = new AI(robot);
	
	ai.winTheRace();
}