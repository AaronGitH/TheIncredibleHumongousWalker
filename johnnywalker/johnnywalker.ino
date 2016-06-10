/* Serial device defines for dxl bus */
#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 2  //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 3  //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
/* Control table defines */
#define MOVING_SPEED 32
#define TORQUE_LIMIT 34
#define GOAL_POSITION 30

Dynamixel Dxl(DXL_BUS_SERIAL3);


void setup() {
  // Dynamixel 2.0 Baudrate -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps 
  Dxl.begin(3);
  
  for(int motor_id = 1; motor_id <= 8; motor_id++){
    Dxl.writeWord(motor_id, MOVING_SPEED, 80);  //Dynamixel ID 1 Speed Control (Address_data : 0~1024)
    Dxl.writeWord(motor_id, TORQUE_LIMIT, 1023);
    Dxl.jointMode(motor_id); //jointMode() is to use position mode
  }
}

void moveMotor(int motor_id, int delta_degree){
    int neutral_position = 512;
    int direction_flag = -1;
    if(motor_id % 2 != 0){
      direction_flag = 1;
      if(motor_id == 7){
        direction_flag = -1;
      }
    }
    Dxl.writeWord(motor_id , GOAL_POSITION, neutral_position + (delta_degree * direction_flag));
}

void moveLeg(int leg, int walking_mode){
  // enum walking_mode {support_releaxed, support_deployed, crawl_forward, crawl_backwards;
  int hip = 0, knee = 0; // motor angles

  switch(walking_mode){
    case 3: // crawl_forward
      hip = 0, knee = 410;
      break;
    case 4: // crawl_backwards
      hip = 0, knee = 200;
      break;
    case 1: // support_releaxed
      hip = -130, knee = 420;
      break;
    default: // support_deployed
      hip = 20, knee = 270;
      break;
  }
  moveMotor((leg*2) + 1, hip);
  moveMotor((leg*2) + 2, knee);
}

void walking_routine(int routine_id){
  int FRONT_LEG_ID = 0, BACK_LEG_ID = 2, LEFT_LEG_ID = 3, RIGHT_LEG_ID = 1;
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
    default: // down
      moveLeg(FRONT_LEG_ID, crawl_forward);
      moveLeg(BACK_LEG_ID, crawl_backwards);
      moveLeg(LEFT_LEG_ID, support_releaxed);
      moveLeg(RIGHT_LEG_ID, support_releaxed);
      break;
  }
}

void moveRobot(int robot_direction){
    int front = 1, up = 2, back = 3, down = 4; // walking_routine_id
    
    int s = 1000; // <-- TODO delete this! and ALL delays
    
    switch(robot_direction){
    case 1: // left
      // TODO: do all cases!!!!!!!!!!!!!!!!!!!!!
      break;
    case 2: // right
      break;
    case 3: // backward
      break;
    default: // forward
      walking_routine(front);
      delay(s);
      walking_routine(up);
      delay(s/2);
      walking_routine(back);
      delay(s);
      walking_routine(down);
      delay(s/2);
      break;
  }
}

void loop() {  
  //Enum track_part {START, MIDDLE, FINISH};
  
  int forward = 4, backward = 3, right =2, left = 1; // robot_direction  
  moveRobot(forward);
  
  //delay(1000);              
}
