#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <ctime>
#include <windows.h>
#include <pthread.h>
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

// string dataPath = "test.txt";
string dataPath = "wiki-topcats.txt";
string resultPath = "base1Result.txt";

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

void computeBlackLabel() {
    int count, v;
    for (int i = 0; i < vertexNumber; ++i) {
        v = allVertex[i];
        count = 0;
        unordered_map<int, int> twoHopFriends;
        for (int j = 0; j < g[v].oneHopFriends.size(); ++j) {
            int k = g[v].oneHopFriends[j];
            if (twoHopFriends[k] == 0) {
                if (g[k].isBlack) ++count;
                twoHopFriends[k] == 1;
            } 
            for (int r = 0; r < g[k].oneHopFriends.size(); ++r) {
                int s = g[k].oneHopFriends[r];
                if (twoHopFriends[s] == 0) {
                    if (g[s].isBlack) ++count;
                    twoHopFriends[s] = 1;
                }
            }
                
        }
        g[v].twoHopBlackCount = count;
    }
}

void writeResult(clock_t t) {
    ofstream fout(resultPath);
    for (int i = 0; i < vertexNumber; ++i) {
        cout << allVertex[i] << ' ' << g[allVertex[i]].twoHopBlackCount << endl;
        fout << allVertex[i] << ' ' << g[allVertex[i]].twoHopBlackCount << endl;
    }
    cout << "runtime: " << t << 's' << endl;
    cout << (vertexNumber+0.0)/t << "/s" << endl; 
    fout << "runtime: " << t << 's' << endl;
    fout << (vertexNumber+0.0)/t << "/s" << endl; 
}

int 
main() {
    initGraph(285118);
    assignBlackLabel();
    system("pause");
    clock_t start = clock();
    computeBlackLabel();
    clock_t end = clock();
    printf("vertex number: %d\n", vertexNumber);
    printf("edge number: %d\n", lineCount);
    cout << "runtime: " << (end-start)/1000 << " s" << endl; 
    cout << "write the result into file?" << endl;
    system("pause");
    writeResult((end-start)/1000);
    return 0;
}

