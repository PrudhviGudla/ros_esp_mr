
/*

*/



#include <ros.h>
#include <geometry_msgs/Twist.h>

ros::NodeHandle  nh;


const uint8_t R_BACK = 2;
const uint8_t R_FORW = 15;


const uint8_t L_BACK = 4;
const uint8_t L_FORW = 0;

const uint8_t channel_LF = 0;
const uint8_t channel_RF = 1;
const uint8_t channel_LB = 2;
const uint8_t channel_RB = 3;

float left_wheel;
float right_wheel;


void cmdVel_to_pwm_cb( const geometry_msgs::Twist& velocity_msg){

    right_wheel = (velocity_msg.linear.x + velocity_msg.angular.z ) / 2 ;
    left_wheel = (velocity_msg.linear.x - velocity_msg.angular.z ) /2 ;

    speed();
    if ( velocity_msg.linear.x ==0.0 & velocity_msg.angular.z ==0.0){
        stop();
    }
    Serial.print(left_wheel);Serial.print(" / ");Serial.println(right_wheel);

}


ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &cmdVel_to_pwm_cb );

void setup(){
nh.initNode();
nh.subscribe(sub);
Serial.begin(57600);
pin_def();
stop();
// Serial.println("Get Ready");
delay(2000);
}

void pin_def(){

  const int freq = 5000;
  const int res = 8;
  
  
  pinMode(L_FORW, OUTPUT);
  pinMode(L_BACK, OUTPUT);
  
  pinMode(R_FORW, OUTPUT);
  pinMode(R_BACK, OUTPUT);
  
  ledcSetup(channel_RF ,freq , res);
  ledcSetup(channel_LF ,freq , res);
  ledcSetup(channel_RB ,freq , res);
  ledcSetup(channel_LB ,freq , res);

  ledcAttachPin(R_BACK,channel_RB);
  ledcAttachPin(L_BACK,channel_LB);
  ledcAttachPin(R_FORW,channel_RF);
  ledcAttachPin(L_FORW,channel_LF);

}


void loop(){

  nh.spinOnce();

}


void speed (){

    if(left_wheel>0){
        ledcWrite(channel_LF, 200);
    }

    else if(left_wheel<0){
        ledcWrite(channel_LB, 200); 
    }

    if(right_wheel>0){
        ledcWrite(channel_RF, 200);
    }

    else if(right_wheel<0){
        ledcWrite(channel_RB, 200); 
    }
     
    
}

void stop()
{

   ledcWrite(channel_RF, 0);  
   ledcWrite(channel_LF, 0);
   ledcWrite(channel_RB, 0);  
   ledcWrite(channel_LB, 0);
}