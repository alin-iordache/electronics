/*
 I used the LCD graphic library from here http://www.henningkarlsen.com/electronics
*/

#include <LCD5110_Graph.h>

LCD5110 lcd(9,8,7,5,6);

extern uint8_t SmallFont[];
extern uint8_t TinyFont[];
extern uint8_t MediumNumbers[];

void initLCD(){
  lcd.InitLCD();
}

//------------------------------------------------ MENU -----------------------------------------------

#define SET_MODE 0
#define NAV_MODE 1
int menuMode = NAV_MODE;

#define MENU_VALUE 0
#define MENU_X 1
#define MENU_Y 2

char menu[][3] = {{95, 32, 10},
              {25, 32, 20}};
              
#define TEMP_MENU_TEXT "Temp:   C"
#define TIME_MENU_TEXT "Time:   Sec"

#define NO_OF_MENU_OPTIONS(x) (sizeof(x) / sizeof(x[0]))
#define TEMP_MENU 0
#define TIME_MENU 1
int selectedMenu = TIME_MENU;
int previousSelectedMenu = TEMP_MENU;

void initialiseMenu(){
  lcd.setFont(TinyFont);
  lcd.print ("C:      sec:", LEFT, 0);
  lcd.drawLine(0, 6, 83, 6);
  lcd.setFont(SmallFont);
  lcd.print (TEMP_MENU_TEXT, 2, menu[TEMP_MENU][MENU_Y]);
  lcd.print (TIME_MENU_TEXT, 2, menu[TIME_MENU][MENU_Y]);
  
  updateCurrentTemp("--.--");
  updateCurrentTime(menu[TIME_MENU][MENU_VALUE]);
  
  for (int i = 0; i<NO_OF_MENU_OPTIONS(menu); i++){
    updateMenu(menu[i]);
  }
  
  up();
  
  lcd.update();
}

// updates the temperature from the status bar with a given value
void updateCurrentTemp(String currTemp){
  //gotoXY(12,0);
  lcd.setFont(TinyFont);
  lcd.print(currTemp, 7, 0);
  lcd.setFont(SmallFont);
  lcd.update();
}

// updates the temperature from the status bar with the sensor value
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
//  gotoXY(70,0);
  lcd.setFont(TinyFont);
  lcd.printNumI(currTime, 48, 0);
  lcd.update();
  lcd.setFont(SmallFont);
  Serial.print("Updated time: ");
  Serial.println(currTime);
}

//updates the menu values
void updateMenu(char option[]){
  int rightX = ((int)option[MENU_X])+12;
  
  if (menuMode == SET_MODE){
    lcd.invertText(true);
    lcd.drawLine((int)option[MENU_X], (int)option[MENU_Y]-1, rightX, (int)option[MENU_Y]-1);
    lcd.drawLine(rightX, (int)option[MENU_Y]-1, rightX, (int)option[MENU_Y]+8);
  }
  else{
    lcd.invertText(false);
    lcd.clrLine((int)option[MENU_X], (int)option[MENU_Y]-1, rightX, (int)option[MENU_Y]-1);
    lcd.clrLine(rightX, (int)option[MENU_Y]-1, rightX, (int)option[MENU_Y]+8);
  }
    
  lcd.printNumI(option[MENU_VALUE], (int)option[MENU_X], (int)option[MENU_Y]);
  lcd.update();
  
  Serial.print("update: ");
  Serial.println(option[MENU_VALUE]);
}

//updates the selector cursor
void updateSelector(char option[], boolean invertSelection){
  if (not invertSelection)
    lcd.clrRect(0, ((int)option[MENU_Y])-2, 83, ((int)option[MENU_Y])+8);
  else
    lcd.drawRect(0, ((int)option[MENU_Y])-2, 83, ((int)option[MENU_Y])+8);

  if (menuMode == SET_MODE)
    lcd.invertText(true);
  else
    lcd.invertText(false);
    
  updateMenu(option);  
    
  lcd.update();
}

//change the number values from the menu in SET_MODE when press up or down
void changeOption(int newOption){
    //remove selection cursor from the previous menu
    updateSelector(menu[previousSelectedMenu], false);
    
    //make new option to be the selected option
    selectedMenu = newOption % NO_OF_MENU_OPTIONS(menu);
    
    //set the selection cursor to the current menu
    updateSelector(menu[selectedMenu], true);
    previousSelectedMenu = selectedMenu;
}

int getTargetedTemp(){
  return menu[TEMP_MENU][MENU_VALUE];
}

int getExtractionTime(){
  return menu[TIME_MENU][MENU_VALUE];
}






//            ------------------------------------------------ BUTTONS -----------------------------------------------


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
    updateSelector(menu[selectedMenu], true);
  }
  else{
    menuMode = SET_MODE;
    updateSelector(menu[selectedMenu], true);
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
