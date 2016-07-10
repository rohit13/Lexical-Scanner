///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis and generates    //
//						semiexpressions out of them					 //
// ver 3.3                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Lexical Analyzer, CSE687 - Object Oriented Design    //
// Author:      Rohit Sharma, SUID-242093353						 //
//				Syracuse University, rshar102@syr.edu				 //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.
*
* Build Process:
* --------------
* Required Files:
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp,
*   TestExecutive.h, TestExecutive.cpp, itokcollection.h
*
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* Ver 3.3 :10 Feb 2016
* - defined methods for showing tokens and semiexpressions
* - defined method to detect for loop in stream
* - defined method to skip semicolons when for loop is detected
* - defined method to check access modifier to apply folding rule
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
#define _CRT_SECURE_NO_DEPRECATE
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "../TestExec/TestExecutive.h"

using namespace Scanner;

/*----------------method to find token - returns index if found - else returns length of _tokens------------------*/
size_t SemiExp::find(const std::string& token) {
	for (size_t i = 0; i < length(); ++i)
		if (token == _tokens[i])
			return i;
	return length();
}

/*----------------method to check if for exists in _tokens------------------*/
bool SemiExp::findFor(const std::string& token) {
	for (size_t i = 0; i < length(); ++i)
		if (token == _tokens[i])
			return true;
	return false;
}

/*----------------method to skip two semicolons in for loop------------------*/
bool SemiExp::skipSemiColons(std::string token) {
	int semiColonCount = 0;
	while (true) {
		token = _pToker->getTok();
		_tokens.push_back(token);
		if (token == ";")
			semiColonCount += 1;
		if (token == "{" || semiColonCount == 3)
			return true;
	}
}

/*----------------method to apply folding rule for access modifiers------------------*/
bool SemiExp::checkAccessModifier(std::string token) {
	bool returnVal = false;
	if (token == "public" || token == "protected" || token == "private")
		flag2 = true;
	if (token == ":" && flag2 == true)
		returnVal = true;
	return returnVal;
}

/*----------------method to merge two tokens into a single semiexpresssion------------------*/
bool SemiExp::merge(const std::string & firstTok, const std::string & secondTok)
{
	size_t firstToken = find(firstTok);
	size_t secondToken = find(secondTok);
	if (firstToken < length() && secondToken < length()) {
		for (size_t i = firstToken + 1; i < secondToken; ++i)
			(*this)[firstToken] += (*this)[i];
		for (size_t i = firstToken + 1; i < secondToken; ++i)
			remove(firstToken + 1);
		return true;
	}
	return false;
}

/*----------------method to push_back token into _tokens (Not Used)------------------*/
void SemiExp::push_back(const std::string & tok)
{//NO IMPLEMENTATION
}

/*----------------method to remove token from _tokens------------------*/
bool SemiExp::remove(const std::string & tok)
{
	std::vector<std::string>::iterator it;
	it = std::find(_tokens.begin(), _tokens.end(), tok);
	if (it != _tokens.end())
	{
		_tokens.erase(it);
		return true;
	}
	return false;
}

/*----------------method to remove token at a particular index------------------*/
bool SemiExp::remove(size_t i)
{
	if (i < 0 || _tokens.size() <= i)
		return false;
	std::vector<std::string>::iterator iterator = _tokens.begin();
	_tokens.erase(iterator + i);
	return true;
}

/*----------------method to lower case token (Not used)------------------*/
void SemiExp::toLower()
{//NO IMPLEMENTATION
}

/*----------------method to trim \n char from front------------------*/
void SemiExp::trimFront()
{
	while (_tokens.size() > 0 && (_tokens[0] == "\n" || _tokens[0] == ""))
		remove(0);
}

/*----------------method to clear _tokens------------------*/
void SemiExp::clear()
{//NO IMPLEMENTATION
}

/*----------------method to clear tokens on the basis of flag clear------------------*/
void SemiExp::clear(bool clear)
{
	if (clear)
		_tokens.clear();
}

/*----------------SemiExp constructor------------------*/
SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

