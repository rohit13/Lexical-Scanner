//////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Test Executive for Lexical Analyzer           //
// Ver 1.0		Calls Tokernizer and SemiExp Modules				 //
// Language:    C++, Visual Studio 2015                              //
// Application: Lexical Analyzer, CSE687 - Object Oriented Design    //
// Author:      Rohit Sharma, SUID-242093353						 //
//				Syracuse University									 //
//              rshar102@syr.edu									 //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public TestExecutive class.
* This file is the starting point for the project. Executes requirement demonstration.
* TestExecutive provides methods for requirements demonstration.
* TestExecutive provides getters and setters for Toker and SemiExpression objects.
*
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp, SemiExp.h, SemiExp.cpp
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Feb 2016
* - first release of Lexical Analyzer
*  with new design using TestExec
*
* Planned Additions and Changes:
* ------------------------------
*/
#define _CRT_SECURE_NO_DEPRECATE
#include "TestExecutive.h"
#include<iostream>
#include<fstream>

using namespace Scanner;

Toker* TestExecutive::_toker = nullptr;
SemiExp* TestExecutive::_semiExp = nullptr;
bool TestExecutive::storeOutput = false;
bool TestExecutive::displayComments = false;

/*----------------Test Executive Constructor------------------*/
Scanner::TestExecutive::TestExecutive()
{//No Implementation
}

/*----------------Test Executive Destructor------------------*/
Scanner::TestExecutive::~TestExecutive()
{//No Implementation
}

/*----------------Method to get Toker object------------------*/
Toker * Scanner::TestExecutive::getTokerObj()
{
	return _toker;
}

/*----------------Method to set Toker object------------------*/
void Scanner::TestExecutive::setTokerObj(Toker * toker)
{
	_toker = toker;
}

/*----------------Method to get SemiExpression object------------------*/
SemiExp * Scanner::TestExecutive::getSemiExpObj()
{
	return _semiExp;
}

/*----------------Method to set SemiExpression object------------------*/
void Scanner::TestExecutive::setSemiExpObj(SemiExp* semiExp)
{
	_semiExp = semiExp;
}

/*----------------Demo method for requirement 1------------------*/
void Scanner::TestExecutive::req1Demo()
{
	titleDecorator("Demonstrating Requirement 1");
	std::cout << "Using Visual Studio 2015 and its C++ Windows Console Projects, as provided in the ECS computer labs." << endl;
	std::cout << "To verify check .sln (solution file) for the project." << endl;
}

/*----------------Demo method for requirement 2------------------*/
void Scanner::TestExecutive::req2Demo()
{
	titleDecorator("Demonstrating Requirement 2");
	std::cout << "Using the C++ standard library's streams for all I/O and new and delete for all heap-based memory management." << endl;
}

/*----------------Demo method for requirement 3------------------*/
void Scanner::TestExecutive::req3Demo()
{
	titleDecorator("Demonstrating Requirement 3");
	std::cout << "Provided C++ packages (Tokenizer, SemiExp, and TestExec) for Tokenizing, collecting SemiExpressions,"
		"and requirement demonstration respectively.\nA scanner interface, ITokCollection is implemented by SemiExp header file."
		"\nCheck the code (Line 67 in SemiExp.h) for verification." << endl;
}

/*----------------Demo method for requirement 4------------------*/
void Scanner::TestExecutive::req4Demo()
{
	titleDecorator("Demonstrating Requirement 4");
	ifstream inputFile("./Inputs/Req4.txt");
	getSemiExpObj()->showTokens(getTokerObj(),&inputFile,getStoreInFile(),getShowComments(),1);
	setTokerObj(nullptr);
}

/*----------------Demo method for requirement 5------------------*/
void Scanner::TestExecutive::req5Demo()
{
	titleDecorator("Demonstrating Requirement 5");
	std::cout << "The Toker class, contained in the Tokenizer package is producing one token for each call to a member function getTok().\n"
		"Check code (Line 572-getTok method in Tokenizer.cpp) and output file for requirement 4 for verification."<< endl;
}

/*----------------Demo method for requirement 6------------------*/
void Scanner::TestExecutive::req6Demo()
{
	titleDecorator("Demonstrating Requirement 6");
	std::cout << "Provided a SemiExpression package that contains a class SemiExp used to retrieve collections of tokens by calling Toker::getTok()"
		"repeatedly until one of the SemiExpression termination conditions is satisfied.\n"
		"Check code (Line 175-get method of SemiExp.cpp)."<< endl;
}

