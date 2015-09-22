#include <stdio.h>
#include <math.h>
int main(){
	float f=1.3124;
	float d= 1.4332;
	printf("double:%lf,float:%f\n",d,f);
	printf("scientific notation:%e\n%E\n",d,f);
	printf("1/0.0=%f\n",1/0.0);
	printf("-1/0.0=%f\n",-1/0.0);
	printf("0.0/0.0=%f\n",0.0/0.0);
	//float only precise to 7 decimal places 	
	if(fabs(f+d-2.7456)<1e-7)
		printf("equal\n");
	return 0;
}
