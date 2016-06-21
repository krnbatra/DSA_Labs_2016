#include <stdio.h>
#include <stdlib.h>
#include <math.h>
long long int countDigits(long long int a){
	long long int count = 0;
	while(a > 0){
		a/=10;
		count++;
	}
	return count;
}

long long int findLower(long long int a, long long int digits){
	return a%(long long int)pow(10, digits);
}

long long int findUpper(long long int a, long long int lowerDigits){
	long long int i = 0;
	while(i < lowerDigits){
		a/=10;
		i++;
	}
	return a;
}

typedef struct TreeNode{
	long long int a;
	long long int b;
	long long int answer;
	struct TreeNode *left, *center, *right, *parent;
}treeNode;

typedef struct QNode{
	treeNode* tNode;
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

void enQueue(que* q, treeNode* node){
	link newQNode = (link)malloc(sizeof(qNode));
	newQNode->tNode = node;
	newQNode->next = NULL;
	if(q->rear == NULL){
		q->front = newQNode;
		q->rear = newQNode;
		return;
	}
	q->rear->next = newQNode;
	q->rear = newQNode;
	return;
}

link dequeue(que *q){
	if(q->front==NULL)
		return NULL;
	link temp = q->front;
	q->front = q->front->next;
	if(q->front==NULL)
		q->rear = NULL;
	return temp;
}

void push(que* q, treeNode* node){
	link newSNode = (link)malloc(sizeof(qNode));
	newSNode->tNode = node;
	if(q->front==NULL){
		q->front = newSNode;
	}else{
		newSNode->next = q->front;
		q->front = newSNode;
	}
}

link pop(que* q){
	if(q->front==NULL){
		return NULL;
	}
	link temp = q->front;
	q->front = q->front->next;
	return temp;
}

int isEmpty(que *q){
	return q->front==NULL;
}

int isLeaf(treeNode* currentNode){
	return (countDigits(currentNode->a)<=1 && countDigits(currentNode->b)<=1);
}

treeNode* createLeftNode(long long int h1, long long int h2, long long int l1, long long int l2){
	treeNode* newNode = (treeNode*)malloc(sizeof(treeNode));
	newNode->a = h1+l1;
	newNode->b = h2+l2;
	return newNode;
}

treeNode* createCenterNode(long long int h1, long long int h2, long long int l1, long long int l2){
	treeNode* newNode = (treeNode*)malloc(sizeof(treeNode));
	newNode->a = h1;
	newNode->b = h2;
	return newNode;
}

treeNode* createRightNode(long long int h1, long long int h2, long long int l1, long long int l2){
	treeNode* newNode = (treeNode*)malloc(sizeof(treeNode));
	newNode->a = l1;
	newNode->b = l2;
	return newNode;
}

treeNode* buildTree(long long int a,long long int b){
	treeNode* root = (treeNode*)malloc(sizeof(treeNode));
	root->a = a;
	root->b = b;
	que* q = createQueue();
	que* stack = createQueue();
	enQueue(q, root);
	while(!isEmpty(q)){
		link temp = dequeue(q);
		treeNode* currentNode = temp->tNode;
		if(!isLeaf(currentNode)){
			long long int max = (currentNode->a > currentNode->b)?currentNode->a:currentNode->b;
			long long int countD = countDigits(max);
			long long int lowerD = 0;
			if(countD%2==0){
				lowerD = countD/2;
			}else{
				lowerD = (countD+1)/2;
			}
			long long int h1 = findUpper(currentNode->a, lowerD);
			long long int l1 = findLower(currentNode->a, lowerD);
			long long int h2 = findUpper(currentNode->b, lowerD);
			long long int l2 = findLower(currentNode->b, lowerD);
			treeNode* leftNode = createLeftNode(h1, h2, l1, l2);
			treeNode* centerNode = createCenterNode(h1, h2, l1, l2);
			treeNode* rightNode = createRightNode(h1, h2, l1, l2);
			currentNode->left = leftNode;
			currentNode->center = centerNode;
			currentNode->right = rightNode;
			leftNode->parent = currentNode;
			centerNode->parent = currentNode;
			rightNode->parent = currentNode;
			enQueue(q, leftNode);
			enQueue(q, centerNode);
			enQueue(q, rightNode);
		}else{
			push(stack, currentNode);
		}
	}
	while(!isEmpty(stack)){
		enQueue(q, pop(stack)->tNode);
	}
	while(!isEmpty(q)){
		link temp = dequeue(q);
		treeNode* currentNode = temp->tNode;
		if(isLeaf(currentNode)){
			printf("%lldX%lld=%lld\n", currentNode->a, currentNode->b, currentNode->a*currentNode->b);
			currentNode->answer = currentNode->a*currentNode->b;
		}
		else{
			currentNode->answer = 0;
			printf("%lldX%lld=", currentNode->a, currentNode->b);
			int max = (currentNode->a > currentNode->b)?currentNode->a:currentNode->b;
			int countD = countDigits(max);
			int p = 0;
			if(countD%2==0){
				p = countD/2;
			}else
				p = (countD+1)/2;
			printf("%lldX%lldX%lld+", currentNode->center->answer, (long long int)pow(10, p), (long long int)pow(10, p));
			currentNode->answer += currentNode->center->answer*(long long int)pow(10, p)*(long long int)pow(10, p);
			printf("(%lld-%lld-%lld)X%lld+", currentNode->left->answer, currentNode->center->answer, currentNode->right->answer, (long long int)pow(10, p));
			currentNode->answer += (currentNode->left->answer-currentNode->center->answer-currentNode->right->answer)*(long long int)pow(10, p);
			printf("%lld=", currentNode->right->answer);
			currentNode->answer += currentNode->right->answer;
			printf("%lld\n", currentNode->answer);
		}
		if(currentNode->parent!=NULL && currentNode->parent->left==currentNode){
			enQueue(q, currentNode->parent);
		}
	}
	return root;
}

int main(){
	long long int a, b;
	char d;
	scanf("%lld%c%lld", &a, &d, &b);
	treeNode* root = buildTree(a, b);
	return 0;
}
