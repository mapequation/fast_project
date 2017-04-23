#if !defined(PRINT_INCLUDED)
#define PRINT_INCLUDED	
	


void cherr() {
	
	cerr<<"the check failed"<<endl;
	int e;
	cin>>e;
	
}


void cherr(double a) {
	
	cerr<<"the check failed because of "<<a<<endl;
	int e;
	cin>>e;
	
}


void cherr(double a, double ee) {
	
	if(fabs(a)>ee) {
	
		cerr<<"the check failed because of "<<a<<endl;
		int e;
		cin>>e;
	}
	
	
}

// print pairs
template <typename one, typename two>
void prints(const pair <one, two> & sq,  ostream & out = cout) {
	out<<sq.first<<" "<<sq.second<<endl;
}

// print maps
template <typename one, typename two>
void prints(const map<one, two> & sq,  ostream & out = cout) {
    for(const auto & p : sq) {
        prints(p, out);
    }
}

// print multimap
template <typename one, typename two>
void prints(const multimap<one, two> & sq,  ostream & out = cout) {
    for(const auto & p : sq) {
        prints(p, out);
    }
}

// print arrays
template <typename type_>
void prints(type_ *a, int b, ostream & out = cout) {
	for (int i=0; i<b; i++)
		out<<a[i]<<" ";
	out<<endl;
}

// print double deques
template <typename one, typename two>
void prints(const deque<one> & a, const deque<two> &b, ostream &out = cout) {
	for(UI i=0; i<a.size(); i++)
		out<<a[i]<<" "<<b[i]<<endl;
}

// print deque
template <typename type>
void prints(const deque<type> & sq,  ostream & out = cout) {
	for(const auto & v : sq)
		out<<v<<" ";
	out<<endl;
}


// print vector
template <typename type>
void prints(const vector<type> & sq,  ostream & out = cout) {
	for(const auto & v : sq)
		out<<v<<" ";
	out<<endl;
}


// print set
template <typename type>
void prints(const set<type> & sq,  ostream & out = cout) {
	for(const auto & v : sq)
		out<<v<<" ";
	out<<endl;
}


// print matrix deque<seq>
template <typename type>
void printm(const deque<type> & M, ostream & pout = cout) {
	for(UI i=0; i<M.size(); i++)
		prints(M[i], pout);
} 


// print matrix vector<seq>
template <typename type>
void printm(const vector<type> & M, ostream & pout = cout) {
	for(UI i=0; i<M.size(); i++)
		prints(M[i], pout);
} 

template <typename one, typename two>
void printDeqPair(const deque<pair<one, two> > & a, ostream &out = cout) {
	for(UI i=0; i<a.size(); i++)
		out<<a[i].first<<" "<<a[i].second<<endl;
}






template <typename type>
void get_data_from_file(string s, deque<type> & a1, int col) {

	
	// default will be col=1
	
	
	char b[s.size()+1];
	cast_string_to_char(s, b);
	ifstream lin(b);
	
	
	a1.clear();
	col--;
	
	string sas;
	while(getline(lin, sas)) {
		
		deque<double> s;
		cast_string_to_doubles(sas, s);
		if(int(s.size())>col) {
			a1.push_back(s[col]);
		}
	
	}
	
	

}



template <typename type>
void get_data_from_file(string s, deque<type> & a1) {

	get_data_from_file(s, a1, 1);	

}



template <typename type>
void get_data_from_file(string s, deque<type> & a1, deque<type> & a2, int col1, int col2) {

	
	// default will be col1=1, col2=2
	
	
	char b[s.size()+1];
	cast_string_to_char(s, b);
	ifstream lin(b);
	
	
	a1.clear();
	a2.clear();
	col1--;
	col2--;
	
	string sas;
	while(getline(lin, sas)) {
		
		deque<double> s;
		cast_string_to_doubles(sas, s);
		if(s.size()>col2) {
			
			a1.push_back(s[col1]);
			a2.push_back(s[col2]);
		
		}
	
	}
	
	
	
}



template <typename type>
void get_data_from_file(string s, deque<type> & a1, deque<type> & a2) {

	
	get_data_from_file(s, a1, a2, 1, 2);
	

}



void get_data_from_file_string(string s, deque<string> & a1, int col) {

	
	// default will be col=1
	
	
	char b[s.size()+1];
	cast_string_to_char(s, b);
	ifstream lin(b);
	
	
	a1.clear();
	col--;
	
	string sas;
	while(getline(lin, sas)) {
		
		deque<string> v;
		separate_strings(sas,  v);
		 
		 //prints(v);
		 
		if(int(s.size())>col) {
			a1.push_back(v[col]);
		}
	
	}
	
	

}


bool check_if_file_exists(string netfile) {			/* check if file_name exists */
	
	char b[netfile.size()+1];
	cast_string_to_char(netfile, b);
	ifstream inb(b);
	if(inb.is_open()==false) {
		
		cout<<"File "<<netfile<<" not found"<<endl;
		return false;
		
	}	
	
	return true;
	
}



#endif
