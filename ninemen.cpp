#include <iostream>
#include <cassert>
#include <queue>
#include <unordered_set>
#include <stdlib.h>
#include <vector>

using namespace std;

struct Node{
  Node(){
    state = "123456789****";
    depth = 0;
    heuristic = 0;
  }
  Node(string s, int d, int h){
    state = s;
    depth = d;
    heuristic = h;
  }
  string state;
  int depth;
  int heuristic;

  //these are backwards because priority queue finds the largest item
  friend bool operator > (const Node& lhs, const Node& rhs){
    int lhsF = lhs.depth + lhs.heuristic;
    int rhsF = rhs.depth + rhs.heuristic;
    if(lhsF == rhsF){
      return lhs.heuristic < rhs.heuristic;
    }
    return lhsF < rhsF;
  }

  friend bool operator < (const Node& lhs, const Node& rhs){
    int lhsF = lhs.depth + lhs.heuristic;
    int rhsF = rhs.depth + rhs.heuristic;
    if(lhsF == rhsF){
      return lhs.heuristic > rhs.heuristic;
    }
    return lhsF > rhsF;
  }
};


//checks if string is goal state
bool goal(const string& state){
  return (state == "123456789****");
}

//prints the string state in a visually understandable way
void prettyPrint(const string& state){
  cout<<"   "<<state[10]<<' '<<state[11]<<' '<<state[12]<<'\n';
  cout<<state.substr(0,10)<<'\n';
}

//checks if a state is valid
bool validState(const string& state){
  int counts[10] =  {4,1,1,1,1,1,1,1,1,1};
  if(state.length() != 13){
    return false;
  }
  for(int i = 0; i < 13; i++){
    if(state[i] == '*'){
      counts[0]--;
    }
    else{
      counts[state[i] - '0']--;
    }
  }
  for(int i = 0; i < 10; i++){
    if(counts[i] != 0){
      return false;  
    }
  }
  return true;
}

//breadth first search 'heuristic'
int bfsHeuristic(const string& state){
  return 0;
}


//misplaced tile heuristic
int misplacedTile(const string& state){
  int count = 0;
  for(int i = 0; i < 9; i++){
    count += ( i == (state[i] - '1'))?0:1; 
  }
  return count; 
}

//manhattan distance heuristic
int manhattanDistance(const string& state){
  int totalDist = 0;
  int i;
  for(i = 0; i < 10; i++){
    if(state[i] != '*'){
      totalDist += abs(i - (state[i] - '1'));
    }
  }
  for(i = 10; i < 13; i++){
    if(state[i] != '*'){
      totalDist += 1 + abs( ((i-10) * 2 + 3) -  (state[i] - '1') );
    }
  }
  return totalDist;
}

//This function returns an array of all states that 
//are 1 valid move away from a state
vector<string> validMoves(const string& state){
  vector<string> output;
  int numberOfMoves = 0;
  int foundStars = 0;
  int underneathI, aboveI;
  string tempState = "";
  for(int i = 0; i < state.length(); i++){
    if(state[i] == '*'){
      foundStars++;
      switch(i){
        case 10:
        case 11:
        case 12:
          underneathI = (i-10) * 2 + 3;
          tempState = state.substr(0, underneathI) + "*" + state.substr(underneathI+1, i-(underneathI+1))+state[underneathI] + state.substr(i+1);
          output.push_back(tempState);
          break;
        case 3:
        case 5:
        case 7:
          aboveI = (i-3) / 2 + 10;
          tempState = state.substr(0, i) + state[aboveI] + state.substr(i+1, aboveI - (i+1)) + '*' + state.substr(aboveI+1);
          output.push_back(tempState);
        default:
          if(i > 0){
            output.push_back(state.substr(0, i-1) + '*' + state[i-1] + state.substr(i+1));
          }
          if(i < 9){
            output.push_back(state.substr(0, i) + state[i+1] + '*' + state.substr(i+2));
          }
      }
    }
  }
  if(foundStars != 4){
    prettyPrint(state);
    assert(foundStars == 4);
    return vector<string>();
  }
  return output;
}

bool a_star(string startState, int (*heuristic)(const string&)){
  unordered_set<string> visitedStates;
  priority_queue<Node> theQueue;
  vector<string> moves;
  int depth = 0;
  int maxQueueSize = 0;
  int steps = 0;
  Node currentNode;
  theQueue.push(Node(startState, 0, heuristic(startState)));
  visitedStates.insert(startState);
  while(!theQueue.empty()){
    currentNode = theQueue.top();
    prettyPrint(currentNode.state);
    maxQueueSize = max(maxQueueSize, int(theQueue.size()));
    std::cout<<"Heuristic: "<<currentNode.heuristic<<'\n';
    std::cout<<"Depth: "<<currentNode.depth<<'\n';
    std::cout<<"h()+g(): "<<currentNode.depth + currentNode.heuristic<<'\n';
    std::cout<<"Queue Size: "<<theQueue.size()<<'\n';
    std::cout<<"Visited Nodes: "<<visitedStates.size()<<"\n\n";
//    assert((currentNode.depth+ currentNode.heuristic) % 2 == 1);
    if(goal(currentNode.state)){ 
      std::cout<<"Solved in "<<steps<<" steps.\n";
      std::cout<<"Max Queue Size: "<<maxQueueSize<<'\n';
      return true;
    }
    else{
      steps++;
      moves = validMoves(currentNode.state);
      depth = currentNode.depth + 1;
      theQueue.pop();
      for(auto move: moves){
        if(visitedStates.find(move) == visitedStates.end()){
          visitedStates.insert(move);
          theQueue.push(Node(move, depth, heuristic(move)));
        }
      }
    }
  }
  std::cout<<"Solver never found answer in "<<steps<<" steps. \n";
  std::cout<<"Max Queue Size: "<<maxQueueSize<<'\n';
  return false;
}



int main(){
  int algorithm = 0;
  string line;
  while(algorithm < 1 || algorithm > 5){
    cout<<"Choose an algorithm:\n \
    1) Breadth-First Search\n \
    2) A* with Misplaced Tile Heuristic\n \
    3) A* with Manhattan Distance Heuristic\n";
    getline(cin, line);
    algorithm = stoi(line, nullptr); 
    if(algorithm < 0 || algorithm > 5){
      cout<<"Please Input a valid Algorithm\n";
    }
  }
  string startState = "";
  cout<<"\nStart State for the Nine Men in a Trench Problem (No Spaces, Men are 1-9, * is blank):\n";
  cin>>startState;
  assert(validState(startState));
  switch(algorithm){
    case 1:
      a_star(startState, bfsHeuristic);
      break;
    case 2:
      a_star(startState, misplacedTile);
      break;
    case 3:
      a_star(startState, manhattanDistance);
      break;
    case 4:
      prettyPrint(startState);
      cout<<"Manhattan Distance: "<<manhattanDistance(startState)<<'\n';
      break;
    case 5:
      prettyPrint(startState);
      cout<<"-------Valid Moves-------\n";
      for(auto move : validMoves(startState)){
        prettyPrint(move);
      }
  }
  return 0;
}
