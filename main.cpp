#include <iostream>

#include <iterator>

#include <string>
#include <regex>
#include <stack>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <math.h>
#include <sstream>

using namespace std;

void BEGCommand(string, unordered_map < string, string > & , string & );
void PRINTCommand(string, unordered_map < string, string > , string & );
bool validIdentifier(string);
bool checkIfAssignment(string, string & , string & );
bool isOperator(char);
int checkIfCommand(string);
bool parseNumber(unordered_map < string, string > , int & , string, deque < string > & , string & );
int evaluate_number(string);
string varExists(int & , string & , unordered_map < string, string > , string & );
string convertToMath(string, unordered_map < string, string > , string & );
int operatorPriority(char);
int evalIntOp(char, int, int);
double evalDoubleOp(char, double, double);
int checkDataType(string);
string floatEval(string);
string intEval(string);
string evaluate(string);

int main() {
  string line = "";
  string
  var;
  string key;
  int evalID;
  string expression;
  string error = "0";
  unordered_map < string, string > memory;
  memory.insert(make_pair("num", "-5"));
  memory.insert(make_pair("var", "5"));
  cout << "The SNOL environment is now active, you may proceed with\ngiving your commands.";

  do {
    cin.clear();
    error = "0";
    var = "";
    expression = "";
    cout << "\n\nCommand: ";
    getline(cin, line);
    line = regex_replace(line, regex("^ +| +$|( ) +"), "$1");
    if (checkIfAssignment(line,
        var, expression)) {
      if (validIdentifier(var)) {
        expression = convertToMath(expression, memory, error);
        if(expression == "ERROR") error = "Unknown command! Does not match any valid command of the language.";
        if (error != "0") {
          cout << "SNOL> "<<error;
          continue;
        } else {
          if (memory.find(var) == memory.end()) memory.insert(make_pair(var, expression));
          else memory[var] = expression;
        }
      } else {
        cout << "SNOL> Unknown word [" <<
          var << "].";
        continue;
      }
      continue;
    }

    int command = checkIfCommand(line);
    if (line == "EXIT!") {
      break;
    } else if (command == 0) {
      error = "Unknown command! Does not match any valid command of the language.";
      cout << "SNOL> "<<error;
    } else if (command == 1) {
      key = line.substr(4, line.length());
      if (validIdentifier(key)) BEGCommand(key, memory, error);
      else cout << "Error! Input variable is invalid!";
    } else if (command == 2) {
      key = line.substr(6, line.length());
      evalID = evaluate_number(key);
      switch (evalID) {
      case 1:
      case 2:
        cout << "SNOL> " << key;
        break;
      case 3:
        if (validIdentifier(key)) PRINTCommand(key, memory, error);
        else cout << "Error! Input variable is invalid!";
        break;
      }
      if (error != "0") cout << "SNOL> "<<error;
    } else if (command == 3) {

      expression = convertToMath(line, memory, error);
      if(expression == "ERROR") error = "Unknown command! Does not match any valid command of the language.";
      if (error != "0") cout << "SNOL> "<<error;
    }
  } while (line != "EXIT!");
  cout << "\n\nInterpreter is now terminated..." << endl;
  return 0;
}

void BEGCommand(string key, unordered_map < string, string > & memory, string & error) {
  string value;
  cout << "SNOL> Please enter value for [" << key << "]: " << endl;
  cout << "Input: ";
  getline(cin, value);
  if (evaluate_number(value) == 3) error = "Error! Unknown command, does not match any valid command of the language.";
  if (error != "0") return;
  if (memory.find(key) == memory.end())
    memory.insert(make_pair(key, value));
  else memory[key] = value;
}

//A function to do the PRINT command, has three parameters, the key which is the variable name,
//an unordered_map where the variables are stored, and a string error to keep track.
void PRINTCommand(string key, unordered_map < string, string > memory, string & error) {
  if (memory.find(key) == memory.end())
    error = "Error! [" + key + "] is not defined.";
  else {
    cout << "SNOL> [" << key << "] = " << memory[key] << endl;
  }
}

