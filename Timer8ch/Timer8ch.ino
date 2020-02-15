#include <U8glib.h>
#include <Keypad.h>
#include <Wire.h>
#include<EEPROM.h>
#include <RTClib.h>

#define Buzzer   11
#define LOAD1    A0
#define LOAD2    A1
#define LOAD3    A2
#define LOAD4    A3
#define LOAD5    A4
#define LOAD6    A5
#define LOAD7    A6
#define LOAD8    A7

U8GLIB_ST7920_128X64_1X u8g(52, 51, 53, U8G_PIN_NONE);
int Select_Date = 0, stopTime = 0;
bool checkOut = false;
bool checkOut2 = false;
bool checktime = false;
bool out =false;
bool Times = true;
const byte rows = 4;
const byte columns = 4;

char hexaKeys[rows][columns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte row_pins[rows] = {9, 8, 7, 6};
byte column_pins[columns] = {5, 4, 3, 2};

Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

String passwd = "";
String newpasswd = "";
String newpasswd2 = "";
String Adinpasswd = "2535";
char ss[4];
char initial_password[4], new_password[4];
int i = 0;
char key_pressed = 0;
bool changepass = 0;

RTC_DS3231 RTC;
DateTime Time;
char Year, Mon, Day, Hour, Min, Sec;

unsigned char DTime[9] = {"12:20:30"};
char daysOfTheWeek[7][12] = {"Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday", "Monday"};
unsigned char DDate[9] = {"26/09/16"};
unsigned char TDate[9] = {"--/--/--"};
unsigned char DChannel[3] = {"0 "};
unsigned char ProgTemp[17] = {"--:--    --:--  "};
unsigned int  TimerOn[8][7][6], TimerOff[8][7][6];
unsigned char Prog[6][17];
unsigned char TimeTemp[9] = {"  :  :  "}  ;
unsigned char TimeTemp2[9] ={"--:--:--"};

unsigned char Ptr, Cursor = 0, Blink = 0, Toggle = 0, Cha = 0;

long time;

const unsigned char img[1024] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xE0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFC, 0x01, 0xC0, 0x00, 
0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0x00, 0x20, 0x00, 
0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 
0x00, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 
0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 
0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 
0x00, 0x00, 0x7F, 0xFF, 0xC0, 0x7F, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 0x01, 0xFF, 0xFE, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xFC, 0x00, 0x07, 0xFF, 0xE0, 0x07, 0xFF, 0xF0, 0x00, 0x3F, 0xFF, 0x00, 0x00, 
0x00, 0x00, 0xFF, 0xF0, 0x00, 0x01, 0xFF, 0xF0, 0x0F, 0xFF, 0x80, 0x00, 0x07, 0xFF, 0x80, 0x00, 
0x00, 0x01, 0xFF, 0xC0, 0x00, 0x00, 0x7F, 0xF8, 0x1F, 0xFF, 0x00, 0x00, 0x03, 0xFF, 0xC0, 0x00, 
0x00, 0x03, 0xFF, 0x80, 0x00, 0x00, 0x3F, 0xFC, 0x3F, 0xFC, 0x00, 0x00, 0x00, 0xFF, 0xC0, 0x00, 
0x00, 0x03, 0xFF, 0x00, 0x00, 0x00, 0x0F, 0xFE, 0x3F, 0xF8, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x00, 
0x00, 0x07, 0xFE, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x7F, 0xF0, 0x00, 0x00, 0x00, 0x3F, 0xE0, 0x00, 
0x00, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 

0x00, 0x0F, 0xFC, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xC0, 0x00, 0xF8, 0x00, 0x1F, 0xF0, 0x00, 
0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0x80, 0x00, 0xF8, 0x00, 0x1F, 0xF8, 0x00, 
0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xF8, 0x00, 0x0F, 0xF8, 0x00, 
0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0xF8, 0x00, 0x0F, 0xF8, 0x00, 
0x00, 0x0F, 0xF0, 0x07, 0xFF, 0xFE, 0x00, 0x3F, 0xFE, 0x00, 0x1F, 0xFF, 0x80, 0x0F, 0xF8, 0x00, 
0x00, 0x0F, 0xF0, 0x07, 0xFF, 0xFE, 0x00, 0x1F, 0xFC, 0x00, 0x1F, 0xFF, 0x80, 0x0F, 0xF8, 0x00, 
0x00, 0x0F, 0xF0, 0x07, 0xFF, 0xFE, 0x00, 0x1F, 0xF8, 0x00, 0x1F, 0xFF, 0x80, 0x07, 0xF8, 0x00, 
0x00, 0x0F, 0xF0, 0x07, 0xFF, 0xFE, 0x00, 0x3F, 0xFC, 0x00, 0x1F, 0xFF, 0x80, 0x07, 0xF8, 0x00, 
0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFE, 0x00, 0x00, 0xF8, 0x00, 0x0F, 0xF8, 0x00, 
0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0xF8, 0x00, 0x0F, 0xF8, 0x00, 
0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xF8, 0x00, 0x0F, 0xF8, 0x00, 
0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00, 0xF8, 0x00, 0x0F, 0xF8, 0x00, 
0x00, 0x0F, 0xFC, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x00, 
0x00, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x00, 
0x00, 0x07, 0xFE, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x7F, 0xF0, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 
0x00, 0x07, 0xFF, 0x00, 0x00, 0x00, 0x0F, 0xFE, 0x3F, 0xF8, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x00, 

0x00, 0x03, 0xFF, 0x80, 0x00, 0x00, 0x1F, 0xFC, 0x3F, 0xFC, 0x00, 0x00, 0x00, 0xFF, 0xE0, 0x00, 
0x00, 0x01, 0xFF, 0xC0, 0x00, 0x00, 0x3F, 0xF8, 0x1F, 0xFE, 0x00, 0x00, 0x01, 0xFF, 0xC0, 0x00, 
0x00, 0x01, 0xFF, 0xE0, 0x00, 0x00, 0xFF, 0xF0, 0x0F, 0xFF, 0x00, 0x00, 0x07, 0xFF, 0x80, 0x00, 
0x00, 0x00, 0xFF, 0xF8, 0x00, 0x03, 0xFF, 0xF0, 0x07, 0xFF, 0xC0, 0x00, 0x1F, 0xFF, 0x80, 0x00, 
0x00, 0x00, 0x7F, 0xFF, 0x00, 0x0F, 0xFF, 0xE0, 0x03, 0xFF, 0xF8, 0x00, 0xFF, 0xFF, 0x00, 0x00, 
0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 
0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 
0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 
0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xF0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x3E, 0x07, 0xFC, 0x0F, 0xF8, 0x1C, 0x0F, 0x1F, 0xFC, 0x78, 0x3C, 0x1F, 0xC0, 0x00, 
0x00, 0x00, 0x7E, 0x07, 0xFF, 0x0F, 0xFE, 0x1C, 0x0F, 0x1F, 0xFC, 0x7C, 0x3C, 0x7F, 0xE0, 0x00, 
0x00, 0x00, 0x7E, 0x07, 0xFF, 0x8F, 0xFF, 0x1C, 0x0F, 0x1F, 0xFC, 0x7C, 0x3C, 0x7F, 0xF0, 0x00, 
0x00, 0x00, 0x7F, 0x07, 0x87, 0x8E, 0x0F, 0x1C, 0x0F, 0x03, 0xC0, 0x7E, 0x3C, 0xF0, 0x78, 0x00, 
0x00, 0x00, 0xFF, 0x07, 0x87, 0x8E, 0x07, 0x9C, 0x0F, 0x03, 0xC0, 0x7E, 0x3C, 0xE0, 0x78, 0x00, 
0x00, 0x00, 0xE7, 0x07, 0x87, 0x8E, 0x07, 0x9C, 0x0F, 0x03, 0xC0, 0x7F, 0x3D, 0xE0, 0x78, 0x00, 
0x00, 0x00, 0xE7, 0x87, 0x9F, 0x0E, 0x07, 0x9C, 0x0F, 0x03, 0xC0, 0x77, 0x3D, 0xE0, 0x38, 0x00, 
0x00, 0x01, 0xE7, 0x87, 0xFE, 0x0E, 0x07, 0x9C, 0x0F, 0x03, 0xC0, 0x73, 0xBD, 0xE0, 0x38, 0x00, 
0x00, 0x01, 0xC3, 0x87, 0xFC, 0x0E, 0x07, 0x9C, 0x0F, 0x03, 0xC0, 0x73, 0xFD, 0xE0, 0x38, 0x00, 
0x00, 0x01, 0xC3, 0xC7, 0x9E, 0x0E, 0x07, 0x9C, 0x0F, 0x03, 0xC0, 0x71, 0xFD, 0xE0, 0x78, 0x00, 
0x00, 0x03, 0xFF, 0xC7, 0x8F, 0x0E, 0x07, 0x9C, 0x0F, 0x03, 0xC0, 0x71, 0xFD, 0xE0, 0x78, 0x00, 
0x00, 0x03, 0xFF, 0xE7, 0x8F, 0x0E, 0x0F, 0x1E, 0x0E, 0x03, 0xC0, 0x70, 0xFC, 0xF0, 0x78, 0x00, 
0x00, 0x07, 0x81, 0xE7, 0x87, 0x8E, 0x3E, 0x1E, 0x1E, 0x03, 0xC0, 0x70, 0xFC, 0xF9, 0xF0, 0x00, 
0x00, 0x07, 0x81, 0xE7, 0x87, 0x8F, 0xFC, 0x0F, 0xFC, 0x1F, 0xFC, 0x70, 0x7C, 0x7F, 0xE0, 0x00, 
0x00, 0x07, 0x00, 0xF7, 0x83, 0xCF, 0xF8, 0x07, 0xF8, 0x1F, 0xFC, 0x70, 0x7C, 0x3F, 0xC0, 0x00 

};


