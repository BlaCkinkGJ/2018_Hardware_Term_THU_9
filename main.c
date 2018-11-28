// flash load "D:\Hardware Experiment 3 2\THU09\Week10\flashclear.axf"
// flash load "D:\Hardware Experiment 3 2\THU09\Week10\Debug\Week10.axf"

#include "pipe.h"


int main(){
	_System_Init();
	while(true){
		int32_t dist = ultra_get_distance();
		// execution HERE
	}
}