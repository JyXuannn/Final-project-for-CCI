#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library
#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED 0xF000
// 初始化LCD
LCDWIKI_KBV mylcd(ILI9486, A3, A2, A1, A0, A4); // model, cs, cd, wr, rd, reset
String preText = "000000";

#include <Servo.h>

Servo myservo;  // Creating Servo Objects to Control the SG-5010
int pos = 0;    // Servo Position Variables
int flag = 1;
int spd = 1;
int flag1 = 0;
int count = 0;
unsigned long lastTime = 0;  // Used to record the last time
unsigned long interval = 10000;  // time interval
unsigned long recordTime = millis();
int preFlag = 0;

void setup() {
  Serial.begin(9600); // Initialise serial communication
  myservo.attach(53);   // Connect servo to digital pin 53

  mylcd.Init_LCD(); // Initialise LCD
  //  mylcd.Fill_Screen(BLACK); // Clear screen
  int textSize = 12; //Text size
  //    String input = Serial.readStringUntil('\n'); // Read serial input up to line breaks
  mylcd.Fill_Screen(BLACK); // Clear screen
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Rotation(1); // 1 for 90 degree rotation

  mylcd.Set_Text_Size(textSize); // Text size
  //    mylcd.Print_String(input.c_str(), 10, 10); // Displays the text entered through the serial port
 
  int x = 10;
  int y = 100;
  String text = preText; // Text to be displayed
  for ( int i = 0; i < 6; i++) {
    char c = text.charAt(i);
    mylcd.Set_Draw_color(BLACK);
    mylcd.Fill_Rectangle(x + i * 80, 0,  x + i * 80 + 60, 360); // Adjust this area according to the font size


    mylcd.Set_Text_colour(WHITE);
    int num = c - '0';
    mylcd.Print_Number_Int(num, x + i * 80, y, 0, ' ', 10); // Display the number to the screen
    //    mylcd.Print_String(num, x + i * 80, y);
  }
}

//void loop() {
//
//  Serial.begin(9600);  
//  //  Serial.begin(9600);  
//  myservo.attach(9);   // Servo
//}
void loop()
{
   flag =  digitalRead(33); // Infrared sensor
   if(preFlag!=flag){
    lastTime = millis(); // Record the time when the change occurs
    }
  Serial.println(String(flag) + "," + flag1);

  
  if (flag == 0) {


    // Checks the difference between the current time and the last recorded time
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= interval) {
      // Set flag1 to 1 and randomise the next interval.
      flag1 = 1;
      lastTime = currentTime;
      recordTime = millis();
    }
    if (flag1 == 1) {
      myservo.write(pos);  
      pos += spd;
      if (pos == 45 || pos == 0) {
        delay(500);
        spd *= -1; // Turn in the opposite direction
      }

      if (pos == 0) {
        flag1 = 0;
        count += 1; // LCD+1
      }
    }


  } else {
    // When flag is not 0, reset timer and flag1
    flag1 = 0;
    count = 0;
  }
  updateNum(count);
  preFlag = flag;
}
void updateNum(int count) {
  // put your main code here, to run repeatedly:
  char buffer[7]; // Create an array of characters large enough to hold "000010" and a terminating character '\0'.
  // Use sprintf to format numbers as 6-digit strings with leading zeros
  sprintf(buffer, "%06d", count);
  String curCount = buffer;
//  Serial.println(curCount + "," + preText);
  if (curCount != preText) {
    int textSize = 12;
    //    String input = Serial.readStringUntil('\n'); // Read the serial input up to the newline character
    int x = 10; 
    int y = 100; 
    for ( int i = 0; i < 6; i++) {
      char c = curCount.charAt(i);
      char b = preText.charAt(i);
      if (b != c) {
        mylcd.Set_Draw_color(BLACK);
        mylcd.Fill_Rectangle(x + i * 80, 0,  x + i * 80 + 60, 360); 

        
        mylcd.Set_Text_colour(WHITE);
        int num = c - '0';
        mylcd.Print_Number_Int(num, x + i * 80, y, 0, ' ', 10);
      }
    }
    preText = curCount;
  }
}
