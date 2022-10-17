test:
	g++ -std=c++17 -pthread main_test.cpp -lgtest_main -lgtest -lpthread -o test
	./test
clean:
	rm -rf test
	rm -rf build/*
	rf -rf smartcalc_v1.tar.gz
uninstall: clean
dvi:
	open manual/documentation
leaks:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./test
dist:
	tar -czvf smartcalc_v1.tar.gz build