//---------------------------------------------------------------------------------------------
void setup(void)
{
  Serial.begin(9600);
  unsigned char i, j, k;
  unsigned char T;
  Wire.begin();
  pinMode(Buzzer, OUTPUT); digitalWrite(Buzzer, LOW);
  pinMode(LOAD1, OUTPUT); digitalWrite(LOAD1, LOW);
  pinMode(LOAD2, OUTPUT); digitalWrite(LOAD2, LOW);
  pinMode(LOAD3, OUTPUT); digitalWrite(LOAD3, LOW);
  pinMode(LOAD4, OUTPUT); digitalWrite(LOAD4, LOW);
  pinMode(LOAD5, OUTPUT); digitalWrite(LOAD5, LOW);
  pinMode(LOAD6, OUTPUT); digitalWrite(LOAD6, LOW);
  pinMode(LOAD7, OUTPUT); digitalWrite(LOAD7, LOW);
  pinMode(LOAD8, OUTPUT); digitalWrite(LOAD8, LOW);

  changepass = EEPROM.read(4);
  if (changepass == 1) {
    for (int j = 0; j < 4; j++)
      initial_password[j] = EEPROM.read(j);
  } else {
    initialpassword();
  }

  for (i = 0; i < 8; i++)
    for (j = 0; j < 7; j++)
      for (k = 0; k < 6; k++)
      { TimerOn[i][j][k] = 0;
        T = EEPROM.read((i * 168) + (j * 24) + (k * 4) + 5); TimerOn[i][j][k] = (TimerOn[i][j][k] << 8) + T;
        T = EEPROM.read((i * 168) + (j * 24) + (k * 4) + 6); TimerOn[i][j][k] = (TimerOn[i][j][k] << 8) + T;
        TimerOff[i][j][k] = 0;
        T = EEPROM.read((i * 168) + (j * 24) + (k * 4) + 7); TimerOff[i][j][k] = (TimerOff[i][j][k] << 8) + T;
        T = EEPROM.read((i * 168) + (j * 24) + (k * 4) + 8); TimerOff[i][j][k] = (TimerOff[i][j][k] << 8) + T;
      }
 noInterrupts();           // disable all interrupts
  Init_Timer(); //500 ms
  interrupts();             // enable all interrupts   
  beep();
  Prog[0][16] = 0;
  Prog[1][16] = 0;
  Prog[2][16] = 0;
  Prog[3][16] = 0;
  Prog[4][16] = 0;
  Prog[5][16] = 0;

  Show();


}
//---------------------------------------------------------------------------------------------
void loop(void)
{ char key = 0 ;

  while (key == NO_KEY) {


   
    Get_DateTime();
    Update_Time();
    CheckTime();
    

key = keypad_key.getKey();
    stopTime++;
    if (stopTime % 480 == 0) {
      int che = 0;
      while (!che) {
        che = Show();
        delay(200);
      }
    }
    
  }
  beep();
  stopTime = 0;
  if (key != NO_KEY) {
     Times =true;
    Showpassword1();
    delay(1000);
    Password();
    Clear();
    delay(500);
    SetTimeAll();
  }
}
//---------------------------------------------------------------------------------------------
void checkkey(){
   //key = keypad_key.getKey();
    stopTime++;
    if (stopTime % 80 == 0) {
      Show();
      while (keypad_key.getKey() == NO_KEY) {
        CheckTime();
        delay(200);
      }
    }
  
  
  }
