


class parameters {
	
public:
	
	parameters(){};
	~parameters(){};
	
	/* if required==true, the program will not run if the parameter is not set by the user */
	void set_double(string flag, double default_p, bool required, string statement);	
	void set_int(string flag, int default_p, bool required, string statement);	
	void set_string(string flag, string default_p, bool required, string statement);	
	void set_bool(string flag, bool default_p, bool required, string statement);	
	void printing(ostream & pout);
	void set_from_argv(int argc, char * argv[]);
	
	/* maps the flag to the parameters */
	map<string, double> double_ps;	
	map<string, int> int_ps;
	map<string, string> string_ps;
	map<string, bool> bool_ps;
	/* maps the flag to the parameters */

	
private:
	
	void set_the_rest(string flag, bool required, string statement);
	void required_flags_are_set(char * argv[]);
	void error_statement(char * argv[]);
	
	deque<pair<string, string> > flag_statement;
	set<string> required_flags;
	
};


void parameters::set_the_rest(string flag, bool required, string statement) {
	
	flag_statement.push_back(make_pair(flag,statement));
	if(required)
		required_flags.insert(flag);
	
	

}

void parameters::set_double(string flag, double default_p, bool required, string statement) {

	double_ps[flag] = default_p;
	set_the_rest(flag, required, statement);
}


void parameters::set_int(string flag, int default_p, bool required, string statement) {
	
	int_ps[flag] = default_p;
	set_the_rest(flag, required, statement);
}



void parameters::set_string(string flag, string default_p, bool required, string statement) {
	
	string_ps[flag] = default_p;
	set_the_rest(flag, required, statement);
}



void parameters::set_bool(string flag, bool default_p, bool required, string statement) {
	
	bool_ps[flag] = default_p;
	set_the_rest(flag, required, statement);
}



void parameters::error_statement(char * argv[]) {

	// here there could be a front statement
    cerr<<"Code for fast projecting a bipartite network"<<endl;
    cerr<<"Output:"<<endl;
    cerr<<"1. bip_file_aggregated.txt is the bipartite network without features which appear only once"<<endl;
    cerr<<"2. projected.txt is the unipartite network"<<endl;
    cerr<<endl<<endl;
    
	for(deque<pair<string, string> >::iterator itm=flag_statement.begin(); itm!=flag_statement.end(); itm++) {
		cerr<<itm->first<<" "<<itm->second<<endl;
	}
    
    cerr<<endl<<"Basic example: "<<endl;
    cerr<<argv[0]<<" -f [network_file]"<<endl;
    cerr<<"Please look at ReadMe.txt for more info"<<endl;
}

void parameters::set_from_argv(int argc, char * argv[]) {
	
	int i=1;
	
	if(i==argc) {
        error_statement(argv);
        exit(-1);
    }
	while(i<argc) {
		
		string temp(argv[i]);
		
		
		if(required_flags.find(temp)!=required_flags.end())
			required_flags.erase(temp);

		/***************** find what temp is *****************/
		if(double_ps.find(temp)!=double_ps.end()) {
			
			++i;
			if(i==argc) {
				error_statement(argv);
                cerr<<"ERROR:: "<<temp<<" should be followed by a number (float)\n\n"<<endl;
                exit(-1);
            }
			string temp2(argv[i]);
			double_ps[temp]=cast_string_to_double(temp2);
			
		} else if(int_ps.find(temp)!=int_ps.end()) {
			
			++i;
			if(i==argc) {
				error_statement(argv);
                cerr<<"ERROR:: "<<temp<<" should be followed by a number (int)\n\n"<<endl;
                exit(-1);
            }
			string temp2(argv[i]);
			int_ps[temp]=cast_int(cast_string_to_double(temp2));
			
		} else if(string_ps.find(temp)!=string_ps.end()) {
			
			++i;
			if(i==argc) {
				error_statement(argv);
                cerr<<"ERROR:: "<<temp<<" should be followed by a string\n\n"<<endl;
                exit(-1);
            }
			string temp2(argv[i]);
			string_ps[temp]=temp2;
			
		} else if(bool_ps.find(temp)!=bool_ps.end()) {
			
			bool_ps[temp]= !bool_ps[temp];
		} else {
			error_statement(argv);
            cerr<<"ERROR:: "<<temp<<" is an unknown flag!\n\n"<<endl;
            exit(-1);
		}
		
		/***************** find what temp is *****************/
		++i;		
	}

	required_flags_are_set(argv);
	
}


void parameters::required_flags_are_set(char * argv[]) {
	
	if(required_flags.size()!=0) {		
		error_statement(argv);
		cerr<<"ERROR:: flag "<<*required_flags.begin()<<" needs to be set\n\n\n"<<endl;
        exit(-1);
	}

}


void parameters::printing(ostream & pout) {
	
	for(map<string, int>::iterator itm=int_ps.begin(); itm!=int_ps.end(); itm++) {
		pout<<itm->first<<" "<<itm->second<<endl;
	}

	for(map<string, double>::iterator itm=double_ps.begin(); itm!=double_ps.end(); itm++) {
		pout<<itm->first<<" "<<itm->second<<endl;
	}
	
	for(map<string, string>::iterator itm=string_ps.begin(); itm!=string_ps.end(); itm++) {
		pout<<itm->first<<" "<<itm->second<<endl;
	}
	
	for(map<string, bool>::iterator itm=bool_ps.begin(); itm!=bool_ps.end(); itm++) {
		if(itm->second)
			pout<<itm->first<<" true"<<endl;
		else
			pout<<itm->first<<" false"<<endl;
			
	}
}



void set_parameters_for_ibuhm(parameters & P, int argc, char * argv[]) {
    
    P.set_string("-f", "nofile", true, "[string]: name of the network file [node feature weight]");
    P.set_string("-outfolder", ".", false, "[string]: name of outfolder. default is cwd.");
    P.set_int("-cf", 1000, false, "[int]: number of candidates per feature. Default is 1000");
    P.set_int("-neigh", 10, false, "[int]: minimum number of neighbors which are printed for each node. Default is 10");
    P.set_double("-th", 0.3, false, "[float]: cosine similarity threshold. Values higher than this are printed. Default is 0.3");
    P.set_bool("-cosine", false, false, "use this flag to use cosine similarity intead of random walk similarity");
    P.set_bool("-okself", false, false, "prints self-loops too (rw only).");
    
    
    // append "/" to folder name
    P.string_ps.at("-outfolder") = P.string_ps.at("-outfolder") + "/";

    
	P.set_from_argv(argc, argv);
	P.printing(cout);
    
    
    
    {
        // check network file exists
        ifstream gin(P.string_ps.at("-f").c_str());
        if (!gin.is_open()) {
            cout<<"ERROR:: file "<<P.string_ps.at("-f")<<" file not found"<<endl;
            exit(-1);
        }
    }
    
    cout<<"parameters OK"<<endl;
    


}

