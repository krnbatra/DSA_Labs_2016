#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct BinaryTreeNode{
    int data;
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
    struct BinaryTreeNode *parent;
}BTNode;

typedef struct BinaryTreeNode *BTNodePtr;

typedef struct QNode{
	BTNodePtr btnode;
	struct QNode *next;
}qNode;

typedef qNode *link;

typedef struct Queue{
	qNode *front, *rear;
}que;

que* createQueue(){
	que* q = (que*)malloc(sizeof(que));
	q->front = NULL;
	q->rear = NULL;
	return q;
}

void enQueue(que* q, BTNodePtr node){
	link newQNode = (link)malloc(sizeof(qNode));
	newQNode->btnode = node;
	newQNode->next = NULL;
	if(q->rear == NULL){
		q->front = newQNode;
		q->rear = newQNode;
		return;
	}
	q->rear->next = newQNode;
	q->rear = newQNode;
}

link dequeue(que *q){
	if(q->front==NULL)
		return NULL;
	link temp = q->front;
	q->front = q->front->next;
	if(q->front == NULL)
		q->rear = NULL;
	return temp;
}
int isEmpty(que *q){
	return q->front == NULL;
}

BTNodePtr makeLeftChild(BTNodePtr root){
	BTNodePtr newBTNode = (BTNodePtr)malloc(sizeof(BTNode));
	newBTNode->parent = root;
	root->left = newBTNode;
	return root;
}

BTNodePtr makeRightChild(BTNodePtr root){
	BTNodePtr newBTNode = (BTNodePtr)malloc(sizeof(BTNode));
	newBTNode->parent = root;
	root->right = newBTNode;
	return root;
}
void preOrder(BTNodePtr root){
    if(root==NULL)
        return;
    printf("%d", root->data);
    preOrder(root->left);
    preOrder(root->right);
}
int evaluateExpression(char str[], int bin[], int size){
	int i = 0, res;
	while(str[i]!='\0'){
		if(str[i]=='('){
			res = 0;
			while(str[i]!=')'){
				if(str[i]=='-'){
					res = !(bin[str[i+1]-'A']);
					i++;
				}
				else if(str[i] >= 'A' && str[i] <= 'Z'){
					res = bin[str[i]-'A'];
				}
				if(res==1){
					while(str[i]!=')'){
						i++;
					}
					i++;
					if(str[i] == '\0'){
						return 1;
					}
					break;
				}
				i++;
			}
			if(res==0)
				return 0;
		}			
		else if(str[i]=='*'){
			i++;
		}
	}
	return 1;
}

int findDigits(int n){
	int count = 0;
	while(n > 0){
		count++;
		n=n>>1;
	}
	return count;
}

void decimalToBinary(int arr[], int size, int num){
	int i;
	for(i = 0;i < size; i++){
		arr[i] = 0;
	}
	int j = size-1;
	while(num > 0){
		arr[j] = num%2;
		num/=2;
		j--;
	}
}
int main(int argc,char **argv){
    char str[500];
    scanf("%s", str);
    int i;
    int count[26];
    for(i = 0;i < 26; i++){
    	count[i] = 0;
    }
    for(i = 0; str[i]!='\0'; i++){
    	if( (str[i] >= 'A' && str[i] <= 'Z')  && count[str[i]-'A'] == 0){
    		count[str[i]-'A'] = 1;
    	}
    }
    int var = 0;
    for(i = 0;i < 26; i++){
    	if(count[i]==1)
    		var++;
    }
    int totalNodes = (int)pow(2, var+1) - 1;
    BTNodePtr root = (BTNodePtr)malloc(sizeof(BTNode));
	que* q = createQueue();
	enQueue(q, root);
	totalNodes--;
	while(totalNodes > 0){
		link temp = dequeue(q);
		BTNodePtr currentNode = temp->btnode;
		currentNode = makeLeftChild(currentNode);
		currentNode = makeRightChild(currentNode);
		enQueue(q, currentNode->left);
		enQueue(q, currentNode->right);
		totalNodes-=2;
	}
	int counter = 0;
	
	int dp = (int)pow(2, var);
	
	int digits = findDigits(dp-1);

	while(counter < dp){
		link temp = dequeue(q);
		int num = counter;
		int bin[digits];
		decimalToBinary(bin, digits, num);
		int res = evaluateExpression(str, bin, digits);
		counter++;
		temp->btnode->data = res;
		link temp2 = dequeue(q);
		num = counter;
		decimalToBinary(bin, digits, num);
		res = evaluateExpression(str, bin, digits);
		counter++;
		temp2->btnode->data = res;
		enQueue(q, temp2->btnode->parent);
	}
	int level = var-1;
	while(!isEmpty(q)){
		int temp1 = (int)pow(2, level);
		if(level==0){
			link temp = dequeue(q);
			temp->btnode->data = (temp->btnode->left->data | temp->btnode->right->data);
		}else{
			while(temp1 > 0){
				link temp = dequeue(q);
				if(level%2 != 0)
					temp->btnode->data = (temp->btnode->left->data & temp->btnode->right->data);
				else 
					temp->btnode->data = (temp->btnode->left->data | temp->btnode->right->data);
				
				temp = dequeue(q);
				
				if(level%2 != 0)
					temp->btnode->data = (temp->btnode->left->data & temp->btnode->right->data);
				else 
					temp->btnode->data = (temp->btnode->left->data | temp->btnode->right->data);
				enQueue(q, temp->btnode->parent);
				temp1-=2;
			}
			level--;
		}
	}
	preOrder(root);
	return 0;
}