//--------------------------------------หน้าพาสเวิด--------------------------------------------------------
void Password() {
  while (1) {
    key_pressed = keypad_key.getKey();
    if (key_pressed  == '#')
      change();

    if (key_pressed)
    {

      passwd += char(key_pressed);
      ss[i] = '*';
      i++;
      Showpassword2();
      beep();
    }

    if (i == 4)
    {
      delay(200);
      String y = "";
      for (int j = 0; j < 4; j++)
        y += char(EEPROM.read(j));

      if (passwd == y || passwd == Adinpasswd)
      {
        beep();
        for (int i = 0; i <= 3; i++) {

          ss[i] = 0;
        }
        delay(1000);
        i = 0;
        passwd = "";
        break;
      }

      else
      {
        digitalWrite(Buzzer, LOW);
        delay(300);
        digitalWrite(Buzzer, HIGH);
        Wrongpassword();
        delay(1300);
        for (int i = 0; i <= 3; i++) {

          ss[i] = 0;
        }

        passwd = "";

        Showpassword1();
        i = 0;

        digitalWrite(Buzzer, HIGH);
      }
    }

  }
}
//--------------------------------------เปลี่ยนพาสเวิด-------------------------------------------------------
void change()
{
  int j = 0;
  passwd = "";
  newpasswd = "";
  Currentpassword1();

  while (j < 4)
  {
    char key = keypad_key.getKey();
    if (key)
    {
      newpasswd += char(key);
      ss[j] = '*';
      j++;
      Currentpassword2();
      beep();
    }
    key = 0;
  }
  delay(300);
  String w = "";
  for (int j = 0; j < 4; j++)
    w += char(EEPROM.read(j));

  if (newpasswd != w && newpasswd != Adinpasswd)
  {
    digitalWrite(Buzzer, LOW);
    delay(300);
    digitalWrite(Buzzer, HIGH);
    Worngcurrentpassword();
    delay(500);
    newpasswd = "";
    for (int i = 0; i < 4; i++) {
      ss[i] = 0;
    }
    delay(500);

  }

  else {
    newpasswd = "";
    for (int i = 0; i < 4; i++) {
      ss[i] = 0;
    }
    beep();
    delay(500);
    j = 0;
    Enternewpassword1();


    while (j < 4)
    {
      char key = keypad_key.getKey();
      if (key)
      {
        newpasswd2 += char(key_pressed);
        ss[i] = '*';
        i++;
        Enternewpassword2();
        EEPROM.write(j, key);
        j ++;

        beep();
      }

    }
    changepass = 1;
    EEPROM.write(4, changepass);
    delay(500);
    Passwordchange();
    delay(500);
    newpasswd2 = "";
    for (int i = 0; i < 4; i++) {
      ss[i] = 0;
    }
    delay(1000);

  }

  Showpassword1();
  i = 0;

  key_pressed = 0;

}
//---------------------------------------------------------------------------------------------
void Clear() {
  u8g.firstPage();
  do {

  } while ( u8g.nextPage() );
}

