/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::istream and generates     //
//						tokens out of them	                       //
// ver 3.5                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Lexical Analyzer, CSE687 - Object Oriented Design  //
// Author:      Rohit Sharma, SUID-242093353					   //
//				Syracuse University, rshar102@syr.edu			   //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This file provides methods to read stream and collect tokens. 
* Provides methods to validate certain rules and conditions to construct
* a token.
* Toker reads words from a std::istream, throws away whitespace and comments
* and returns words from the stream in the order encountered.  Quoted
* strings and certain punctuators and newlines are returned as single tokens.
*
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.5 : 10 Feb 2016
* - Provided methods to read stream and collect tokens. 
*   Provided methods to validate certain rules and conditions to construct
*   a token
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
*/
#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>

using namespace std;

namespace Scanner
{
	class ConsumeState
	{
	public:
		ConsumeState();
		ConsumeState(const ConsumeState&) = delete;
		ConsumeState& operator=(const ConsumeState&) = delete;
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pIn = pIn; }
		virtual void eatChars() = 0;
		void consumeChars() {
			_pState->eatChars();
			_pState = nextState();
		}
		bool canRead() { return _pIn->good(); }
		std::string getTok() { return token; }
		bool hasTok() { return token.size() > 0; }
		void setSingleSpecialChars(std::string);
		void setDoubleSpecialChars(std::string);
		bool IsSpecialSingle(std::string str);
		bool IsSpecialDouble(std::string str);
		static void reset();
		bool getShowComments() { return showComments; }
		void setShowComments(bool value) { showComments = value; }
	protected:
		static std::string token;
		static std::istream* _pIn;
		static int prevChar;
		static int currChar;
		static const char doubleQuote;
		static ConsumeState* _pState;
		static ConsumeState* _pEatCppComment;
		static ConsumeState* _pEatCComment;
		static ConsumeState* _pEatWhitespace;
		static ConsumeState* _pEatPunctuator;
		static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatNewline;
		static ConsumeState* _pQuotedString;
		static ConsumeState* _pSpclSingleCharToken;
		static ConsumeState* _pEatSpecialOneChar;
		static ConsumeState* _pEatSpecialTwoChar;
		static ConsumeState* _pSingleQuotedString;
		static bool ConsumeState::first_ctor;
		static bool ConsumeState::first_dtor;
		static bool showComments;
		ConsumeState* checkState(int chNext);
		ConsumeState* checkSpecialChars();
		ConsumeState* checkForComments(int chNext);
		ConsumeState* nextState();
	};
}

using namespace Scanner;

bool ConsumeState::showComments = true;
std::string ConsumeState::token;
std::istream* ConsumeState::_pIn = nullptr;
int ConsumeState::prevChar;
int ConsumeState::currChar;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
ConsumeState* ConsumeState::_pQuotedString = nullptr;
ConsumeState* ConsumeState::_pSpclSingleCharToken = nullptr;
ConsumeState* ConsumeState::_pEatSpecialOneChar = nullptr;
ConsumeState* ConsumeState::_pEatSpecialTwoChar = nullptr;
ConsumeState* ConsumeState::_pSingleQuotedString = nullptr;
char const ConsumeState::doubleQuote = '"';
bool ConsumeState::first_ctor = true;
bool ConsumeState::first_dtor = true;
std::vector<std::string> singleCharVector = { "<", ">","[","]", "(",")","{" , "}", ":", "=", "+", "-", "*", "\n" };
std::vector<std::string> doubleCharVector = { "<<" , ">>" , "::", "++", "--", "==" , "+=", "-=", "*=", "/=" };
std::string tempSingleStr;
std::string tempDoubleStr;

void testLog(const std::string& msg);

/*----------------method to reset first_ctor and first_dtor booleans------------------*/
void ConsumeState::reset()
{
	first_ctor = first_dtor = true;
}

/*----------------method to check special characters------------------*/
ConsumeState* ConsumeState::checkSpecialChars() {
	std::string match(1, (char)currChar);
	tempSingleStr = match;
	std::string second(1, (char)_pIn->peek());
	tempDoubleStr = tempSingleStr + second;
	if (IsSpecialDouble(tempDoubleStr))
		return _pEatSpecialTwoChar;
	if (IsSpecialSingle(match))
		return _pEatSpecialOneChar;
	return nullptr;
}

/*----------------method to check for comments------------------*/
ConsumeState* ConsumeState::checkForComments(int chNext) {
	if (currChar == '/' && chNext == '/') {
		testLog("state: eatCppComment");
		return _pEatCppComment;
	}
	else if (currChar == '/' && chNext == '*') {
		testLog("state: eatCComment");
		return _pEatCComment;
	}
	else
		return nullptr;
}

/*----------------method to append new single special characters set------------------*/
void ConsumeState::setSingleSpecialChars(std::string singleCharStr)
{
	size_t position = 0;
	std::string delimiter = ",";
	std::string token;
	while ((position = singleCharStr.find(delimiter)) != std::string::npos) {
		token = singleCharStr.substr(0, position);
		std::vector<std::string>::iterator it;
		it = find(singleCharVector.begin(), singleCharVector.end(), token);
		if (it != singleCharVector.end())
			continue;
		singleCharVector.push_back(token);
		singleCharStr.erase(0, position + delimiter.length());
	}
}

