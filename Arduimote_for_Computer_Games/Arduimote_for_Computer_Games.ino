#include <SoftEasyTransfer.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);
SoftEasyTransfer ET; 

//Easy Transfer Struct
struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int fi;
  int se;
  int th;
  int fo;
};
RECEIVE_DATA_STRUCTURE mydata;
int pos1 = 0;
int pos2 = 0;
        
int first = 0, second = 0, third = 0, forth = 0;
int i = 0;
unsigned long x = 0;
unsigned long y = 0;
int leftPow = 0;
int rightPow = 0;
int j = 0;
bool leftRev;
bool rightRev;
bool right;
bool left;
bool up;
bool down;
bool select;
bool f;
bool e;
bool toggle;
unsigned long x1 = 0;
unsigned long y1 = 0;
bool right1;
bool left1;
bool up1;
bool down1;
bool select1;
bool f1;
bool e1;


void setup(){
    //Initialize software serial and hardware serial
    mySerial.begin(115200);
    //start the library, pass in the data details and the name of the serial port.
    ET.begin(details(mydata), &mySerial);
    Serial.begin(115200);
}

void loop(){
     if (ET.receiveData()){
        //assign software serial struct objects to variables
        //message is 4 bits long
        //protocol = first[XXXX XXXX] second[XXYY YYYY] third[YYYY RLUD] forth[SFE0 0000]
        first = mydata.fi;
        second = mydata.se;
        third = mydata.th;
        forth = mydata.fo;
        //decode analog x, y and button values 
        x = first * 4 + second / 64;
        y = (second % 64) * 16 + (third / 16);
        up = !bitRead(third,3);
        down = !bitRead(third,2);
        right = !bitRead(third,1);
        left = !bitRead(third,0);
        select = !bitRead(forth,7);
        f = !bitRead(forth, 6);
        e = !bitRead(forth,5);
        // mouse
        if(up)
          Mouse.move(0,-35,0);
        if(left)
          Mouse.move(-35,0,0);
        if(right)
          Mouse.move(35,0,0);
        if(down)
          Mouse.move(0,35,0);
        // keyboard WASD
        if(y > 750)
          Keyboard.press('w');
        else if(y < 750 && y > 250)
          Keyboard.release('w');
        if(x < 250)
          Keyboard.press('a');
        else if(x > 250 && x < 750)
          Keyboard.release('a');
        if(y < 250)
          Keyboard.press('s');
        else if(y > 250 && y < 750)
          Keyboard.release('s');
        if(x > 750)
          Keyboard.press('d');
        else if(x > 250 && x < 750)  
          Keyboard.release('d');
        // mouse click
        if(e)
          Mouse.press(MOUSE_RIGHT);
        else if(!e)
          Mouse.release(MOUSE_RIGHT); 
        if(f)
          Keyboard.press(' ');
        else if(!f)
          Keyboard.release(' ');
//for serial monitor debugging
#if 1
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
            
  } 
  delay(2);

}