//---------------------------Showbegin---------------------------------------------------------
//---------------------------------------------------------------------------------------------
int Show() {
  int i=0;
  u8g.firstPage();
  do {
    u8g.drawBitmapP( 0, 0, 16, 64, img);
  } while ( u8g.nextPage() );
  while(i<=30){
      i++;
      delay(100);
      CheckTime();
      if(keypad_key.getKey() != NO_KEY) return 1;
    }
    i=0;
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_lucasfont_alternate);
    u8g.drawStr( 0, 7, "---------------------------");
    u8g.drawStr( 0, 20, "    Timer8ch By new  ");
    u8g.drawStr( 0, 31, "--------------------------");
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr( 0, 39, "Tel: 092-4184558  ");
    u8g.drawStr( 0, 50, "FB : Patcharaphon T.");
    u8g.drawStr( 0, 61, "Line ID : newcy123");
  } while ( u8g.nextPage() );
   while(i<=50){
      i++;
      delay(100);
      CheckTime();
      if(keypad_key.getKey() != NO_KEY) return 1;
    }
    return 0;
}
//----------------------------ข้อความบน LCD Showpassword1---------------------------------------------------------
//---------------------------------------------------------------------------------------------
void Showpassword1() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(8, 24);
    u8g.print("Enter your password");
  } while ( u8g.nextPage() );
}
//---------------------------ข้อความ บน LCD Showpassword2-------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------------------------------------------

