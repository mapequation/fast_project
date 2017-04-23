
/*
    reads a network such as doc_id feat_id weight
    and computed tf-idf similarities between the doc_ids
*/

// #include "standard_package/standard_include.cpp"
#include "utilities.h"
#include "set_parameters.h"

void read_weighted_network(string infile, map<int, mapid> & network, string type_of_network) {
    
    cout<<"reading "<<infile<<endl;
    ifstream gin(infile.c_str());
    
    string gins;
    network.clear();
    
    while(getline(gin, gins)) {
    
        DD vs;
        cast_string_to_doubles(gins, vs);
        double weight = 1;
        //
        if (vs.size() >= 2) {

            if (vs.size() > 2) {
                weight = vs[2];
            }
            
            if (type_of_network == "inlinks") {
                update_network(cast_int(vs[1]), cast_int(vs[0]), network, weight);
            } else if (type_of_network == "standard") {
                update_network(cast_int(vs[0]), cast_int(vs[1]), network, weight);
            } else if (type_of_network == "both") {
                update_network(cast_int(vs[0]), cast_int(vs[1]), network, weight);
                update_network(cast_int(vs[1]), cast_int(vs[0]), network, weight);
            } else {
                general_assert(false, "type_of_network");
            }
        }
        //
    }
}



double cosine_similarity(mapid & m_ref, mapid & m1) {
    
    double score = 0.0;
    for(const auto & x : m_ref) {
        if (m1.count(x.first) > 0) {
            score += x.second * m1.at(x.first);
        }
    }
    return score;
}


void random_walk_similarity(mapid & outdegrees, map<int, mapid> & concepts_to_documents,
                            mapid & concept_degree, mapid & landing_probabilities,
                            int doc_id, bool ok_self) {
    
    
    // { node: probability of landing there in two steps }
    landing_probabilities.clear();
    
    // loop through all nodes (these are already normalized)
    for(auto & x : outdegrees) {
        
        // look at the concepts
        //general_assert(concepts_to_documents.count(x.first) > 0, "concept missing?");
        //general_assert(concept_degree.count(x.first) > 0, "concept missing?");
        
        for (auto & c : concepts_to_documents[x.first]) if(ok_self or c.first != doc_id) {
            int_histogram(c.first, landing_probabilities, x.second * c.second / concept_degree[x.first]);
        }
    }
    
}




void compare_documents( map<int, mapid> & documents_to_concepts,
                        map<int, mapid> & concepts_to_documents, ostream & pout, parameters & P,
                        mapid & concept_degree) {
    
    
    // find all candidates for this node
    // top nodes for all its features


    map<int, mapid> projected_network;

    if (P.bool_ps.at("-cosine")) {

        for(auto doc_id_outlinks : documents_to_concepts) {
            
            int doc_id = doc_id_outlinks.first;
            
            SI candidates;
            for(auto & x : doc_id_outlinks.second ) {
                for(auto & y : concepts_to_documents.at(x.first)) {
                    candidates.insert(y.first);
                }
            }
            
            cout<<doc_id<<" candidates:: "<<candidates.size()<<endl;
            
            deque<pair<double, int> > sim_doc_id;
            
            for(auto & x : candidates) if (x != doc_id) {
                double sim = cosine_similarity(doc_id_outlinks.second, documents_to_concepts.at(x));
                sim_doc_id.push_back(make_pair(sim, x));
            }
            
            // sort similarities
            sort(sim_doc_id.rbegin(), sim_doc_id.rend());
            
            int filter_counter = 0;
            
            RANGE_loop(i, sim_doc_id) {
                if (int(i) < P.int_ps.at("-neigh") or sim_doc_id[i].first > P.double_ps.at("-th")) {
                    //pout<<doc_id<<" "<<sim_doc_id[i].second<<" "<<sim_doc_id[i].first<<endl;
                    update_network(doc_id, sim_doc_id[i].second, projected_network, sim_doc_id[i].first);
                    update_network(sim_doc_id[i].second, doc_id, projected_network, sim_doc_id[i].first);
                    filter_counter+=1;
                }
            }
            //cout<<"filtered "<<filter_counter<<endl;
        } // end loop
    
    } else {
    
        
        // random walk similarity
        for(auto & x : documents_to_concepts) {
            mapid landing_probabilities;
            random_walk_similarity( x.second, concepts_to_documents,
                                    concept_degree, landing_probabilities,
                                    x.first, P.bool_ps.at("-okself"));
            
            /*
            cout<<"node:" <<x.first<<endl;
            cout<<"landing probability: "<<endl;
            prints(landing_probabilities);
            */
            
            /*
            double eff_pvalue= pow(0.05, 1.0/(landing_probabilities.size() - 1.0));
            double sum_m = sum_mapid(landing_probabilities);
            prints(landing_probabilities);
            */
            
            cout<<"node "<<x.first<<" candidates: "<<landing_probabilities.size()<<endl;
            filter_top_threshold(landing_probabilities, 2.0, P.int_ps.at("-neigh"));
            
            for(const auto & nw : landing_probabilities) {
                update_network(x.first, nw.first, projected_network, nw.second);
                update_network(nw.first, x.first, projected_network, nw.second);
            }
            
        }
    }
    
    // print projected network
    for (const auto & x : projected_network) {
        for (const auto & y : x.second) {
            pout<<x.first<<" "<<y.first<<" "<<y.second<<endl;
        }
    }
    
    
}



int main(int argc, char * argv[]) {

    parameters P;
	set_parameters_for_ibuhm(P, argc, argv);
    
    // read the networks
    map<int, mapid> documents_to_concepts;
    map<int, mapid> concepts_to_documents;
    read_weighted_network(P.string_ps.at("-f"), documents_to_concepts, "standard");
    read_weighted_network(P.string_ps.at("-f"), concepts_to_documents, "inlinks");
    
    
    cout<<documents_to_concepts.size()<<" left nodes found"<<endl;
    cout<<concepts_to_documents.size()<<" right nodes found"<<endl;
    
    // {concept : strength}
    mapid concept_degree;
    for(auto & x : concepts_to_documents) {
        concept_degree[x.first] = sum_mapid(x.second);
    }
    
    //cout<<"concept_degree"<<endl;
    //prints(concept_degree);
    
    // trim concepts
    for(auto & x : concepts_to_documents) {
        filter_top(x.second, 0.0, P.int_ps.at("-cf"));
    }
    
    // normalize docs
    ofstream bipfile(P.string_ps.at("-outfolder") + "bip_file_aggregated.txt");
    for(auto & x : documents_to_concepts) {
        for (auto & y : x.second) {
            // if a feature appears in one document only,
            // it is not printed in bip_file_aggregated
            if (concepts_to_documents.at(y.first).size() > 1) {
                bipfile<<x.first<<" "<<y.first<<" "<<y.second<<endl;
            }
        }
        if (P.bool_ps.at("-cosine")) {
            normalize_mapid_square(x.second);
        } else {
            normalize_mapid(x.second);
        }
    }
    bipfile.close();
    
    
    cout<<"normalization done"<<endl;
    
    ofstream pout(P.string_ps.at("-outfolder") + "projected.txt");
    
    compare_documents(documents_to_concepts, concepts_to_documents, pout, P, concept_degree);
    
    pout.close();
    
    return 0;
}





