/*
This Code has extra features 
including a XY positioning function on Display
and a Line Draw function on Nokia 3310 LCD 
It is modded from the original 
http://playground.arduino.cc/Code/PCD8544
*/
// Mods by Jim Park 
// jim(^dOt^)buzz(^aT^)gmail(^dOt^)com
// hope it works for you
#define PIN_SCE   6  // LCD CS  .... Pin 3
#define PIN_RESET 5  // LCD RST .... Pin 1
#define PIN_DC    7  // LCD Dat/Com. Pin 5
#define PIN_SDIN  8  // LCD SPIDat . Pin 6
#define PIN_SCLK  9  // LCD SPIClk . Pin 4
                     // LCD Gnd .... Pin 2
                     // LCD Vcc .... Pin 8
                     // LCD Vlcd ... Pin 7
                     
#define LCD_C     LOW
#define LCD_D     HIGH

#define LCD_X     84
#define LCD_Y     48
#define LCD_CMD   0


static const byte ASCII[][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j 
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ←
,{0x00, 0x06, 0x09, 0x09, 0x06} // 7f →
};


void LcdCharacter(char character)
{
  LcdWrite(LCD_D, 0x00);
  for (int index = 0; index < 5; index++)
  {
    LcdWrite(LCD_D, ASCII[character - 0x20][index]);
  }
  LcdWrite(LCD_D, 0x00);
}

void LcdClear(void)
{
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++)
  {
    LcdWrite(LCD_D, 0x00);
  }
}

void LcdInitialise(void)
{
  pinMode(PIN_SCE,   OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC,    OUTPUT);
  pinMode(PIN_SDIN,  OUTPUT);
  pinMode(PIN_SCLK,  OUTPUT);

  digitalWrite(PIN_RESET, LOW);
 // delay(1);
  digitalWrite(PIN_RESET, HIGH);

  LcdWrite( LCD_CMD, 0x21 );  // LCD Extended Commands.
  LcdWrite( LCD_CMD, 0xBf );  // Set LCD Vop (Contrast). //B1
  LcdWrite( LCD_CMD, 0x04 );  // Set Temp coefficent. //0x04
  LcdWrite( LCD_CMD, 0x14 );  // LCD bias mode 1:48. //0x13
  LcdWrite( LCD_CMD, 0x0C );  // LCD in normal mode. 0x0d for inverse
  LcdWrite(LCD_C, 0x20);
  LcdWrite(LCD_C, 0x0C);
}

void LcdString(char *characters)
{
  while (*characters)
  {
    LcdCharacter(*characters++);
  }
}

void LcdInt(int num)
{
  int remainder = 0;
  char str[2]; 
  sprintf(str, "%d", num);
  LcdString(str);
}

void LcdWrite(byte dc, byte data)
{
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}

// gotoXY routine to position cursor 
// x - range: 0 to 84
// y - range: 0 to 5
void gotoXY(int x, int y)
{
  LcdWrite( 0, 0x80 | x);  // Column.
  LcdWrite( 0, 0x40 | y);  // Row.  

}

//                                   ------------ MENU -----------

#define SET_MODE 0
#define NAV_MODE 1
int menuMode = SET_MODE;

#define MENU_VALUE 0
#define MENU_X 1
#define MENU_Y 2

char menu[][3] = {{95, 35, 2},
              {25, 35, 4}};
              
#define NO_OF_MENU_OPTIONS(x) (sizeof(x) / sizeof(x[0]))
#define TEMP_MENU 0
#define TIME_MENU 1
int selectedMenu = TEMP_MENU;
int previousSelectedMenu = TEMP_MENU;

void initialiseMenu(){
  gotoXY(0,0);
  LcdString ("C:      s:");
  gotoXY(0,1);
  LcdString ("------------");
  gotoXY(0,2);
  LcdString ("Temp:    C");
  gotoXY(0,4);
  LcdString ("Time:    sec");
  
  updateCurrentTemp("--.--");
  updateCurrentTime(menu[TIME_MENU][MENU_VALUE]);
  
  for (int i = 0; i<NO_OF_MENU_OPTIONS(menu); i++){
    updateMenu(menu[i]);
  }
}

