#include "Graph.h"

void randomGraph(int f_vertex, int f_density, int f_amount) // 10, 25(%), 1000, 100
{
    for (int graphNum = 0; graphNum < f_amount; graphNum++)
    {
        fstream writeFile;
        string nameFile = ".\\Graph\\Graph" + to_string(graphNum + 1) + ".txt";
        writeFile.open(nameFile, ios::out);

        int edge = f_density * f_vertex * (f_vertex - 1) / 200;
        writeFile << edge << " " << f_vertex << " " << 0 << endl; // 11 10 0 //ilosc_krawedzi ilosc_wierzcholkow wierzcholek_startowy

        int weight = 0;

        //POLACZENIE RESZTY WIERZCHOLKOW  MIEDZY SOBA LOSOWO
        //tab kwadratowa
        bool** tempTab = new bool* [f_vertex];
        for (int i = 0; i < f_vertex; i++)
        {
            tempTab[i] = new bool[f_vertex];
        }
        //wyzerowanie tempTab
        for (int i = 0; i < f_vertex; i++)//0...9
        {
            for (int j = 0; j < f_vertex; j++)
            {
                tempTab[i][j] = false;

                if (i == j)
                {
                    tempTab[i][j] = true;
                }
            }
        }
        //POLACZENIE WSZYSTKICH WIERZCHOLKOW PO KOLEI 0-1-2-3-4-5-6-7-8-9-0
        cout << "LACZENIE WIERZCOLKOW PO KOLEI" << endl;
        for (int i = 1; i < f_vertex; i++)
        {
            weight = rand() % f_vertex + 1; // 1...10 dla 10 wierzcholkow
            writeFile << i - 1 << " " << i << " " << weight << endl;
            tempTab[i - 1][i] = true;
            tempTab[i][i - 1] = true;
            if (i == 9)
            {
                writeFile << i << " " << 0 << " " << weight << endl;
                edge--;
                tempTab[0][i] = true;
                tempTab[i][0] = true;
            }
            edge--;
        }

        int randVertexBegin = 0;
        int randVertexEnd = 0;
        bool check = false;

        cout << "LOSOWANIE RESZTY KRAWEDZI DO ODPOWIEDNICH WIERZCHOLKOW" << endl;
        //pozostala liczba krawedzi do rozlosowania
        for (int i = 0; i < edge; i++)
        {
            //losowanie wierzcholka DO

            do
            {
                //losowanie wierzcholka OD 
                randVertexBegin = rand() % f_vertex;//0...9

                check = false;
                randVertexEnd = rand() % f_vertex; //0...9

                // jesli jest sasiadem np 0 1 lub 0 9 
                if (tempTab[randVertexBegin][randVertexEnd] == true) // dla tab [0][1] jest 1 wiec nie moze byc taki randVertexEnd
                {
                    check = true;
                }

                if (randVertexBegin == randVertexEnd) // te same liczby np 2 2
                {
                    check = true;
                }

                if (!check) // jesli nie wystapilo takie zdarzenie to zapisujemy to polaczenie do tablicy
                {
                    tempTab[randVertexBegin][randVertexEnd] = true;
                    tempTab[randVertexEnd][randVertexBegin] = true;
                }
            } while (check);
            writeFile << randVertexBegin << " " << randVertexEnd << " ";

            //losowanie WAGI
            weight = rand() % f_vertex + 1;
            writeFile << weight << endl;
        }


        //usuwanie tempTab
        for (int i = 0; i < f_vertex; i++)
        {
            delete[] tempTab[i];
        }
        delete[] tempTab;

        writeFile.close();

        cout << "UTWORZONO GRAF NR" << graphNum + 1 << endl;
    }
}
void checkFile(ofstream& file)
{
    if (!file.is_open())
    {
        std::cout << "File is not open!\nProgram closing...";
        std::cin.get();
        exit(1);
    }
}

Vertex::Vertex(int vertexNumber, int weight)
{
    _weight = weight;
    _vertexNumber = vertexNumber;
}
Vertex::Vertex()
{
    _weight = 0;
    _vertexNumber = 0;
}
void Vertex::set_Vertex(int vertexNumber, int weight)
{
    _weight = weight;
    _vertexNumber = vertexNumber;
}
void Vertex::changeWeight(int weight)
{
    _weight = weight;
}