/*----------------Method to get tokens and break them into semiexps using folding rules------------------*/
bool SemiExp::get(bool clear)
{
	bool flag1 = false;
	SemiExp::clear(clear);
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	while (true) {
		std::string token = _pToker->getTok();
		if (token == "")
			return false;
		_tokens.push_back(token);
		if (token == "{" || token == "}" || token == ";")
			return true;
		if (token == "#")
			flag1 = true;
		if (token == "\n" && flag1 == true)
			return true;
		if ((token.at(0) == '/' && token.at(1) == '/') || (token.at(0) == '/' && token.at(1) == '*'))
			return true;
		bool returnV = checkAccessModifier(token);
		if (returnV) return true;
		if (findFor("for") == true) 
			return skipSemiColons(token);
	}
	return false;
}

/*---------------- Method to check if index is out of bound------------------*/
std::string & SemiExp::operator[](int num)
{
	if (num < 0 || num >= static_cast<int> (_tokens.size()))
		throw(std::invalid_argument("index out of range"));
	return _tokens[num];
}

/*----------------Method to return size of _tokens------------------*/
size_t SemiExp::length()
{
	return _tokens.size();
}

/*----------------Method to show Semiexpression------------------*/
std::string SemiExp::show(bool showNewLines)
{
	string semiExp = "";
	std::cout << "  ";
	for (auto token : _tokens)
		if (token != "\n") {
			std::cout << token << " ";
			if (semiExp == "") {
				semiExp = token;
			}
			else {
				semiExp = semiExp + " " + token;
			}
		}
	std::cout << "\n";
	return semiExp;
}

/*----------------Method to store Semiexp in the file and display on console, uses show function------------------*/
void Scanner::SemiExp::showSemiExpression(Toker* toker, ifstream* inputFile, bool storeInFile)
{
	if (!inputFile->good())	{
		std::cout << "\n  can't open " << inputFile << "\n\n";
	}
	toker->attach(inputFile);
	_pToker = toker;
	FILE *file = (storeInFile) ? fopen(outputFileReq7_8, "w") : nullptr;
	while (get(true)) {
		std::cout << "\n  -- semiExpression --";
		string semiExp = show();
		if(storeInFile)
			fputs(("\n  -- semiExpression --" + semiExp).c_str(), file);
	}
	/* May have collected tokens, but reached end of stream
	before finding SemiExp terminator. */
	if (length() > 0) {
		std::cout << "\n  -- semiExpression --";
		string semiExp1 = show();
		if(storeInFile)
			fputs(("\n  -- semiExpression --" + semiExp1).c_str(), file);
		std::cout << "\n\n";
	}
	if(storeInFile)
		fclose(file);
}

/*----------------Method to store Tokens in the file and display on console, uses show function------------------*/
void Scanner::SemiExp::showTokens(Toker* toker, ifstream* inputFile, bool storeInFile, bool showComments, int caseId)
{
	if (!inputFile->good()) {
		std::cout << "\n  can't open " << inputFile << "\n\n";
	}
	toker->setShowComments(showComments);
	toker->attach(inputFile);
	FILE *file;
	if(caseId==1)
		file = (storeInFile) ? fopen(outputFileReq4, "w") : nullptr;
	else if(caseId==2)
		file = (storeInFile) ? fopen(outputFileReqSingleSpcl, "w") : nullptr;
	else
		file = (storeInFile) ? fopen(outputFileReqDoubleSpcl, "w") : nullptr;
	do
	{
		std::string tok = toker->getTok();
		if (tok == "\n") {
			tok = "newline";
		}
		std::cout << "\n -- " << tok;
		if (storeInFile) {
			fputs(("\n -- " + tok).c_str(), file);
		}
	} while (!inputFile->eof());
	if(storeInFile)
		fclose(file);
}

#ifdef TEST_SEMIEXP
int main()
{
	Toker toker;
	std::string fileSpec = "../Tokenizer/Test.txt";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker.attach(&in);

	SemiExp semi(&toker);
	while (semi.get(true))
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
	}
	/*
	   May have collected tokens, but reached end of stream
	   before finding SemiExp terminator.
	 */
	if (semi.length() > 0)
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
		std::cout << "\n\n";
	}
	return 0;
}
#endif