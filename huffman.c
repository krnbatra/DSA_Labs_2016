#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
typedef struct frequency{
	char a;
	int count;
}fTable;
int printing[500], top = 0;
int countDistinctChar(char str[]){
	int i = 0;
	int arr[26];
	for(i = 0;i < 26; i++){
		arr[i] = 0;
	}
	int count = 0;
	i = 0;
	while(str[i]!='\0'){
		arr[str[i]-'A']++;
		i++;
	}
	for(i = 0;i < 26; i++){
		if(arr[i] > 0){
			count++;
		}
	}
	return count;
}

int isPresent(fTable arr[], char c, int size){
	int i = 0;
	for(i = 0;i < size; i++){
		fTable f = arr[i];
		if(f.a==c)
			return i;
	}
	return -1;
}

typedef struct HuffmanTreeNode{
	char data;
	int priority;
	int freq;
	struct HuffmanTreeNode *left, *right, *up;
}HFNode;

void minHeapify(HFNode* arr[], int i, int lastIndex){
	int left = 2*i;
	int right = 2*i+1;
	int smallest;
	if(left <= lastIndex && ((arr[left]->freq < arr[i]->freq) || (arr[left]->freq==arr[i]->freq && arr[left]->priority < arr[i]->priority)))
		smallest = left;

	else
		smallest = i;

	if(right <= lastIndex && ((arr[right]->freq < arr[smallest]->freq) || (arr[right]->freq==arr[smallest]->freq && arr[right]->priority < arr[smallest]->priority)))
		smallest = right;


	if(smallest!=i){
		HFNode* temp = arr[i];
		arr[i] = arr[smallest];
		arr[smallest] = temp;
		minHeapify(arr, smallest, lastIndex);
	}
}


void buildMinHeap(HFNode* arr[], int length){
	int i = 0;
	for(i = length/2;i >= 1; i--){
		minHeapify(arr, i, length-1);
	}
}

HFNode* extractMin(HFNode* arr[], int lastIndex){
	if(lastIndex==0){
		return NULL;
	}
	HFNode* min = arr[1];
	arr[1] = arr[lastIndex];
	lastIndex = lastIndex-1;
	minHeapify(arr, 1, lastIndex);
	return min;
}

void decreaseKey(HFNode* arr[], HFNode* key, int i){
	if(key > arr[i])
		return;
	arr[i] = key;
	while(i > 1 && arr[i/2]->freq > arr[i]->freq){
		HFNode* temp = arr[i];
		arr[i] =  arr[i/2];
		arr[i/2] = temp;
		i/=2;
	}
}

void insert(HFNode* arr[], HFNode* key, int lastIndex){
	lastIndex = lastIndex+1;
	arr[lastIndex] = key;
	// decreaseKey(arr, key, lastIndex);
	int i = lastIndex;
	while(i > 1 && (arr[i/2]->freq > arr[i]->freq || (arr[i/2]->freq==arr[i]->freq && arr[i/2]->priority > arr[i]->priority))){
		HFNode* temp = arr[i];
		arr[i] =  arr[i/2];
		arr[i/2] = temp;
		i/=2;
	}
}

