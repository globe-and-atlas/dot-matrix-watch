#pragma once
#include <stdbool.h>
#include "glyphs.h"
static inline bool uses_24(int mode,bool watch24){return mode==2 || (mode==0 && watch24);}
static inline int display_hour(int hour,int mode,bool watch24){return uses_24(mode,watch24)?hour:(hour%12?hour%12:12);}
static const int DIGIT_X[4]={9,50,113,154};
static inline void clock_bounds(int hour,int minute,int *left,int *right){
 int values[4]={hour/10,hour%10,minute/10,minute%10};*left=97;*right=101;
 for(int i=0;i<4;i++)for(int row=0;row<7;row++)for(int col=0;col<5;col++)if(NUMBERS[values[i]][row]&(1<<(4-col))){
  int x=DIGIT_X[i]+col*8;
  if(x-2<*left)*left=x-2;
  if(x+2>*right)*right=x+2;
 }
}
static inline int clock_offset(int hour,int minute){int left,right;clock_bounds(hour,minute,&left,&right);return (199-left-right)/2;}
static inline void text_bounds(const char *text,int pitch,int radius,int *left,int *right){
 *left=32767;*right=-1;
 for(int i=0;text[i];i++)for(int row=0;row<7;row++)for(int col=0;col<5;col++)if(glyph(text[i])[row]&(1<<(4-col))){
  int x=(i*6+col)*pitch;
  if(x-radius<*left)*left=x-radius;
  if(x+radius>*right)*right=x+radius;
 }
 if(*right<0){*left=0;*right=0;}
}
static inline int text_left(const char *text,int pitch,int radius){int left,right;text_bounds(text,pitch,radius,&left,&right);return (199-left-right)/2;}
static inline int battery_bars(int percent){return (percent+24)/25;}
static inline int battery_left(const char *percent){int left,right;text_bounds(percent,4,1,&left,&right);return (200-(68+right))/2;}
