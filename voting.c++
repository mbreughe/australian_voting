#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <sstream>
using namespace std;

typedef queue<unsigned int> Vote;

pair<unsigned int, Vote> parse_vote(string s, unsigned int num_candidates){

    return make_pair(0, queue<unsigned int>());
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
    
    // As long as there is input and we don't see a newline
    while (getline(cin, s) && s != "\n"){
        // Create a ballot
        pair<unsigned int, Vote> vote = parse_vote(s, num_candidates);        
    }
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