//A function to check if an identifier is valid, it follows a regex pattern and checks whether
//the identifier is a keyword of SNOL, which is BEG or PRINT.
bool validIdentifier(string input) {
  if (regex_match(input, regex("[a-zA-Z][a-zA-Z0-9]*")) && input != "BEG" && input != "PRINT") {
    return true;
  }
  return false;
}

//A function to check whether an assignment command is found, if an assignment is found,
//it uses pass-by-reference to store the variable name to `var`, and the RHS expression to
//`expression`.
bool checkIfAssignment(string input, string &
  var, string & expression) {
  if ((input.find("=") != std::string::npos)) {
    int position_equal = input.find('=');
    var = input.substr(0, position_equal);
    expression = input.substr(position_equal + 1, input.length() -
      var.length());
    var = regex_replace(var, regex("^ +| +$|( ) +"), "$1");
    expression = regex_replace(expression, regex("^ +| +$|( ) +"), "$1");
    return true;
  } else return false;
}

//Checks whether a character is an operator. Returns true if yes, else return false.
bool isOperator(char character) {
  if (character == '+' || character == '-' || character == '*' || character == '/' || character == '%') return true;
  else return false;
}

//A function to check if a `BEG` or `PRINT` command is found, also checks if the keyword `BEG`
//or `PRIINT` is found within the input string, if yes then returns 0 or an error.
int checkIfCommand(string input) {
  if (input.substr(0, 3) == "BEG")
    return 1;
  if (input.substr(0, 5) == "PRINT") return 2;
  else if (input.find("BEG") != std::string::npos || input.find("PRINT") != std::string::npos)
    return 0;
  else return 3;
}

//A function to evaluate the data type of a number, it follows a regex pattern to identify
//whether an input is an integer, or a float. It returns 3 if the string does not follow an integer
//or float.
int evaluate_number(string number) {
  int type = 3;
  if (regex_match(number, regex("[+-]?([0-9]*)?[.][0-9]+"))) {
    type = 1;
  } else if (regex_match(number, regex("(\\+|-)?[[:digit:]]+"))) {
    type = 2;
  }
  return type;
}

//A function to check if a variable exists,if a valid identifier is found, then it finds the variable
//in the map or memory. If the variable DNE in the memory then it returns false, if the variable name
//is not a valid identifier, then it creates an error that an `Unknown word is found!`.
string varExists(int & type, string & operand, unordered_map < string, string > memory, string & error) {
  if (validIdentifier(operand)) {
    if (memory.find(operand) == memory.end())
      return "false";
    operand = memory[operand];
    type = evaluate_number(operand);
    return "true";
  } else {
    error = "Error! Unknown word [" + operand + "].";
    return "Invalid";
  }
}

//A function to parse a number and checks whether it has the same data type as the previous operands from the input.
//If the current type of a number is 3, it checks whether it is a valid variable and if it exists in the memory.
//Moreoever, it checks every number if it has the same data type from the previous checked number.
bool parseNumber(unordered_map < string, string > memory, int & type, string number, deque < string > & eqn, string & error) {
  if (number == "") {
    error = "Error: Invalid command!";
    return false;
  } else if (type == 0) {
    type = evaluate_number(number);
    if (type == 3) {
      if (varExists(type, number, memory, error) == "true") {
        type = evaluate_number(number);
        if (number[0] == '-') eqn.push_back("( " + number + " )");
        else eqn.push_back(number);
        return true;
      } else if (varExists(type, number, memory, error) == "false") {
        error = "Error! Undefined variable [" + number + "]";
        return false;
      }
    }
    eqn.push_back(number);
    return true;
  } else if (evaluate_number(number) == 3) {
    int temp = type;
    if (varExists(type, number, memory, error) == "true") {
      if (temp == evaluate_number(number)) {
        if (number[0] == '-') eqn.push_back("( " + number + " )");
        else eqn.push_back(number);
        return true;
      } else {
        error = "Error! Operands must be of the same type in an arithmetic operation!";
        return false;
      }
    } else if (varExists(type, number, memory, error) == "false") {
      error = "Error! Undefined variable [" + number + "]";
      return false;
    } else {
      error = "Error! Unknown word!hello";
      return false;
    }
  } else if (type == evaluate_number(number)) {
    eqn.push_back(number);
    return true;
  } else {
    error = "Error! Operands must be of the same type in an arithmetic operation!";
    return false;
  }
}

