/*
Author : Ali Cagatay Kuru
*/

#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#include <pthread.h>

typedef struct node {
    int val;
    struct node *next;
}node_l;

/*
	adds a node to the linked list 
	with head pointer head.
*/
void addNode(node_l **head, int newVal){	
	node_l *newNode;						
	newNode = malloc(sizeof(node_l));
	newNode->val = newVal;

	if(*head== NULL){
		newNode->next = NULL;
		*head= newNode;
	}
	else{
		node_l *curr = *head;
		while(curr->next != NULL){
			curr = curr->next;
		}
		curr->next = newNode;
		newNode->next = NULL;
	}
}

/*
	removs a node with given value from the linked list 
	with head pointer head.
*/
void removeNode(node_l **head, int value)
{
  node_l *curr, *prev;

  prev = NULL;
  curr = *head;

  while (curr != NULL){

    if (curr->val == value) {
      if (prev == NULL) {
        *head = curr->next;
      } else {
        prev->next = curr->next;
      }

      free(curr);
      return;
    }
    prev = curr;
    curr = curr->next;
 }
}

node_l *hashTable[MAXBUCKETS];	//create the hashtable with max capacity
int numberOfBuckets;	
pthread_mutex_t lock[MAXBUCKETS];	//defines mutex variables

void hash_init(int N)
{
	if(N<MINBUCKETS || N >MAXBUCKETS){	//range of bucket number is between 10 and 100
		exit(1);
	}
	int i = 0; 
	while(i<N){
		hashTable[i] = NULL;	//for each bucket create an empty linked list
		pthread_mutex_init(&lock[i], NULL);	//for each bucket create a mutex variable
		i++;
	}
	numberOfBuckets = N;
}

/*
	this method is to understand whether
	the corresponding linked list is empty or 
	containing the value given as parameter
*/
int isExist(node_l **head, int value){
	node_l *curr = NULL;
	curr = *head;
	if(curr != NULL){
		while(curr){
			if(curr->val == value){
				return 1;
			}
			curr = curr->next;
		}
		return 0;
	}
	else return 0;
}

int hash_insert(int k)
{
	int placeInBucket = k%numberOfBuckets;	//index of bucket is exual to key k mod N
	pthread_mutex_lock(&lock[placeInBucket]);	//lock corresponding mutex variable before entering the critical section

	int i = isExist(&hashTable[placeInBucket], k);

	if(i == 0){		//if corresponding list does not contain the key k, add new one
		addNode(&hashTable[placeInBucket], k);	//add corresponding node

		pthread_mutex_unlock(&lock[placeInBucket]);	//unlock corresponding mutex variable after exiting the critical section
		return (0);
	}
	else{
		pthread_mutex_unlock(&lock[placeInBucket]);	//unlock corresponding mutex variable after exiting the critical section

		return (-1);
	}
	
}

int hash_delete(int k)
{
	int placeInBucket = k%numberOfBuckets;	//index of bucket is exual to key k mod N
	pthread_mutex_lock(&lock[placeInBucket]);	//lock corresponding mutex variable before entering the critical section

	int i = isExist(&hashTable[placeInBucket], k);

	if(i == 1){	//if corresponding list contains the key k, delete it
		removeNode(&hashTable[placeInBucket], k);	//delete corresponding node

		pthread_mutex_unlock(&lock[placeInBucket]);	//unlock corresponding mutex variable after exiting the critical section
		return (0);
	}
	else{
		pthread_mutex_unlock(&lock[placeInBucket]);	//unlock corresponding mutex variable after exiting the critical section

		return (-1);
	}
}

int hash_get(int k)
{	
	int placeInBucket = k%numberOfBuckets;	//index of bucket is exual to key k mod N
	pthread_mutex_lock(&lock[placeInBucket]);	//lock corresponding mutex variable before entering the critical section

	node_l *curr = NULL;
	curr = hashTable[placeInBucket];

	while(curr!= NULL){
		if(curr->val == k){	//if found, return key k
			pthread_mutex_unlock(&lock[placeInBucket]);	//unlock corresponding mutex variable after exiting the critical section
			return k;
		}
		curr = curr->next;
	}

    pthread_mutex_unlock(&lock[placeInBucket]);	//unlock corresponding mutex variable after exiting the critical section
	return(-1);
}
