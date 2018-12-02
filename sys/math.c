/*
 * Author: Rahul Sethi
 * Last Modified: 09/16/2018
 */
#include <stdio.h>
#define RAND_MAX 077777
/*
 * This function is an implementation of pow function.
 * This function complexity is O(log power).
 */
double pow(double base, int power) { 
	double res; 

    	if( power == 0) 
       	return 1; 

    	res = pow(base, power/2); 

    	if (power%2 == 0){ 
        	return res*res;
    	} 
    	else{ 
        	if(power > 0) 
            	return res*res*base; 
        	else
        	    return (res*res)/base; 
    	} 
} 

/*
 * This function is an implementation of log function.
 * This function uses taylor series to calculate the.
 * log(x) = 2*[y + (y^3)/3 + (y^5)/5 + ....] : y = (x-1)/(x+1). 
 */
double log(double x){
	double y = (x-1) / (x+1);
	int n = 1000; //No of iterations for approximation
    	double ele = y;
    	double res = 0.0;
    	double den = 1.0;
    	while(n--){
        	res += (ele/den);
        	ele *= (y*y);
        	den += 2.0;
    	}
	double retValue = res+res;
    	return retValue;
} 

int expdev(double lamb){
	double temp;
	do{
		temp = (double) rand() / RAND_MAX;
	} while(temp == 0.0);
	temp = temp;
	double retValue = -log(temp);
	retValue = retValue / lamb;
	return (int)retValue;
}
