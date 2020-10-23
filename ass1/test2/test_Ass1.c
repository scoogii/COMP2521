/** 
   test_Ass1.c - 
   Simple tests for Ass1 

   1) copy (link) example data files and test_Ass1.c to a new dir, say "ass1_test"
   2) cd to say "ass1_test"
   3) copy or link your files (the files you need to submit) to "ass1_test"
   4) Generate executable using  the following command
      % gcc -Wall -Werror -lm -std=c11 *.c  -o  test_Ass1
   5) Run
      %  ./test_Ass1
   6) Check your answers with the expected answers (read comments below)

*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "invertedIndex.h" 



/** Util function below ...
*/
void printTfIdfList(char *filename, TfIdfList list){
	
	FILE *fp = fopen(filename, "w");
	if( fp == NULL ) { 
		printf("Error opening file : %s \n", filename );
		return;
	}

	TfIdfList cur = list;
	while(cur != NULL) {
		fprintf(fp, "%.6f  %s\n",  cur->tfIdfSum, cur->filename );
		cur = cur->next;
	}
	fclose(fp);
}


void checkNormalisedString(char *origString, char *answerString){

	char originalString[100];
	char *stuString;

	strcpy(originalString, origString);
	stuString = normaliseWord(originalString);

	if( (originalString == stuString) && 
	    (strcmp(stuString, answerString) == 0) ){
		printf("> Test Passed: %s to %s\n", origString, stuString);
	}
	else {
		printf("> Test Failed: %s to %s [Expected %s]\n", origString, stuString, answerString);
	}	
}


void testNormalise(){
	printf("Testing function  normaliseWord \n");
	checkNormalisedString(".Net", ".net");
	checkNormalisedString("smh.com.au", "smh.com.au");
	checkNormalisedString("Sydney!", "sydney!");
	checkNormalisedString("wHy?", "why");
	checkNormalisedString("ORDER.", "order");
	checkNormalisedString("Text;", "text");
	checkNormalisedString("abc.net.au.", "abc.net.au");
	checkNormalisedString("Sydney???", "sydney??");
}


int main (int argc, char *argv[]) {

	// =========   Part-1 Testing  =========  

	/** You should pass all the following tests!
	*/
	testNormalise(); 

	// ---------------------------------------------------------

	InvertedIndexBST invertedTree =  generateInvertedIndex("collection.txt");

	/** Your output in "invertedIndex.txt" should be 
	    same as the expected answer in "invertedIndex_exp.txt"
	*/
        printInvertedIndex(invertedTree); 


	// =========   Part-2 Testing =========  


	TfIdfList list1 = calculateTfIdf(invertedTree, "network" , 141); 

	/** Your output in "network.txt" should be 
	    same as the expected answer in "network_exp.txt"
	*/
	printTfIdfList("network.txt" , list1);


	TfIdfList list2 = calculateTfIdf(invertedTree, "computer" , 141); 

	/** Your output in "computer.txt" should be 
	    same as the expected answer in "computer_exp.txt"
	*/
	printTfIdfList("computer.txt" , list2);

		
	// ---------------------------------------------------------


	/**  -----  The following will be available over the weekend -----
	*/

	char *words[] = { "network", "computer",  NULL }; 
	TfIdfList listM1 = retrieve(invertedTree, words , 141);

	/** Your output in "network_computer.txt" should be 
	    same as the expected answer in "network_computer_exp.txt"
	*/
	printTfIdfList("network_computer.txt" , listM1);


	char *words2[] = { "software", "security",  NULL }; 
	TfIdfList listM2 = retrieve(invertedTree, words2 , 141);

	/** Your output in "software_security.txt" should be 
	    same as the expected answer in "software_security_exp.txt"
	*/
	printTfIdfList("software_security.txt" , listM2);

	char *words3[] = { "database", NULL }; 
	TfIdfList listM3 = retrieve(invertedTree, words3 , 141);

	/** Your output in "database.txt.txt" should be 
	    same as the expected answer in "database_exp.txt"
	*/
	printTfIdfList("database.txt" , listM3);


	/**  I am not providing a free function here, because that will expose some logic 
	     required for the assignment!

		 You should implement a "free" function to free "invertedTree" and the related memory,
		 and call it here. However, note that it is not part of the requirement and will not be marked. 
		 If you don't free "invertedTree" here, all the memory associated with this process will be 
		 reclaimed by the OS at the termination of this program. 
	*/

	return 0;

}