void Showpassword2() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(8, 24);
    u8g.print("Enter your password");
    u8g.setPrintPos(40, 40);
    u8g.print(ss);
  } while ( u8g.nextPage() );
}
//---------------------------ข้อความบนจอ LCD---------------------------------------------------------
//---------------------------------------------------------------------------------------------
void Wrongpassword() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(8, 24);
    u8g.print("Password is Wrong...");
    u8g.setPrintPos(12, 40);
    u8g.print("#.Change Password");
  } while ( u8g.nextPage() );

}
//---------------------------ข้อความบนจอ LCD---------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
void Currentpassword1() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(16, 24);
    u8g.print("Current Password");

  } while ( u8g.nextPage() );

}
//---------------------------ข้อความบนจอ LCD---------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
void Currentpassword2() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(16, 24);
    u8g.print("Current Password");
    u8g.setPrintPos(40, 40);
    u8g.print(ss);
  } while ( u8g.nextPage() );

}
//---------------------------ข้อความบนจอ LCD---------------------------------------------------------
//---------------------------------------------------------------------------------------------
void Worngcurrentpassword() {
  u8g.firstPage();
  do {

    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(14, 24);
    u8g.print("Wrong Password...");
    u8g.setPrintPos(28, 40);
    u8g.print("Try Again");
  } while ( u8g.nextPage() );



}
//---------------------------ข้อความบนจอ LCD---------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
void Enternewpassword1() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(8, 24);
    u8g.print("Enter New Password");
  } while ( u8g.nextPage() );

}
//---------------------------ข้อความบนจอ LCD---------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
void Enternewpassword2() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(8, 24);
    u8g.print("Enter New Password");
    u8g.setPrintPos(40, 40);
    u8g.print(ss);
  } while ( u8g.nextPage() );

}
//---------------------------ข้อความบนจอ LCD---------------------------------------------------------
//---------------------------------------------------------------------------------------------
void Passwordchange() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(8, 24);
    u8g.print("Password is Changed...");
  } while ( u8g.nextPage() );


}
//------------------------Buzzer------------------------------------------------------
//----------------------------------------------------------------------------------
void beep()
{
  digitalWrite(Buzzer, LOW);
  delay(20);
  digitalWrite(Buzzer, HIGH);
}
//--------------------EEPROM PASSWORD------------------------------------------------
//-----------------------------------------------------------------------------------
void initialpassword() {
  for (int j = 0; j < 4; j++)
    EEPROM.write(j, j + 49);
  for (int j = 0; j < 4; j++)
    initial_password[j] = EEPROM.read(j);
}

