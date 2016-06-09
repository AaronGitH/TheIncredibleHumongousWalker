
#include <leg.cpp> 

class Robot{
	private:
	// directions
	int LEFT = 0;
	int RIGHT = 0;
	int FORWARD = 0;
	int BACKWARD = 0;
	int UP = 0;
	int DOWN = 0;
	
	// robot parts
	int FRONT_LEG_ID = 0, BACK_LEG_ID = 1, LEFT_LEG_ID = 2, RIGHT_LEG_ID = 3;
	Legs[4] legs;
	
	public:
	Robot(int[] motor_ports){
		for(int leg_id = 0; i < legs.size; i++){
			
			legs[leg_id] = new Leg(motor_ports[leg_id * 2], motor_ports[ (leg_id * 2) + 1])
		} 
	}
	
	WaitObj moveTo(int direction){
		switch(){
			case 1:
		}
	}
	
	int[] readDistance(){
		int[2] dist;
		return dist;
	}
}