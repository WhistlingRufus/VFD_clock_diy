#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>

#define DELAY_VALUE  3.5
#define sda_clock PB_11
#define scl_clock PB_10

#define __DATE__1 "Jan  2 2022"
#define __TIME__1 "22:51:52"


const uint8_t segms[] = {PB_3, PB_4, PB_5, PB_6, PB_7, PB_8, PB_9, PB_0, PB_1}; //segms
const uint8_t digs[] = {PA_0, PA_1, PA_2, PA_3, PA_4, PA_5, PA_6, PA_7};        //parts
const char digits_masks[] = {0b000111111, 0b000001010, 0b001110011,
                             0b001101011, 0b001001110, 0b001101101,
                             0b001111101, 0b000001011, 0b001111111, 0b001101111, 0b00000000,0b11111111}; //digits 0-9 + nothing+ seconds 
const int days_of_week[] = {1,2, 3, 4, 5, 6, 7};
TwoWire *i2c_clock = new TwoWire(sda_clock,scl_clock);
RTC_DS3231 rtc;
int day_ind = 0;
int hour = 0;
int minute =  0;


DateTime now ;
uint32_t time_second = millis();
bool opt = false;
void show_mask(char d, bool day_week = false)
{
  for (int i = 0; i < sizeof(segms) / sizeof(*segms); i++)
    if ((bitRead(d, i) == 1) || (i == 8 && day_week))
    {
      digitalWrite(segms[i], LOW);
    }
}
void clear_segs()
{
  for (int i = 0; i < sizeof(segms) / sizeof(*segms); i++)
  {
    digitalWrite(segms[i], HIGH);
  }
}
void clear_digs()
{
  for (int i = 0; i < sizeof(digs) / sizeof(*digs); i++)
  {
    digitalWrite(digs[i], HIGH);
  }
}
void activate_dig(int dig)
{
  for (int i = 0; i < sizeof(digs) / sizeof(*digs); i++)
  {
    if (i == dig)
      digitalWrite(digs[i], LOW);
      
          
    else
      digitalWrite(digs[i], HIGH);
      
        }
}
void draw_digit(int *time, int day)
{
  bool disp_day;
  for (int d = 0; d < sizeof(digs) / sizeof(*digs); d++)
  {
    disp_day = (d == days_of_week[day]);
    activate_dig(d);
    if (d >= 2 && d < 4)
      show_mask(digits_masks[time[d - 2]], disp_day);
    else if (d > 4 && d < 7)
      show_mask(digits_masks[time[d - 3]], disp_day);
    else if (d == 7)
      show_mask(digits_masks[2], disp_day);
    else if (d == 0)
      show_mask(digits_masks[1]); //выкл
    else if (d == 4){
      uint32_t delta = millis() - time_second;
      if (delta  > 1000){
        time_second =  millis();
        opt=!opt;
      }
      if (opt)
        show_mask(digits_masks[11], disp_day);
      else
        show_mask(digits_masks[10], disp_day);
      
      
      
    }
    else if (d == 1)
      show_mask(digits_masks[10], disp_day);
    delay(DELAY_VALUE);
    clear_segs();
  }
}
void setup()
{ 
   //Serial.begin(9600);
  for (int i = 0; i < sizeof(segms) / sizeof(*segms); i++)
  {
    pinMode(segms[i], OUTPUT);
    digitalWrite(segms[i], HIGH);
  }
  for (int i = 0; i < sizeof(digs) / sizeof(*digs); i++)
  {
    pinMode(digs[i], OUTPUT);
    digitalWrite(digs[i], HIGH);
  }
  clear_segs();
  clear_digs();
  if (!rtc.begin(i2c_clock)) 
    while (1) {
    activate_dig(0);
    digitalWrite(segms[5], LOW);
    
  }
  //Serial.print("Is Clock Running: ");
   
  rtc.adjust(DateTime(F(__DATE__1), F(__TIME__1)));

   
   
  
}

void loop()
{
  now = rtc.now();
  day_ind = now.dayOfTheWeek();
  hour = now.hour();
  minute = now.minute();
  
  int time[] = {hour/10,hour%10,minute/10,minute%10};    
  draw_digit(time,day_ind);
  
}