//--------------------´ดึงวัน เดือน ปี และเวลา-----------------------------------------
//--------------------------------------------------------------------------
void Get_DateTime()
{ Time     = RTC.now();
  DTime[0] = (Time.hour() / 10) + '0';
  DTime[1] = (Time.hour() % 10) + '0';
  DTime[3] = (Time.minute() / 10) + '0';
  DTime[4] = (Time.minute() % 10) + '0';
  DTime[6] = (Time.second() / 10) + '0';
  DTime[7] = (Time.second() % 10) + '0';

  DDate[0] = (Time.day() / 10) + '0';
  DDate[1] = (Time.day() % 10) + '0';
  DDate[3] = (Time.month() / 10) + '0';
  DDate[4] = (Time.month() % 10) + '0';
  DDate[6] = ((Time.year() % 100) / 10) + '0';
  DDate[7] = (Time.year() % 10) + '0';
}
//---------------------------------แสดงเวลา วันที่ วันของสัปดาห์---------------------
//---------------------------------------------------------------------------------
void  Update_Time()
{
  u8g.firstPage();
  do
  { u8g.setFont(u8g_font_helvB14);
    u8g.drawStr( 28, 30, DTime);
    u8g.setFont(u8g_font_6x13);
    u8g.drawStr( 75, 55, DDate);
    u8g.drawStr(5, 55, daysOfTheWeek[Time.dayOfTheWeek()]);
  } while (u8g.nextPage());
}
//---------------------------เลือกวัน+เลือกแชแนล+ตั้งเวลา-------------------------------------
//------------------------------------------------------------------------------------
void SetTimeAll() {
  char Key = 0, key1 = 0,Key1=0;
  unsigned char Ch, i, k;
  int u;

  while (Key == NO_KEY && Key !='A')
  { 
   
    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_6x12);
      u8g.drawStr( 0, 18,  "Chanel 1");
      u8g.drawStr( 70, 18, "Chanel 2");
      u8g.drawStr( 0, 29, "Chanel 3");
      u8g.drawStr( 70, 29, "Chanel 4");
      u8g.drawStr( 0, 40, "Chanel 5");
      u8g.drawStr( 70, 40, "Chanel 6");
      u8g.drawStr( 0, 51, "Chanel 7");
      u8g.drawStr( 70, 51, "Chanel 8");
    } while ( u8g.nextPage() );
  
    Key = keypad_key.getKey();
       
  }

  beep();
  while (key1 == NO_KEY)
  { 
    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_6x12);
      u8g.drawStr( 0, 7,  "1:Monday");
      u8g.drawStr( 0, 18, "2:Tuesday");
      u8g.drawStr( 0, 29, "3:Wednesday");
      u8g.drawStr( 0, 40, "4:Thursday");
      u8g.drawStr( 0, 51, "5:Friday");
      u8g.drawStr( 0, 62, "6:Saturday");
      u8g.drawStr( 70, 7, "7:Sunday");
    } while ( u8g.nextPage() );
    key1 = keypad_key.getKey();
  }
  beep();
  while (Key != NO_KEY)
  {
    while(Times){
    
    if (((Key >= '1') && (Key <= '8') && Key != 'A') && (key1 >= '1') && (key1 <= '7')){
      Ch = (Key - '0') - 1; // 0-7
      DChannel[0] = Key;
      switch (key1) {
        case '1': Select_Date = 6; break;
        case '2': Select_Date = 0; break;
        case '3': Select_Date = 1; break;
        case '4': Select_Date = 2; break;
        case '5': Select_Date = 3; break;
        case '6': Select_Date = 4; break;
        case '7': Select_Date = 5; break;
      }
     ShowProg(Ch, Select_Date); 
     delay(500);
      Times = false; 
    }
    else{
      out =true;
      break;
      }
 }
    if(out){ out =false ; break;}  

    if (Key == 'A')
    { for (i = 0; i < 6; i++) {
        GetTime(Ch, i, Select_Date);
        if (checkOut) {
          checkOut = false;
          checkOut2 = true;
          break;
        }
        TimerOn[Ch][Select_Date][i] = 0;
        TimerOn[Ch][Select_Date][i] = (TimerOn[Ch][Select_Date][i] << 8) + ((Prog[i][0] - '0') * 10) + (Prog[i][1] - '0');
        EEPROM.write((Ch * 168) + (Select_Date * 24) + (i * 4) + 5, TimerOn[Ch][Select_Date][i]);
        TimerOn[Ch][Select_Date][i] = (TimerOn[Ch][Select_Date][i] << 8) + ((Prog[i][3] - '0') * 10) + (Prog[i][4] - '0');
        Serial.println(Select_Date);
        EEPROM.write((Ch * 168) + (Select_Date * 24) + (i * 4) + 6, TimerOn[Ch][Select_Date][i] & 0xFF);

        TimerOff[Ch][Select_Date][i] = 0;
        TimerOff[Ch][Select_Date][i] = (TimerOff[Ch][Select_Date][i] << 8) + ((Prog[i][9] - '0') * 10) + (Prog[i][10] - '0');
        EEPROM.write((Ch * 168) + (Select_Date * 24) + (i * 4) + 7, TimerOff[Ch][Select_Date][i]);
        TimerOff[Ch][Select_Date][i] = (TimerOff[Ch][Select_Date][i] << 8) + ((Prog[i][12] - '0') * 10) + (Prog[i][13] - '0');
        EEPROM.write((Ch * 168) + (Select_Date * 24) + (i * 4) + 8, TimerOff[Ch][Select_Date][i] & 0xFF);
        Key = '#';
        
      }
     
     
      delay(300);
      beep();
    }

    if(checkOut2){
      checkOut2 = false;
      break;
      }


    Key = KeyPress(); beep();

    if(Key == 'D'){
      
      break;
      }
      
     

    

  }

}

//----------------ฟังก์ชัน keyPress--------
//----------------------------------------------------
unsigned char  KeyPress(void)
{ unsigned char Key;
  Key = NO_KEY;
  while (Key == NO_KEY)
  { Key = keypad_key.getKey();
  }
  return (Key);
}
//---------------แสดงเวลาแต่ละโปรแกรม------------------
//---------------------------------------------------------------------------------
void  Update_Display(unsigned char P)
{
  u8g.firstPage();
  do
  { u8g.setFont(u8g_font_6x12);
    if ((P == 10))
    {
      u8g.drawStr( 0, 8, "Channel =  ");
      u8g.drawStr( 64, 8, DChannel);
      u8g.drawStr( 0, 16, "    ON       OFF     ");
      u8g.drawStr( 0, 24, "1)  ");
      u8g.drawStr( 0, 32, "2)  ");
      u8g.drawStr( 0, 40, "3)  ");
      u8g.drawStr( 0, 48, "4)  ");
      u8g.drawStr( 0, 56, "5)  ");
      u8g.drawStr( 0, 64, "6)  ");
    }
    if ((P == 10) || (P == 0)) {
      u8g.drawStr( 16, 24, &Prog[0][0]);
    }
    if ((P == 10) || (P == 1)) {
      u8g.drawStr( 16, 32, &Prog[1][0]);
    }
    if ((P == 10) || (P == 2)) {
      u8g.drawStr( 16, 40, &Prog[2][0]);
    }
    if ((P == 10) || (P == 3)) {
      u8g.drawStr( 16, 48, &Prog[3][0]);
    }
    if ((P == 10) || (P == 4)) {
      u8g.drawStr( 16, 56, &Prog[4][0]);
    }
    if ((P == 10) || (P == 5)) {
      u8g.drawStr( 16, 64, &Prog[5][0]);
    }
   


  } while (u8g.nextPage());

}
/*-------------------------------------------------------------*
  ---------------  Check Timer  -------------------------------
  -------------------------------------------------------------*/
