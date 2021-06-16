#include <iostream> 
#include <algorithm>
#include <string.h>
#include <stack>
#include <ctype.h> 
#include <math.h> //fmod
#include <sstream>

using namespace std; 

// helper funcs for eval infix to postfix
int prio(char c) 
{
    if(c == '%')
        return 5;
    else if(c == '*')
        return 4;
    else if(c == '/')
        return 3;
    else if(c == '-')
        return 2;
    else if(c == '+')
        return 1;
    else
        return -1;
}

//function to evaluate int operands
int evalintop(char op, int val1, int val2){
	switch(op){
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
double evaldcop(char op, double val1, double val2){
	switch(op){
		case '+':
			return (val2 + val1);
		case '-':
			return (val2 - val1);
		case '*':
			return (val2 * val1);
		case '/':
			return (val2 / val1);
		case '%':
			return(fmod(val2, val1));
	}
}

// helper function to determine data type
int checkdatatype(string result){
	int i, flag = 0;
	
	for(i = 0; i < result.length(); i++){
		if (result[i] == '.'){
			flag = 1;
			exit;
		}	
	}
	return flag;
}

//evaluates the float expression
string floateval(string postfix){
	ostringstream ss;
	ss.precision(10);
	string results;
	stack <double> operands;
	int i = 0, j = 0;
	double val1 = 0, val2 = 0, num = 0;  
	int dot = 0, len = 0;
	
	for (i = 0; i < postfix.length(); i++){
		
		//if character is blank space then continue
		if (postfix[i] == ' ') continue;
		
		//if character is a number, push to stack
		else if (isdigit(postfix[i])){
			num = 0.0;
			j = i;
			len = 0;
			while (isdigit(postfix[j]) || postfix[j] == '.'){ //count length of the number
				len++;
				j++;
			}
			j = 0;
			while(isdigit(postfix[i]) || postfix[i] == '.'){
				if(postfix[i] == '.')
					dot = len - j - 1;
				else
					num = num * 10 + (postfix[i] - '0');
				
				i++;
				j++;	
			}
		 	i--;
		 	while(dot != 0){
		 		num /= 10;
		 		dot--;
			}
			operands.push(num);
		}
		
		//negative number '~'
		else if (postfix[i] == '~'){
			operands.top() = operands.top() * -1;
		}
		
		//if character is an operator, pop 2 elements from stack
		else{
			val1 = operands.top();
			operands.pop();
			val2 = operands.top();
			operands.pop();
			
			num = evaldcop(postfix[i], val1, val2);
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
string inteval(string postfix){
	stack <int> operands;
	int i, val1 = 0, val2 = 0;
	int num = 0;
	stringstream ss;
	string results;
	
	for (i = 0; i < postfix.length(); i++){
		
		//if character is blank space then continue
		if (postfix[i] == ' ') continue;
		
		//if character is a number, push to stack
		else if(isdigit(postfix[i])){
			num = 0;
			while(isdigit(postfix[i])){
				num = num * 10 + (int)(postfix[i] - '0');
				i++;
		 	}
			i--;
			//push num to stack
			operands.push(num);
		}
		
		//negative number '~'
		else if (postfix[i] == '~'){
			operands.top() = operands.top() * -1;
		}
		
		//if character is an operator, pop 2 elements from stack
		else{
			val1 = operands.top();
			operands.pop();
			val2 = operands.top();
			operands.pop();
			
			num = evalintop(postfix[i], val1, val2);
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


string evaluate(string infix) 
{
  	// using prebuilt stack funcs
    stack <char> charstack; 
    string result;
    
    
    
    // remove all whitespace, will consider numbers with spaces and no operators as single num
  	infix.erase(remove(infix.begin(), infix.end(), ' '), infix.end());
  	
  	// for negative numbers
  	for (int x = 0; x < infix.length(); x++) 
	  {
        if (infix[x] != '-') {
            continue;
        }
        if (x == 0 || infix[x - 1] == '(' || (prio(infix[x - 1]) != -1)) 
		{
            infix[x] = '~';
        }
    }
  	
    for(int i = 0; i < infix.length(); i++) 
	{
		
        char c = infix[i];
  
       	// since no letters, add number to stack 
        if(isdigit(c) || c == '.')
        {	
        	if (result.length() > 0 && !isdigit(result[result.length() - 1])) 
			{
                if (!isspace(result[result.length() - 1]) && !(result[result.length() - 1] == '.')) 
				{
                    result += " ";
                }
            }
           
            result += c;
		}
		
        // if ( is encontered add to stack
        else if(c == '(')
        {
            charstack.push('(');
		}
  
  		else if (isspace(infix[c])) 
		{
            continue;
        } 
  
        // if ) is encountered, pop to result until ( is encountered
        else if (c == ')') 
		{
            while(!charstack.empty() && charstack.top() != '(')
            {
            	if (result.length() > 0 && !isspace(result[result.length() - 1])) 
				{
                	result += " ";
            	}
                char temp = charstack.top();
                charstack.pop();
                result += temp;
            }
            charstack.pop();
        }
        
        // if operator
        else 
		{
			if(infix[i] == infix[i+1] || prio(infix[i+1]) != -1)
			{
				return "error";
			}
			
			if (result.length() > 0 && !isspace(result[result.length() - 1])) {
                result += " ";
            }
            
            
            while (!charstack.empty() && prio(infix[i]) < prio(charstack.top()) ) 
			{
                char temp = charstack.top();
                charstack.pop();
                if (result.length() > 0 && !isspace(result[result.length() - 1])) 
				{
                	result += " ";
            	}
                result += temp;
            }
            charstack.push(c);
        }
    }
  
    // pop then result
    while(!charstack.empty()) {
        char temp = charstack.top();
        charstack.pop();
		if (result.length() > 0 && !isspace(result[result.length() - 1])) 
		{
            result += " ";
    	}
        result += temp;
    }
    
  	// main calculation function for postfix
	int type;
	//check data type (1 for float, 0 for int)
	type = checkdatatype(result);
	if(type == 1){
		result = floateval(result);
	}
	else{
		result = inteval(result);
	}
    return result;
}


int main()
{
	
	/*
	#include <algorithm>
	#include <string.h>	
	#include <stack>
	needed headers for this func
	*/
	
	
	string exp = "( 15 / 2 ) % ( 16 + 12 )";
	string result = evaluate(exp);
	cout << result;
	return 0;
}