/*----------------Demo method for requirement 7 and 8------------------*/
void Scanner::TestExecutive::req7n8Demo()
{
	titleDecorator("Demonstrating Requirement 7 and 8");
	ifstream inputFile("./Inputs/Req7_8.txt");
	getSemiExpObj()->showSemiExpression(getTokerObj(), &inputFile,getStoreInFile());
	setTokerObj(nullptr);
}

/*----------------Demo method for requirement 9------------------*/
void Scanner::TestExecutive::req9Demo() {
	titleDecorator("Demonstrating Requirement 9");
	std::cout << "The SemiExp class implemented the interface ITokenCollection with a declared method get().\n"
		"Check Line 67 and 72 in SemiExp.h file." << endl;
}

/*----------------Demo method for requirement 10------------------*/
void Scanner::TestExecutive::req10Demo() {
	titleDecorator("Demonstrating Requirement 10");
	std::cout << "An automated unit test suite is provided for all of the special cases \nthat seem appropriate for these two packages"
		"Check output files at Home directory i.e. Project1 folder. Instructions in Readme.txt"<< endl;
}

/*----------------Method to demonstrate handling of adding new single special chars and double special chars in the existing vectors------------------*/
void Scanner::TestExecutive::reqSpecialCharDemo() {
	titleDecorator("Demonstrating Special Requirement - Single special char");
	std::cout << "Single special character vector is augmented with new special characters @ and $.\n Test will run on input file ReqSpecialChars.txt "
		"kept in the Inputs folder" << endl;
	std::cout << "Output file is generated at home directory with name SingleSpecialCharReq_Output.txt" << endl;
	ifstream inputFile1("./Inputs/ReqSpecialChars.txt");
	getTokerObj()->setSingleSpecialChars("@,$");
	getSemiExpObj()->showTokens(getTokerObj(), &inputFile1, true, false, 2);

	titleDecorator("Demonstrating Special Requirement - Double special char");
	std::cout << "Double special character vector is augmented with new special characters @@ and $$.\n Test will run on input file ReqSpecialChars.txt "
		"kept in the Inputs folder" << endl;
	std::cout << "Output file is generated at home directory with name DoubleSpecialCharReq_Output.txt" << endl;
	std::cout << "NOTE: Input file is same for both the cases." << endl;
	ifstream inputFile2("./Inputs/ReqSpecialChars.txt");
	getTokerObj()->setDoubleSpecialChars("@@,$$");
	getSemiExpObj()->showTokens(getTokerObj(), &inputFile2, true, false, 3);
	setTokerObj(nullptr);
}

/*----------------Method to decorate title for each requirement demo method------------------*/
void Scanner::TestExecutive::titleDecorator(string str) {
	std::cout << endl << "************************" << str << "************************" << endl;
}

/*----------------Method to set command line arguments to storeInFile and showComments Flag------------------*/
void TestExecutive::setArgs(int arg, char** argv)
{
	std::string isStore(argv[1]);     //set store flag to true from argument 1
	if (isStore == "y")
		setStoreInFile(true);
	else
		setStoreInFile(false);

	std::string showComment(argv[2]); // set comment flag
	if (showComment == "y")
		setShowComments(true);
	else
		setShowComments(false);
}

/*----------------Test Executive Main method - start of the program------------------*/
int main(int arg, char** argv) {
	Toker toker1;
	SemiExp semi;
	TestExecutive::setArgs(arg, argv);
	TestExecutive::setTokerObj(&toker1);
	TestExecutive::setSemiExpObj(&semi);
	TestExecutive::req1Demo();
	TestExecutive::req2Demo();
	TestExecutive::req3Demo();
	TestExecutive::req4Demo();
	TestExecutive::req5Demo();
	TestExecutive::req6Demo();
	Toker::reset();//reset ConsumeState bools first_ctor and first_dtor to true
	Toker toker2;
	TestExecutive::setTokerObj(&toker2);
	TestExecutive::req7n8Demo();
	TestExecutive::req9Demo();
	TestExecutive::req10Demo();
	Toker::reset();
	Toker toker3;
	TestExecutive::setTokerObj(&toker3);
	TestExecutive::reqSpecialCharDemo();
	return 0;
}