//A function to convert the inputted arithemtic expression into a valid expression with
//numbers or values only, extracts the variable values

string convertToMath(string input, unordered_map < string, string > memory, string & error) {
  input = regex_replace(input, regex("^ +| +$|( ) +"), "$1");
  int length = input.length(), op = 0, terms = 0;
  bool neg = false;
  stack < char > balance;
  deque < string > eqn;
  string temp = "", number = "";
  int type = 0;
  for (int ctr = 0; ctr < length; ctr++) {
    if ((error.find("Error!") != std::string::npos)) {
      break;
    }
    //--------------------------------------------------------//
    if (input[ctr] == '(' || input[ctr] == ')') {
      if (input[ctr] == '(') {
        eqn.push_back("(");
        balance.push(input[ctr]);

      } else {
        if (input[ctr - 1] == ' ') {
          eqn.push_back(")");
          balance.pop();
        } else {
          if (number != "") {
            if (!parseNumber(memory, type, number, eqn, error)) break;
            if (neg) {
              string neg_num = "( -" + eqn.back() + " )";
              eqn.pop_back();
              eqn.push_back(neg_num);
              neg = false;
            }
            number = "";
            terms++;
          }
          eqn.push_back(")");
          balance.pop();
        }
      }
      continue;
    }

    //--------------------------------------------------------//

    //--------------------------------------------------------//
    if ((input[ctr] == '-' && ctr == 0)) {
      if (!isdigit(input[ctr + 1])) error = "Error! Unknown command, does not match any valid command of the language.";
      else neg = true;
      continue;
    }

    if (input[ctr] == '-' && eqn.size() != 0) {
      if (input[ctr - 1] == '(' || input[ctr - 2] == '(') {
        if (!isdigit(input[ctr + 1])) error = "Error! Unknown command, does not match any valid command of the language.";
        else neg = true;
        continue;
      }
      if (isOperator(input[ctr - 1])) {
        cout<<input[ctr-1]<<" "<<input[ctr+1]<<endl;
        if (!isdigit(input[ctr + 1])) error = "Error! Unknown command, does not match any valid command of the language.";
        else neg = true;
        continue;
      }
      if(input[ctr-1]==' '&&isOperator(eqn.back()[0])){
         if (!isdigit(input[ctr + 1])) error = "Error! Unknown command, does not match any valid command of the language.";
        else neg = true;
        continue;
      }
    }

    //--------------------------------------------------------//
    if (isOperator(input[ctr])) {
      if (ctr == 0) {
        error = "Unknown command! Does not match any valid command of the language. first operator";
        break;
      }
      if (!(input[ctr - 1] == ' ') && !(input[ctr - 1] == ')')) {
        if (!parseNumber(memory, type, number, eqn, error)) break;
        if (neg) {
          string neg_num = "( -" + eqn.back() + " )";
          eqn.pop_back();
          eqn.push_back(neg_num);
          neg = false;
        }
        terms++;
      }
      if (eqn.size() != 0) {
        if (isOperator(eqn.back()[0]) && eqn.back().length() == 1) {
          error = "Unknown command! Does not match any valid command of the language.";
          break;
        }
      }
      number = input[ctr];
      eqn.push_back(number);
      op++;
      number = "";
      if (terms != op) error = "Error! Unknown command, does not match any valid command of the language.";
      continue;
    }
    //--------------------------------------------------------//
    if (input[ctr] == ' ') {
      if (eqn.size() != 0) {
        if (number != "") {
          if (!parseNumber(memory, type, number, eqn, error)) break;
          terms++;
          continue;
        }
        if (!isOperator(eqn.back()[0]) && (eqn.back()[0] != ')' && eqn.back()[0] != '(')) {
          if (!parseNumber(memory, type, number, eqn, error)) break;
          if (!isOperator(eqn.back()[0]) && isdigit(eqn.back()[1])) error = "Error! Unknown command, does not match any valid command of the language.";
          terms++;
        }
      } else {
        if (!parseNumber(memory, type, number, eqn, error)) break;
        terms++;
      }
      number = "";
      continue;
    }
    //--------------------------------------------------------//
    if (!(input[ctr] == ')') && !(input[ctr] == '('))
      number += input[ctr];
    //--------------------------------------------------------//
    if (ctr == length - 1) {
      if (!parseNumber(memory, type, number, eqn, error)) break;
      if (neg) {
        string neg_num = "( -" + eqn.back() + " )";
        eqn.pop_back();
        eqn.push_back(neg_num);
        neg = false;
      }
      terms++;
    }
  }
  if (error != "0") return error;
  if (terms != 1) {
    if (terms != op + 1) {
      cout << number << " " << terms << " " << op << " " << eqn.back() << endl;
      error = "Error! Unknown command, does not match any valid command of the language.too many";
    }
  }
  if (error != "0") return error;
  int len = eqn.size();
  for (int ctr = 0; ctr < len; ctr++) {
    string result = eqn.front();
    temp += result + " ";
    eqn.pop_front();
  }
  string result = evaluate(temp);
  return result;
}