Graph::Graph(string filePath, int type)
{
    if (type == 1) // 1 - lista | 2 - macierz
    {
        _edge = 0;
        _vertex = 0;
        _beginVertex = 0;

        // BLOK ODCZYTU GRAFU Z PLIKU I SPRAWDZENIE POPRAWNOSCI
        ifstream file;
        file.open(filePath);

        if (file.good() == false)
        {
            cout << "Blad wczytania pliku. Kod 7001." << endl;
            exit(7001);
        }

        file >> _edge >> _vertex >> _beginVertex;
        file.close();

        if (_vertex == 0)
        {
            cout << "Brak wierzcholkow w grafie! Kod 1000" << endl;
            exit(1000);
        }
        //=================================================================================
        //TABLICE POMOCNICZE
        int tempTab[3];
        int* tempSizeX = new int[_edge];
        for (int i = 0; i < _edge; i++)
        {
            tempSizeX[i] = 0;
        }

        //OS Y LISTY np. dla ilosc wierzcholkow (_vertex) 10 tworzy tablice od 0 do 9
        _representation = new Vertex * [_vertex];
        //tablica przechowujaca ilosc polaczen w liscie dla danego wierzcholka
        _adjListSize = new int[_vertex];

        //STWORZENIE POLACZEN OD WIERZCHOLKA (W OSI X)
        for (int i = 0; i < _vertex; i++)
        {
            file.open(filePath);
            //zabranie wartosci z 1 linijki
            file >> tempTab[0] >> tempTab[1] >> tempTab[2];
            tempSizeX[i] = 0;

            //dla danego wierzcholka szukamy wszystkich polaczen i zliczamy je do tempSizeX
            for (int j = 0; j < _edge; j++)
            {
                file >> tempTab[0] >> tempTab[1] >> tempTab[2];
                if (tempTab[0] == i || tempTab[1] == i)
                {
                    tempSizeX[i] += 1;
                }
            }

            //TWORZENIE NA OSI X LISTY DLA DANEGO WIERZCHOLKA 0->1->5->9
            //                                                |
            _representation[i] = new Vertex[tempSizeX[i]];
            //zapisanie informacji nt. ilosci polaczen danego wierzcholka
            _adjListSize[i] = tempSizeX[i];
            file.close();
        }

        //INICJALIZACJA TYCH WIERZCHOLKOW DO LISTY
        for (int i = 0; i < _vertex; i++)
        {
            file.open(filePath);
            //zabranie 1 linijki
            file >> tempTab[0] >> tempTab[1] >> tempTab[2];
            //"j" zlicza ilosc ju¿ dokonanych polaczen
            int j = 0;

            //przejscie po calym pliku (ilosc krwawedzi to ilosc linijek w pliku)
            for (int k = 0; k < _edge; k++)
            {
                file >> tempTab[0] >> tempTab[1] >> tempTab[2];
                //jesli wierzcholek "i" (np. 0) jest wierzcholkiem wystepujacym w pobranej linijce
                if (i == tempTab[0])
                {
                    //wystepuje na pierwszym miejscu to zapisz wierzcholek z drugiego miejsca (np. 0 1 55 to dla wierzcholka 0 -> 1(55) )
                    _representation[i][j].set_Vertex(tempTab[1], tempTab[2]);
                    j++;
                }
                else if (i == tempTab[1])
                {
                    //odwrotnie do porzedniego if'a
                    _representation[i][j].set_Vertex(tempTab[0], tempTab[2]);
                    j++;
                }
                //jesli juz wszystkie polaczenia zostaly dobrane a sa jeszcze informacje z pliku to przerwij i idz do nastepnego wierzcholka (bo juz nie bedzie wiecej polaczen)
                if (j >= tempSizeX[i])
                {
                    break;
                }
            }
            file.close();
        }
    }
    else if (type == 2)
    {
        _edge = 0;
        _vertex = 0;
        _beginVertex = 0;
        _adjListSize = nullptr;

        ifstream file;
        file.open(filePath);

        if (file.good() == false)
        {
            cout << "Blad wczytania pliku. Kod 7001." << endl;
            exit(7001);
        }

        file >> _edge >> _vertex >> _beginVertex;
        file.close();

        if (_vertex == 0)
        {
            cout << "Brak wierzcholkow w grafie! Kod 1000" << endl;
            exit(1000);
        }

        int tempTab[3];
        //stworzenie macierzy kwadratowej o rozmiarze _vertex
        _representation = new Vertex * [_vertex];
        for (int i = 0; i < _vertex; i++)
        {
            _representation[i] = new Vertex[_vertex];
        }
        /*
        //WYZEROWANIE MACIERZY
        for (int i = 0; i < _vertex; i++)
        {
            for (int j = 0; j < _vertex; j++)
            {
                _representation[i][j] = Vertex();
            }
        }*/
        //zapelnienie macierzy relacjami wierzcholkow grafu
        //int *tempTabOfVertex = new int*_edge;

        for (int i = 0; i < _vertex; i++)
        {
            int j = 0;
            file.open(filePath);
            file >> tempTab[0] >> tempTab[1] >> tempTab[2]; // wyczyszczenie 1 linii
            for (int k = 0; k < _edge; k++)
            {
                file >> tempTab[0] >> tempTab[1] >> tempTab[2];
                if (tempTab[0] == i)
                {
                    _representation[i][tempTab[1]].set_Vertex(tempTab[1], tempTab[2]);
                    _representation[tempTab[1]][i].set_Vertex(tempTab[0], tempTab[2]);
                    j++;
                }
                else if (tempTab[1] == i)
                {
                    _representation[i][tempTab[0]].set_Vertex(tempTab[0], tempTab[2]);
                    _representation[tempTab[0]][i].set_Vertex(tempTab[1], tempTab[2]);
                    j++;
                }
            }
            file.close();
        }
    }
}

