#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
int printarr(char arr[2048],int newlinesz,int printmax){ //print array with newline
	int x = 0;
	int y = 0;
	char lf = 32;
	int z = newlinesz;
	while(printmax != x){
		if(arr[x] == 0){
		arr[x] = lf;
		}
		if(x == z){
			z = z + newlinesz;
			printf("\n");
			printf("|%c|",arr[x]);
		}
		else if(x == 0){
			printf("|%c|",arr[x]);
		}
		else{
			printf("%c|",arr[x]);
		}
		x++;
	}
	printf("\n");
	return 0;
}

int solve(char puzzle[2000],char word[40],int pzret){ //solve
	char solved[2000] = ""; //buffer for solution
	int fail = -1;
	int i = 0; //counter for puzzle
	int x = 0; //counter for word
	//char lf = word[0]; //compared to different chars of the puzzle (SET +1 every time sucess)
	printf("length of puzzle is : %i *&& word is %s\n",strlen(puzzle),word);
	while(strlen(word) == x){
	//while(strlen(word) != x){ //LOOP FOR CHECKING IF WORD EXISTS GOING FORWARDS.
		if(i-1 == strlen(puzzle)){
			x = 0;
			i = 0;
			break;
		}		
		else if(word[x] == puzzle[i]){
			printf("reached(forwards) sm. value: %i(wc: %c -- pz: %c)\n",i,word[x],puzzle[i]);
			solved[i] = puzzle[i];
			while(1){ // check if word exists going forwards (puzzle char +1 ...)
				if(i == strlen(puzzle) && x == strlen(word)){ //to make sure that i dosent climb uncontrolled
					fail = 0;
					break;
				}				
				x++;
				i++;
				if(word[x] == puzzle[i]){
				printf("reached(forwards) sm. value: %i(wc: %c -- pz: %c)\n",i,word[x],puzzle[i]);
				solved[i] = puzzle[i];
				}
				else if(strlen(word) == x){ // check if we found the word
					fail = 0;
					puts("Word Found!");
					printarr(solved,pzret,strlen(puzzle));
					return 0;
					break;
				}
				else{
					//puts("error,resetting x");
					memset(solved,0,sizeof(solved));
					x = 0;
					fail = 1;
					break;
				}
			}
			if(fail == 0){
				puts("fail = 0, exiting");
				break;
			}
			if(i == strlen(puzzle)-1){
				fail = 1;
				memset(solve,0,sizeof(solve));
				//puts("MAX int i reached in forwards");
				break;		
			}
		}
		else{
			i++;
			if(i == strlen(puzzle)){
				puts("word not found");
				fail = 1;
				break;
			}
		}
	}
printarr(solved,pzret,strlen(puzzle));
//if(fail == 1){ //backwards test if word was not found
if(fail == 4){
	//reset variables for #2 backwards check
		x = 0;
		i = strlen(word);
		int i2 = 0;
		fail = -1;
		memset(solved,0,sizeof(solved));
		while(1){
			if(word[x] == puzzle[i]){
			i2 = i + 1;
			printf("reached(backmode) sm. value: %i(wc: %c -- pz: %c)\n",i,word[x],puzzle[i]);
			solved[i] = puzzle[i];
			while(1){
				x++;
				if(i > 0){ //subtract 1 from puzzle and add one to x 
					if(i == strlen(puzzle)){ //to make sure that i dosent climb uncontrolled
					puts("error i == strlen puzzle");
					fail = 1;
					break;
					}
					i = i - 1; //set i -1 (on fail restored to before+1)
				}
				else{
					fail = 1;
					break;
				}
				if(strlen(puzzle) <= i){
					puts("err");
					return -1;				
				}
				else if(word[x] == puzzle[i]){
					printf("reached(backmode) sm. value: %i(wc: %c -- pz: %c)\n",i,word[x],puzzle[i]);
					solved[i] = puzzle[i];
				}
				else if(strlen(word) == x){
					fail = 0;
					printarr(solved,pzret,strlen(puzzle));
					return 0;
					break;
				}
				else{
					puts("error,resetting(in backwards mode) x ");
                                        fail = 1;
                                        break;
				}
			}
			if(fail == 1){ //if fail is set to 1 reset solution
				memset(solved,0,sizeof(solved));
				x = 0;
				fail = 0;
				i = i2;
			}
		}
		else{
			i++;
			if(i == strlen(puzzle)){
				puts("Word n/a");
				fail = 1;
				break;
			}
		}
	}
}
//if(fail == 1){
if(fail == 4){
	x = 0;
	i = 0;
	fail = -1;
	int i2  = 0;
	memset(solved,0,sizeof(solved));
	while(i != strlen(puzzle)){
			printf("i=%i & x=%i // puzzle[i](%c), i+pzret(%c)\n",i,x,puzzle[i],puzzle[i+pzret]);
		if(puzzle[i] == word[x]){
			printf("match at %i,word %c,& puzzle %c\n",i,word[x],puzzle[i]);
			solved[i] = puzzle[i];
			x++;
			i2 = i + pzret;
			while(1){
				if(strlen(puzzle) <= i2 || strlen(puzzle) <= i){
				puts("length reached");
				fail = 2;
				break;
				}
				else if(strlen(word) == x){
					puts("Word found!");
					fail = 0;
					printarr(solved,pzret,strlen(puzzle));
					return 0;
					break;
				}
				else if(word[x] == puzzle[i2]){
					printf("match at %i,wrd(%c) & pzl(%c)\n",i,word[x],puzzle[i2]);
					solved[i2] = puzzle[i2];
					i2 = i2 + pzret;
					x++;
				}
				else{
					printf("failed match wrd(%c) & pzl(%c)\n", word[x],puzzle[i2]);
					x = 0;
					i2 = 0;
					memset(solved,0,sizeof(solved));
					fail = 1;
					i++;
					break;
				}
			}
			if(fail == 0){
				puts("read success");
				break;
			}
			else if(fail == 2){
				puts("read fail");
				printarr(solved,pzret,strlen(puzzle));
				memset(solved,0,sizeof(solved));
				break;
			}
	}
	else{
		i++;	
	}
}
}
//if(fail == 1){
if(1){
	x = 0;
	i = 0;
	fail = -1;
	int i2 = 0;
	memset(solved,0,sizeof(solved));
	while(1){
		if(strlen(puzzle) == i){
			puts("max length exx.");
			printarr(solved,pzret,strlen(puzzle));
			break;
		}
		else if(puzzle[i] == word[x]){
			solved[i] = puzzle[i];
			printf("mch fnd puzzle[%i] = %c // solved[%i] = %c\n",i,puzzle[i],x,solved[x]);
		//	printf("char puzzle[i] is %c\n",puzzle[i]);
			x++;
			i2 = i - pzret;
			while(strlen(puzzle) > i && strlen(puzzle) > i2 && i - pzret >= 0 && i2 - pzret >= 0){
				sleep(1);
				if(strlen(word) <= x){
				fail = 0;
				puts("w found");
				break;
				}
				else if(puzzle[i2] == word[x]){
					solved[i2] = puzzle[i2];
					printf("mch fnd puzzle[%i] = %c // solved[%i] = %c\n",i2,puzzle[i2],x,solved[x]);
					i2 = i2 - pzret;
					x++;
				}
				else if(strlen(puzzle) == i2){
				fail = 2;
				puts("did not find the word going upwards");
				memset(solved,0,sizeof(solved));
				x = 0;
				i2 = 0;
				i = 0;
				break;
				}
				else if(strlen(word) == x){
				fail = 0;
				puts("word found");
				printarr(solved,pzret,strlen(puzzle));
				break;
				}
				else{
					fail = 1;
					x = 0;
					puts("failed comp, resetting x");
					memset(solved,0,sizeof(solved));
					i2 = 0;
					break;
				}
			}
		}
		i++;
		if(fail == 0 || fail == 2){
		printarr(solved,pzret,strlen(puzzle));
		puts("caught interrupt, exiting");
		break;
		}
	}
}
return 0;
}
int main(int argc,char** argv[]){
	int pzret; //length of one line in challenge
	char puzzle[2000]; //puzzle variable used to store the challenge 
	char word[40];//word that is being looked for (string)
	while(1){
		puts("Please enter the puzzle -->");
		scanf("%s",&puzzle); 
		puts("Please enter the amount of letters in each line -->");
		scanf("%i",&pzret);
		printarr(puzzle,pzret,strlen(puzzle));
		char r2edo;
		puts("Is this correct (Y/N)?");
		scanf("%s",&r2edo);
		if(r2edo == 121 || r2edo == 89){
			break;		
		}
	}
	
		while(1){
			printarr(puzzle,pzret,strlen(puzzle));		
			printf("Please enter a word to query:\n---> ");
			scanf("%s",&word);
			solve(puzzle,word,pzret);
		}
return 0;
}
