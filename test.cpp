#include<iostream>

using namespace std;
int priorityOp(char oprt){
  if(oprt == '^')
       return 5;
  else if(oprt== '*')
       return 4;
  else if(oprt== '/')
      return 3;
  else if(oprt == '-')
      return 2;
  else if(oprt == '+')
      return 1;
  else
      return -1;
}

bool inFixToPostfix(string input, string& output){
  if()
}
int main(){

}