
#include <math.h>
#include <iostream>
#include <deque>
#include <set>
#include <vector>
#include <map>
#include <string> 
#include <fstream>
#include <ctime>
#include <iterator>
#include <algorithm>



using namespace std;


#include "tabvalue.h"


int main() {

	
	tabvalue C;
	C.edinsert(2, 14);
	C.edinsert(7, 24);
	C.edinsert(10, -1.2);
	C.edinsert(11, 12.8);
	
	cout<<"--------------------"<<endl;
	C.print_items(cout);
	
	C.erase(11);
	
	cout<<"--------------------"<<endl;
	C.print_items(cout);
	
	
	cout<<"valueof "<<C.valueof(10)<<endl;
	
	cout<<"best item "<<C.worst_item()<<endl;
	int best_item=C.worst_item();
	C.erase(best_item);
	cout<<"--------------------"<<endl;
	C.print_items(cout);

	return 0;
}







