#ifndef TESTEXEC_H
#define TESTEXEC_H
///////////////////////////////////////////////////////////////////////
// TestExecutive.h - Header file for Test Executive				     //
// ver 1.0      Declares member variables and member function to be  //
//				used in TestExecutive.cpp							 //
// Language:    C++, Visual Studio 2015                              //
// Application: Lexical Analyzer, CSE687 - Object Oriented Design    //
// Author:      Rohit Sharma, SUID-242093353,						 //
//				Syracuse University									 //
//              rshar102@syr.edu	                                 //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public Toker class and private ConsumeState class.
* Toker reads words from a std::stream, throws away whitespace and comments
* and returns words from the stream in the order encountered.  Quoted
* strings and certain punctuators and newlines are returned as single tokens.
*
* This is a new version, based on the State Design Pattern.  Older versions
* exist, based on an informal state machine design.
*
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp
* Build Command: devenv Tokenizer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.4 : 03 Feb 2016
* - fixed bug that prevented reading of last character in source by
*   clearing stream errors at beginning of ConsumeState::nextState()
* ver 3.3 : 02 Feb 2016
* - declared ConsumeState copy constructor and assignment operator = delete
* ver 3.2 : 28 Jan 2016
* - fixed bug in ConsumeState::nextState() by returning a valid state
*   pointer if all tests fail due to reaching end of file instead of
*   throwing logic_error exception.
* ver 3.1 : 27 Jan 2016
* - fixed bug in EatCComment::eatChars()
* - removed redundant statements assigning _pState in derived eatChars()
*   functions
* - removed calls to nextState() in each derived eatChars() and fixed
*   call to nextState() in ConsumeState::consumeChars()
* ver 3.0 : 11 Jun 2014
* - first release of new design
*
* Planned Additions and Changes:
* ------------------------------
* - Return quoted strings as single token.  This must handle \" and \'
*   correctly.
* - Consider converting eatNewLine() to eatSpecialTokens() - see below
* - Return [, ], {, }, (, ), <, >, :, ; as single character tokens
* - Return <<, >>, +=, -=, *=, /=, :: as two character tokens
*/
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include <string>

namespace Scanner
{
	class TestExecutive
	{
	public:
		TestExecutive();
		~TestExecutive();
		static Toker* getTokerObj();
		static void setTokerObj(Toker* toker);
		static SemiExp* getSemiExpObj();
		static void setSemiExpObj(SemiExp* semiExp);
		static void req1Demo();
		static void req2Demo();
		static void req3Demo();
		static void req4Demo();
		static void req5Demo();
		static void req6Demo();
		static void req7n8Demo();
		static void req9Demo();
		static void req10Demo();
		static void decorator(string str);
	private:
		static Toker* _toker;
		static SemiExp* _semiExp;
	};
}
#endif
#define a