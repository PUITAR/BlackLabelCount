#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <ctime>
#include <windows.h>
#include <stdlib.h>
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
int v = 0;
vector<int> blackQueue;

// string dataPath = "test.txt";
string dataPath = "wiki-topcats.txt";
string resultPath = "base2Result.txt";

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


void writeResult(clock_t t) {
    ofstream fout(resultPath);
    for (int i = 0; i < vertexNumber; ++i) {
        cout << allVertex[i] << ' ' << g[allVertex[i]].twoHopBlackCount << endl;
        fout << allVertex[i] << ' ' << g[allVertex[i]].twoHopBlackCount << endl;
    }
    cout << "runtime: " << t << " ms" << endl;
    fout << "runtime: " << t << " ms" << endl;
}


void computeBlackLabel() {
    int v;
    for (int i = 0; i < vertexNumber; ++i) {
        v = allVertex[i];
        if (g[v].isBlack) {
            blackQueue.push_back(v);
            unordered_map<int, int> twoHopFriends;
            for (int j = 0; j < g[v].oneHopFriends.size(); ++j) {
                int k = g[v].oneHopFriends[j];
                if (twoHopFriends[k] == 0) {
                    ++g[k].twoHopBlackCount;
                    twoHopFriends[k] == 1;
                } 
                for (int r = 0; r < g[k].oneHopFriends.size(); ++r) {
                    int s = g[k].oneHopFriends[r];
                    if (twoHopFriends[s] == 0) {
                        ++g[s].twoHopBlackCount;
                        twoHopFriends[s] = 1;
                    }
                } 
            }
        }
        else continue;
    }
}




int 
main() {
    initGraph();
    assignBlackLabel();
    system("pause");
    clock_t start = clock();
    computeBlackLabel();
    clock_t end = clock();
    cout << "black queue size: " << blackQueue.size() << endl;
    printf("vertex number: %d\n", vertexNumber);
    printf("edge number: %d\n", lineCount);
    cout << "runtime: " << (end-start) << " ms" << endl; 
    cout << "write the result into file?" << endl;
    system("pause");
    writeResult(end-start);
    return 0;
}