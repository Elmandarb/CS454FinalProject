#include <vector>
#include <queue>
#include <cmath>
#include <iostream>
// CS 454 Final Project
// Question 1 Lights out puzzle
// Eric Fleming, Trent Jones, Cody Rimes


// fill out our dfa
/*
for dfa on 0 and 1
//qs = 0, on 0 go to {q1,fo} in slot 1, on 1 go to {q2,f1} in slot 2, non accepting
//{q1,fo} = 1, on 0 go to {R,f0,q2} in slot 5, on 1 go to {R,q1,f1} in slot 3, accepting
//{q2,f1} = 2, on 0 go to {R,q1,f1} in slot 3, on 1 go to {R,f0,q2} in slot 5, accepting
//{R,q1,f1} = 3, on 0 go to {R,f1,f0} in slot 4, on 1 go to {R,f1,f0} in slot 4, non accepting
//{R,f1,f0} = 4, on 0 go to {R,q2,f1} in slot 8, on 1 go to {R,q1,f0} in slot 7, accepting
//{R,f0,q2} = 5, on 0 go to {R,q2,q1} in slot 6, on 1 go to {R,q2,q1} in slot 6, accepting
//{R,q2,q1} = 6, on 0 go to {R,q1,f0} in slot 7, on 1 go to {R,q2,f1} in slot 8, accepting
//{R,q1,f0} = 7, on 0 go to {R,f0,q2} in slot 5, on 1 go to {R,q1,f1} in slot 3, accepting
//{R,q2,f1} = 8, on 0 go to {R,q1,f1} in slot 3, on 1 go to {R,f0,q2} in slot 5, accepting

for dfa on 0, 1, 0*, 1*
0* and 1* are in columns 2 and 3 so we can add 2 to input to get their values
6 states, do a bfs with any time we encounter a zero or one on input we also add the zero* or one* to the queue
only use this after the 0, 1 dfa above to compute if it can work, throw out any paths that go to r or end on a non accepting,
keep track of how many flips and where they happen, if we get to the same state only keep the path with the fewest flips

s  | 0 | on 0 go to q1 | on 1 go to q2 | on 0* go to f0 | on 1* go to f1 |
q1 | 1 | on 0 go to r  | on 1 go to r  | on 0* go to f0 | on 1* go to f1 |
q2 | 2 | on 0 go to q1 | on 1 go to q2 | on 0* go to r  | on 1* go to r  | 
f0 | 3 | on 0 go to q2 | on 1 go to q1 | on 0* go to r  | on 1* go to r  |
f1 | 4 | on 0 go to r  | on 1 go to r  | on 0* go to f1 | on 1* go to f0 | 
r  | 5 | on 0 go to r  | on 1 go to r  | on 0* go to r  | on 1* go to r  |



*/ 
struct node {
    int state;
    int flips;
    std::vector<int> fPos;
};
bool validCheck(std::string entry) {
    int delta[9][2];
    bool accepting[9];
    //qs = 0, on 0 go to {q1,fo} in slot 1, on 1 go to {q2,f1} in slot 2, non accepting
    delta[0][0] = 1;
    delta[0][1] = 2;
    accepting[0] = false;
    //{q1,fo} = 1, on 0 go to {R,f0,q2} in slot 5, on 1 go to {R,q1,f1} in slot 3, accepting
    delta[1][0] = 5;
    delta[1][1] = 3;
    accepting[1] = true;
    //{q2,f1} = 2, on 0 go to {R,q1,f1} in slot 3, on 1 go to {R,f0,q2} in slot 5, accepting
    delta[2][0] = 3;
    delta[2][1] = 5;
    accepting[2] = true;
    //{R,q1,f1} = 3, on 0 go to {R,f1,f0} in slot 4, on 1 go to {R,f1,f0} in slot 4, non accepting
    delta[3][0] = 4;
    delta[3][1] = 4;
    accepting[3] = false;
    //{R,f1,f0} = 4, on 0 go to {R,q2,f1} in slot 8, on 1 go to {R,q1,f0} in slot 7, accepting
    delta[4][0] = 8;
    delta[4][1] = 7;
    accepting[4] = true;
    //{R,f0,q2} = 5, on 0 go to {R,q2,q1} in slot 6, on 1 go to {R,q2,q1} in slot 6, accepting
    delta[5][0] = 6;
    delta[5][1] = 6;
    accepting[5] = true;
    //{R,q2,q1} = 6, on 0 go to {R,q1,f0} in slot 7, on 1 go to {R,q2,f1} in slot 8, accepting
    delta[6][0] = 7;
    delta[6][1] = 8;
    accepting[6] = true;
    //{R,q1,f0} = 7, on 0 go to {R,f0,q2} in slot 5, on 1 go to {R,q1,f1} in slot 3, accepting
    delta[7][0] = 5;
    delta[7][1] = 3;
    accepting[7] = true;
    //{R,q2,f1} = 8, on 0 go to {R,q1,f1} in slot 3, on 1 go to {R,f0,q2} in slot 5, accepting
    delta[8][0] = 3;
    delta[8][1] = 5;
    accepting[8] = true;

    //dfa is now filled out
    //with accepting states
    //now need to parse string and trace dfa;
    int index = 0;
    int state = 0;
    while(index < entry.length()) {
        if(entry.at(index) == '1') {
            state = delta[state][1];
        }
        else if(entry.at(index) == '0') {
            state = delta[state][0];
        }
        else {
            std::cout << "Error, unexpected value at space " << index << " character is : " << entry.at(index) << std::endl;
            std::cout << "Please only input a string of 0's and 1's" << std::endl;
            exit(1);
        } 
        index++;
    }
    //string is parsed return accepting
    return accepting[state];
}


