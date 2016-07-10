/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.2                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////
/*
  Helper code that does not attempt to handle all tokenizing
  special cases like escaped characters.
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
    ConsumeState* nextState();
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
  };
}

using namespace Scanner;

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
const char ConsumeState::doubleQuote = '"';
std::vector<std::string> singleVec = { "<", ">","[","]", "(",")","{" , "}", ":", "=", "+", "-", "*", "\n" };
std::vector<std::string> doubleVec = { "<<" , ">>" , "::", "++", "--", "==" , "+=", "-=", "*=", "/=" };
std::string temp1;
std::string temp2;

void testLog(const std::string& msg);

ConsumeState* ConsumeState::nextState()
{
  if (!(_pIn->good()))
  {
    return nullptr;
  }
  int chNext = _pIn->peek();
  if (chNext == EOF)
  {
    _pIn->clear();
    // if peek() reads end of file character, EOF, then eofbit is set and
    // _pIn->good() will return false.  clear() restores state to good
  }
  if (std::isspace(currChar) && currChar != '\n')
  {
    testLog("state: eatWhitespace");
    return _pEatWhitespace;
  }
  if (currChar == '/' && chNext == '/')
  {
    testLog("state: eatCppComment");
    return _pEatCppComment;
  }

  if (currChar == '/' && chNext == '*')
  {
    testLog("state: eatCComment");
    return _pEatCComment;
  }
	std::vector<std::string>::iterator it;

  std::string match(1, (char)currChar);

  it = find(singleVec.begin(), singleVec.end(), match);

  if (it != singleVec.end())
  {
	  temp1 = match;
	  std::string second(1, (char)_pIn->peek());
	  //std::cout << "\nTemp1" << second;
	  temp2 = temp1 + second;
	  //std::cout << "\nINSIDE"<< temp2;
	  std::vector<std::string>::iterator itd = std::find(doubleVec.begin(), doubleVec.end(), temp2);
	  if (itd != doubleVec.end()) {
		  //std::cout << "Double Char";
		  return _pEatSpecialTwoChar;
	  }
	  else {
		  //std::cout << "Single char";
		  return _pEatSpecialOneChar;
	  }
  }

  if (currChar == '\n')
  {
    testLog("state: eatNewLine");
    return _pEatNewline;
  }
  if (std::isalnum(currChar))
  {
    testLog("state: eatAlphanum");
    return _pEatAlphanum;
  }
  if ((currChar == '"')) {
	  testLog("state: eatDoubleQuote");
	  return _pQuotedString;
  }
  if (ispunct(currChar))
  {
    testLog("state: eatPunctuator");
    return _pEatPunctuator;
  }

  if (!_pIn->good())
  {
    //std::cout << "\n  end of stream with currChar = " << currChar << "\n\n";
    return _pEatWhitespace;
  }
  //std::vector<std::string>::iterator it;

  //std::string match(1, (char)currChar);
  //std::cout << "\nMatch ::" << match;

  throw(std::logic_error("invalid type"));
}

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

class EatCppComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating C++ comment";
    do {
		if (currChar != '/') {
			token += currChar;
		}
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (currChar != '\n');
  }
};

class EatCComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating C comment";
    do {
		if (currChar != '*' && currChar != '/') {
			token += currChar;
		}
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (currChar != '*' || _pIn->peek() != '/');
    _pIn->get();
    currChar = _pIn->get();
  }
};

class EatPunctuator : public ConsumeState
{
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
    } while (ispunct(currChar));
  }
};

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

class EatDoubleQuote : public ConsumeState {
public:
	virtual void eatChars() {
		token.clear();
		int count = 0;
		do {
			/*if (currChar == '"')
			{
				currChar = _pIn->get();
				return;
			}*/
			//char charac = (char)currChar;
			if (currChar != '\\' && currChar != '"') {
				token += currChar;
			}
			else if (currChar=='\\' && _pIn->peek()=='n') {
				token += currChar;
			}
			else if (currChar == '"') {
				count += 1;
				if (prevChar == '\\' && currChar == '"') {
					token = token + "\"";
					count -= 1;
				}
			}
			if (!_pIn->good())
					return;
			prevChar = currChar;
			currChar = _pIn->get();
		} while (count!=2);
			//std::cout << "Inside eat double quote";
			//char charac = (char) currChar;
		//	if (currChar != '"') {
		//		if (currChar == '\\' && _pIn->peek() == '"') {
		//			currChar = _pIn->get();
		//			while (currChar == '"') {
		//				currChar = _pIn->get();
		//			}
		//			token = token+"\""+(char)currChar;
		//		}
		//		else {
		//			//if (currChar != '\\') {
		//			token += currChar;
		//		}
		//		//take prev char
		//	}
		//	if (!_pIn->good())
		//		return;
		//	currChar = _pIn->get();
		//} while (currChar!= '"'); //check starting quotes here..
		//update prev and curr char
	}
};

class EatNewline : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating alphanum";
    token += currChar;
    if (!_pIn->good())  // end of stream
      return;
    currChar = _pIn->get();
  }
};


class EatSpecialOneChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating whitespace";
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		return;
	}
};

class EatSpecialTwoChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating whitespace";

		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		token += currChar;
		currChar = _pIn->get();
		return;
	}
};

ConsumeState::ConsumeState()
{
  static bool first = true;
  if (first)
  {
    first = false;
    _pEatAlphanum = new EatAlphanum();
    _pEatCComment = new EatCComment();
    _pEatCppComment = new EatCppComment();
    _pEatPunctuator = new EatPunctuator();
    _pEatWhitespace = new EatWhitespace();
    _pEatNewline = new EatNewline();
	_pQuotedString = new EatDoubleQuote();
	_pEatSpecialOneChar = new EatSpecialOneChar();
	_pEatSpecialTwoChar = new EatSpecialTwoChar();
    _pState = _pEatWhitespace;
  }
}

ConsumeState::~ConsumeState()
{
  static bool first = true;
  if (first)
  {
    first = false;
    delete _pEatAlphanum;
    delete _pEatCComment;
    delete _pEatCppComment;
    delete _pEatPunctuator;
    delete _pEatWhitespace;
    delete _pEatNewline;
	delete _pQuotedString;
	delete _pEatSpecialOneChar;
	delete _pEatSpecialTwoChar;
  }
}

Toker::Toker() : pConsumer(new EatWhitespace()) {}

Toker::~Toker() { delete pConsumer; }

bool Toker::attach(std::istream* pIn)
{
  if (pIn != nullptr && pIn->good())
  {
    pConsumer->attach(pIn);
    return true;
  }
  return false;
}

std::string Toker::getTok()
{
  while(true) 
  {
    if (!pConsumer->canRead())
      return "";
    pConsumer->consumeChars();
    if (pConsumer->hasTok())
      break;
  }
  return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

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