int Graph::findMinVertex(int* distance, bool* visited)
{
    int minVertex = -1;
    for (int i = 0; i < _vertex; i++)
    {
        if (!visited[i] && (minVertex == -1 || distance[i] < distance[minVertex]))
        {
            minVertex = i;
        }
    }
    return minVertex;
}
void Graph::dijkstraAlgorithmMatrix()
{
    //dystans od 0 dla danego wierzcholka
    int* distance = new int[_vertex];
    bool* visited = new bool[_vertex];

    for (int i = 0; i < _vertex; i++)
    {
        distance[i] = INF;
        visited[i] = false;
    }
    distance[0] = 0;

    for (int i = 0; i < _vertex - 1; i++)
    {
        //wierzcholek ktory ma najkrotsza droge
        int minVertex = findMinVertex(distance, visited);
        visited[minVertex] = true;
        //cout << "WIERZCHOLEK: " << minVertex << endl;
        for (int j = 0; j < _vertex; j++)
        {
            //jesli jest polaczenie od tego wierzcholka i wierzcholek ten nie jest juz przeszukany
            if (_representation[minVertex][j].returnWeight() != 0 && !visited[j])
            {
                //dystans od poprzedniego do przeszukiwanego wierzcholka
                int dist = distance[minVertex] + _representation[minVertex][j].returnWeight();
                //jak dystans jest mniejszy niz byl to zapisz
                if (dist < distance[j])
                {
                    distance[j] = dist;
                }
            }
            //cout << "DYSTANS do "<< j << " wierzcholka: " << distance[j] << endl;
        }

    }

    /*
    for (int i = 0; i < _vertex; i++)
    {
        cout << i << " DYSTANS " << distance[i] << endl;
    }
    */

    delete[] visited;
    delete[] distance;
}
void Graph::dijkstraAlgorithmList()
{
    int* distance = new int[_vertex];
    bool* visited = new bool[_vertex];

    for (int i = 0; i < _vertex; i++)
    {
        distance[i] = INF;
        visited[i] = false;
    }
    distance[0] = 0;

    for (int i = 0; i < _vertex - 1; i++)
    {
        //wierzcholek ktory ma najkrotsza droge
        int minVertex = findMinVertex(distance, visited);
        visited[minVertex] = true;
        //cout << "WIERZCHOLEK: " << minVertex << endl;
        for (int j = 0; j < _adjListSize[minVertex]; j++)
        {
            //jesli wierzcholek ten nie jest juz przeszukany
            if (!visited[_representation[minVertex][j].returnVertexNumber()])
            {
                //dystans od poprzedniego do przeszukiwanego wierzcholka
                int dist = distance[minVertex] + _representation[minVertex][j].returnWeight();
                //jak dystans jest mniejszy niz byl dla tego wierzcholka to zapisz
                if (dist < distance[_representation[minVertex][j].returnVertexNumber()])
                {
                    distance[_representation[minVertex][j].returnVertexNumber()] = dist;
                }
            }
            //cout << "DYSTANS do " << _representation[minVertex][j].returnVertexNumber() << " wierzcholka: " << distance[_representation[minVertex][j].returnVertexNumber()] << endl;
        }

    }
    /*
    for (int i = 0; i < _vertex; i++)
    {
        cout << i << " DYSTANS " << distance[i] << endl;
    }*/

    delete[] visited;
    delete[] distance;
}

void Graph::displayMatrix()
{
    cout << endl << "DISPLAY MATRIX" << endl;
    for (int i = 0; i < _vertex; i++)
    {
        cout << i << "| ";
        for (int j = 0; j < _vertex; j++)
        {
            if (_representation[i][j].returnWeight() != 0)
            {
                cout << _representation[i][j].returnVertexNumber() << "(" << _representation[i][j].returnWeight() << ") ";
            }
            else
            {
                cout << "XXXX ";
            }
        }
        cout << endl;
    }
}
void Graph::displayList()
{
    cout << endl << "DISPLAY LIST" << endl;
    for (int i = 0; i < _vertex; i++)
    {
        cout << i << "| ";
        for (int j = 0; j < _adjListSize[i]; j++)
        {
            if (_representation[i][j].returnWeight() != 0)
            {
                cout << _representation[i][j].returnVertexNumber() << "(" << _representation[i][j].returnWeight() << ") ";
            }
            else
            {
                cout << "XXXX ";
            }
        }
        cout << endl;
    }
}
