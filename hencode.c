#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include "hencode.h"
int toDec(int a[]){
	int i =0, ttl = 0;
	for (i=0; i<8; i++){
		ttl = ttl + ((1<<i) * a[7-i]);
		
	}
	return ttl;
}
int main(int argc, char** argv){
	int i,count=0,k,occList[256], *binList[256], buff[8],output;
	FILE *f;
	char c;
	int8_t temp8;
	tree t;
	list l = create_list();
	tNode newNode;
	if(argc>1){
		f = fopen(argv[1], "r");
	}
	else{
		perror("usage: ");
		return 1;
	}
	if(f == NULL){
		fprintf(stderr, "%s: ", argv[1]);
		perror("");
		return 1;
        }
	for (i =0; i<256; i++){
		occList[i] = 0;
		binList[i] = NULL;
	}
	while ((c = getc(f))!=EOF) {
		if(c <= 256 && c>=0){
			occList[(int)(c)]++;
		}	
	}
	
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
	make_bin_list(t, binList);
        for (i =0; i<256; i++){
                if(binList[i]){
                        count++;
                }
        }
	if(argc >= 3){
		output = open(argv[2],O_RDWR|O_CREAT, 00777);
	}
	else{
		output = 1;
	}
	
	write(output, &(count), 4);
	for (i =0; i<256; i++){
                if(binList[i]){	
			write(output, &i, 1);
			write(output, &occList[i],4);
                }
        }

	k = 0;
	rewind(f);
      	while ((c = getc(f))!=EOF) {
		if(c >= 0 && c<=256){
			for (i =0; i<256 && binList[(int)(c)][i] != -1; i++){
				buff[k] = binList[(int)c][i];
				k++;
				if(k > 7){
					temp8 = toDec(buff);
					
					write(output, &(temp8), 1);
					k = 0;	
				}
			}
		}
		
        }
	
	if(k != 0){
		for (; k<8 ; k++){
			buff[k] = 0;
		}
		temp8 = toDec(buff);
		write(output, &(temp8), 1);
	}


	delete_list(l);
	free(t);
	for (i = 0; i<256; i++){
		if(binList[i]){
			free(binList[i]);
		}
	}
	fclose(f);
	
	return 0;	
}

