test:
	echo "Assignment 1"
	$(MAKE) -C 01 test
	echo "Assignment 2"
	$(MAKE) -C 02 test
	echo "Assignment 3"
	$(MAKE) -C 03 test
	echo "Assignment 4"
	$(MAKE) -C 04 test
	echo "Assignment 5"
	$(MAKE) -C 05 test

clean:
	$(MAKE) -C 01 clean
	$(MAKE) -C 02 clean
	$(MAKE) -C 03 clean
	$(MAKE) -C 04 clean
	$(MAKE) -C 05 clean