void CheckTime()
{ unsigned char i;
  Time     = RTC.now();
  long Timer;
  Timer = 0;
  Timer = (Timer << 8) + Time.hour();
  Timer = (Timer << 8) + Time.minute ();
  Serial.print(Timer);
  Serial.print(":");
  Serial.println(TimerOn[0][6][1]);
  for (i = 0; i < 6; i++) {
    if (Timer == TimerOn[0][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD1, HIGH); // on
    }
    if (Timer == TimerOff[0][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD1, LOW); //off
    }

    if (Timer == TimerOn[1][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD2, HIGH);
    }
    if (Timer == TimerOff[1][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD2, LOW);
    }

    if (Timer == TimerOn[2][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD3, HIGH);
    }
    if (Timer == TimerOff[2][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD3, LOW);
    }

    if (Timer == TimerOn[3][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD4, HIGH);
    }
    if (Timer == TimerOff[3][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD4, LOW);
    }

    if (Timer == TimerOn[4][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD5, HIGH);
    }
    if (Timer == TimerOff[4][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD5, LOW);
    }

    if (Timer == TimerOn[5][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD6, HIGH);
    }
    if (Timer == TimerOff[5][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD6, LOW);
    }

    if (Timer == TimerOn[6][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD7, HIGH);
    }
    if (Timer == TimerOff[6][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD7, LOW);
    }

    if (Timer == TimerOn[7][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD8, HIGH);
    }
    if (Timer == TimerOff[7][Time.dayOfTheWeek()][i]) {
      digitalWrite(LOAD8, LOW);
    }


  }


}

/*---------------------------------------------------------*
  ---------------  Get Time -------------------------------
  ----------------------------------------------------------*/
