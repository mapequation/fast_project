



void splitString(const string & s, char delim, deque<string> & elems) {
    
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }

}



template <typename one, typename two>
double compute_similarity_map(const map<one, two> & m1, const map<one, two> & m_ref, const double & denominator_exp) {
    
    double score = 0.0;
    double norm = m1.size();
    
    if (m1.size() == 0) {
        return 0;
    }
    
    
    for(const auto & x : m_ref) {
        if (m1.count(x.first) > 0) {
            score += x.second * m1.at(x.first);
        }
    }
    return score / pow(norm, denominator_exp);
    
}



void get_idfs(const string & filename, map<string, int> & idfs) {

    idfs.clear();
    ifstream gin(filename.c_str());
    general_assert(gin.is_open(), filename + " not found");
    string gins;
    while(getline(gin, gins)) {
        deque<string> vs;
        separate_strings(gins, vs);
        general_assert(vs.size() == 2, "error with " + filename);
        idfs[vs[0]] = cast_int(cast_string_to_double(vs[1]));
    }
    
}



template <typename one>
double compute_similarity_map_normalized(const map<one, double> & m1, const map<one, double> & m_ref) {
    
    double score = 0.0;
    
    // this could be optimized!!!!
    double norm1 = 0.0;
    double norm2 = 0.0;
    
    for(const auto & x : m_ref) {
        norm1 += x.second * x.second;
    }

    for(const auto & x : m1) {
        norm2 += x.second * x.second;
    }
    
    if (norm1 <= 0 or norm2 <= 0) {
        return 0;
    }
    
    for(const auto & x : m_ref) {
        if (m1.count(x.first) > 0) {
            score += x.second * m1.at(x.first);
        }
    }
    return score / pow(norm1 * norm2, 0.5);
    
}


template <typename one>
double compute_similarity_entropy(const map<one, double> & m1, const map<one, double> & m_ref, const map<one, double> & baseline) {
    
    
    double norm1 = 0.0;
    double norm2 = 0.0;

    for(const auto & x : m1) {
        norm1 += x.second;
    }

    for(const auto & x : m_ref) {
        norm2 += x.second;
    }
    general_assert(norm1 > 0 and norm2 > 0, "similarity entropy");
    
    double h = 0.0;
    double h_norm = 0.0;
    for(const auto & x : m1) {
    
        double g = 1.0/m_ref.size();
        /*if (baseline.count(x.first) > 0) {
            g = baseline.at(x.first);
        }
        */
        
        double g2 = 0.0;
        if (m_ref.count(x.first) > 0) {
            g2 = m_ref.at(x.first);
        }
        h_norm -= x.second/norm1 * log(x.second/norm1);
        h -= x.second/norm1 * log(0.99 * g2/norm2 + 0.01 * g);
    }
    
    return h_norm/h;
    
}



void filter_top(mapid & m, double weight_threshold, int num_of_concepts_threshold, deque<pair<double, int> > & p) {
    
    p.clear();
    for(auto & x : m) {
        p.push_back(make_pair(x.second, x.first));
    }
    sort(p.rbegin(), p.rend());
    // pop small elements until we reach the right size
    while(p.size() > 0 and (int(p.size()) > num_of_concepts_threshold or p.at(p.size()-1).first < weight_threshold * p.at(0).first)) {
        p.pop_back();
    }
    m.clear();
    RANGE_loop(i, p) {
        m[p[i].second] = p[i].first;
    }
    
}




void filter_top(mapid & m, double weight_threshold, int num_of_concepts_threshold) {


    deque<pair<double, int> > p;
    filter_top(m, weight_threshold, num_of_concepts_threshold, p);
    
    
}



