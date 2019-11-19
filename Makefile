test:
	echo "Assignment 1"
	$(MAKE) -C 01 test
	echo "Assignment 3"
	$(MAKE) -C 03 test
	
clean:
	$(MAKE) -C 01 clean
	$(MAKE) -C 03 clean
