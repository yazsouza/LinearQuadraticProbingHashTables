#include "HashTableQuad.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
using namespace std;

HashTableQuad::HashTableQuad(int maxNum, double load)
{
    numKeys = 0;
    maxLoad = load;
    size = smallestPrime(maxNum, load); // make size next prime number

    std::vector<int> temp(size); 
    table = temp;
}

int HashTableQuad::isPrime(int num) { //checks if number is prime
    int i, j;
    int prime = 1;

    if (num == 1) { //1 not prime
        prime = 0;
        return prime;
    }

    for (i = 0; i < num; i++)
    {
        for (j = 2; j <= num / 2; j++) 
        {
            if (num % j == 0)
                prime = 0; //if number is divisible by some factor, not prime
        }
    }
    return prime;
 }

int HashTableQuad::smallestPrime(int maxNum, double load) { //finds smallest prime number
    int num = maxNum / load + 0.9; //add 0.9 in case of decimal value, gets next whole number

    while (isPrime(num) == 0) { //keep incrimenting size until it reaches a prime number
        num++;
    }
    return num;
}

int HashTableQuad::insertCheck(int n) {
    int co = 0;
    int i = n % size;
    if (isIn(n) == true) //if already in table, cannot be inserted
        return 0;

    while (true) { 
        if (co != 0 && ((i + co * co) % size) == i)  
            return 0;

        if (table[(i + co * co) % size] == 0)
            return 1; 
        else
            co++;
    }
}

void HashTableQuad::insert(int n)
{
    if (insertCheck(n) == false) //return if not able to insert
        return;
    
    double load = (numKeys + 1.0) / size;

    if (load > maxLoad)   //if limit exceeded, rehash
        rehash();

    int i = n % size;
    int co = 0;
    while (true) {

        if (table[(i + (co * co)) % size] == 0) //quadratic probing to find empty spot
        {
            table[(i + (co * co)) % size] = n; //insert element
            numKeys++;
            return; 
        }

        else
            co++;
    }
}

void HashTableQuad::rehash()
{
    std::vector<int> temp = table; //create temporary table to store values

    int size2 = size * 2; //double size

    while (isPrime(size2) == 0)
        size2++; //get new size from prime number func

    std::vector<int> newTable(size2); //make new hashTable with new size

    int size1 = size;
    table = newTable; //update values
    numKeys = 0;
    size = size2;

    for (int i = 0; i < size1; i++) {
        if (temp[i] != NULL)
            insert(temp[i]); //insert elements from temp to new table
    }
}

bool HashTableQuad::isIn(int n)
{
    int i = n % size;
    int co = 0;

    if (table[(i + (co * co)) % size] == 0) //if index is 0 return false
        return false;


    while (1) {
        int hash = (i + (co * co)) % size; //quadratic probe
        if (co != 0 && hash == i) 
            return false;

        if (table[hash] == n)
            return true;


        if (table[hash] == 0)
            return false;

        else
            co++;

    }

    return false;
}

void HashTableQuad::printKeys()
{
    for (int i = 0; i < size; i++) {
        if (table[i] != NULL)
            std::cout << "index:" << i << ", ";
    }
}

void HashTableQuad::printKeysAndIndexes()
{
    for (int i = 0; i < size; i++) {
        if (table[i] != NULL){
            std::cout << "index:" << i << ", "; 
            std::cout << "key:" << table[i] << ", ";
        }
    }
}

int HashTableQuad::getNumKeys() {
    return numKeys;
}

int HashTableQuad::getTableSize() {
    return size;
}

double HashTableQuad::getMaxLoadFactor() {
    return maxLoad;
}

std::vector<double> HashTableQuad::simProbeSuccess()
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
                    randNum = rand(); //if it is, set as new random num
                    break;
                }
            }
            nums[i] = randNum;
        }
    }

    std::vector<double> loads = { 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9 }; //diff load factors
    double sum = 0;
    double count = 0;

    for (int i = 0; i < 9; i++) {
        sum = 0;
        for (int j = 0; j < 100; j++) {
            HashTableQuad table2 = HashTableQuad(100, loads[i]);
            for (int k = 0; k < 100; k++) {
                count = table2.insertCount(nums[k]);
                sum += count / 100;
            }
        }
        result[i] += sum / 100;
    }
    return result;
}

int HashTableQuad::insertCount(int n) {

    int co = 0;
    int count = 0;
    int i = n % size;

    if (insertCheck(n) == 0) //if cannot be inserted, 0 probes
        return 0;

    while (true) {

        if (table[(i + co*co) % size] == n) {
            count++;
            return count; 
        }

        else if (table[(i + co*co) % size] == 0)
        {
            table[(i + co*co) % size] = n;
            count++;
            numKeys++;
            return count;
        }

        else
        {
            co++;
            count++;
        }
    }
}