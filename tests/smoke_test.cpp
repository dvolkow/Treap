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

	std::cout << my_tree.upper_bound(1) << "\n";
	std::cout << my_tree.find(1) << "\n";
	my_tree.insert(1);
	std::cout << my_tree.find(1) << "\n";
	std::cout << my_tree.k_max(2) << "\n";
	std::cout << my_tree.k_max(1) << "\n";
	std::cout << my_tree.k_max(0) << "\n";
	std::cout << my_tree.remove(-1) << "\n";
	std::cout << my_tree.remove(-1) << "\n";
	std::cout << my_tree.find(-1) << "\n";
	std::cout << my_tree.was(-1) << "\n";
	std::cout << my_tree.was(-8) << "\n";
	std::cout << my_tree.previous(-8) << "\n";
	std::cout << my_tree.previous(8) << "\n";
	my_tree.erase(-1);

	for (auto i = 0; i < 10000; ++i)
		my_tree.insert(i);

	std::cout << my_tree.size() << ", size: " << sizeof(my_tree) << "\n";
}
