#include <OLLO.h> 
/* Serial device defines for dxl bus */
#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 2  //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 3  //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
/* Control table defines */
#define MOVING_SPEED 32
#define TORQUE_LIMIT 34
#define GOAL_POSITION 30
/* IR Sensors */
OLLO myOLLO;
#define IR_FRONT 3
#define IR_LEFT 4
#define IR_RIGHT 1


int last_robot_direction = 0;
boolean start_button_pushed = false;
//boolean get_rid_of_time_based_delay_stuff_pirate_arrrr[8] = {false, false, ..};

Dynamixel Dxl(DXL_BUS_SERIAL3);

void setup() {
  pinMode(BOARD_BUTTON_PIN, INPUT_PULLDOWN);
  
  Dxl.begin(3); // Dynamixel 2.0 Baudrate -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps 
  
  for(int motor_id = 1; motor_id <= 8; motor_id++){
    if(motor_id % 2 != 0){ // outer motors (knee)
      Dxl.writeWord(motor_id, MOVING_SPEED, 95);  // Dynamixel ID 1 Speed Control (Address_data : 0~1024)
    }
    else{ // inner motors (hip)
      Dxl.writeWord(motor_id, MOVING_SPEED, 110);
    }
    Dxl.writeWord(motor_id, TORQUE_LIMIT, 1023);
    Dxl.jointMode(motor_id); // jointMode() is to use position mode
  }
  
  myOLLO.begin(IR_FRONT, IR_SENSOR);
  myOLLO.begin(IR_LEFT, IR_SENSOR);
  myOLLO.begin(IR_RIGHT, IR_SENSOR);
}

void moveMotor(int motor_id, int delta_degree){
  int neutral_position = 512;
  int direction_flag = -1;
  if(motor_id % 2 != 0){
    direction_flag = 1;
  }
  Dxl.writeWord(motor_id , GOAL_POSITION, neutral_position + (delta_degree * direction_flag));
}

void moveLeg(int leg, int walking_mode){
  // enum walking_mode {support_releaxed, support_deployed, crawl_forward, crawl_backwards;
  int hip = 0, knee = 0; // motor angles

  switch(walking_mode){
    case 3: // crawl_forward
      hip = 0, knee = 362;
      break;
    case 4: // crawl_backwards
      hip = 0, knee = 200;
      break;
    case 1: // support_releaxed
      hip = -75, knee = 380;
      break;
    default: // support_deployed
      hip = 32, knee = 283;
      break;
  }
  moveMotor((leg*2) + 1, hip);
  moveMotor((leg*2) + 2, knee);
}

void walking_routine(int routine_id, int robot_direction){
  int FRONT_LEG_ID = (robot_direction) % 4, BACK_LEG_ID = (robot_direction + 2) % 4, 
      RIGHT_LEG_ID = ( robot_direction + 1) % 4, LEFT_LEG_ID = (4 + robot_direction - 1) % 4;
    
  int support_releaxed = 1, support_deployed = 2, crawl_forward = 3, crawl_backwards = 4;
  
  switch(routine_id){
    case 1: // front
      moveLeg(FRONT_LEG_ID, crawl_backwards);
      moveLeg(BACK_LEG_ID, crawl_forward);
      moveLeg(LEFT_LEG_ID, support_releaxed);
      moveLeg(RIGHT_LEG_ID, support_releaxed);
      break;
    case 2: // up
      moveLeg(FRONT_LEG_ID, crawl_backwards);
      moveLeg(BACK_LEG_ID, crawl_forward);
      moveLeg(LEFT_LEG_ID, support_deployed);
      moveLeg(RIGHT_LEG_ID, support_deployed);
      break;
    case 3: // back
      moveLeg(FRONT_LEG_ID, crawl_forward);
      moveLeg(BACK_LEG_ID, crawl_backwards);
      moveLeg(LEFT_LEG_ID, support_deployed);
      moveLeg(RIGHT_LEG_ID, support_deployed);
      break;
    case 4: // down
      moveLeg(FRONT_LEG_ID, crawl_forward);
      moveLeg(BACK_LEG_ID, crawl_backwards);
      moveLeg(LEFT_LEG_ID, support_releaxed);
      moveLeg(RIGHT_LEG_ID, support_releaxed);
      break;
    default: // neutral
      moveLeg(FRONT_LEG_ID, support_deployed);
      moveLeg(BACK_LEG_ID, support_deployed);
      moveLeg(LEFT_LEG_ID, support_deployed);
      moveLeg(RIGHT_LEG_ID, support_deployed);
      break;
  }
}

void moveRobot(int robot_direction){
  int front = 1, up = 2, back = 3, down = 4, neutral = 0; // walking_routine_id
 
  int time = 760; // ms 
  // future consideration: delete time and ALL corresponding delays - logic based code refactoring 
  
  int button_state = digitalRead(BOARD_BUTTON_PIN); // read the state of the push button value
  if(button_state == HIGH){ // if button is pushed, means 3.3V(HIGH) is connected to BOARD_BUTTON_PIN
    start_button_pushed = true;
  }
  
  if(start_button_pushed && robot_direction == last_robot_direction){
    walking_routine(back, robot_direction);
    delay(time);
    walking_routine(down, robot_direction);
    delay(time/2);
    walking_routine(front, robot_direction);
    delay(time);
    walking_routine(up, robot_direction);
    delay(time/1.5);
  }
  else{ // switch to neutral after direction change
    walking_routine(neutral, robot_direction);
    delay(time);
  }
  last_robot_direction = robot_direction; // needs to be assigned as the last operation for the next main loop
}

void loop(){
  //Enum track_part {START, MIDDLE, FINISH};
  
  int robot_direction = last_robot_direction;
  int backward = 0, forward = 2, right = 1, left = 3; // robot_direction (equal to leg index)
  
  int front_IR_signal = myOLLO.read(IR_FRONT);
  int right_IR_signal = myOLLO.read(IR_RIGHT);
  int left_IR_signal = myOLLO.read(IR_LEFT);
  
  int lower_distance_threshold = 1000; // shorter range means higher value
  int higher_distance_threshold = 700;
  
  int right_signal_threshold = 105;
  int left_signal_threshold = 125;
  
  /* follow the track */
  if(front_IR_signal < higher_distance_threshold){
    robot_direction = forward;
  }
  
  if(front_IR_signal > lower_distance_threshold){
    robot_direction = right;
  }
  
  /* obstacle avoidance */
  if(left_IR_signal > left_signal_threshold){
    robot_direction = right;
  }
  
  if(right_IR_signal > right_signal_threshold){
    robot_direction = left;
  }
  
  
  moveRobot(robot_direction); // only call once in the main loop
  
}
