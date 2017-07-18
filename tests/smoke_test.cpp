#include "../debug/treap.hpp"
#include <iostream>
#include <set>

int main()
{
	bst::treap<int> my_tree;
	std::cout << my_tree.size() << ", size: " << sizeof(my_tree) << "\n";
	my_tree.insert(0);
	std::cout << my_tree.size() << ", size: " << sizeof(my_tree) << "\n";
	my_tree.insert(0);
	std::cout << my_tree.size() << ", size: " << sizeof(my_tree) << "\n";
	my_tree.insert(-1);
	std::cout << my_tree.size() << ", size: " << sizeof(my_tree) << "\n";

	bst::Randomizer a;
	std::cout << a() << "\n";
	std::cout << a() << "\n";

	std::set<int> set_int;

	std::cout << "your sizeof: " << sizeof(my_tree) << "\n";
	std::cout << "std::set sizeof: " << sizeof(set_int) << "\n";
	std::cout << sizeof(bst::rand_) << "\n";
}
