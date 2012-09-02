
tests: relexec
	./copy-tree top ttop
	./run-tests

clean:
	rm -f relexec 
	rm -rf ttop

