#include <iostream>
#include <iterator>
#include <string>
#include <regex>
#include <stack>
#include <deque>
#include <unordered_map>
#include <algorithm>

using namespace std;

void BEGCommand(string,unordered_map<string,string>&,string&);
void PRINTCommand(string, unordered_map<string,string>,string&);
bool validIdentifier(string);
bool checkIfAssignment(string,string&,string&);
bool isOperator(char);
int checkIfCommand(string);
bool parseNumber(unordered_map<string,string>, int&, string, deque<string>&, string&);
int evaluate_number(string);
string varExists(int&,string&,unordered_map<string,string> ,string&);
string convertToMath(string,unordered_map<string, string>,string&);




int main(){
  string line_input;
  string var;
  string expression;
  string error="0";
  unordered_map<string,string> memory;
  memory.insert(make_pair("num","-5"));
  memory.insert(make_pair("var","5"));
  cout<<"Converted: "<<convertToMath("num BEG+var",memory,error)<<endl;
  cout<<"Input a command: ";
  getline(cin,line_input);
  line_input = regex_replace(line_input, regex("^ +| +$|( ) +"), "$1");
  if(checkIfAssignment(line_input,var,expression)){
    if(validIdentifier(var)){
      expression = convertToMath(expression,memory,error);
            cout<<expression;
      if((expression.find("error") != std::string::npos)) cout<<expression;
      else{
        if(memory.find(var)==memory.end()) memory.insert(make_pair(var,expression));
        else memory[var] = expression;
      }
    }else{
      cout<<"Unknown word!";
    }
  }
  int command = checkIfCommand(line_input);
  if(command==1){
    //Step 1: Ihiwalay ang BEG ug katong variable name ex. BEG var, ang var ang icheck
    //Step 2: Icheck if ang variable name, is valid na identifier, validIdentifier() na function
    //Step 3: If valid siya, icheck if existing siya sa map, sample code: "if(memory.find(key)==memory.end())", if true ang if na na kay meaning wala siya sa map, else naa siya sa map
    //Step 4: call na ang function na BEGCommand(variable name, memory,error) 
  }
  if(command==2){
    //Step 1: Ihiwalay ang PRINT ug katong next sa print, PRINT var or PRINT 100.
    //Step 2: Icheck nimo kung variable or number, evaluateNumber(),
    //evaluateNumber() will return 3 if hindi number, will return 1 if float, will return 2 if integer
    //Step 3: If variable name, kailangan nimo icheck if valid identifier, if valid, tawagon na nimo si printcommand
    //Step 4: If dili variable name, iprint lang nimo dayon ang number
    //
    //To print variable and value, for example "num", cout<<memory["num"];
  }

}

//A function to do the BEG command, has three parameters, the key which is the variable name,
//an unordered_map where the variables are stored, and a string error to keep track.
void BEGCommand(string key, unordered_map<string,string>& memory,string& error){
  string value;
  cout<<"SNOL> Please enter value for ["<<key<<"]: "<<endl;
  cout<<"Input: ";
  getline(cin,value);
  if(evaluate_number(value)==3) error = "Error! Unknown command, does not match any valid command of the language."; 
  if(error!="0") return;
  if(memory.find(key)==memory.end())
    memory.insert(make_pair(key,value));
  else memory[key] = value;
}

//A function to do the PRINT command, has three parameters, the key which is the variable name,
//an unordered_map where the variables are stored, and a string error to keep track.
void PRINTCommand(string key, unordered_map<string,string> memory,string& error){
  if(memory.find(key)==memory.end())
    error = "Error! ["+key+"] is not defined.";
  else{
    cout<<"["<<key<<"] = "<<memory[key]<<endl; 
  }
}

//A function to check if an identifier is valid, it follows a regex pattern and checks whether
//the identifier is a keyword of SNOL, which is BEG or PRINT.
bool validIdentifier(string input){
  if(regex_match(input,regex("[a-zA-Z][a-zA-Z0-9]*"))&&input!="BEG"&&input!="PRINT"){
    return true;
  }
  return false;
}

