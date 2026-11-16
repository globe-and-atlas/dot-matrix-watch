#include <pebble.h>
#include "config.h"
#include "policy.h"
enum{P_PALETTE=1,P_BG,P_FG,P_ACCENT,P_TIME,P_DATE};
typedef struct{int palette,bg,fg,accent,time,date;}Settings;
static Settings s={0,0,0x55FF55,0x55FF55,0,1};
static const uint32_t PALETTES[6][3]={{0,0x55FF55,0x55FF55},{0,0xFFAA00,0xFFAA00},{0x000055,0xAAFFFF,0x55AAFF},{0x550055,0xFFAAFF,0xFF55AA},{0,0xFFFFFF,0xFFFFFF},{0xFFFFFF,0,0xAA0000}};
static Window *window;
static Layer *layer;
static struct tm current;
static int battery;
#if DOT_FIXTURE
static int fh=DOT_HOUR,fm=DOT_MINUTE,fb=DOT_BATTERY;
#endif
static const char *DAYS[]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
static const char *MONTHS[]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
static uint32_t color(int i){if(s.palette<6)return PALETTES[s.palette][i];return i==0?(uint32_t)s.bg:i==1?(uint32_t)s.fg:(uint32_t)s.accent;}
static void dots(GContext *ctx,const uint8_t *rows,int x,int y,int pitch_x,int pitch_y,int radius){
 for(int row=0;row<7;row++)for(int col=0;col<5;col++)if(rows[row]&(1<<(4-col)))
  {
   if(radius)graphics_fill_circle(ctx,GPoint(x+col*pitch_x,y+row*pitch_y),radius);
   else graphics_fill_rect(ctx,GRect(x+col*pitch_x,y+row*pitch_y,1,1),0,GCornerNone);
  }
}
static void text(GContext *ctx,const char *value,int x,int y,int pitch,int radius){
 for(int i=0;value[i];i++)dots(ctx,glyph(value[i]),x+i*6*pitch,y,pitch,pitch,radius);
}
static void centered_text(GContext *ctx,const char *value,int y){text(ctx,value,text_left(value,2,0),y,2,0);}
static void draw_battery(GContext *ctx){
 char percent[8];snprintf(percent,sizeof(percent),"%d%%",battery);int left=battery_left(percent);
 for(int x=0;x<=48;x+=4){graphics_fill_circle(ctx,GPoint(left+x,198),1);graphics_fill_circle(ctx,GPoint(left+x,220),1);}
 for(int y=202;y<220;y+=4){graphics_fill_circle(ctx,GPoint(left,y),1);graphics_fill_circle(ctx,GPoint(left+48,y),1);}
 for(int y=206;y<=214;y+=4)graphics_fill_circle(ctx,GPoint(left+52,y),1);
 for(int bar=0;bar<battery_bars(battery);bar++)for(int col=0;col<2;col++)for(int row=0;row<3;row++)
  graphics_fill_circle(ctx,GPoint(left+7+bar*10+col*4,205+row*4),1);
 text(ctx,percent,left+68,197,4,1);
}
#if defined(PBL_ROUND)
#include "round_digital.h"
static void round_numeral(GContext *ctx,int value,int x,int y,int w,int h){for(int row=0;row<7;row++)for(int col=0;col<5;col++)if(NUMBERS[value][row]&(1<<(4-col))){rdisk(ctx,x+col*w/5+w/10,y+row*h/7+h/14,1,color(1));}}
static void round_current(GContext *ctx){uint32_t bg=color(0),fg=color(1),accent=color(2);RoundData d={0};d.now=current;d.bg=bg;d.fg=fg;d.accent=accent;d.battery=battery;d.time_mode=s.time;d.date_mode=s.date;int hour=rhour(&d);uint8_t first[7],last[7];for(int row=0;row<7;row++){first[row]=NUMBERS[hour/10][row]<<3;last[row]=NUMBERS[d.now.tm_min%10][row]<<3;}d.clock_shift=rpair_shift(first,last,5,7,1,18,132,30,42,90);round_digital(ctx,&d,6,round_numeral);}
#endif
static void draw(Layer *own,GContext *ctx){
#if defined(PBL_ROUND)
 (void)own;round_current(ctx);rproof_marker(ctx);return;
#endif

 graphics_context_set_antialiased(ctx,false);
 graphics_context_set_fill_color(ctx,GColorFromHEX(color(0)));graphics_fill_rect(ctx,layer_get_bounds(own),0,GCornerNone);
 graphics_context_set_fill_color(ctx,GColorFromHEX(color(1)));
 int hour=display_hour(current.tm_hour,s.time,clock_is_24h_style());
 int values[4]={hour/10,hour%10,current.tm_min/10,current.tm_min%10};int offset=clock_offset(hour,current.tm_min);
 for(int i=0;i<4;i++)dots(ctx,NUMBERS[values[i]],DIGIT_X[i]+offset,52,8,10,2);
 graphics_fill_circle(ctx,GPoint(99+offset,72),2);graphics_fill_circle(ctx,GPoint(99+offset,92),2);
 if(!uses_24(s.time,clock_is_24h_style()))centered_text(ctx,current.tm_hour>=12?"PM":"AM",128);
 char date[32]="";
 switch(s.date){
 case 1:snprintf(date,sizeof(date),"%s %s %02d",DAYS[current.tm_wday],MONTHS[current.tm_mon],current.tm_mday);break;
 case 2:snprintf(date,sizeof(date),"%04d-%02d-%02d",current.tm_year+1900,current.tm_mon+1,current.tm_mday);break;
 case 3:snprintf(date,sizeof(date),"%02d/%02d/%04d",current.tm_mon+1,current.tm_mday,current.tm_year+1900);break;
 case 4:snprintf(date,sizeof(date),"%02d/%02d/%04d",current.tm_mday,current.tm_mon+1,current.tm_year+1900);break;
 }
 centered_text(ctx,date,151);
 graphics_context_set_fill_color(ctx,GColorFromHEX(color(2)));
 for(int x=12;x<=186;x+=6)graphics_fill_circle(ctx,GPoint(x,180),1);
 draw_battery(ctx);
}
static void tick(struct tm *now,TimeUnits changed){
 (void)changed;current=*now;
#if DOT_FIXTURE
 current.tm_hour=fh;current.tm_min=fm;current.tm_year=126;current.tm_mon=10;current.tm_mday=5;current.tm_wday=4;
#endif
 layer_mark_dirty(layer);
}
static void battery_changed(BatteryChargeState state){battery=state.charge_percent;
#if DOT_FIXTURE
 battery=fb;
#endif
 layer_mark_dirty(layer);
}
static void set(DictionaryIterator *iter,uint32_t key,int persist,int maximum,int *target){
 Tuple *t=dict_find(iter,key);
 if(t && (t->type==TUPLE_INT || t->type==TUPLE_UINT)){int v=t->value->int32;if(v>=0 && v<=maximum){*target=v;persist_write_int(persist,v);}}
}
#if DOT_FIXTURE
static void fixture(DictionaryIterator *iter,int key,int maximum,int *target){
 Tuple *t=dict_find(iter,key);if(t && t->type==TUPLE_INT && t->value->int32>=0 && t->value->int32<=maximum)*target=t->value->int32;
}
#endif
static void inbox(DictionaryIterator *iter,void *context){
#if defined(PBL_ROUND) && (DOT_FIXTURE)
 Tuple *round_proof=dict_find(iter,11022);if(round_proof && round_proof->type==TUPLE_INT){r_proof=round_proof->value->int32==1;layer_mark_dirty(layer);}
#endif

 (void)context;
 set(iter,MESSAGE_KEY_Palette,P_PALETTE,6,&s.palette);set(iter,MESSAGE_KEY_Background,P_BG,0xFFFFFF,&s.bg);
 set(iter,MESSAGE_KEY_Foreground,P_FG,0xFFFFFF,&s.fg);set(iter,MESSAGE_KEY_Accent,P_ACCENT,0xFFFFFF,&s.accent);
 set(iter,MESSAGE_KEY_TimeFormat,P_TIME,2,&s.time);set(iter,MESSAGE_KEY_DateFormat,P_DATE,4,&s.date);
#if DOT_FIXTURE
 fixture(iter,11000,23,&fh);fixture(iter,11001,59,&fm);fixture(iter,11004,100,&fb);battery=fb;
#endif
 time_t now=time(NULL);tick(localtime(&now),MINUTE_UNIT);
}
static void restore(int key,int maximum,int *target){if(persist_exists(key)){int v=persist_read_int(key);if(v>=0 && v<=maximum)*target=v;}}
static void init(void){
 restore(P_PALETTE,6,&s.palette);restore(P_BG,0xFFFFFF,&s.bg);restore(P_FG,0xFFFFFF,&s.fg);restore(P_ACCENT,0xFFFFFF,&s.accent);restore(P_TIME,2,&s.time);restore(P_DATE,4,&s.date);
 window=window_create();layer=layer_create(PBL_IF_ROUND_ELSE(GRect(0,0,PBL_DISPLAY_WIDTH,PBL_DISPLAY_HEIGHT),GRect(0,0,200,228)));layer_set_update_proc(layer,draw);layer_add_child(window_get_root_layer(window),layer);window_stack_push(window,true);
 time_t now=time(NULL);tick(localtime(&now),MINUTE_UNIT);battery_changed(battery_state_service_peek());
 tick_timer_service_subscribe(MINUTE_UNIT,tick);battery_state_service_subscribe(battery_changed);
 app_message_register_inbox_received(inbox);app_message_open(256,64);
}
static void deinit(void){tick_timer_service_unsubscribe();battery_state_service_unsubscribe();app_message_deregister_callbacks();layer_destroy(layer);window_destroy(window);}
int main(void){init();app_event_loop();deinit();}
