#include "HashTableLin.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <random>
using namespace std;

HashTableLin::HashTableLin(int maxNum, double load){
    numKeys = 0;
    maxLoad = load;
    size = smallestPrime(maxNum, load);
    std::vector<int> temp(size);
    table = temp;
}

int HashTableLin::isPrime(int num) { //checks if number is prime
    int i, j;
    int prime = 1; //1 = true = prime

    if (num == 1) {
        prime = 0; //0 = false = not prime
        return prime;
    }

    for (i = 0; i < num; i++)
    {
        for (j = 2; j <= num / 2; j++)
        {
            if (num % j == 0)
                prime = 0;
        }
    }
    return prime;
}

int HashTableLin::smallestPrime(int maxNum, double load) { //finds smallest prime number

    int num = maxNum / load +0.9;
    
    while (isPrime(num)==0) {
        num++;
    }
    return num;
}

//O(n^2) for rehashing

void HashTableLin::insert(int n)
{
    if (isIn(n) == true) //do not insert if already in hashTable
        return; 

    double load = (numKeys + 1.0) / size;

    if (load > maxLoad) //if limit is exceeded, rehash
        rehash();

    int i = n % size;

    while (table[i] != NULL) 
        i = (i + 1) % size; //linear probing to find empty spot

    table[i] = n; //insert at empty index
    numKeys++; //incriment number of keys
    
 }


void HashTableLin::rehash()
{
    std::vector<int> temp = table; //create temporary table to store values

    int size2 = size * 2; //double size

    while (isPrime(size2) == 0)
        size2++; //get new size from prime number func

    std::vector<int> newTable(size2); //make new hashTable with new size

    int size1 = size;
    table = newTable; 
    numKeys = 0;
    size = size2;

    for (int i = 0; i < size1; i++){
        if (temp[i] != NULL)
            insert(temp[i]); //insert elements from temp to new table
    }
}

bool HashTableLin::isIn(int n)
{
    int i = n % size;

    if (table[i] == n)
        return true; 

    else{ 
        i = (i + 1) % size; //linear probing to find element
        while (table[i] != NULL) { 
            if (table[i] == n) 
                return true; //return true if there
            else
                i = (i + 1) % size; //else iterate again
        }
        return false; //return false if loop ends
    }
}

void HashTableLin::printKeys()
{
    for (int i = 0; i < size; i++) {
        if (table[i] != NULL) 
            std::cout << "index:" << i << ", ";
    }
}

void HashTableLin::printKeysAndIndexes()
{
    for (int i = 0; i < size; i++) {
        if (table[i] != NULL) {
            std::cout << "index:" << i << ", ";
            std::cout << "key:" << table[i] << ", ";
        }
    }
}

int HashTableLin::getNumKeys() {
	
    return numKeys;
}

int HashTableLin::getTableSize() {

    return size;
}

double HashTableLin::getMaxLoadFactor() {

    return maxLoad;
}


std::vector<double> HashTableLin::simProbeSuccess()
{
    vector<double> result(9);
    int nums[100]; //list to store random nums

    for (int i = 0; i < 100; i++) {
        bool isIn = true;
        int randNum = rand();
        nums[i] = randNum; //set element as random num
        while (isIn == true) { //checking if element already in list
            isIn = false;
            for (int j = 0; j < i; j++) {
                if (randNum == nums[j]) {
                    isIn = true;
                    randNum = rand()*rand(); //if it is, set as new random num
                    break;
                }
            }
            nums[i] = randNum;
        }
    }

    std::vector<double> loads = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9}; //diff load factors
    double sum = 0; 
    double count = 0;

    for (int i = 0; i < 9; i ++) {
        sum = 0;
        for (int j = 0; j < 100; j++) {
            HashTableLin table2 = HashTableLin(100, loads[i]); //make new table
            for (int k = 0; k < 100; k++) {
                count = table2.insertCount(nums[k]); // add numbber of probes per each insertion to table
                sum += count/100;
            }
        }
        result[i] += sum / 100; 
    }
    return result;
}


// for bonus
std::vector<double> HashTableLin::simProbeUnsuccess()
{
    // TODO, change following code after completing this function
    vector<double> result(9);
    return result;
}

int HashTableLin::insertCount(int n) {
    
    double count = 0;
    int i = n % size;

    while (true) {

        if (table[i] == n)
            return 0;

        else if (table[i] == 0)
        {
            table[i] = n;
            count++;
            numKeys++;
            return double(count);
        }

        else
        {
            count++;
            i = (i + 1) % size;
        }
    }
}
