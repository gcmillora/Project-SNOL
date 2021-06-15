#include <iostream> 
#include <algorithm>
#include <string.h>
#include <stack>

using namespace std; 

// helper funcs for eval infix to postfix
int prio(char c) 
{
    if(c == '^')
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


string evaluate(string infix) 
{
  	// using prebuilt stack funcs
    stack <char> charstack; 
    string result;
    // remove all whitespace
  	infix.erase(remove(infix.begin(), infix.end(), ' '), infix.end());
  	cout << infix << endl;
  	
  	
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
  	// i.e. postfixeval(result);
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
	
	string exp = "1 * 2 + 3 * 4";
	string result = evaluate(exp);
	cout << result;
	return 0;
}
