#include<bits/stdc++.h>
#include<thread>
#include<mutex>
#include<chrono>
#include<condition_variable>
#include<fstream>
#define N 5


using namespace std;

ofstream fout;
string line;

//Global variales.
mutex jok,m[N];
vector<bool> eatingDone(N,0);

void put_fork(int i, int j) {
    jok.lock();
    fout<<"Philosopher-"<<i<<" has put the fork "<<j<<endl;
    cout<<"Philosopher-"<<i<<" has put the fork "<<j<<endl;
    jok.unlock();
}

void take_fork(int i, int j) {
     jok.lock();
     fout<<"Philosopher-"<<i<<" has picked the fork "<<j<<endl;
     cout<<"Philosopher-"<<i<<" has picked the fork "<<j<<endl;
     jok.unlock();
}

void eating(int i) {
     jok.lock();
     fout<<"Philosopher-"<<i<<" is Eating........................................."<<endl;
     cout<<"Philosopher-"<<i<<" is Eating........................................."<<endl;
     jok.unlock();
    this_thread::sleep_for(chrono::seconds(2));
    eatingDone[i] = 1;
}

void thinking(int i) {
    jok.lock();
    fout<<"Philisopher-"<<i<<" is thinking"<<endl;
    cout<<"Philisopher-"<<i<<" is thinking"<<endl;
    jok.unlock();
    this_thread::sleep_for(chrono::seconds(1));
}

void func(int i) {
    int j;
    for(j=0; j<N; j++) {
        if(eatingDone[j] == 0) break;
    }
    if(j == N) return;
    
    thinking(i);
    
    int x;
    if(i != 4) {
        x = try_lock(m[i], m[(i+1)%N]);
    }
    else if(i == 4) {
        x = try_lock(m[(i+1)%N], m[i]);
    }
        
    if(x == -1) {
        if(i != 4) {
            take_fork(i,i);
            take_fork(i,(i+1)%N);
        }
        else {
            take_fork(i,(i+1)%N);
            take_fork(i,i);
        }
        
        eating(i);
        
        if(i != 4) {
            put_fork(i,i);
            put_fork(i,(i+1)%N);   
        }
        else {
            put_fork(i,(i+1)%N); 
            put_fork(i,i);
        }
        
        m[i].unlock();
        m[(i+1)%N].unlock();
    }
    
    func(i);
}

int main() {
    thread philosophers[N];
    fout.open("Output.txt");
    
    for(int i=0; i<N; i++) {
        philosophers[i] = thread(func, i);
    }
    
    for(int i=0; i<N; i++) {
        philosophers[i].join();
    }
    
    fout.close();
    
    return 0;
}
