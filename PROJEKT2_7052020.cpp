#include "Graph.h"
#include <time.h>
#include <windows.h>
#include <iomanip>

const int VERTEX_SIZE = 5;
const int DENSITY_SIZE = 4;

const int GRAPH_AMOUNT = 100;
int vertex[VERTEX_SIZE] = { 10,50,100,500,250 };
int density[DENSITY_SIZE] = { 25,50,75,100 };				// w %

string fileName;

clock_t start, stop;
double timeList = 0;
double timeMatrix = 0;

int main()
{
	ofstream file(".\\Results\\Times.txt");
	checkFile(file);

	for (int i = 0; i < VERTEX_SIZE; i++)							//VERTEX_SIZE
	{
		file << i + 1 << " WIELKOSC" << endl << endl;
		for (int j = 0; j < DENSITY_SIZE; j++)						//DENSITY_SIZE
		{
			//GENEROWANIE GRAFOW !!! WYMAGANY FOLDER "Graph" W TYM SAMYM FOLDERZE CO "PROJEKT2_7052020.cpp" !!!
				//dla ustawien z petli
			randomGraph(vertex[i], density[j], GRAPH_AMOUNT);
			for (int graphNumber = 0; graphNumber < GRAPH_AMOUNT; graphNumber++)
			{

				fileName = ".\\Graph\\Graph" + to_string(graphNumber + 1) + ".txt";

				cout << "KONSTRUOWANIE LISTY" << endl;
				//.\\Graph\\Graph1.txt
				Graph l(fileName, 1);

				cout << "KONSTRUOWANIE MACIERZY" << endl;
				Graph m(fileName, 2);

				//l.displayList();
				//m.displayMatrix();
				cout << endl << endl << "========================" << endl << "ROZPOCZECIE SORTOWANIA LISTA" << endl;
				start = clock();
				l.dijkstraAlgorithmList();
				stop = clock();
				cout << endl << endl << "========================" << endl << "ZAKONCZENIE SORTOWANIA LISTY" << endl;
				timeList += (double)(stop - start) / CLOCKS_PER_SEC;

				cout << endl << endl << "========================" << endl << "ROZPOCZECIE SORTOWANIA MACIERZY" << endl;
				start = clock();
				m.dijkstraAlgorithmMatrix();
				stop = clock();
				cout << endl << endl << "========================" << endl << "ZAKONCZENIE SORTOWANIA MACIERZY" << endl;
				timeMatrix += (double)(stop - start) / CLOCKS_PER_SEC;

			}
			cout << setprecision(10);
			cout << "LISTA rozmiar(" << vertex[i] << ") gestosc(" << density[j] << ") ilosc " << GRAPH_AMOUNT << " CZAS: " << timeList << endl;
			cout << "MACIERZ rozmiar(" << vertex[i] << ") gestosc(" << density[j] << ") ilosc " << GRAPH_AMOUNT << " CZAS: " << timeMatrix << endl;
			cout << endl;

			file << j + 1 << " GESTOSC" << endl << endl;
			file << "LISTA rozmiar(" << vertex[i] << ") gestosc(" << density[j] << ") ilosc " << GRAPH_AMOUNT << " CZAS: " << timeList << endl;
			file << "MACIERZ rozmiar(" << vertex[i] << ") gestosc(" << density[j] << ") ilosc " << GRAPH_AMOUNT << " CZAS: " << timeMatrix << endl;
			file << endl;
		}
	}

	file.close();
	return 0;
}
