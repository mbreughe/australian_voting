#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <utility>
using namespace std;

typedef vector<unsigned int> Vote;

pair<unsigned int, Vote> parse_vote(string s, unsigned int num_candidates){
    istringstream line(s);
    Vote vote;
    for(unsigned int i=0; i < num_candidates; i++){
        unsigned int cand;
        line >> cand;
        vote.push_back(cand);
    }
    return make_pair(vote.front(), vote);
}

struct CompareSecond{
    bool operator()(const pair<unsigned int, unsigned int>& left, const pair<unsigned int, unsigned int>& right) const{
        return left.second < right.second;
    }
};

void remove_worst_cand(multimap<unsigned int, Vote> & votes, map<unsigned int, unsigned int> & vote_count){
    unsigned int min_cand =  min_element(vote_count.begin(), vote_count.end(), CompareSecond())->first;

    // First change the key of the votes for the worst candidate
    //  Duplicate the elements for the losing candidate, but with 
    //  that candidate's vote removed. 
    auto min_cand_loc = votes.equal_range(min_cand);
    for(auto it = min_cand_loc.first; it != min_cand_loc.second; ++it){
        it->second.erase(it->second.begin());
        auto new_vote_pair = make_pair(*(it->second.begin()), it->second);
        votes.insert(new_vote_pair);
    }
 
    //  Erase the original elements where the losing candidate had the first vote
    votes.erase(min_cand_loc.first, min_cand_loc.second);   
 
    // Now update all other entries
    // Loop over all candidates
    for (auto votes_it = votes.begin(); votes_it!= votes.end(); ++votes_it){
        auto current_cand = votes.equal_range(votes_it->first);
        // Loop over all votes for this candidate
        for (auto it = current_cand.first; it != current_cand.second; ++it){
            Vote & vote = it->second;
            // Remove the candidate from the ballet
            auto rm_it = find(vote.begin(), vote.end(), min_cand);
            if (rm_it != vote.end()){
                vote.erase(rm_it);
            }
        }
    }
  
    for(auto it = votes.begin() ; it!=votes.end(); ++it){
        cout << it->first << " : ";
        for (auto it_2 = it->second.begin(); it_2 != it->second.end(); ++it_2){
            cout << *it_2;
        } 
        cout << endl;
    }
}

void eval_votes(multimap<unsigned int, Vote> & votes, unsigned int num_cands){
    map<unsigned int, unsigned int> vote_count;
    for(int i=1; i <= num_cands; i++){
        vote_count[i] = 0;
    }

    for(int i=1; i <= num_cands; i++){
        vote_count[i] = votes.count(i);
    }

    remove_worst_cand(votes, vote_count);
}

void solve_case(istream& input){
    string s;
    
    // We wrap a stringstream around, so we can use
    // getline --> this flushes until the next newline
    getline(input, s);
    istringstream line(s);
    unsigned int num_candidates;
    line >> num_candidates;

    // Build name mapping
    map<unsigned int, string> name_mapping;
    for (unsigned int j=1; j <= num_candidates; j++){
        getline(input, s);
        name_mapping[j] = s;
    }
    
    // Create a multimap of votes
    multimap<unsigned int, Vote> votes;
    // As long as there is input and we don't see a newline
    while (getline(cin, s) && s != "\n"){
        // Create a ballot
        pair<unsigned int, Vote> vote = parse_vote(s, num_candidates);
        votes.insert(vote);
    }

    eval_votes(votes, num_candidates);
}

int main(){
    unsigned int num_cases;
    cin >> num_cases;

    string s;
    getline(cin, s);    // get the first blank line

    for (unsigned int i=0; i < num_cases; i++){
        solve_case(cin);
    }
}
