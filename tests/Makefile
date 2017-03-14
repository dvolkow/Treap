release_treap: test_header.cpp ../release/treap.hpp
	g++ -Wall -DCASUAL=1 -Wextra -pedantic -std=c++11 -O2  -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector test_header.cpp -o treap
treap_strong: test_header.cpp ../debug/treap.hpp
	g++ -Wall -DCASUAL=1 -Wextra -pedantic -std=c++11 -O2  -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector test_header.cpp -o treap
debug_casual: test_header.cpp ../debug/treap.hpp
	g++ -Wall -DCASUAL=1 -Wextra -pedantic -std=c++11 -Ofast -Wconversion -Wlogical-op test_header.cpp -o treap
debug_set_vs_treap: test_header.cpp ../debug/treap.hpp
	g++ -Wall -DSET_VS_TREAP=1 -Wextra -pedantic -std=c++11 -Ofast -Wconversion -Wlogical-op test_header.cpp -o treap
utest: gentest.cpp
	g++ -Ofast -DSERIAL_=1 -Wall -std=c++11 gentest.cpp -o gentest
paralleltest: gentest.cpp
	g++ -Ofast -DPARALLEL_=1 -DWRITE_RES=1 -Wall -pthread -std=c++11 gentest.cpp -o gentest
paralleltest_dbg: gentest.cpp
	g++ -Ofast -DPARALLEL_=1 -DDEBUG=1 -DWRITE_RES=1 -Wall -pthread -std=c++11 gentest.cpp -o gentest
test_generator_performance:
	g++ -Ofast -DPARALLEL_=1 -Wall -pthread -std=c++11 gentest.cpp -o GTST
set: set.cpp
	g++ -Wall -Wextra -pedantic -std=c++11 -Ofast -Wconversion -Wlogical-op set.cpp -o set
set_dbg: set.cpp
	g++ -Wall -DDEBUG=1 -Wextra -pedantic -std=c++11 -Ofast -Wconversion -Wlogical-op set.cpp -o set
simplejava:
	javac SimpleTree.java 
clean:
	rm *.class gentest