int operatorPriority(char c) {
  if (c == '%')
    return 5;
  else if (c == '*')
    return 4;
  else if (c == '/')
    return 3;
  else if (c == '-')
    return 2;
  else if (c == '+')
    return 1;
  else
    return -1;
}

//function to evaluate int operands
int evalIntOp(char op, int val1, int val2) {
  switch (op) {
  case '+':
    return (val2 + val1);
  case '-':
    return (val2 - val1);
  case '*':
    return (val2 * val1);
  case '/':
    return (val2 / val1);
  case '%':
    return (val2 % val1);
  }
}

//function to evaluate double operands
double evalDoubleOp(char op, double val1, double val2) {
  switch (op) {
  case '+':
    return (val2 + val1);
  case '-':
    return (val2 - val1);
  case '*':
    return (val2 * val1);
  case '/':
    return (val2 / val1);
  }
}

// helper function to determine data type
int checkDataType(string result) {
  int i, flag = 0;

  for (i = 0; i < result.length(); i++) {
    if (result[i] == '.') {
      flag = 1;
      break;
    }
  }
  return flag;
}

//evaluates the float expression
string floatEval(string postfix) {
  ostringstream ss;
  ss.precision(10);
  string results;
  stack < double > operands;
  int i = 0, j = 0;
  double val1 = 0, val2 = 0, num = 0;
  int dot = 0, len = 0;

  for (i = 0; i < postfix.length(); i++) {

    //if character is blank space then continue
    if (postfix[i] == ' ') continue;

    //if character is a number, push to stack
    else if (isdigit(postfix[i])) {
      num = 0.0;
      j = i;
      len = 0;
      while (isdigit(postfix[j]) || postfix[j] == '.') { //count length of the number
        len++;
        j++;
      }
      j = 0;
      while (isdigit(postfix[i]) || postfix[i] == '.') {
        if (postfix[i] == '.')
          dot = len - j - 1;
        else
          num = num * 10 + (postfix[i] - '0');

        i++;
        j++;
      }
      i--;
      while (dot != 0) {
        num /= 10;
        dot--;
      }
      operands.push(num);
    }

    //negative number '~'
    else if (postfix[i] == '~') {
      operands.top() = operands.top() * -1;
    }

    //if character is an operator, pop 2 elements from stack
    else {
      //no modulo op for float numbers
      if (postfix[i] == '%') {
        return "ERROR";
      }
      val1 = operands.top();
      operands.pop();
      val2 = operands.top();
      operands.pop();

      num = evalDoubleOp(postfix[i], val1, val2);
      operands.push(num);
    }
  }
  //convert final result to string and pass back the converted string
  num = operands.top();
  operands.pop();
  ss << num;
  results = ss.str();
  return results;
}

