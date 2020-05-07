#ifndef _GRAPH_H_
#pragma once
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <cstdlib>
#include <limits>
#include <memory>

using namespace std;



const int INF = 999999;

void randomGraph(int f_vertex, int f_density, int f_amount);
void checkFile(ofstream& path);

class Vertex
{
public:
    Vertex(int vertexNumberm, int weight);
    Vertex();
    ~Vertex() {}
    void set_Vertex(int vertexNumberm, int weight);
    int returnVertexNumber() { return _vertexNumber; }
    int returnWeight() { return _weight; }
    void changeWeight(int weight);

    int _vertexNumber;
    int _weight;
};

class Graph
{
public:
    //type = 1 LISTA | type = 2 MACIERZ
    Graph(string filePath, int type);
    ~Graph() {}

    void dijkstraAlgorithmMatrix();
    void dijkstraAlgorithmList();

    void displayMatrix();
    void displayList();

    //void savePathToFile(int* dist);


    Vertex** _representation;
    int* _adjListSize;
    int _edge;
    int _vertex;
    int _beginVertex;
    string _resultsFile;

    //pomocnicze do dijkstry
    int findMinVertex(int* distance, bool* visited);
    //do zapisu wyniku grafu w funkcji dijkstraAlgorithmMatrix() lub dijkstraAlgorithmList()
    //void savePathToFile(int* dist);
};

#endif