HFNode* buildHuffmanTree(char data[], int freq[], int size){
	int i = 0;
	int j = 0;
	HFNode* arr[size+1];
	for(i = 0;i < size; i++){
		HFNode* newNode = (HFNode*)malloc(sizeof(HFNode));
		newNode->data = data[i];
		newNode->freq = freq[i];
		newNode->priority = j;
		j++;
		arr[i+1] = newNode;
	}

	buildMinHeap(arr, size+1);
	// for(i = 1;i < size+1; i++){
	// 	printf("%c %d\n", arr[i]->data, arr[i]->freq);
	// }
	int temp = size;
	for(i = 1;i <= temp-1; i++){
		HFNode* leftNode = extractMin(arr, size);
		// printf("%c %d\n", leftNode->data, leftNode->freq);
		size-=1;
		HFNode* rightNode = extractMin(arr, size);
		size-=1;
		// printf("%c %d\n", rightNode->data, rightNode->freq);
		HFNode* newNode = (HFNode*)malloc(sizeof(HFNode));
		newNode->left = leftNode;
		newNode->right = rightNode;
		newNode->up = NULL;
		leftNode->up = newNode;
		rightNode->up = newNode;
		newNode->data = '0';
		newNode->freq = leftNode->freq+rightNode->freq;
		newNode->priority = j;// leftNode->priority+rightNode->priority;
		// printf("%c %d\n", newNode->data, newNode->freq);
		j++;
		insert(arr, newNode, size);
		size+=1;
	}

	HFNode* root = extractMin(arr, size);
	// printf("%c", root.data);

	// preOrder(root);
	return root;
}


void postOrder(HFNode *root){
	if(root==NULL)
		return;
	postOrder(root->left);
	postOrder(root->right);
	printf("%c", root->data);
}
void inOrder(HFNode *root){
	if(root==NULL)
		return;
	inOrder(root->left);
	printf("%c", root->data);
	inOrder(root->right);

}

int findIndex(char c, char characters[], int countChar){
	int i = 0;
	for(i = 0;i < countChar; i++){
		if(characters[i]==c)
			return i;
	}
	return -1;
}

void printCode(HFNode *root, char characters[], char codes[][100], int countChar){
	if(root==NULL){
		return;
	}
	if(root->left){
		printCode(root->left, characters, codes, countChar);
	}
	if(root->right){
		printCode(root->right, characters, codes, countChar);
	}
	if(root->left==NULL && root->right==NULL){
		int index = findIndex(root->data, characters, countChar);
		HFNode* leaf = root;
		while(leaf->up != NULL){
			if(leaf->up->left == leaf){
				strcat(codes[index], "0");
			}else{
				strcat(codes[index], "1");
			}
			leaf = leaf->up;
		}
	}
}

int totalNodes(HFNode* root){
	if(root==NULL)
		return 0;
	return 1+totalNodes(root->left)+totalNodes(root->right);
}

void printReverse(char *str){
	int len = strlen(str);
	len = len-1;
	int i = len;
	for(i = len;i >= 0; i--){
		printf("%c", str[i]);
	}

}
int main(){
	char str[500];
	scanf("%s", str);
	int countChar = countDistinctChar(str);
	int i = 0;
	fTable arr[countChar];
	int k = 0;
	while(str[i]!='\0'){
		//if present then update count else add that character and make it count to 1.
		char c = str[i];

		int checkPresent = isPresent(arr, c, countChar);
		if(checkPresent!=-1){
			fTable f = arr[checkPresent];
			int countTemp = f.count;
			countTemp+=1;
			f.count = countTemp;
			arr[checkPresent] = f;
		}
		else{
			fTable f;
			f.a = c;
			f.count = 1;
			arr[k] = f;
			k++;
		}
		i++;
	}
	char characters[countChar];
	int freq[countChar];
	for(i = 0;i < countChar; i++){
		fTable f = arr[i];
		characters[i] = f.a;
		freq[i] = f.count;
	}
	HFNode* root = buildHuffmanTree(characters, freq, countChar);
	int count = 0;
	count = totalNodes(root);
	printf("%d\n", count);
	postOrder(root);
	printf("\n");
	inOrder(root);
	printf("\n");
	char codes[countChar][100];
	int j = 0;
	for(i = 0;i < countChar; i++){
		// for(j = 0;j < 100; j++){
		// 	codes[i][j] = ' ';
		// }
		strcpy(codes[i], "");
	}
	printCode(root, characters, codes, countChar);
	i = 0;
	while(str[i]!='\0'){
		char c = str[i];
		int index = findIndex(c, characters, countChar);
		printReverse(codes[index]);
		i++;
	}
	printf("\n");
	return 0;
}
