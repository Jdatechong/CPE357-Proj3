#include "hencode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>

int* decToBin(int n){
	int mask, temp;
	static int arr[8];
	for (mask = 7; mask >= 0; mask--)
	{
		temp = n >> mask;
		if (temp & 1){
			arr[7-mask] = 1;
		}
		else{
			arr[7-mask] = 0;
		}
	}
	return arr;
}


int main(int argc, char** argv){
	int i,j,*arr,ttlChar =0,input, output, charCount = 0, occList[256], buff4 =0, buff1 = 0;
        list l = create_list();
        tNode newNode, currNode;	
	tree t;
	/*get the input*/
	if(argc < 2){
		perror("usage: ");
		return 1;
	}	
	else {
		if(strcmp(argv[1], "-") == 0){
			input = STDIN_FILENO;
		}
		else{
			input = open(argv[1], O_RDONLY, 00777);
			if(input == -1){
				fprintf(stderr, "%s: ", argv[1]);
				perror("");
				return 1;
			}
		}
	}
	/*get the output*/
	if(argc<3){
		output = STDOUT_FILENO;
	}
	else{
		output = open(argv[2], O_WRONLY|O_CREAT, 00777);
	}
	for (i = 0; i<256; i++){
		occList[i] =0;
	}
	read(input,&charCount,4);
	for (i=0; i<charCount; i++){
		buff1 = 0;
		buff4 = 0;
		read(input, &buff1, 1);
		read(input, & buff4, 4);
		occList[buff1] = buff4;
		ttlChar = ttlChar +buff4;
	}
	/*make linked list out of occList*/
        for(i=255; i>=0; i--){
                if(occList[i] != 0){
                        newNode = (tNode)malloc(sizeof(binTreeNode));
                        newNode->key = (char)(i);
                        newNode->value = occList[i];
                        newNode->isLeaf = 1;
                        newNode->left = NULL;
                        newNode->right = NULL;
                        insert_sorted_lNode(l,newNode);
                }
        }
	t = make_tree(l);
	j=0;
	if(t !=NULL){
		currNode = t->head;
		buff1 = 0;
		while(j<ttlChar && (read(input, &buff1, 1))==1){
			
			arr = decToBin(buff1);
			for(i = 0; i < 8; i++){
				if(arr[i] == 0){
					currNode = currNode->left;
				}
				else if(arr[i] == 1){
					currNode = currNode->right;
				}
				
				if((j<ttlChar) && (currNode ->isLeaf)){
					j++;
					write(output,&(currNode->key),1);
					currNode = t->head;
				}
			}
			buff1 = 0;
		}
		/*one char case*/
		while(j<ttlChar){
			j++;
                        write(output,&(currNode->key),1);
		}
	}
	delete_list(l);
	free(t);
	return 0;
}
