/**************************************
Recommend to use Servos on Pins 20,21,22 and 23
Valid Servos based on test are 0,1,3,6,7,8,10,14,15,20,21,22,23
Servo scale value between 0 and 180
**/

#include <Servo.h> 


class Leg{
	private:
		Servo hip, knee; 
	
	public:
	Leg(int hip_port, int knee_port) { 
		hip.attach(hip_port);
		knee.attach(knee_port);
	} 
	
	int getHipAngle(){
		return hip.read(); // is that a method ? I hope so
	}
	
	int getKneeAngle(){
		return knee.read();
	}
	
	WaitObj lift(int hip_angle, int knee_angle){
		hip.wirte(hip_angle);
		knee.write(knee_angle);
	}

}