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
* This package provides a public TestExecutive class.
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
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"

namespace Scanner
{
	class TestExecutive
	{
	public:
		TestExecutive(); //Test Exec constructor
		~TestExecutive(); //Test Exec destructor
		static void setTokerObj(Toker* toker); //method to set object of Toker class
		static void setSemiExpObj(SemiExp* semiExp); //method to set object of SemiExp class
		/*method for requirement demonstration*/
		static void req1Demo(); 
		static void req2Demo();
		static void req3Demo(); 
		static void req4Demo(); 
		static void req5Demo(); 
		static void req6Demo(); 
		static void req7n8Demo(); 
		static void req9Demo(); 
		static void req10Demo(); 
		static void reqSpecialCharDemo();
		static void titleDecorator(string str); //method to decorate title for each requirement demo method
		static void setStoreInFile(bool storeInFile) { storeOutput = storeInFile; }
		static bool getStoreInFile() { return storeOutput; }
		static void setShowComments(bool showComments) { displayComments = showComments; }
		static bool getShowComments() { return displayComments; }
		static void setArgs(int arg, char** argv);//method to set command line arguments into flags
	private:
		static Toker* _toker; //Pointer type object of Toker class
		static SemiExp* _semiExp; //Pointer type object of SemiExp class
		static Toker* getTokerObj(); //method to get object of Toker class
		static SemiExp* getSemiExpObj(); //method to get object of SemiExp class
		static bool storeOutput; //This flag identifies if the output is to be stored in the file or not
		static bool displayComments; //This flag identifies if the comments are to be displayed or not
	};
}
#endif
#define a