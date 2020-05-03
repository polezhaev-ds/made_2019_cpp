test:
	@for f in $(shell ls .); \
	do \
		if [ -f  $${f}/Makefile ]; then \
			echo "Assignment $${f}"; \
			$(MAKE) -C "$${f}" test; \
		fi \
	done

clean:
	@for f in $(shell ls .) ; \
	do \
		if [ -f  $${f}/Makefile ]; then \
			echo "Assignment $${f}"; \
			$(MAKE) -C "$${f}" clean; \
		fi \
	done
