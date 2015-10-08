#include <iostream>
#include <string>
#include <map>
#include <deque>
#include <sstream>
#include <algorithm>
#include <utility>
using namespace std;

typedef deque<unsigned int> Vote;

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

void eval_votes(multimap<unsigned int, Vote> & votes, unsigned int num_cands){
    for(int i=1; i <= num_cands; i++){
        cout << i << ": " << votes.count(i) << endl;
    }
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
