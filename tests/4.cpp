#include "../debug/treap.hpp"
#include <iostream>
#include <vector>

#define TEST_REMOVE 1

size_t global = 1;
using namespace bst;

/*
struct roup
{
	size_t index_;

	roup()
	{
		 index_ = global++;
	}

	roup(size_t start_index)
	{
		index_ = ++start_index;
	}

	bool operator< (const roup & a) const {
		return this->index_ < a.index_;
	}
	bool operator> (const roup & a) const {
		return this->index_ > a.index_;
	}
	bool operator== (const roup & a) const {
		return this->index_ == a.index_;
	}

};
*/


treap<size_t> tree;


int main(int argc, char const *argv[])
{
	size_t n;
	std::cin >> n;

	for (size_t i = 1; i <= n; ++i)
	{
		tree.insert(i);
	}

//	tree.print();

	std::cout << std::endl;
	tree.replace_in_head(2, 4);
	tree.replace_in_head(3, 5);
	tree.replace_in_head(2, 2);
//	tree.print();
	std::cout << std::endl;

	return 0;
}