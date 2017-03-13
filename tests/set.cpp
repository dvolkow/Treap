//============================================================================
// Name        : simple_bst.cpp
// Author      : Volkov Daniil
// Version     : 0.1
// University  : Saint-Petersburg State University
// Departament : The Faculty of Mathematics and Mechanics
// Course	   : Algorithms and Structure of Data
// Other	   : Computer Science Center 		  
// Description : This use binary search tree in C++ aka std::set  
//============================================================================
#include <string>
#include <iostream>
#include <fstream>
#include <set>

//#define DEBUG 1 

using namespace std;

const char* in_name_file = {"input.txt"};
const char* out_name_file = {"output_set.txt"};

set<int> my_tree;

bool exist(const int number)
{
	return my_tree.count(number);
}


void caller(const string & op, const int number, ofstream & fout)
{
	if (op == "insert")
	{
		my_tree.insert(number);
		return;
	}

	if (op == "delete")
	{
		my_tree.erase(number);
		return;
	}

	if (op == "exists")
	{
		if (exist(number))
		{
			fout << 
#ifdef DEBUG 
			"exist " << number << " : " << 
#endif		
			"true\n";
		} else {
			fout <<
#ifdef DEBUG 
			 "exist " << number << " : " << 
#endif		
			 "false\n";
		}
		return;
	}

	if (op == "next")
	{
		auto it = my_tree.upper_bound(number);
		if (it != my_tree.end())
		{
			fout <<
#ifdef DEBUG 
			 "next " << number << " : " <<
#endif		
			  *it << "\n";
		} else {
			fout <<
#ifdef DEBUG 
			 "next " << number << " : " << 
#endif		
			 "none" << "\n";
		}
		return;

	}


	if (op == "prev")
	{
		auto it = my_tree.lower_bound(number);
		//--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (!my_tree.empty() && it != my_tree.begin())
		{
			auto goal = *(--it);
			if (goal < number)
			{
				fout <<
#ifdef DEBUG 
				"prev " << number << " : " << 
#endif		
				goal << "\n";
			} else {
				fout << 
#ifdef DEBUG 
				"prev " << number << " : " << 
#endif		
				"none" << "\n";
			}
		} else {
			fout << 
#ifdef DEBUG 
			"prev " << number << " : " << 
#endif		
			"none" << "\n";
		}
	}
}

#ifdef DEBUG 
void printTree(ofstream & fout)
{
	for (auto member : my_tree)
	{
		fout << member << " ";
	}
	fout << endl;
	fout << "==========================\n";
}
#endif		

int main(int argc, char const *argv[])
{
	ifstream fin;
	fin.open(in_name_file);
	ofstream fout;
	fout.open(out_name_file);
	string buff_str;
	int number;
	while(fin >> buff_str >> number)
	{
		caller(buff_str, number, fout);	
#ifdef DEBUG 
		printTree(fout);
#endif		
	}

	return 0;
}