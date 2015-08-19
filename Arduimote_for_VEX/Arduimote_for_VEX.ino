#include <SoftEasyTransfer.h>
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(9, 10);
SoftEasyTransfer ET; 
Servo servo1, servo2; 

// Easy transfer struct
struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int fi;
  int se;
  int th;
  int fo;
};

RECEIVE_DATA_STRUCTURE mydata;

int pwm_a = 3;   //PWM control for motor outputs 1 and 2 is on digital pin 3
int pwm_b = 11;  //PWM control for motor outputs 3 and 4 is on digital pin 11
int dir_a = 12;  //direction control for motor outputs 1 and 2 is on digital pin 12
int dir_b = 13;  //direction control for motor outputs 3 and 4 is on digital pin 13

int pos1 = 0;
int pos2 = 0;
        
int first = 0, second = 0, third = 0, forth = 0;
int i = 0;
unsigned long x = 0;
unsigned long y = 0;
int leftPow = 0;
int rightPow = 0;

bool leftRev;
bool rightRev;
bool right;
bool left;
bool up;
bool down;
bool select;
bool f;
bool e;



void setup(){
    pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
    pinMode(pwm_b, OUTPUT);
    pinMode(dir_a, OUTPUT);
    pinMode(dir_b, OUTPUT);

    
    analogWrite(pwm_a, 0);
    analogWrite(pwm_b, 0);
    
    mySerial.begin(115200);
    //start the library, pass in the data details and the name of the serial port.
    ET.begin(details(mydata), &mySerial);
    Serial.begin(115200);
  
    servo1.attach(5);
    servo2.attach(6);
    

}

void loop(){

    if (ET.receiveData()){
      
          // assign software serial struct objects to variables
          // message is 4 bits long
          // protocol = first[XXXX XXXX] second[XXYY YYYY] third[YYYY RLUD] forth[SFE0 0000]
          
          first = mydata.fi;
          second = mydata.se;
          third = mydata.th;
          forth = mydata.fo;
          
          //decode analog x, y and button values 
          x = first * 4 + second / 64;
          y = (second % 64) * 16 + (third / 16);
          right = !bitRead(third,3);
          left = !bitRead(third,2);
          up = !bitRead(third,1);
          down = !bitRead(third,0);
          select = !bitRead(forth,7);
          f = !bitRead(forth, 6);
          e = !bitRead(forth,5);

    
      #if 0 // change 0 to 1 for serial debugging
      
          Serial.print(" x: ");
          Serial.print(x);
          Serial.print(" y: ");
          Serial.print(y);
          Serial.print(" right: ");
          Serial.print(right);
          Serial.print(" left: ");
          Serial.print(left);
          Serial.print(" up: ");
          Serial.print(up);
          Serial.print(" down: ");
          Serial.print(down);
          Serial.print(" select: ");
          Serial.print(select);
          Serial.print(" f: ");
          Serial.print(f);
          Serial.print(" e: ");
          Serial.println(e);
          
      #endif
  
            //-**********************************************************
            if (x < 508 && y >= 515){ //upper left quadrant
              leftPow = (y - 515) - (508 - x);
              rightPow = (y - 515) + (508 - x);
            }else if (x <= 508 && y <= 515){ //lower left quadrant
              leftPow = -((515 - y) + (508 - x));
              rightPow = -((515 - y) - (508 - x));
            }else if (x >= 508 && y > 515){ //upper right quadrant
              leftPow = (y - 515) + (x - 508);
              rightPow = (y - 515) - (x - 508);
            }else if (x > 508 && y <= 515){ //lower right quadrant
              leftPow = (-(515 - y) + (x - 508));
              rightPow = (-(515 - y) - (x - 508));
            }
            leftPow /= 3;
            rightPow /= 3;
            digitalWrite(dir_a, HIGH);
            digitalWrite(dir_b, HIGH);
            rightRev = 0;
            leftRev = 0;
            int lim = 170;
            if (leftPow < 0){
              digitalWrite(dir_a, LOW);
              leftPow = abs(leftPow);
              leftRev = 1;
            }
            if (rightPow < 0){
              digitalWrite(dir_b, LOW);
              rightPow = abs(rightPow);
              rightRev = 1;
            }
            if (leftPow > 170)
              leftPow = 170;
            if (rightPow > 170)
              rightPow = 170;
    
            analogWrite(pwm_a, leftPow);
            analogWrite(pwm_b, rightPow);
            
            if (up){
              servo1.write(60);
            }else if (down){
              servo1.write(130);
            }else{
              servo1.write(92);
            }
            
            if(right){
              servo2.write(60);
            }else if (left){
              servo2.write(130);
            }else{
              servo2.write(92);
            }
      
    } 
    delay(15);

}
