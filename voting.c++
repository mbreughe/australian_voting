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

struct CompareSecondMin{
    bool operator()(const pair<unsigned int, unsigned int>& left, const pair<unsigned int, unsigned int>& right) const{
        return left.second < right.second;
    }
};
struct CompareSecondMax{
    bool operator()(const pair<unsigned int, unsigned int>& left, const pair<unsigned int, unsigned int>& right) const{
        return left.second > right.second;
    }
};

void remove_cand(multimap<unsigned int, Vote> & votes, unsigned int min_cand){

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
 
}
void print_votes(multimap<unsigned int, Vote> & votes){
    for(auto it = votes.begin() ; it!=votes.end(); ++it){
        cout << "votes[" << it->first << "] = ";
        for (auto it_2 = it->second.begin(); it_2 != it->second.end(); ++it_2){
            cout << *it_2;
        } 
        cout << endl;
    }
}

vector<unsigned int> get_remaining_candidates(const multimap<unsigned int, Vote> & votes){
    vector<unsigned int> remainder;
    for( auto it = votes.begin(), end = votes.end();
           it != end;
           it = votes.upper_bound(it->first)){
        
        remainder.push_back(it->first);

    }
    return remainder;
}

vector <unsigned int> get_tie_candiates(const map<unsigned int, unsigned int> & vote_count, unsigned int num_votes){
    vector <unsigned int> candidates;
    for(auto it = vote_count.begin(); it != vote_count.end(); ++it){
        if (it->second == num_votes){
            candidates.push_back(it->first);
        }
    }
    return candidates;
}

vector<unsigned int> eval_votes(multimap<unsigned int, Vote> & votes, unsigned int num_cands){
    map<unsigned int, unsigned int> vote_count;
    for(int i=1; i <= num_cands; i++){
        vote_count[i] = 0;
    }
    
    unsigned int num_voters = votes.size();

    while(true){
        // Remove the latest counting
        vote_count.clear();
  
        //print_votes(votes); 
        
        // Collect the remaining candidates
        vector<unsigned int> remaining_cands = get_remaining_candidates(votes);
        unsigned int num_remaining_cands = remaining_cands.size();
        // Count votes for the remaining candidates
        for(auto &el: remaining_cands){
            vote_count[el] = votes.count(el);
        }

        unsigned int max_vote_count =  min_element(vote_count.begin(), vote_count.end(), CompareSecondMax())->second;
        vector<unsigned int> tie_candidates = get_tie_candiates(vote_count, max_vote_count);

        // Break in case we have a single winner
        unsigned int votes_required = (unsigned int)(0.5f * num_voters) + 1;
        if (max_vote_count >= votes_required){
            return tie_candidates;
        }

        // Break in case all remaining candidates tie
        if(tie_candidates.size() == num_remaining_cands){
            return tie_candidates;
        }
        
        // Remove candidates with the lowest vote
        unsigned int min_vote_count =  min_element(vote_count.begin(), vote_count.end(), CompareSecondMin())->second;
        vector<unsigned int> candidates = get_tie_candiates(vote_count, min_vote_count);
        for (auto it = candidates.begin(); it != candidates.end(); ++it){
            remove_cand(votes, *it);  
        }
       
    }
}

bool has_only_whitespace(string s){
    return (s.find_first_not_of(' ') == std::string::npos);
}

void solve_case(istream& input){
    string s;
    
    // We wrap a stringstream around input , so we can use
    // getline --> this flushes until the next newline
    
    // Remove all whitelines at beginning
    while(getline(input, s) && has_only_whitespace(s)){
        ;
    }
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
    while (getline(cin, s) && !has_only_whitespace(s)){
        // Create a ballot
        pair<unsigned int, Vote> vote = parse_vote(s, num_candidates);
        votes.insert(vote);
    }

    vector<unsigned int> winners = eval_votes(votes, num_candidates);
    for (auto it = winners.begin(); it != winners.end(); ++it){
        cout << name_mapping[*it] << endl;
    }
}

int main(){
    unsigned int num_cases;
    cin >> num_cases;

    string s;

    for (unsigned int i=0; i < num_cases; i++){
        solve_case(cin);
        if (i != num_cases-1){
            cout << endl;
        }
    }
}
