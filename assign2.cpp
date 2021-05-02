#include <bits/stdc++.h>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include<mutex>
#include<chrono>
#include<condition_variable>
#define left 0
#define right 1

using namespace std;

//Global variables
mutex m,k;
condition_variable cv;
bool isNotPresent = 1;

void Depart(int i, int direction) {
	this_thread::sleep_for(chrono::milliseconds(10));
	string str = ((direction+1)%2 == right)? "Right" : "Left";
	unique_lock<mutex> tl(k);
	cout<<"Vehicle-"<<i+1<<" departed the tunnel from "<<str<<endl;
	k.unlock();
}

void Arrive(int i, int direction) {
	//Checking the direction of vehicle
      unique_lock<mutex> tl(k);
      string str = (direction == right)? "Right" : "Left";
      cout<<"Vehicle-"<<i+1<<" entered the Queue with direction "<<str<<endl;
      k.unlock();
	//This will just take the ownership of lock, if not it will go to sleep.
	unique_lock<mutex>ul(m);
	cv.wait(ul, [] { return isNotPresent;});
	isNotPresent = 0;

	cout<<"Vehicle-"<<i+1<<" entered the tunnel from "<<str<<endl;

	Depart(i, direction);

    isNotPresent = 1;

	//Notify the other vehicle currently present in the front of the Queue.
	cv.notify_one();
}

int main()
{
	cout<<"Enter the number of vehicles in the System"<<endl;
	int n;
	cin>>n;

	//creating arrays of n threads
	thread vehicles[n];

	//Calling Arrive for each Thread.
	for(int i=0; i<n; i++) {
	   bool direction = rand()%2;
		vehicles[i] = thread(Arrive, i, direction);
		if(n%2&&i==(n-1)/2)
		sleep(1);
		else if(i==n/2)
		sleep(1);
	}

	for(int i=0; i<n; i++) {
		// Wait for vehicle[i] thread to finish
		vehicles[i].join();
	}

	return 0;
}