/*----------------method to append new double special characters set------------------*/
void ConsumeState::setDoubleSpecialChars(std::string doubleCharStr)
{
	size_t position = 0;
	std::string delimiter = ",";
	std::string token;
	while ((position = doubleCharStr.find(delimiter)) != std::string::npos) {
		token = doubleCharStr.substr(0, position);
		std::vector<std::string>::iterator it;
		it = find(doubleCharVector.begin(), doubleCharVector.end(), token);
		if (it != doubleCharVector.end())
			continue;
		doubleCharVector.push_back(token);
		doubleCharStr.erase(0, position + delimiter.length());
	}
}

/*----------------method to decide state and return its object------------------*/
ConsumeState* ConsumeState::checkState(int chNext) {
	if (std::isspace(currChar) && currChar != '\n') {
		testLog("state: eatWhitespace");
		return _pEatWhitespace;
	}
	ConsumeState* commentType = checkForComments(chNext);
	if (commentType != nullptr)
		return commentType;
	ConsumeState* isSpecialChar = checkSpecialChars();
	if (isSpecialChar != nullptr)
		return isSpecialChar;
	if ((currChar == '\n') || (currChar == '\\' && _pIn->peek() == 'n')) {
		testLog("state: eatNewLine");
		return _pEatNewline;
	}
	if (std::isalnum(currChar)) {
		testLog("state: eatAlphanum");
		return _pEatAlphanum;
	}
	if ((currChar == doubleQuote)) {
		testLog("state: eatDoubleQuote");
		return _pQuotedString;
	}
	if ((currChar == '\'')) {
		testLog("state: eatSingleQuote");
		return _pSingleQuotedString;
	}
	if (ispunct(currChar) && currChar != ':') {
		testLog("state: eatPunctuator");
		return _pEatPunctuator;
	}
	if (!_pIn->good()) {
		//std::cout << "\n  end of stream with currChar = " << currChar << "\n\n";
		return _pEatWhitespace;
	}
	throw(std::logic_error("invalid type"));
}

/*----------------method to check single special character------------------*/
bool ConsumeState::IsSpecialSingle(std::string match)
{
	std::vector<std::string>::iterator iterator;
	iterator = find(singleCharVector.begin(), singleCharVector.end(), match);
	if (iterator != singleCharVector.end()) {
		return true;
	}
	return false;
}

/*----------------method to check double special character------------------*/
bool ConsumeState::IsSpecialDouble(std::string tempDoubleStr)
{
	std::vector<std::string>::iterator itDouble = std::find(doubleCharVector.begin(), doubleCharVector.end(), tempDoubleStr);
	if (itDouble != doubleCharVector.end()) {
		return true;
	}
	return false;
}

/*----------------method to get the next state - uses checkState function------------------*/
ConsumeState* ConsumeState::nextState()
{
	if (!(_pIn->good())) {
		return nullptr;
	}
	int chNext = _pIn->peek();
	if (chNext == EOF) {
		/*if peek() reads end of file character, EOF, then eofbit is set and
		_pIn->good() will return false.  clear() restores state to good*/
		_pIn->clear();
	}
	return checkState(chNext);
}

/*----------------eatChars() -Method to eat whitespace- EatWhitespace class------------------*/
class EatWhitespace : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating whitespace";
		do {
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (std::isspace(currChar) && currChar != '\n');
	}
};

/*----------------eatChars() -Method to eat cpp comments- EatCppComment class------------------*/
class EatCppComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating C++ comment";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '\n');
		if (!getShowComments())
			token.clear();
	}
};

/*----------------eatChars() -Method to eat c comments- EatCComment class------------------*/
class EatCComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating C comment";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '*' || _pIn->peek() != '/');
		token += '*'; token += '/'; //populates end of comment in token
		if (!getShowComments())
			token.clear();
		_pIn->get();
		currChar = _pIn->get();
	}
};

/*----------------eatChars() -Method to eat punctuator- EatPunctuator class------------------*/
class EatPunctuator : public ConsumeState
{
private:
	int flag = 0;
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  Inside eat punctuator";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
			std::vector<std::string>::iterator iterator;
			std::string match(1, (char)currChar);
			iterator = find(singleCharVector.begin(), singleCharVector.end(), match);
			if (iterator != singleCharVector.end())
				flag = 1;
		} while (ispunct(currChar) && flag != 1 && currChar != '\'');
	}
};

/*----------------eatChars() -Method to eat alphanumeric chars- EatAlphanum class------------------*/
class EatAlphanum : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isalnum(currChar));
	}
};

