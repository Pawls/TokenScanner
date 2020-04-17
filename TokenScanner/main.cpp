// CS4303 Programming Language Concepts
// Lab exercise - Parser
//
// Name ______Paul Davis________________ ID ________1850953_____________
//
// A simple parser for C/C++-style variable declarations.
// THe grammar is as follows:
//
// <declaration> 	::= 	<type>  <var> ';' | <type> <var> '=' <number> ';'
// <type> 		::= 	int | float
// <var> 		::= 	A | B | C | D | E
// <number> 	::= 	<integer> | <float>
// <integer> 	::= 	<integer> <digit> | <digit>
// <digit> 		::= 	0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
// <float> 		::= 	<integer> '.' <integer>
// 
// Input is entered at the keyboard.
// If the input is correct, the program should print
// "no error found", otherwise, it should print the type
// of error and terminate its execution. There are four
// possible errors:
// 
// "unrecognizable type"
// "illegal variable name"
// "unexpected number"
// "; expected"
//
// Error message is printed out by calling function 
// "error". An error code ranging from 0 to 4 can be
// passed as an argument to the function indicating what
// message is to be printed. The mapping from the error
// code to the message can be found by looking at the
// definition of function "error".
//
// The following are some sample input and the response
// from the program:
//
// Please enter a declaration in format <type> <variable> [= number] ;
// int A;
// no error found
//
// Please enter a declaration in format <type> <variable> [= number] ;
// int a;
// illegal variable name
//
// Please enter a declaration in format <type> <variable> [= number] ;
// short B;
// unrecognizable type
// 
// Please enter a declaration in format <type> <variable> [= number] ;
// float C = 0.5;
// no error found
// 
// Please enter a declaration in format <type> <variable> [= number] ;
// int A = 10,
// ; expected
//
// float C = ;
// unexpected number
//
// float D = 2;
// no errors found
//
// int E = 2.2;
// no errors found
//
// float B 40.5;
// ; expected
//
// Other sample input:
//		float B;
//		float C=0.2;
//		short D;
//		float F;
//
// The last two sample inputs should generate errors because "short" and "F" are
// not acceptable tokens.


#include <iostream>
#include <string>
#include <map>
#include <vector>
#define watch(x) cout << (#x) << " is " << (x) << endl;

using namespace std;

string GetToken();
void error(int);
void init();

const int tkn_N = 10; // Total number of acceptable tokens.
char reg_ex[tkn_N][6] = { ".","=",";","int","float","A","B","C","D","E" };
map<string, string> lexicon; // (token, type)


int main() {
	string token;
	vector<string> syntax;
	vector<int> error_codes;
	vector<string>::iterator itr;
	string prev_token = "";
	string this_token = "";
	
	init();

	cout << "Please enter a declaration in format "
		<< "<type> <variable> [= number];" << endl;
	cout << "The following are the tokens in the input:" << endl;

	token = GetToken();

	// Write the code here
	while (token != "") {
		syntax.push_back(token);
		token = GetToken();
	}

	// Check syntax for errors now
	for (itr = syntax.begin(); itr != syntax.end(); itr++) {
		this_token = lexicon[*itr];
		if (prev_token == "") {
			if (this_token != "type" && this_token != "separator")
				error_codes.push_back(1); // first token must be an identifier, or separator	
		}
		else
			if (prev_token == "type") {
				if (this_token == "number")
					error_codes.push_back(3); // Variable name expected. Unexpected number.
				else
					if (this_token != "variable")
						error_codes.push_back(2); // Illegal variable name
			}
			else
				if (prev_token == "variable") {
					if (this_token != "separator" && this_token != "operator")
						error_codes.push_back(4); // Separator or Operator expected
				}
				else
					if (prev_token == "operator")
						if (this_token != "number")
							error_codes.push_back(3); // Number is expected after operator. Unexpected number error.
		prev_token = this_token;
	}

	if (syntax.back() != ";") {
		error_codes.push_back(4); //End of line should be a semicolon
	}

	if (error_codes.size() == 0) // Push the "no errors found" error code
		error_codes.push_back(0);
	error(error_codes[0]);
	
	// This section would display all error codes, but the assignment only requires one
	//for (int i = 0; i < error_codes.size(); i++) {
	//	error(error_codes[i]);
	//}

	system("Pause");
	return 0;
}

void init() {
	lexicon.insert(pair<string, string>("=", "operator"));
	lexicon.insert(pair<string, string>(";", "separator"));
	lexicon.insert(pair<string, string>("int", "type"));
	lexicon.insert(pair<string, string>("float", "type"));
	lexicon.insert(pair<string, string>("A", "variable"));
	lexicon.insert(pair<string, string>("B", "variable"));
	lexicon.insert(pair<string, string>("C", "variable"));
	lexicon.insert(pair<string, string>("D", "variable"));
	lexicon.insert(pair<string, string>("E", "variable"));
}

string GetToken() {
	string token;
	char ch;
	int token_length;

	do { // Gets rid of leading spaces
		cin.get(ch);
	} while (ch == ' ');

	// This section handles tokens that ARE numbers
	while ((ch >= '0' && ch <= '9') || ch == '.')
	{
		token += ch;
		cin.get(ch);
		if (ch < '0' || ch > '9') { // After we're reached the end of the number, return
			if (ch != '.') {
				cin.putback(ch);
				lexicon.insert(pair<string, string>(token, "number"));
				return token;
			}
		}
	}

	// This section handles tokens that aren't numbers
	for (int i = 0; i < tkn_N; i++) {
		token_length = strlen(reg_ex[i]) - 1;
		for (int j = 0; j <= token_length; j++) {
			if (ch == reg_ex[i][j]) {
				if (j == token_length) { // If input matched the full token, it is accepted
					token = reg_ex[i];
					return token;
				}
				else
					cin.get(ch);
			}
			else
				break;
		}
	}

	token = ch;
	if (token != "\n" && token != "") {
		lexicon.insert(pair<string, string>(token, "error"));//cout << ch << ": Error: Unrecognizable token\n";
	}
	else {
		if (token == "\n")
			token = "";
	}
	return token;
}

void error(int code) {
	switch (code) {
	case 0: cout << "no errors found" << endl; break;
	case 1: cout << "unrecognizable type" << endl; break;
	case 2: cout << "illegal variable name" << endl; break;
	case 3: cout << "unexpected number" << endl; break;
	case 4: cout << "; expected" << endl; break;
	}
	return;
}