std::vector<int> flippedPlaces(std::string input) {
    // have to use original dfa to determine flips?
    //flippable dfa:
    int flippableDelta[6][4];
    int rejectState = 5;
    int flipOffset = 2;
    int accept1 = 3;
    int accept2 = 2;
    /*
    s  | 0 | on 0 go to q1 | on 1 go to q2 | on 0* go to f0 | on 1* go to f1 |
    q1 | 1 | on 0 go to r  | on 1 go to r  | on 0* go to f0 | on 1* go to f1 |
    q2 | 2 | on 0 go to q1 | on 1 go to q2 | on 0* go to r  | on 1* go to r  | 
    f0 | 3 | on 0 go to q2 | on 1 go to q1 | on 0* go to r  | on 1* go to r  |
    f1 | 4 | on 0 go to r  | on 1 go to r  | on 0* go to f1 | on 1* go to f0 | 
    r  | 5 | on 0 go to r  | on 1 go to r  | on 0* go to r  | on 1* go to r  |
    */
    // s
    flippableDelta[0][0] = 1;
    flippableDelta[0][1] = 2;
    flippableDelta[0][2] = 3;
    flippableDelta[0][3] = 4;

    //q1
    flippableDelta[1][0] = 5;
    flippableDelta[1][1] = 5;
    flippableDelta[1][2] = 3;
    flippableDelta[1][3] = 4;

    //q2
    flippableDelta[2][0] = 1;
    flippableDelta[2][1] = 2;
    flippableDelta[2][2] = 5;
    flippableDelta[2][3] = 5;

    //f0
    flippableDelta[3][0] = 2;
    flippableDelta[3][1] = 1;
    flippableDelta[3][2] = 5;
    flippableDelta[3][3] = 5;

    //f1
    flippableDelta[4][0] = 5;
    flippableDelta[4][1] = 5;
    flippableDelta[4][2] = 4;
    flippableDelta[4][3] = 3;

    //r
    flippableDelta[5][0] = 5;
    flippableDelta[5][1] = 5;
    flippableDelta[5][2] = 5;
    flippableDelta[5][3] = 5;

    //now do a bfs with a queue and compare to reject state
    //make a queue and push the starting node
    std::queue<node> nodes;
    struct node root = { 0, 0 };
    int stringIndex = 0;
    nodes.push(root);
    //iterate through each input, adding the char and their dot to the queue if they dont end in a reject state
    while(stringIndex < input.length()) {
        int queueSize = nodes.size();
        int queueIndex = 0;
        int currentChar;
        if(input.at(stringIndex) == '1') {
            currentChar = 1;
        }
        else {
            currentChar = 0;
        }
        while(queueIndex < queueSize) {
            // node has state, counter for flips, and vector to determine flip position
            node current = nodes.front();
            nodes.pop();
            node regNode = {flippableDelta[current.state][currentChar]  , current.flips, current.fPos};
            regNode.fPos.push_back(0);
            node flipNode = {flippableDelta[current.state][currentChar + flipOffset]  , current.flips+1, current.fPos}; 
            flipNode.fPos.push_back(1);
            if(regNode.state != rejectState) {
                nodes.push(regNode);
            }
            if(flipNode.state != rejectState) {
                nodes.push(flipNode);
            }
            queueIndex++;
        }

        stringIndex++;
    }

    
    node winner;
    int leastFlips = input.length()+1;
    while(!nodes.empty()) {
        if((nodes.front().flips < leastFlips) && (nodes.front().state == accept1 || nodes.front().state == accept2)) {
            winner = nodes.front();
            leastFlips = winner.flips;
        }
        nodes.pop();
    }
    std::cout << "least amount of flips is " << leastFlips << std::endl;
    return winner.fPos;
}
// todo
void flipSimulator(std::string input, std::vector<int> flipPlaces) {
    std::cout << "original string is                " << input << std::endl;
    int index = 0;
    if(input.length() < 2) {
        if(flipPlaces.at(index) == 1) {
            std::cout << "flipping place " << index << " new string is : " ;
            std::cout << 1 << std::endl;
            std::cout << "This is the final string!" << std::endl;
        }
        else {
            std::cout << "This is the final string!" << std::endl;
            
        }
        
        return;
    }
    while(index < flipPlaces.size()) {
        if(flipPlaces.at(index) == 1) {
            std::cout << "flipping place " << index << " new string is : " ;
            if(index == 0) {
                if(input.at(index) == '0') {
                    input.at(index) = '1';
                }
                else {
                    input.at(index) = '0';
                }
                if(input.at(index+1) == '0') {
                    input.at(index+1) = '1';
                }
                else {
                    input.at(index+1) = '0';
                }
            }
            else if(index == flipPlaces.size()-1) {
                if(input.at(index) == '0') {
                    input.at(index) = '1';
                }
                else {
                    input.at(index) = '0';
                }
                if(input.at(index-1) == '0') {
                    input.at(index-1) = '1';
                }
                else {
                    input.at(index-1) = '0';
                }
            }
            else {
                if(input.at(index) == '0') {
                    input.at(index) = '1';
                }
                else {
                    input.at(index) = '0';
                }
                if(input.at(index-1) == '0') {
                    input.at(index-1) = '1';
                }
                else {
                    input.at(index-1) = '0';
                }
                if(input.at(index+1) == '0') {
                    input.at(index+1) = '1';
                }
                else {
                    input.at(index+1) = '0';
                }
            }
            std::cout << input << std::endl;
        }
        index++;
    }
    std::cout<< "This is the final string!" << std::endl;
}

int main() {

    

    std::string input;
    std::cout << "Please Enter a string of 0's and 1's: " << '\n';
    while(std::cin >> input){
        
        if(validCheck(input)) {
            std::cout << "The string " << input << " can be solved! " << std::endl;
            std::vector<int> ans = flippedPlaces(input);
            flipSimulator(input, ans);
        }
        else {
            std::cout << "The string " << input << " cannot be solved :( " << std::endl;
        }
        std::cout << "Please Enter another string " << std::endl;
        
    }
    return 0;
}


