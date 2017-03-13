treap_strong: treap.cpp
	g++ -Wall -Wextra -pedantic -std=c++11 -O2  -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector test_header.cpp -o treap
treap: test_header.cpp treap.hpp
	g++ -Wall -Wextra -pedantic -std=c++11 -Ofast -Wconversion -Wlogical-op test_header.cpp -o treap
newtest: gentest.cpp
	g++ -Ofast -Wall -std=c++11 gentest.cpp -o gentest
simplejava:
	javac SimpleTree.java 
runtreap:
	./treap
runsimplej:
	java SimpleTree
gentest:
	./gentest