//A function to check whether an assignment command is found, if an assignment is found,
//it uses pass-by-reference to store the variable name to `var`, and the RHS expression to
//`expression`.
bool checkIfAssignment(string input,string& var,string& expression){
  if((input.find("=") != std::string::npos)){
    int position_equal = input.find('=');
    var = input.substr(0,position_equal);
    expression = input.substr(position_equal+2,input.length()-var.length());
    var = regex_replace(var, regex("^ +| +$|( ) +"), "$1");
    expression = regex_replace(expression, regex("^ +| +$|( ) +"), "$1");
    return true;
}else return false;
}

//Checks whether a character is an operator. Returns true if yes, else return false.
bool isOperator(char character){
  if(character=='+'||character=='-'||character=='*'||character=='/'||character=='%') return true;
  else return false;
}

//A function to check if a `BEG` or `PRINT` command is found, also checks if the keyword `BEG`
//or `PRIINT` is found within the input string, if yes then returns 0 or an error.
int checkIfCommand(string input){
  if(input.substr(0,3)=="BEG")
    return 1;
  if(input.substr(0,5)=="PRINT") return 2;
  else if(input.find("BEG") != std::string::npos||input.find("PRINT") != std::string::npos)
    return 0;
  else return 3;
}

//A function to evaluate the data type of a number, it follows a regex pattern to identify
//whether an input is an integer, or a float. It returns 3 if the string does not follow an integer
//or float.
int evaluate_number(string number){
  int type=3;
  if(regex_match(number,regex("[+-]?([0-9]*)?[.][0-9]+"))){
    type=1;
  }
  else if(regex_match(number,regex("(\\+|-)?[[:digit:]]+"))){
    type=2;
  }
  return type;
}

//A function to check if a variable exists,if a valid identifier is found, then it finds the variable
//in the map or memory. If the variable DNE in the memory then it returns false, if the variable name
//is not a valid identifier, then it creates an error that an `Unknown word is found!`.
string varExists(int& type,string& operand,unordered_map<string,string> memory,string& error){
  if(validIdentifier(operand)){
    cout<<"VARIABLE NAME: "<<operand<<endl;
    if(memory.find(operand)==memory.end())
            return "false";
    cout<<"VARIABLE VALUE: "<<memory[operand]<<endl;
    operand = memory[operand];
    type = evaluate_number(operand);
    return "true";
  }else{
    error = "Error! Unknown word ["+operand+"].";
    return "Invalid";
  }
}

//A function to parse a number and checks whether it has the same data type as the previous operands from the input.
//If the current type of a number is 3, it checks whether it is a valid variable and if it exists in the memory.
//Moreoever, it checks every number if it has the same data type from the previous checked number.
bool parseNumber(unordered_map<string,string> memory, int& type, string number, deque<string>& eqn, string& error){
     cout<<"PARSING NUMBER: "<<number<<endl<<endl;
     if(number=="") {
       cout<<eqn.back();
       error = "Error: Invalid command";
       return false;
     }
     else if(type==0){
        type = evaluate_number(number);
        if(type==3){
          if(varExists(type,number,memory,error)=="true"){
            type = evaluate_number(number);
            if(number[0]=='-') eqn.push_back("("+number+")");
            else eqn.push_back(number);
            return true;
          } else if(varExists(type,number,memory,error)=="false") {
            error = "Error! Undefined variable ["+number+"]";
            return false;
          }
        }
        eqn.push_back(number);
        return true;
      } 
      else if(evaluate_number(number)==3){
          int temp = type;
          if(varExists(type,number,memory,error)=="true") {
            if(temp == evaluate_number(number)){
              if(number[0]=='-') eqn.push_back("("+number+")");
              else eqn.push_back(number);
              return true;
            }
            else{
              error= "Error! Operands must be of the same type in an arithmetic operation!";
              return false;
            }
        }else if(varExists(type,number,memory,error)=="false"){
          error= "Error! Undefined variable ["+number+"]";
          return false;}
        else {
          error = "Error! Unknown word!";
          return false;
        }
      }
      else if(type==evaluate_number(number)){
        cout<<type;
        eqn.push_back(number);
        return true;
      }
      else {
        error= "Error! Operands must be of the same type in an arithmetic operation!";
        return false;
      }
}