// updates the temperature from the status bar
void updateCurrentTemp(char* currTemp){
  gotoXY(12,0);
  LcdString(currTemp);
}

// updates the temperature from the status bar
void updateCurrentTemp(){
  char currentTemp[6]; 
  float currTemp = getFreshTemp();
  int d1 = currTemp;            // Get the integer part (678).
  float f2 = currTemp - d1;     // Get fractional part (678.0123 - 678 = 0.0123).
  int d2 = trunc(f2 * 100);
  
  sprintf(currentTemp, "%d.%d", d1, d2);
  updateCurrentTemp(currentTemp);
}

// updates the time from the status bar
void updateCurrentTime(int currTime){
  gotoXY(70,0);
  LcdInt(currTime);
  Serial.print("Updated time: ");
  Serial.println(currTime);
}

//updates the menu values
void updateMenu(char option[]){
  gotoXY((int)option[MENU_X],(int)option[MENU_Y]);
  LcdInt(option[MENU_VALUE]);
  Serial.print("update: ");
  Serial.println(option[MENU_VALUE]);
}

//updates the selector cursor
void updateSelector(char selector, char option[]){
  gotoXY(((int)option[MENU_X])+15,(int)option[MENU_Y]);
  LcdCharacter(selector);
}

//change the number values from the menu in SET_MODE when press up or down
void changeOption(int newOption){
    //remove selection cursor from the previous menu
    updateSelector(' ', menu[previousSelectedMenu]);
    
    //make new option to be the selected option
    selectedMenu = newOption % NO_OF_MENU_OPTIONS(menu);
    
    //set the selection cursor to the current menu
    updateSelector('<', menu[selectedMenu]);
    previousSelectedMenu = selectedMenu;
}

int getTargetedTemp(){
  return menu[TEMP_MENU][MENU_VALUE];
}

int getExtractionTime(){
  return menu[TIME_MENU][MENU_VALUE];
}


//                             ------------ BUTTONS -----------


#define PIN_BTN 3

const int UP[] = {200, 530}; //511
const int DOWN[] = {1018, 1020};
const int ENTER[] = {1022, 1023};

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
int lastButtonPressed = 0;
int buttonValue = 0;

void up(){
  if (menuMode == SET_MODE){
    menu[selectedMenu][MENU_VALUE]++;
    updateMenu(menu[selectedMenu]);
  }
  else{
    changeOption(--selectedMenu);
  }
}

void down(){

  if (menuMode == SET_MODE){
    menu[selectedMenu][MENU_VALUE]--;
    updateMenu(menu[selectedMenu]);
  }
  else{
    changeOption(++selectedMenu);
  }
}

void enter(){
  if (menuMode == SET_MODE){
    if (selectedMenu == TIME_MENU)
      updateCurrentTime(menu[selectedMenu][MENU_VALUE]);
    
    menuMode = NAV_MODE;
    updateSelector('<', menu[selectedMenu]);
  }
  else{
    menuMode = SET_MODE;
    updateSelector('#', menu[selectedMenu]);
  }
}

void readButtons(){
  buttonValue = analogRead(PIN_BTN); 
  if (buttonValue >= 200){
    Serial.println(buttonValue);
    if ((millis() - lastDebounceTime) > debounceDelay){

      if (buttonValue >= ENTER[0] && buttonValue <= ENTER[1]){
        Serial.println("ENTER");
        enter();
      }
      if (buttonValue >= UP[0] && buttonValue <= UP[1]){
        Serial.println("UP");
        up();
      }
      if (buttonValue >= DOWN[0] && buttonValue <= DOWN[1]){
        Serial.println("DOWN");
        down();
      }
    }
    
    lastDebounceTime = millis();
    lastButtonPressed = buttonValue;
    /*else
      Serial.println(buttonValue);*/
  }
}
