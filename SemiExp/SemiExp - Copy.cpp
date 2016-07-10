///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.1                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
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

size_t SemiExp::find(const std::string& tok) {
	for (size_t i = 0; i < length(); ++i)
		if (tok == _tokens[i])
			return i;
	return length();
}

bool SemiExp::findFor(const std::string& tok) {
	for (size_t i = 0; i < length(); ++i)
		if (tok == _tokens[i])
			return true;
	return false;
}

bool SemiExp::merge(const std::string & firstTok, const std::string & secondTok)
{
	size_t first = find(firstTok);
	size_t second = find(secondTok);
	if (first < length() && second < length())
	{
		for (size_t i = first + 1; i < second; ++i)
			(*this)[first] += (*this)[i];
		for (size_t i = first + 1; i < second; ++i)
			remove(first + 1);
		return true;
	}
	return false;
}

void SemiExp::push_back(const std::string & tok)
{
}

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

bool SemiExp::remove(size_t i)
{
	if (i < 0 || _tokens.size() <= i)
		return false;
	std::vector<std::string>::iterator it = _tokens.begin();
	_tokens.erase(it + i);
	return true;
}

void SemiExp::toLower()
{
	for (size_t i = 0; i < length(); ++i)
	{
		for (size_t j = 0; j < (*this)[i].length(); ++j)
		{
			(*this)[i][j] = tolower((*this)[i][j]);
		}
	}
}

void SemiExp::trimFront()
{
	while (_tokens.size() > 0 && (_tokens[0] == "\n" || _tokens[0] == ""))
		remove(0);
}

void SemiExp::clear()
{
}

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {
}

bool SemiExp::get(bool clear)
{
	int flag = 0;
	int flag2 = 0;
	bool flag3 = false;
	string prevChar = "";
	int semiColonCount = 0;
	if (clear) {
		_tokens.clear();
	}
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	_tokens.clear();
	while (true)
	{
		std::string token = _pToker->getTok();
		if (token == "")
			return false;
		_tokens.push_back(token);
		if (token == "{" || token == "}" || token == ";")
			return true;
		if (token == "#")
			flag = 1;
		if (token == "\n" && flag == 1)
			return true; 
		if ((token.at(0) == '/' && token.at(1) == '/') || (token.at(0) == '/' && token.at(1) == '*')) {
			token = _pToker->getTok();
			_tokens.push_back(token);
			return true;
		}
		if (token == "public" || token == "protected" || token == "private") {
			flag2 = 1;
		}
		if (token == ":" && flag2 == 1) {
			return true;
		}
		if (findFor("for") == true) {
			while (true) {
				token = _pToker->getTok();
				_tokens.push_back(token);
				if (token == ";")
					semiColonCount += 1;
				if (token == "{" || semiColonCount == 3)
				{
					return true;
				}
			}
		}
	}
	return false;
}

std::string & SemiExp::operator[](int n)
{
	if (n < 0 || n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}


Token SemiExp::operator[](size_t n)
{
	if (n < 0 || n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}

size_t SemiExp::length()
{
	return _tokens.size();
}

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

void Scanner::SemiExp::showSemiExpression(Toker* toker, ifstream* inputFile)
{
	if (!inputFile->good())
	{
		std::cout << "\n  can't open " << "Req7_8.txt" << "\n\n";
	}
	toker->attach(inputFile);
	_pToker = toker;
	FILE *file = fopen("../Outputs/Semi_Req7_8.txt", "w");
	while (get(true))
	{
		std::cout << "\n  -- semiExpression --";
		string semiExp = show();
		fputs(("\n  -- semiExpression --" + semiExp).c_str(), file);
	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (length() > 0)
	{
		std::cout << "\n  -- semiExpression --";
		string semiExp1 = show();
		fputs(("\n  -- semiExpression --" + semiExp1).c_str(), file);
		std::cout << "\n\n";
	}
	fclose(file);
}

//bool SemiExp::PrintSemiExps(Toker* toker, std::string fileSpec, bool storeFlag)
//{
//	clear();
//	std::string out = "../Output/Req7_8Result.txt";
//	FILE *fptr = nullptr;
//	if (storeFlag)
//		fptr = freopen(out.c_str(), "w", stdout);
//
//	std::fstream in(fileSpec);
//	if (!in.good())
//	{
//		std::cout << "\n  can't open file " << fileSpec << "\n\n";
//		return 1;
//	}
//	toker->attach(&in);
//	_pToker = toker;
//	//SemiExp semi(toker);
//	while (get())
//	{
//		std::cout << "\n  -- semiExpression --";
//		show();
//	}
//	/*
//	May have collected tokens, but reached end of stream
//	before finding SemiExp terminator.
//	*/
//	if (length() > 0)
//	{
//		std::cout << "\n  -- semiExpression --";
//		show();
//		std::cout << "\n\n";
//	}
//	if (storeFlag)
//	{
//		fptr = ("Out", "w", stdout);
//		std::cout << "Output file is generated at ../Output directory as Req4Result.txt";
//	}
//	return true;
//}

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