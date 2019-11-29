test:
	echo "Assignment 1"
	$(MAKE) -C 01 test
	echo "Assignment 2"
	$(MAKE) -C 02 test

clean:
	$(MAKE) -C 01 clean
	$(MAKE) -C 02 clean