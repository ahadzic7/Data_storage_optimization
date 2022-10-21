#include <vector>
#include <list>
#include <tuple>
#include <algorithm>
#include "Utils.h"

using namespace std;


int levenstheinDistance(const vector<int> &x, const vector<int> &y) {
    const int SIZEx = x.size();
    const int SIZEy = y.size();

    vector<int> lev1(SIZEy + 1, 0);
    vector<int> lev2(SIZEy + 1, 0);

    for(int i = 1; i <= SIZEy; i++) {
        lev1[i] = i;
    }

    for(int i = 0; i < SIZEx; i++) {
        lev2[0] = i + 1;

        for(int j = 0; j < SIZEy; j++) {
            int deleteCost = lev1[j + 1] + 1;
            int insertCost = lev2[j] + 1;
            int subCost = lev1[j];

            if(x[i] != y[j])
                subCost++;

            int pom = min(deleteCost, min(insertCost, subCost));
            lev2[j + 1] = pom;
        }

        std::swap(lev1, lev2);
    }

    return lev1[SIZEy];
}

void buildGraph(const vector<vector<int>> &records, vector<tuple<int, int, int>> &edges) {
    int RSIZE (records.size());

    for(int i = 0; i < RSIZE; i++) {
        for(int j = 0; j < RSIZE; j++) {
            if(i >= j) continue;
            edges.emplace_back(i, j, levenstheinDistance(records[i], records[j]));
        }
    }
}

int mstFind(int town, std::vector<int> &boss) {
    return (boss[town] == town ? town : (boss[town] = mstFind(boss[town], boss)));
}

int mstKruskal(vector<tuple<int, int, int>> &edges, const int records) {
    sort(edges.begin(), edges.end(), [] (const tuple<int, int, int> &x, const tuple<int, int, int> &y) { return get<2>(x) < get<2>(y); });

    std::vector<int> boss(records);
    std::vector<int> rank(records, 0);

    for(int i = 0; i < records; i++)
        boss[i] = i;

    int mstCost = 0;
    for(const auto &edge: edges) {

        int x = get<0>(edge);
        int y = get<1>(edge);

        int a = mstFind(x, boss);
        int b = mstFind(y, boss);

        if(a != b)
            mstCost += get<2>(edge);

        if(rank[b] > rank[a])
            boss[a] = b;
        else {
            boss[b] = a;
            if(rank[b] == rank[a])
                rank[a]++;
        }
    }

    return mstCost;
}

int main(int argc, char *argv[]) {
    auto programArguments = ProgramArguments::Parse(argc, argv);

    // The input records, e.g., records[0] is the first record in the input file.
    vector<vector<int>> records = readRecords(programArguments.mInputFilePath);

    // TODO: fill the treeCost variable with the MST of the records' edit distances graph.
    vector<tuple<int, int, int>> edges;
    buildGraph(records, edges);


    int treeCost = mstKruskal(edges, records.size());
//    int treeCost = levenstheinDistance(records[0], records[1]);

    cout << treeCost << endl;
    writeCost(treeCost, programArguments.mOutputFilePath);
}
