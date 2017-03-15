#include "../debug/treap.hpp"
#ifdef FILE_OUTPUT 
	#include <fstream>
#endif		
#include <iostream>

//#define TEST_ERASE 1
#define TEST_REMOVE 1

//#define FILE_INPUT 1
//#define FILE_OUTPUT 1
//#define SET_VS_TREAP 1
//#define CASUAL 1
//#define STATS 1

#ifdef DEBUG 
	#define TEST_WAS 1
	#define K_MAX_TEST 0
#endif		
#ifdef STATS 
	#define TEST_DELETED_COUNT 1
#endif		

using namespace bst;

#ifdef FILE_INPUT 
	const char* in_name_file = {"input.txt"};
#else
	#include <string>
	#include <vector>
	#include <random>
	#include <ctime>
#endif		
#ifdef CASUAL 
const char* out_name_file = {"output.txt"};
#endif		
#ifdef SET_VS_TREAP 
const char* out_name_file = {"output_treap.txt"};
#endif		


#ifdef FILE_OUTPUT 
inline void caller(const std::string & op, const int number, std::ofstream & fout, treap<int> & tree)
#else
inline void caller(const std::string & op, const int number, treap<int> & tree)
#endif		
{
	if (op == "insert" 
#ifdef K_MAX_TEST
		|| op == "1" 
#endif		
		)
	{
		tree.insert(number);
		return;
	}

	if (op == "delete"
#ifdef K_MAX_TEST
		|| op == "-1" 
#endif		
		)
	{
		if (tree.find(number))
		{
#ifdef TEST_REMOVE 
			tree.remove(number);
#endif		
#ifdef TEST_ERASE 
			tree.erase(number);
#endif		
		}
		return;
	}

	if (op == "exists")
	{
		if (tree.find(number))
		{
#ifdef DEBUG 
			std::cout << " true : ";
#endif		
#ifdef FILE_OUTPUT 
			fout << "true\n";
#endif		
		} else {
#ifdef DEBUG 
			std::cout << " false : ";
#endif		
#ifdef FILE_OUTPUT 
			fout << "false\n";
#endif		
		}
#ifdef TEST_WAS
		if (tree.was(number))
		{
#ifdef DEBUG 
			std::cout << " WAS : ";
#endif		
		} else {
#ifdef DEBUG 
			std::cout << " NOT WAS : ";
#endif		
		}
#endif		
		return;
	}

	if (op == "next")
	{

		if (!tree.size()) {
#ifdef DEBUG 
			std::cout << " none : ";
#endif		
#ifdef FILE_OUTPUT 
			fout << "none\n";
#endif		
			return;
		} 

		int tmp = tree.upper_bound(number);
		if (tree.get_success())
		{
#ifdef DEBUG 
			std::cout << " " << tmp << " : ";
#endif		
#ifdef FILE_OUTPUT 
			fout << tmp << "\n";
#endif		
		} else {
#ifdef DEBUG 
			std::cout << " none : ";
#endif		
#ifdef FILE_OUTPUT 
			fout << "none\n";
#endif		
		}
		return;
	}


	if (op == "prev")
	{
		if (!tree.size()) {
#ifdef DEBUG 
			std::cout << " none : ";
#endif		
#ifdef FILE_OUTPUT 
			fout << "none\n";
#endif		
			return;
		} 

		int tmp = tree.previous(number);
		tree.previous(number);		
		if (tree.get_success())
		{
#ifdef DEBUG 
			std::cout << " " << tmp << " : ";
#endif		
#ifdef FILE_OUTPUT 
			fout << tmp << "\n";
#endif		
		} else {
#ifdef DEBUG 
			std::cout << " none : ";
#endif		
#ifdef FILE_OUTPUT 
			fout << "none\n";
#endif		
		}
		return;
	}

#ifdef K_MAX_TEST
	if (op == "0")
	{
		int tmp = tree.k_max(number - 1);
#ifdef DEBUG 
			std::cout << tmp;
#endif		
#ifdef FILE_OUTPUT 
			fout << tmp << "\n";
#endif		
	} 
#endif		

}


int main(
#ifndef FILE_INPUT 
	int , char const *argv[]
#endif		
	)
{
	
	treap<int> my_tree;

#ifdef FILE_INPUT 
	std::ifstream fin;
	fin.open(in_name_file);
#endif		

#ifdef FILE_OUTPUT 
	std::ofstream fout;
	fout.open(out_name_file);
#endif		

	int number;
	std::string buff_str;

#ifdef FAST_ALLOCATION
	std::cout << "Fast allocation enabled.\n"; 
#endif		
#ifdef FILE_INPUT 
	while(fin >> buff_str >> number)
	{
#else
	int COUNT_OPERATION = atoi(argv[1]);		
	int RANGE = atoi(argv[2]);		
	std::vector<std::string> OPERATIONS = {"delete", "exists", "next", "prev", "insert", "insert"}; 
	std::mt19937 gen;
	gen.seed(time(0));

	for (int i = 0; i < COUNT_OPERATION; ++i)
	{
		buff_str = OPERATIONS[gen() % 6];
		number = static_cast<int>(gen() % RANGE) - static_cast<int>(gen() % ((3 * RANGE) / 2));
#endif		

#ifdef DEBUG 
		std::cout << buff_str << " " << number << " : ";
#endif		
#ifdef FILE_OUTPUT 
		caller(buff_str, number, fout, my_tree);	
#else
		caller(buff_str, number, my_tree);	
#endif		

#ifdef DEBUG 
		assert(my_tree.size_n() == my_tree.size());
#ifdef K_MAX_TEST
		std::cout << " k_max=" << K_MAX_TEST << ": " << my_tree.k_max(K_MAX_TEST) << "|"
		<< my_tree.get_success() << " "; 
#endif 
#endif 

#ifdef DEBUG 
		my_tree.print();

		std::cout << "\n";
		std::cout << "\n=====" << my_tree.size() 

#ifdef TEST_DELETED_COUNT
		<< ", deleted = " << my_tree.deleted_count() 
#endif		
		<< "==============\n";
#endif		
	}

	size_t stree = my_tree.size_n();
	std::cout << "============Stats===========\n";
	std::cout << "Size node: " << sizeof(node<int>) << " byte\n";
//	std::cout << "Final size: " << my_tree.size() << "\n";
	std::cout << "Final count: " << stree << " -- " << my_tree.size() << "\n";
#ifdef TEST_DELETED_COUNT
	std::cout << "Final deleted: " << my_tree.deleted_count() 
			  <<  " aka " << (float)my_tree.deleted_count() / stree * 100 << "%\n";
#endif		
#ifdef STATS 
	std::cout << "Allocations: " << counter_allocate << " blocks, " << sizeof(node<int>) * counter_allocate << " byte memory\n";
#endif		
	std::cout << "Effective: " << sizeof(node<int>) * stree << " byte\n";
#ifdef STATS 
	std::cout << "Rate: " << (float)stree / counter_allocate * 100 << "%\n";
#endif		
	std::cout << "============================\n";

}