void filter_top_threshold(mapid & m, double weight_threshold, int top_num) {
    
    // keep weights bigger than threshold OR in top_num
    
    deque<pair<double, int> > p;
    for(auto & x : m) {
        p.push_back(make_pair(x.second, x.first));
    }
    sort(p.rbegin(), p.rend());
    
    // pop small elements until we reach the right size or elements are bigger than threshold
    while(int(p.size()) > top_num) {
        
        if (p.at(p.size()-1).first >= weight_threshold) {
            break;
        } else {
            p.pop_back();
        }
    }
    m.clear();
    RANGE_loop(i, p) {
        m[p[i].second] = p[i].first;
    }
    
}




void compute_word_counts(string input_str_raw, map<int, int> & wc, map<pair<int, int>, int> & bc, map<string, int> & word_ids) {
    
    
    wc.clear();
    bc.clear();
    
    deque<string> input_str;
    separate_strings(input_str_raw, input_str);
    
    RANGE_loop(i, input_str) {
        if (word_ids.count(input_str[i]) == 0) {
            int new_id = int(word_ids.size());
            word_ids[input_str[i]] = new_id;
        }
    }
    
    RANGE_loop(i, input_str) {

        int word_id = word_ids.at(input_str[i]);
        
        if (wc.count(word_id) == 0) {
            wc[word_id] = 0;
        }
        wc[word_id] += 1;
        if (i+1 < input_str.size()) {
            pair<int, int> p(word_id, word_ids.at(input_str[i+1]));
            if (bc.count(p) == 0) {
                bc[p] = 0;
            }
            bc[p] += 1;
        }
    }
}




void compute_word_counts(string input_str_raw, map<int, double> & wc, map<pair<int, int>, double> & bc, map<string, int> & word_ids) {
    
    
    wc.clear();
    bc.clear();
    
    deque<string> input_str;
    separate_strings(input_str_raw, input_str);
    
    RANGE_loop(i, input_str) {
        if (word_ids.count(input_str[i]) == 0) {
            int new_id = int(word_ids.size());
            word_ids[input_str[i]] = new_id;
        }
    }
    
    RANGE_loop(i, input_str) {

        int word_id = word_ids.at(input_str[i]);
        
        if (wc.count(word_id) == 0) {
            wc[word_id] = 0;
        }
        wc[word_id] += 1;
        if (i+1 < input_str.size()) {
            pair<int, int> p(word_id, word_ids.at(input_str[i+1]));
            if (bc.count(p) == 0) {
                bc[p] = 0;
            }
            bc[p] += 1;
        }
    }
}


void compute_word_counts_normalized(string input_str_raw, map<int, double> & wc, map<pair<int, int>, double> & bc, map<string, int> & word_ids, map<string, double> & word_docs) {
    
    
    //input_str_raw = "hello hello hello hello world";
    
    wc.clear();
    bc.clear();
    
    deque<string> input_str;
    separate_strings(input_str_raw, input_str);
    
    RANGE_loop(i, input_str) {
        if (word_ids.count(input_str[i]) == 0) {
            int new_id = int(word_ids.size());
            word_ids[input_str[i]] = new_id;
        }
    }
    
    RANGE_loop(i, input_str) {

        int word_id = word_ids.at(input_str[i]);
        
        if (wc.count(word_id) == 0) {
            wc[word_id] = 0;
        }
        double num_docs = 1.0;
        if (word_docs.count(input_str[i]) > 0) {
            num_docs += word_docs[input_str[i]];
        }
        
        wc[word_id] += 1.0 / num_docs;
        
        if (i+1 < input_str.size()) {
            pair<int, int> p(word_id, word_ids.at(input_str[i+1]));
            if (bc.count(p) == 0) {
                bc[p] = 0;
            }
            double num_docs2 = 1.0;
            if (word_docs.count(input_str[i+1])>0) {
                num_docs2 += word_docs[input_str[i+1]];
            }
            bc[p] += 1.0 / num_docs / num_docs2;
        }
    }
    
    
    /*
    prints(wc);
    prints(word_ids);
    for (const auto & x: bc) {
        cout<<x.first.first<<" "<<x.first.second<<" ->> "<<x.second<<endl;
    }
    */
    //exit(-1);
    
    
}