/*----------------eatChars() -Method to eat doublequote chars- EatDoubleQuote class------------------*/
class EatDoubleQuote : public ConsumeState {
public:
	virtual void eatChars()
	{
		token.clear();
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		if (currChar == doubleQuote) {
			currChar = _pIn->get();
			return;
		}
		do {
			if (currChar == '\\' && _pIn->peek() == doubleQuote) {
				currChar = _pIn->get();
				token += currChar;
				currChar = _pIn->get();
				continue;
			}
			if (currChar == '\\' && _pIn->peek() == '\\') {
				token += currChar;
				currChar = _pIn->get();
			}
			if (currChar == '\\' && _pIn->peek() == '\'') {
				currChar = _pIn->get();
				token += currChar;
				currChar = _pIn->get();
				continue;
			}
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != doubleQuote);
		currChar = _pIn->get();
	}
};

/*----------------eatChars() -Method to eat singlequote chars- EatSingleQuote class------------------*/
class EatSingleQuote : public ConsumeState {
public:
	virtual void eatChars()
	{
		token.clear();
		if (!_pIn->good())
			return;
		currChar = _pIn->get();
		if (currChar == '\'') {
			currChar = _pIn->get();
			return;
		}
		if (currChar == '\\') {
			currChar = _pIn->get();
			token += currChar;
			currChar = _pIn->get();
			currChar = _pIn->get();
			return;
		}
		do {
			if (currChar == '\\' && _pIn->peek() == '\'') {
				currChar = _pIn->get();
				token += currChar;
				currChar = _pIn->get();
				continue;
			}
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '\'');
		currChar = _pIn->get();
	}
};

/*----------------eatChars() -Method to eat new line chars- EatNewline class------------------*/
class EatNewline : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		if (currChar == '\\')
			prevChar = currChar;
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		if (prevChar == '\\' && currChar == 'n') {
			token += currChar;
			currChar = _pIn->get();
		}
	}
};

/*----------------eatChars() -Method to eat special single chars- EatSpecialOneChar class------------------*/
class EatSpecialOneChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating single special char";
		token += currChar;
		//}
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		return;
	}
};

/*----------------eatChars() -Method to eat special two chars- EatSpecialTwoChar class------------------*/
class EatSpecialTwoChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating single special double char";
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		token += currChar;
		currChar = _pIn->get();
		return;
	}
};

/*----------------ConsumeState constructor------------------*/
ConsumeState::ConsumeState()
{
	if (first_ctor)
	{
		first_ctor = false;
		_pEatAlphanum = new EatAlphanum();
		_pEatCComment = new EatCComment();
		_pEatCppComment = new EatCppComment();
		_pEatPunctuator = new EatPunctuator();
		_pEatWhitespace = new EatWhitespace();
		_pEatNewline = new EatNewline();
		_pQuotedString = new EatDoubleQuote();
		_pEatSpecialOneChar = new EatSpecialOneChar();
		_pEatSpecialTwoChar = new EatSpecialTwoChar();
		_pSingleQuotedString = new EatSingleQuote();
		_pState = _pEatWhitespace;
	}
}

/*----------------ConsumeState destructor------------------*/
ConsumeState::~ConsumeState()
{
	if (first_dtor)
	{
		first_dtor = false;
		delete _pEatAlphanum;
		delete _pEatCComment;
		delete _pEatCppComment;
		delete _pEatPunctuator;
		delete _pEatWhitespace;
		delete _pEatNewline;
		delete _pQuotedString;
		delete _pEatSpecialOneChar;
		delete _pEatSpecialTwoChar;
		delete _pSingleQuotedString;
	}
}

/*----------------Toker constructor------------------*/
Toker::Toker() : pConsumer(new EatWhitespace()) {}

/*----------------Toker destructor------------------*/
Toker::~Toker() { delete pConsumer; }

/*----------------Method to attach stream to ConsumeState object------------------*/
bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		return true;
	}
	return false;
}

/*----------------Method to get tokens------------------*/
std::string Toker::getTok()
{
	while (true)
	{
		if (!pConsumer->canRead())
			return "";
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;
	}
	return pConsumer->getTok();
}

/*----------------Method to check if stream is good------------------*/
bool Toker::canRead() { return pConsumer->canRead(); }

/*----------------Method to reset first_ctor and first_dtor booleans------------------*/
void Toker::reset()
{
	ConsumeState::reset();
}

/*----------------Method to augment new single special char string in existing vector------------------*/
void Scanner::Toker::setSingleSpecialChars(std::string singleCharStr)
{
	pConsumer->setSingleSpecialChars(singleCharStr);
}

/*----------------Method to augment new Double special char string in existing vector------------------*/
void Scanner::Toker::setDoubleSpecialChars(std::string doubleCharStr)
{
	pConsumer->setDoubleSpecialChars(doubleCharStr);
}

/*----------------Method to set showComments flag------------------*/
void Toker::setShowComments(bool value)
{
	pConsumer->setShowComments(value);
}

/*----------------Method to print test logs------------------*/
void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	//std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	//std::string fileSpec = "../Tokenizer/Tokenizer.h";
	std::string fileSpec = "../Tokenizer/Test.txt";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.attach(&in);
	do
	{
		std::string tok = toker.getTok();
		if (tok == "\n") {
			tok = "newline";
		}
		std::cout << "\n -- " << tok;
	} while (in.good());

	std::cout << "\n\n";
	return 0;
}
#endif
