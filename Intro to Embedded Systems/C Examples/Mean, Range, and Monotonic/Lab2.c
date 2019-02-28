// ****************** Lab2.c ***************

//***************Swathi Dochibhotla*********

// Date Created: 1/18/2017 
// Last Modified: 9/2/2017 
// Brief description of the Lab
// An embedded system is capturing temperature data from a
// sensor and performing analysis on the captured data.
// The controller part of the system is periodically capturing N
// readings of the temperature sensor. Your task is to write three
// analysis routines to help the controller perform its function
//   The three analysis subroutines are:
//    1. Calculate the mean of the temperature readings 
//       rounded down to the nearest integer
//    2. Calculate the range of the temperature readings, 
//       defined as the difference between the largest and smallest reading 
//    3. Check if the captured readings are a non-increasing montonic series
//       This simply means that the readings are sorted in non-increasing order.
//       We do not say "increasing" because it is possible for consecutive values
//       to be the same, hence the term "non-increasing". The controller performs 
//       some remedial operation and the desired effect of the operation is to 
//       lower the the temperature of the sensed system. This routine helps 
//       verify whether this has indeed happened
#include <stdint.h>
#define True 1
#define False 0
#define N 21         // Number of temperature readings
uint8_t Readings[N]; // Array of temperature readings on which to perform analysis  


// Return the computed Mean 
uint8_t Find_Mean(void){
	
	uint8_t i = 0;
	uint16_t mean = 0; 
	
	for(i = 0; i < N; i++){
		mean = mean + Readings[i];
	}
	
  mean = mean/N;
	
	return mean;
}

// Return the computed Range
uint8_t Find_Range(void){
	
	uint16_t range = 0;
	int low = Readings[0];
	int high = Readings[0];

	for(int k = 1; k < N; k++){
		
		if(Readings[k] < low){
			low = Readings[k];
		}
		if(Readings[k] > high){
			high = Readings[k];
		}
		range = high - low;
	}
	
  return range; 
}

// Return True of False based on whether the readings
// a non-increasing montonic series
uint8_t IsMonotonic(void){
  
	uint16_t mono  = 1;
	
	for (int p = 1; p < N; p++){
		if (Readings[p-1] < Readings[p]){
			mono = 0;
			return mono;
		}
  }
	return mono;
}



//Testcase 0:
// Scores[N] = {80,75,73,72,90,95,65,54,89,45,60,75,72,78,90,94,85,100,54,98,75};
// Range=55 Mean=77 IsMonotonic=False
//Testcase 1:
// Scores[N] = {100,98,95,94,90,90,89,85,80,78,75,75,75,73,72,72,65,60,54,54,45};  
// Range=55 Mean=77 IsMonotonic=True
//Testcase 2:
// Scores[N] = {80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80};
// Mean=80 Range=0 IsMonotonic=True
//Testcase 3:
// Scores[N] = {100,80,40,100,80,40,100,80,40,100,80,40,100,80,40,100,80,40,100,80,40};
// Mean=73 Range=60 IsMonotonic=False
//Testcase 4:
// Scores[N] = {100,95,90,85,80,75,70,65,60,55,50,45,40,35,30,25,20,15,10,5,0};
// Range=100 Mean=50 IsMonotonic=True

