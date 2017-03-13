#include "treap.hpp"
#include <fstream>
#include <iostream>

//#define TEST_ERASE 1
#define TEST_REMOVE 1
#ifdef DEBUG 
	#define TEST_WAS 1
	#define TEST_DELETED_COUNT 1
#endif		

using namespace bst;

const char* in_name_file = {"input.txt"};
const char* out_name_file = {"output.txt"};

inline void caller(const std::string & op, const int number, std::ofstream & fout, treap<int> & tree)
{
	if (op == "insert")
	{
		if (!tree.find(number))
		{
			tree.insert(number);
		}
		return;
	}

	if (op == "delete")
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
			fout << "true\n";
		} else {
#ifdef DEBUG 
			std::cout << " false : ";
#endif		
			fout << "false\n";
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
			fout << "none\n";
			return;
		} 

		int tmp = tree.upper_bound(number);
		if (tree.get_success())
		{
#ifdef DEBUG 
			std::cout << " " << tmp << " : ";
#endif		
			fout << tmp << "\n";
		} else {
#ifdef DEBUG 
			std::cout << " none : ";
#endif		
			fout << "none\n";
		}
		return;
	}


	if (op == "prev")
	{
		if (!tree.size()) {
#ifdef DEBUG 
			std::cout << " none : ";
#endif		
			fout << "none\n";
			return;
		} 

		int tmp = tree.previous(number);
		if (tree.get_success())
		{
#ifdef DEBUG 
			std::cout << " " << tmp << " : ";
#endif		
			fout << tmp << "\n";
		} else {
#ifdef DEBUG 
			std::cout << " none : ";
#endif		
			fout << "none\n";
		}
		return;
	}
}


int main()
{
	
	treap<int> my_tree;

	std::ifstream fin;
	fin.open(in_name_file);
	std::ofstream fout;
	fout.open(out_name_file);

	std::string buff_str;
	int number;

	while(fin >> buff_str >> number)
	{
#ifdef DEBUG 
		std::cout << buff_str << " " << number << " : ";
#endif		
		caller(buff_str, number, fout, my_tree);	
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
	std::cout << "Final count: " << stree << "\n";
#ifdef TEST_DELETED_COUNT
	std::cout << "Final deleted: " << my_tree.deleted_count() << "\n";
#endif		
#ifdef DEBUG 
	std::cout << "Allocations: " << counter_allocate << " blocks, " << sizeof(node<int>) * counter_allocate << " byte memory\n";
#endif		
	std::cout << "Effective: " << sizeof(node<int>) * stree << " byte\n";
#ifdef DEBUG 
	std::cout << "Rate: " << static_cast<float>(stree) / counter_allocate * 100 << "%\n";
#endif		
	std::cout << "============================\n";
}
