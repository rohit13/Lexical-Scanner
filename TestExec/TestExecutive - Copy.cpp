//////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Test Executive for Lexical Analyzer           //
// Ver 1.0		Calls Tokernizer and SemiExp Modules				 //
// Language:    C++, Visual Studio 2015                              //
// Application: Lexical Analyzer, CSE687 - Object Oriented Design    //
// Author:      Rohit Sharma, SUID-242093353						 //
//				Syracuse University									 //
//              rshar102@syr.edu									 //
///////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_DEPRECATE
#include "TestExecutive.h"
#include<iostream>
#include<fstream>

using namespace Scanner;

Toker* TestExecutive::_toker = nullptr;
SemiExp* TestExecutive::_semiExp = nullptr;

Scanner::TestExecutive::TestExecutive()
{
}

Scanner::TestExecutive::~TestExecutive()
{
}

Toker * Scanner::TestExecutive::getTokerObj()
{
	return _toker;
}

void Scanner::TestExecutive::setTokerObj(Toker * toker)
{
	_toker = toker;
}

SemiExp * Scanner::TestExecutive::getSemiExpObj()
{
	return _semiExp;
}

void Scanner::TestExecutive::setSemiExpObj(SemiExp* semiExp)
{
	_semiExp = semiExp;
}

void Scanner::TestExecutive::req1Demo()
{
	decorator("Demonstrating Requirement 1");
	std::cout << "Using Visual Studio 2015 and its C++ Windows Console Projects, as provided in the ECS computer labs." << endl;
	std::cout << "To verify check .sln (solution file) for the project." << endl;
}

void Scanner::TestExecutive::req2Demo()
{
	decorator("Demonstrating Requirement 2");
	std::cout << "Using the C++ standard library's streams for all I/O and new and delete for all heap-based memory management." << endl;
}

void Scanner::TestExecutive::req3Demo()
{
	decorator("Demonstrating Requirement 3");
	std::cout << "Provided C++ packages (Tokenizer, SemiExp, and TestExec) for Tokenizing, collecting SemiExpressions,"
		"and requirement demonstration respectively.\nA scanner interface, ITokCollection is implemented by SemiExp header file."
		"\nCheck the code for verification." << endl;
}

void Scanner::TestExecutive::req4Demo()
{
	decorator("Demonstrating Requirement 4");
	ifstream inputFile("../Inputs/Req4.txt");
	if (!inputFile.good())
	{
		std::cout << "\n  can't open " << "Req4.txt" << "\n\n";
	}
	getTokerObj()->attach(&inputFile);
	FILE *file = fopen("../Outputs/Tokens_Req4.txt", "w");
	do
	{
		std::string tok = getTokerObj()->getTok();
		if (tok == "\n") {
			tok = "newline";
		}
		std::cout << "\n -- " << tok;
		fputs(("\n -- "+tok).c_str(),file);
	} while (!inputFile.eof());
	fclose(file);
	setTokerObj(nullptr);
}

void Scanner::TestExecutive::req5Demo()
{
	decorator("Demonstrating Requirement 5");
	std::cout << "The Toker class, contained in the Tokenizer package is producing one token for each call to a member function getTok().\n"
		"Check code and output file for requirement 4 for verification."<< endl;
}

void Scanner::TestExecutive::req6Demo()
{
	decorator("Demonstrating Requirement 6");
	std::cout << "Provided a SemiExpression package that contains a class SemiExp used to retrieve collections of tokens by calling Toker::getTok()"
		"repeatedly until one of the \nSemiExpression termination conditions is satisfied.\n"
		"Check line number 113 of SemiExp.cpp"<< endl;
}

void Scanner::TestExecutive::req7n8Demo()
{
	decorator("Demonstrating Requirement 7 and 8");
	ifstream inputFile("../Inputs/Req7_8.txt");
	getSemiExpObj()->showSemiExpression(getTokerObj(), &inputFile);
	setTokerObj(nullptr);
}

void Scanner::TestExecutive::req9Demo() {
	decorator("Demonstrating Requirement 9");
	std::cout << "The SemiExp class implemented the interface ITokenCollection with a declared method get().\n"
		"Check Line 60 and 67 in SemiExp.h file." << endl;
}

void Scanner::TestExecutive::req10Demo() {
	decorator("Demonstrating Requirement 10");
	std::cout << "An automated unit test suite is provided for all of the special cases \nthat seem appropriate for these two packages"
		"Check output files at ..\\Project1\\Outputs folder"<< endl;
}

void Scanner::TestExecutive::decorator(string str) {
	std::cout << endl << "************************" << str << "************************" << endl;
}

int main() {
	Toker toker1;
	SemiExp semi;
	TestExecutive::setTokerObj(&toker1);
	TestExecutive::setSemiExpObj(&semi);
	TestExecutive::req1Demo();
	TestExecutive::req2Demo();
	TestExecutive::req3Demo();
	TestExecutive::req4Demo();
	TestExecutive::req5Demo();
	TestExecutive::req6Demo();
	Toker::reset();
	Toker toker2;
	TestExecutive::setTokerObj(&toker2);
	TestExecutive::req7n8Demo();
	TestExecutive::req9Demo();
	TestExecutive::req10Demo();
	return 0;
}