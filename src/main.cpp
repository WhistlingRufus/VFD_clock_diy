#include <Arduino.h>

int delay_value = 250 * 2;
const uint8_t segms[] = {PB_3, PB_4, PB_5, PB_6, PB_7, PB_8, PB_9, PB_0, PB_1}; //segms
const uint8_t digs[] = {PA_0, PA_1, PA_2, PA_3, PA_4, PA_5, PA_6, PA_7};        //parts
const char digits_masks[] = {0b000111111, 0b000001010, 0b001110011,
                             0b001101011, 0b001001110, 0b001101101,
                             0b001111101, 0b000001011, 0b001111111, 0b001101111, 0b00000000}; //digits 0-9 + nothing
const int days_of_week[] = {2, 3, 4, 5, 6, 7, 1};

void show_mask(char d, bool day_week = false)
{
  for (int i = 0; i < sizeof(segms) / sizeof(*segms); i++)
    if ((day_week && i == 8) || (bitRead(d, i) == 1))
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
  for (int d = 0; d < sizeof(digs) / sizeof(*digs); d++)
  {
    bool disp_day = (d == days_of_week[day]);
    activate_dig(d);
    if (d >= 2 && d < 4)
      show_mask(digits_masks[time[d - 2]], disp_day);
    else if (d > 4 && d < 7)
      show_mask(digits_masks[time[d - 3]], disp_day);
    else if (d == 7)
      show_mask(digits_masks[2], disp_day);
    else if (d == 0)
      show_mask(digits_masks[1]); //выкл
    else if (d == 4)
      show_mask(digits_masks[10], disp_day);
    else if (d == 1)
      show_mask(digits_masks[10], disp_day);
    clear_segs();
  }
}
void setup()
{
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
}

void loop()
{
  int time[] = {0, 1, 2, 3};
  int day_ind = 0;
  while (true)
  {

    draw_digit(time, day_ind);
  }
}
