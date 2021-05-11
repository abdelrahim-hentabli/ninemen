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

  friend bool operator > (const Node& rhs, const Node& lhs){
    return lhs.depth + lhs.heuristic > rhs.depth + rhs.heuristic;
  }
  friend bool operator < (const Node& rhs, const Node& lhs){
    return lhs.depth + lhs.heuristic < rhs.depth + rhs.heuristic;
  }
};


//checks if string is goal state
bool goal(const string& state){
  return state == "123456789****";
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
    count += ( (i+1) == (state[i] - '0'))?0:1; 
  }
  return count; 
}

//manhattan distance heuristic
int manhattanDistance(const string& state){
  int totalDist = 0;
  int i;
  for(i = 0; i < 10; i++){
    if(state[i] != '*'){
      totalDist += abs(state[i]-'0' - 1 - i);
    }
  }
  for(; i < 13; i++){
    if(state[i] != '*'){
      totalDist += 1 + abs( (state[i] - '0' - 1) - ((i-10) * 2 + 3));
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
          if(i != 0){
            output.push_back(state.substr(0, i-1) + '*' + state[i-1] + state.substr(i+1));
          }
          if(i != 9){
            output.push_back(state.substr(0, i) + state[i+1] + '*' + state.substr(i+2));
          }
      }
    }
  }
  if(foundStars != 4){
    cout<<"Invalid State: "<<state;
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
  theQueue.push(Node(startState,0,heuristic(startState)));
  visitedStates.insert(startState);
  while(!theQueue.empty()){
    prettyPrint(theQueue.top().state);
    maxQueueSize = std::max(maxQueueSize, int(theQueue.size()));
    std::cout<<"Heuristic: "<<theQueue.top().heuristic<<'\n';
    std::cout<<"Depth: "<<theQueue.top().depth<<'\n';
    std::cout<<"Queue Size: "<<theQueue.size()<<'\n';
    std::cout<<"Visited Nodes: "<<visitedStates.size()<<"\n\n";
    if(goal(theQueue.top().state)){ 
      std::cout<<"Solved in "<<steps<<" steps.\n";
      std::cout<<"Max Queue Size: "<<maxQueueSize<<'\n';
      return true;
    }
    else{
      steps++;
      moves = validMoves(theQueue.top().state);
      depth = theQueue.top().depth;
      theQueue.pop();
      for(auto move: moves){
        if(visitedStates.find(move) == visitedStates.end()){
          visitedStates.insert(move);
          theQueue.push(Node(move, depth+1, heuristic(move)));
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
  while(algorithm < 1 || algorithm > 3){
    cout<<"Choose an algorithm:\n \
    1) Breadth-First Search\n \
    2) A* with Misplaced Tile Heuristic\n \
    3) A* with Manhattan Distance Heuristic\n";
    getline(cin, line);
    algorithm = stoi(line, nullptr); 
    if(algorithm < 0 || algorithm > 3){
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
  }
  return 0;
}