//evaluates int expression
string intEval(string postfix) {
  stack < int > operands;
  int i, val1 = 0, val2 = 0;
  int num = 0;
  stringstream ss;
  string results;

  for (i = 0; i < postfix.length(); i++) {

    //if character is blank space then continue
    if (postfix[i] == ' ') continue;

    //if character is a number, push to stack
    else if (isdigit(postfix[i])) {
      num = 0;
      while (isdigit(postfix[i])) {
        num = num * 10 + (int)(postfix[i] - '0');
        i++;
      }
      i--;
      //push num to stack
      operands.push(num);
    }

    //negative number '~'
    else if (postfix[i] == '~') {
      operands.top() = operands.top() * -1;
    }

    //if character is an operator, pop 2 elements from stack
    else {
      val1 = operands.top();
      operands.pop();
      val2 = operands.top();
      operands.pop();

      num = evalIntOp(postfix[i], val1, val2);
      operands.push(num);
    }
  }
  //convert final result to string and pass back the converted string
  num = operands.top();
  operands.pop();
  ss << num;
  results = ss.str();
  return results;
}

string evaluate(string infix) {
  stack < char > charstack;
  string result;
  //Removing all whitespaces from the infix expression input.
  infix.erase(remove(infix.begin(), infix.end(), ' '), infix.end());

  //Check for Negative Numbers
  for (int x = 0; x < infix.length(); x++) {
    if (infix[x] != '-') {
      continue;
    }
    if (x == 0 || infix[x - 1] == '(' || (operatorPriority(infix[x - 1]) != -1)) {
      infix[x] = '~';
    }
  }

  for (int i = 0; i < infix.length(); i++) {
    char c = infix[i];
    //Since there are no letters, add number to stack.
    if (isdigit(c) || c == '.') {
      if (result.length() > 0 && !isdigit(result[result.length() - 1])) {
        if (!isspace(result[result.length() - 1]) && !(result[result.length() - 1] == '.')) {
          result += " ";
        }
      }
      result += c;
    }

    //If `(` is encounters, add to stack
    else if (c == '(') {
      charstack.push('(');
    } else if (isspace(infix[c])) {
      continue;
    }
    //If `)` is encountered, pop to result until `(` is encountered.
    else if (c == ')') {
      while (!charstack.empty() && charstack.top() != '(') {
        if (result.length() > 0 && !isspace(result[result.length() - 1])) {
          result += " ";
        }
        char temp = charstack.top();
        charstack.pop();
        result += temp;
      }
      charstack.pop();
    }
    //If an operator is found,
    else {
      if (infix[i] == infix[i + 1] || operatorPriority(infix[i + 1]) != -1) {
        return "ERROR";
      }
      if (result.length() > 0 && !isspace(result[result.length() - 1])) {
        result += " ";
      }
      while (!charstack.empty() && operatorPriority(infix[i]) < operatorPriority(charstack.top())) {
        char temp = charstack.top();
        charstack.pop();
        if (result.length() > 0 && !isspace(result[result.length() - 1])) {
          result += " ";
        }
        result += temp;
      }
      charstack.push(c);
    }
  }

  // pop then result
  while (!charstack.empty()) {
    char temp = charstack.top();
    charstack.pop();
    if (result.length() > 0 && !isspace(result[result.length() - 1])) {
      result += " ";
    }
    result += temp;
  }

  // main calculation function for postfix
  int type;
  //check data type (1 for float, 0 for int)
  type = checkDataType(result);
  if (type == 1) {
    result = floatEval(result);
  } else {
    result = intEval(result);
  }
  return result;
}