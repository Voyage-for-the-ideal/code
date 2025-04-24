#include <stdio.h>
#include <math.h>

int main(){
double x[]={0.0,0.5,1.0,sqrt(2),10.0,100.0,300.0};
double result=0;
int n=__INT_MAX__;
int n1=10000000;

for(int j=0;j<7;j++){
    for(int k=1;k<n1;k++){
        result+=1/(k*(k+x[j]));
    }
    printf("x=%f,result=%.10f\n",x[j],result);
    result=0;
}
}