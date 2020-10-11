rbt : RBTree.o RBTester.o
	gcc -o rbt RBTester.o RBTree.o

treeLab : treeLab.o BSTree.o
	gcc -o treeLab treeLab.o BSTree.o -lm

treeLab.o : treeLab.c BSTree.h
	gcc -Wall -Werror -c treeLab.c

RBTester.o : RBTester.c RBTree.h
	gcc -Wall -Werror -c RBTester.c

BSTree.o : BSTree.c BSTree.h
	gcc -Wall -Werror -c BSTree.c

RBTree.o : RBTree.c RBTree.h
	gcc -Wall -Werror -c RBTree.c