void GetTime(unsigned char P, unsigned char Ch , int d)
{ unsigned char i, Key, THour, TMin, TSec;
  int u;
  THour = TimerOn[P][d][Ch] >> 8;
  Serial.print(THour, DEC); Serial.print("  ");
  if (THour > 23) {
    Prog[Ch][0] = '-';
    Prog[Ch][1] = '-';
  }
  else {
    Prog[Ch][0] = (THour / 10) + '0';
    Prog[Ch][1] = (THour % 10) + '0';
  }
  Prog[Ch][2] = ':';

  TMin  = TimerOn[P][d][Ch] & 0xFF;
  Serial.print(THour, DEC);
  if (TMin > 59) {
    Prog[Ch][3] = '-';
    Prog[Ch][4] = '-';
  }
  else {
    Prog[Ch][3] = (TMin / 10) + '0';
    Prog[Ch][4] = (TMin % 10) + '0';
  }
  Prog[Ch][5] = ' '; Prog[Ch][6] = ' '; Prog[Ch][7] = ' '; Prog[Ch][8] = ' ';

  THour = TimerOff[P][d][Ch] >> 8;
  Serial.print(THour, DEC); Serial.print("  ");
  if (THour > 23) {
    Prog[Ch][9] = '-';
    Prog[Ch][10] = '-';
  }
  else {
    Prog[Ch][9] = (THour / 10) + '0';
    Prog[Ch][10] = (THour % 10) + '0';
  }
  Prog[Ch][11] = ':';

  TMin  = TimerOff[P][d][Ch] & 0xFF;
  Serial.print(THour, DEC);
  if (TMin > 59) {
    Prog[Ch][12] = '-';
    Prog[Ch][13] = '-';
  }
  else {
    Prog[Ch][12] = (TMin / 10) + '0';
    Prog[Ch][13] = (TMin % 10) + '0';
  }
  Prog[Ch][14] = ' '; Prog[Ch][15] = ' ';
  Serial.print("   \r\n");
  Update_Display(10);
  i = 0;
  Key = 0; Ptr = Ch; Blink = 1;
  while (Key != 'B' )
  { Cursor = i; Cha = Prog[Ch][i];
    Key = NO_KEY;
    TSec = 0;
    while (Key == NO_KEY) {
      Key = keypad_key.getKey();
      delay(10);
      TSec++;
      if (TSec >= 40) {
        TSec = 0;
        Update_Display(10);
      }


    }
    beep(); delay(300);

    if ((Key >= '0') && (Key <= '9') && (i < 14))
    { Blink = 0;
      Prog[Ch][i] = Key;
      if (i == 1) {
        i = i + 1;
      }
      if (i == 4) {
        i = i + 4;
      }
      if (i == 10) {
        i = i + 1;
      }
      Blink = 1;
      if (i == 13) {
        i = i++;
       Blink = 0;

      }
      
          
      
      i++;
      Cursor = i;
      Cha = Prog[Ch][i];

      
    }
  if(i < 15){
    if(Key == '#'){
      i++;
        if (i == 2) {
        i = 3;
      }
        
        if(i == 5){
          i = 9;
          }

           if (i == 11) {
        i = 12;
      }
      }

      if(Key == '*'){
         Blink = 0;
        i--;
        if(i == 2){
          i = 1;
          }

           if (i == 8) {
        i = 4;
      }
        }
      Blink = 1;
        if(i ==11){
          i = 10;
            Blink = 0;
          }
  }
 

    if (Key == 'D') {
       Blink = 0;
      checkOut = true;
      break;
    }

     if(Key != NO_KEY){
       Prog[Ptr][Cursor] = Cha;
      if(Key == 'C'){
        for (i = 0; i < 16; i++) {
        Prog[Ch][i] = ProgTemp[i];
      } i = 0;
        }
    }     


  }
  Blink = 0;
}
//--------------- logic àª¤àÇÅÒã¹¡ÒÃÃÑºà¢éÒÁÒ¨Ò¡ keypad ---------------------
//----------------------------------------------------------------
void ShowProg(unsigned char Ch, int dd)
{ unsigned char i, THour, TMin,Key1=0;
  for (i = 0; i < 6; i++) {
    THour = TimerOn[Ch][dd][i] >> 8;
    if (THour > 23) {
      Prog[i][0] = '-';
      Prog[i][1] = '-';
    }
    else {
      Serial.println("check");
      Prog[i][0] = (THour / 10) + '0';
      Prog[i][1] = (THour % 10) + '0';
    }
    Prog[i][2] = ':';
    TMin  = TimerOn[Ch][dd][i] & 0xFF;
    if (TMin > 59) {
      Prog[i][3] = '-';
      Prog[i][4] = '-';
    }
    else {
      Prog[i][3] = (TMin / 10) + '0';
      Prog[i][4] = (TMin % 10) + '0';
    }
    Prog[i][5] = ' '; Prog[i][6] = ' '; Prog[i][7] = ' '; Prog[i][8] = ' ';
    THour = TimerOff[Ch][dd][i] >> 8;
    if (THour > 23) {
      Prog[i][9] = '-';
      Prog[i][10] = '-';
    }
    else {
      Prog[i][9] = (THour / 10) + '0';
      Prog[i][10] = (THour % 10) + '0';
    }
    Prog[i][11] = ':';
    TMin  = TimerOff[Ch][dd][i] & 0xFF;
    if (TMin > 59) {
      Prog[i][12] = '-';
      Prog[i][13] = '-';
    }
    else {
      Prog[i][12] = (TMin / 10) + '0';
      Prog[i][13] = (TMin % 10) + '0';
    }
    Prog[i][14] = ' '; Prog[i][15] = ' ';
  }
 
  Update_Display(10);
}
void  Init_Timer(void)
{
TCCR1A = 0x00;
TCCR1B = 0x04;
TCNT1  = 34826;
TIMSK1 = 0x01;
}
/*--------------------------------------------*
 * Interrupt Service Routine Counter/Timer2   *
 *--------------------------------------------*/ 
ISR(TIMER1_OVF_vect)        
{ TCNT1 = 34826;
  char Key2 =keypad_key.getKey();
   
  if(Blink == 1)
  { if(Toggle == 0){
    Prog[Ptr][Cursor] = ' '; 
    Toggle = 1;
    }
    else if(Toggle == 1){
      Prog[Ptr][Cursor] = Cha; 
      Toggle = 0;
    
      }
      
  }

   
}
