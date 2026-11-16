from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]


def test_clock_and_text_geometry():
    folder = ROOT / '.tmp'
    folder.mkdir(exist_ok=True)
    source, binary = folder / 'policy.c', folder / 'policy'
    source.write_text('''#include <assert.h>
#include <stdio.h>
#include "policy.h"
int main(void){
 for(int h=0;h<24;h++){
  assert(display_hour(h,2,false)==h);
  assert(display_hour(h,1,true)==(h%12?h%12:12));
  assert(display_hour(h,0,true)==h);
  assert(display_hour(h,0,false)==(h%12?h%12:12));
  for(int m=0;m<60;m++){
   int l,r;clock_bounds(h,m,&l,&r);int offset=clock_offset(h,m);
   assert(l+offset>=0);assert(r+offset<200);
   assert(198<=l+r+2*offset && l+r+2*offset<=200);
  }
 }
 for(int b=0;b<=100;b++){
  char pct[8];snprintf(pct,sizeof(pct),"%d%%",b);int l,r;text_bounds(pct,4,1,&l,&r);
  int x=battery_left(pct);assert(x-1>=0);assert(x+68+r<200);
  int sum=2*x-1+68+r;assert(sum>=198 && sum<=200);
  assert(battery_bars(b)==(b+24)/25);
 }
 const char *dates[]={"THU NOV 05","2026-11-05","11/05/2026","05/11/2026","WED SEP 30"};
 for(int i=0;i<5;i++){
  int l,r;text_bounds(dates[i],2,0,&l,&r);int x=text_left(dates[i],2,0);
  assert(l+x>=0 && r+x<200);assert(l+r+2*x>=198 && l+r+2*x<=200);
 }
 for(int a=0;a<10;a++)for(int b=a+1;b<10;b++){
  int diff=0;for(int row=0;row<7;row++)diff|=NUMBERS[a][row]^NUMBERS[b][row];assert(diff);
 }
 return 0;}
''')
    subprocess.run(['cc', '-Wall', '-Wextra', '-Werror', '-I', str(ROOT / 'watchface/src/c'),
                    str(source), '-o', str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