//A function to convert the inputted arithemtic expression into a valid expression with
//numbers or values only, extracts the variable values

string convertToMath(string input,unordered_map<string, string> memory,string& error){
  input = regex_replace(input, regex("^ +| +$|( ) +"), "$1");
  int length = input.length(), op=0,terms=0;
  bool neg=false;
  stack<char> balance;
  deque<string> eqn;
  string temp="",number="";
  int type=0;
  for(int ctr=0;ctr<length;ctr++){
    if((error.find("Error!") != std::string::npos)){
     break;
    }
    //--------------------------------------------------------//
    if(input[ctr]=='('){
      eqn.push_back("(");
      balance.push(input[ctr]);
      continue;
    }
    //--------------------------------------------------------//
    if(input[ctr]==')') {
      eqn.push_back(")");
      balance.pop();
      continue;
    }
    //--------------------------------------------------------//
    if(input[ctr]=='-'&&ctr==0){
      if(!isdigit(input[ctr+1])) error = "Error! Unknown command, does not match any valid command of the language.";
      else neg=true;
      continue;
    }
    if(input[ctr]=='-'&&isOperator(input[ctr-1])){
      if(!isdigit(input[ctr+1])) error = "Error! Unknown command, does not match any valid command of the language.";
      else neg=true;
      continue;
    }
    //--------------------------------------------------------//
    if(isOperator(input[ctr])){
      if(!parseNumber(memory,type,number,eqn,error)) break;
      cout<<"Type: "<<type<<endl;
      if(neg){
        string neg_num = "(-" + eqn.back()+")";
        eqn.pop_back();
        eqn.push_back(neg_num);
        neg = false;
      }
      terms++;
      char front = eqn.back()[0];
      if(isOperator(front)&&eqn.back().length()==1){
        cout<<"front";
        error = "Unknown command! Does not match any valid command of the language.";
        break;
      }
      number=input[ctr];
      eqn.push_back(number);
      op++;
      number="";
      if(terms!=op) error = "Error! Unknown command, does not match any valid command of the language.";
      continue;
    }
    //--------------------------------------------------------//
    if(input[ctr]==' '){
      if(eqn.size()!=0) {
        if(!isOperator(eqn.back()[0])){
          if(!parseNumber(memory,type,number,eqn,error)) break;
          if(!isOperator(eqn.back()[0])&&isdigit(eqn.back()[1])) error = "Error! Unknown command, does not match any valid command of the language.";
          terms++;
        }
      }else {
        if(!parseNumber(memory,type,number,eqn,error)) break;
        terms++;
        }
      number="";
      continue;
    }
    //--------------------------------------------------------//
    number+=input[ctr];
    //--------------------------------------------------------//
    if(ctr==length-1) {
      if(!parseNumber(memory,type,number,eqn,error)) break;
      cout<<"Last Type: "<<type<<endl<<endl;
        if(neg){
          cout<<"Last Term Negative"<<endl;
        string neg_num = "(-" + eqn.back()+")";
        eqn.pop_back();
        eqn.push_back(neg_num);
        neg = false;
      }
      terms++;
    }
    //--------------------------------------------------------//
    }
    if(error!="0") return error;
    int len = eqn.size();
    for(int ctr=0;ctr<len;ctr++){
      string result = eqn.front();
      temp+=result+" ";
      eqn.pop_front();
    }
  return temp;
}
