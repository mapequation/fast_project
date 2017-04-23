

class tabvalue {


public:
            
    tabvalue(){};
    ~tabvalue(){};
    
    bool is_internal(int );
    void edinsert(int , double );
    bool erase(int);
    double valueof(int );
    int size() {return items_value.size();};
    void print_items(ostream &);
    void print_items_sorted_by_value(ostream & outb);
    int best_item();
    int worst_item();
    double worst_value();
    double sum_values();



private:

    // items_value[item] : iterator to pair value-item
    map <int, muspi::iterator> items_value;
    muspi number_label;

};


bool tabvalue::is_internal(int a) {

	map<int, muspi::iterator >::iterator itm= items_value.find(a);
	if(itm==items_value.end()) {
		return false;
	} else {
		return true;
    }
}

void tabvalue::edinsert(int a, double kp) {		// this function inserts element a (or edit it if it was already inserted)
	
	erase(a);
		
	muspi::iterator itms= number_label.insert(make_pair(kp, a));
	items_value.insert(make_pair(a, itms));
	
}


bool tabvalue::erase(int a) {
    
    // this function erases element a if exists (and returns true)
	
	map<int, muspi::iterator >::iterator itm= items_value.find(a);
	if(itm!=items_value.end()) {
		
		number_label.erase(itm->second);
		items_value.erase(itm);
		return true;
		
	}
		
	
	return false;
	

}


double tabvalue::valueof(int a) {
    // return the value of a, 0 if it's not internal
	map<int, muspi::iterator >::iterator itm= items_value.find(a);
	if(itm!=items_value.end()) {
		return itm->second->first;
	} else {
		return 0;
    }


}



double tabvalue::sum_values() {
    
    double sum = 0.0;
    for (auto & x : number_label) {
        sum += x.first;
    }
    return sum;
	
}



void tabvalue::print_items_sorted_by_value(ostream & outb) {
    
    for (auto & x : number_label) {
        outb<<x.second<<" "<<x.first<<endl;
    }
	
}


void tabvalue::print_items(ostream & outb) {
	
	for(map<int, muspi::iterator >::iterator itm= items_value.begin(); itm!=items_value.end(); itm++) {
		outb<<itm->first<<"\t"<<itm->second->first<<endl;
    }
}

int tabvalue::best_item() {

	
	muspi::iterator itm= number_label.end();
	if(number_label.size()==0) {
		return -1;
	}
	
	itm--;
	return itm->second;
	


}

int tabvalue::worst_item() {

	
	muspi::iterator itm= number_label.begin();
	if(number_label.size()==0) {
		return -1;
	}
    
	return itm->second;
	
}


double tabvalue::worst_value() {

	
	muspi::iterator itm= number_label.begin();
	if(number_label.size()==0) {
		return -1;
	}
    
	return itm->first;
	
}






