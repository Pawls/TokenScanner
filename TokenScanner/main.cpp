// CS4303 Programming Language Concepts
// Lab exercise - Scanner
//
// Name ____Paul Davis__________ ID _____1850953________________
//
// A simple lexical analyzer for C/C++-style variable declarations.
// The grammar for the declarations is as follows:
//
// <declaration> 	::= 	<type>  <var> ’;’ | <type> <var> ’=’ <number> ’;’
// <type> 		::= 	int | float
// <var> 		::= 	A | B | C | D | E
// <number> 	::= 	<integer> | <float>
// <integer> 	::= 	<integer> <digit> | <digit>
// <digit> 		::= 	0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
// <float> 		::= 	<integer> ‘.’ <integer>
// 
// The task of this exercise is to write a lexical analyzer (or 
// scanner) for the tokens used in the above grammar. The following
// is the regular expression that defines the tokens:
//
//		= | ; | int | float | A | B | C | D | E | [0-9]+ | [0-9]+\.[0-9]+
//
// The program will read a declaration from the keyboard, and the
// scanner you design should recognize and print out all tokens
// included in the input. For example, given the following declaration:
//
//		int A = 123;
//
// your program should print:
//
// int
// A
// =
// 123
// ;
//
// Make sure your program print out the token one per line in the order 
// they appear in the input. Once an erroneous token is encountered, your
// scanner should print out an error message and stopped scanning. For
// example, given the following input:
//
//		int A = 0#;
//
// your program should print:
//
// int
// A
// =
// 0
// #: Error: Unrecognizable token
//
// Note that tokens may NOT be separated by spaces. For example, the above
// input:
//
//		int A = 123;
//
// does not have a space to separate 123 and ;. Also, the following inputs
// are also legal and generate the same output:
//
//		int A=123;
//		intA=123;
// 
// However, a whole token cannot be separated by spaces. For example, the
// following input will cause 12 and 3 to be regarded as two distinct tokens.
//
//		int A = 12 3;
//
// The ouput will look like:
// int
// A
// =
// 12
// 3
// ;
//
// Also note that the scanner doesn't check for syntactic errors. Therefore the
// above input is legal to this program.
//
// Other sample input:
//		float B;
//		float C=0.2;
//		short D;
//		float F;
//
// The last two sample inputs should generate errors because "short" and "F" are
// not acceptable tokens.
//
// Important!!! Save your GetToken program. We are going to use it in future 
// labs.


#include <iostream>
#include <string>

using namespace std;

string GetToken();
const int tkn_N = 10; // Total number of acceptable tokens.
char reg_ex[tkn_N][6] = { ".","=",";","int","float","A","B","C","D","E" };

int main() {
	string token;

	cout << "Please enter a declaration in format "
		<< "<type> <variable> [= number];" << endl;
	cout << "The following are the tokens in the input:" << endl;

	token = GetToken();

	while (token != "") {
		cout << token << endl;
		token = GetToken();
	}

	cout << "Done!" << endl;

	system("Pause");
	return 0;
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
		token = "";
		cout << ch << ": Error: Unrecognizable token\n";
	}
	else {
		if (token == "\n")
			token = "";
	}
	return token;
}

