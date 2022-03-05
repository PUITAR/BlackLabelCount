#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <sys/time.h>
#include <windows.h>
#include <thread>
#include <mutex>
#include <set>
#include <algorithm>
using namespace std;


struct info
{
    bool isBlack = 0;
    int twoHopBlackCount = 0;
    vector<int> oneHopFriends;
};

unordered_map<int, info> g;
vector<int> allVertex;
int vertexNumber = 0;
int lineCount = 0;
int gap = 1;
vector<int> blackQueue;
mutex m;


// string dataPath = "test.txt";
string dataPath = "wiki-topcats.txt";
string resultPath = "threadVertionResult.txt";

void initGraph(int num = 0) {
    ifstream fin(dataPath);
    int a, b;
    if (num == 0) {
        while (fin >> a >> b) {
        g[a].oneHopFriends.push_back(b);
        g[b].oneHopFriends.push_back(a);
        ++lineCount;
        }
    } else {
        while (num--) {
            fin >> a >> b;
            g[a].oneHopFriends.push_back(b);
            g[b].oneHopFriends.push_back(a);
            ++lineCount;
        }
    }
    vertexNumber = g.size();
    unordered_map<int, info>::iterator it;
    for (it = g.begin(); it != g.end(); ++it) 
        allVertex.push_back(it->first);
}

void assignBlackLabel() {
    float ratio = -1;
    while (ratio < 0 || ratio > 1) {
        cout << "black proportion: ";
        cin >> ratio;
    }
    int blackNumber = vertexNumber * ratio;
    cout << "vertex number: " << vertexNumber << endl;
    cout << "edge number: " << lineCount << endl;
    cout << "black number: " << blackNumber << endl;
    for (int i = 0; i < blackNumber; ++i) {
        int a = rand() % vertexNumber;
        g[allVertex[a]].isBlack = 1;
    }
}


void writeResult(unsigned long t) {
    ofstream fout(resultPath);
    for (int i = 0; i < vertexNumber; ++i) {
        cout << allVertex[i] << ' ' << g[allVertex[i]].twoHopBlackCount << endl;
        fout << allVertex[i] << ' ' << g[allVertex[i]].twoHopBlackCount << endl;
    }
    cout << "runtime: " << t << " s" << endl;
    fout << "runtime: " << t << " s" << endl;
}


void findBlackLabelID() {
    int v;
    for (int i = 0; i < vertexNumber; ++i) {
        v = allVertex[i];
        if (g[v].isBlack) {
            blackQueue.push_back(v);
        }
        else continue;
    }
}



    


void computeTwoHopBlackNum(int i) {
    while (i < blackQueue.size()) {
        set<int> twoHopFriends;
        int v = blackQueue[i];
        for (int j = 0; j < g[v].oneHopFriends.size(); ++j) {
            int u = g[v].oneHopFriends[j];
            twoHopFriends.insert(u);
            for (int r = 0; r < g[u].oneHopFriends.size(); ++r)
                twoHopFriends.insert(g[u].oneHopFriends[r]);
        }
        set<int>::iterator it;
        m.lock();
        for (it = twoHopFriends.begin(); it != twoHopFriends.end(); ++it)
            ++g[*it].twoHopBlackCount;
        m.unlock();
        i += gap;
    }
}


bool cmp(int a, int b) {
    return g[a].oneHopFriends.size() < g[b].oneHopFriends.size();
}


int 
main() {
    initGraph();
    assignBlackLabel();
    
    cout << "thread number(<=12): ";
    cin >> gap;
    thread threads[gap];
    timeval start, end;
    unsigned long timer;
    findBlackLabelID();
    // sort(blackQueue.begin(), blackQueue.end(), cmp);
    gettimeofday(&start, NULL);
    
    sort(blackQueue.begin(), blackQueue.end(), cmp);
    cout << "black size: " << blackQueue.size() << endl;
    for (int i = 0; i < gap; ++i) {
        threads[i] = thread(computeTwoHopBlackNum, i);
    }
    for (int i = 0; i < gap; ++i) {
        threads[i].join();
    }
    gettimeofday(&end, NULL);
    timer = (1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec)/1000000;
    printf("vertex number: %d\n", vertexNumber);
    printf("edge number: %d\n", lineCount);
    cout << "runtime: " << timer << " s" << endl; 
    cout << "write the result into file?" << endl;
    // system("pause");
    // writeResult(timer);
    return 0;
}