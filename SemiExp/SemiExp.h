#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// ver 3.3                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Lexical Analyzer, CSE687 - Object Oriented Design    //
// Author:      Rohit Sharma, SUID-242093353					     //
//				Syracuse University, rshar102@syr.edu			     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a 
* sequence of tokens that ends in {.  This will be extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
*
* This is a new version for students in CSE687 - OOD, Spring 2016 to use
* for Project #1.
*
* Build Process:
* --------------
* Required Files: 
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp, itokcollection.h
* 
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* Ver 3.3 : 10 Feb 2016
* - declared methods for showing tokens and semiexpressions
* - declared method to detect for loop in stream
* - declared method to skip semicolons when for loop is detected
* - declared method to check access modifier to apply folding rule
* ver 3.2 : 02 Feb 2016
* - declared SemiExp copy constructor and assignment operator = delete
* - added default argument for Toker pointer to nullptr so SemiExp
*   can be used like a container of tokens.
* - if pToker is nullptr then get() will throw logic_error exception
* ver 3.1 : 30 Jan 2016
* - changed namespace to Scanner
* - fixed bug in termination due to continually trying to read
*   past end of stream if last tokens didn't have a semiExp termination
*   character
* ver 3.0 : 29 Jan 2016
* - built in help session, Friday afternoon
*
* Planned Additions and Changes:
* ------------------------------
* - add public :, protected :, private : as terminators
* - move creation of tokenizer into semiExp constructor so
*   client doesn't have to write that code.
*/

#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "itokcollection.h"

namespace Scanner
{
  using Token = std::string;

  class SemiExp : public ITokCollection
  {
  public:
    SemiExp(Toker* pToker = nullptr);
    SemiExp(const SemiExp&) = delete;
	bool get(bool clear = true);
	std::string& operator[](int n);
    size_t length();
	size_t find(const std::string& tok);
	bool merge(const std::string& firstTok, const std::string& secondTok);
	void push_back(const std::string& tok);
	bool remove(const std::string& tok);
	bool remove(size_t i);
	void toLower();
	void trimFront();
	void clear();
	std::string show(bool showNewLines = false);
	/*Methods declared in Ver 3.3*/
	void clear(bool clear);
	bool findFor(const std::string& tok); //detect for loop in stream
	bool skipSemiColons(std::string token); //skip semicolons when for loop detected
	bool checkAccessModifier(std::string token); //checks access modifiers
	void showSemiExpression(Toker* toker, ifstream* fileSpec, bool storeInFile); //show semiexpressions 
	void showTokens(Toker* toker, ifstream* inputFile, bool storeInFile, bool showComments, int caseId); //show tokens
  private:
    std::vector<Token> _tokens;
    Toker* _pToker;
	/*Member variable declared in Ver 3.3*/
	bool flag2 = false; //flag used in access modifiers
	const char* outputFileReq4 = "Tokens_Req4_Output.txt"; //Output file path for requirement 4
	const char* outputFileReq7_8 = "Semi_Req7_8_Output.txt"; //Output file path for requirement 7 and 8
	const char* outputFileReqSingleSpcl = "SingleSpecialCharReq_Output.txt"; //Output file path for single special character requirement handling
	const char* outputFileReqDoubleSpcl = "DoubleSpecialCharReq_Output.txt"; //Output file path for double special character requirement handling
  };
}
#endif