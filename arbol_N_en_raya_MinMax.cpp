#include <iostream>
#include <vector>
#include <queue>
#include<algorithm>
using namespace std;

const char EMPTY = ' ';
struct Board {
    vector<vector<char>> grid;
    int posibilidad1, posibilidadia;
    //int factor = calcJ(board), calcIA(board)


    Board(int n) : grid(n, vector<char>(n, EMPTY)) {}

    void print() {
        int n = grid.size();

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) cout << "| " << grid[i][j] << " |";

            cout << std::endl << " ";

            cout << endl;
        }
    }

    bool makePlay1(char player, int x,int y) {
        int n = grid.size();
        int row = x;
        int col = y;

        if (row >= 0 && row < n && col >= 0 && col < n && grid[row][col] == EMPTY) {
            grid[row][col] = player;
            return true; // jugada valida
        }
        else {
            return false; // jugada no valida
        }
    }
    bool makePlay(char player, int position) {
        int n = grid.size();
        int row = (position - 1) / n;
        int col = (position - 1) % n;

        if (row >= 0 && row < n && col >= 0 && col < n && grid[row][col] == EMPTY) {
            grid[row][col] = player;
            return true; // jugada valida
        }
        else {
            return false; // jugada no valida
        }
    }

    bool checkWin(char player) {
        int n = grid.size();

        // Check rows and columns
        for (int i = 0; i < n; ++i) {
            bool rowWin = true;
            bool colWin = true;
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] != player) rowWin = false;
                if (grid[j][i] != player) colWin = false;
            }
            if (rowWin || colWin) return true;
        }

        // Check diagonals
        bool mainDiagWin = true;
        bool antiDiagWin = true;
        for (int i = 0; i < n; ++i) {
            if (grid[i][i] != player) mainDiagWin = false;
            if (grid[i][n - 1 - i] != player) antiDiagWin = false;
        }
        if (mainDiagWin || antiDiagWin) return true;

        return false; // No win
    }
    bool empate() {
        bool y = true;
        for (size_t t = 0; t < grid.size(); t++) {
            for (size_t r = 0; r < grid[t].size();r++) {
                if (grid[t][r] == ' ') {
                    y = false;
                }
            }
        }
        return y;
    }
};
vector<vector<int>> encontrarPosicionesVacias(vector<vector<char>> matriz) {
    vector<vector<int>> posiciones;

    for (int fila = 0; fila < matriz.size(); fila++) {
        for (int columna = 0; columna < matriz[fila].size(); columna++) {
            if (matriz[fila][columna] == EMPTY) {
                posiciones.push_back({ fila, columna });
            }
        }
    }

    return posiciones;
}

vector<vector<char>> copiar(vector<vector<char>> matriz) {
    vector<vector<char>> copia;

    for (int fila = 0; fila < matriz.size(); fila++) {
        vector<char> po;
        for (int columna = 0; columna < matriz[fila].size(); columna++) {

            po.push_back(matriz[fila][columna]);

        }
        copia.push_back(po);
    }

    return copia;
}

struct nodo {
public:
    vector<vector<char>>raya;
    vector<nodo*> next;
    int pos[2] = { 0,0 };
    bool minmax;
    bool name;
    int funcion;
    int minmax_v;
    int nivel;
    nodo(bool name, bool minmax1, int nivel, vector<vector<char>> raya1) {
        this->name = name;
        this->minmax = minmax1;
        this->nivel = nivel;
        this->raya = copiar(raya1);
    }
    void limpiarArbol() {
        for (auto& child : next) {
            if (child != nullptr) {
                child->limpiarArbol();  
                delete child;  
                child = nullptr;
            }
        }
        next.clear();  
    }
};

struct arbol {
public:
    nodo* head = NULL;
    int n;
    bool name;
    int nivel;
    
    arbol(int n1, bool name1) {
        this->n = n1;
        this->name = name1;
       
    }
    void arma_re(int &cont ,nodo* head1)
    {
        if (head1->nivel >= nivel) {
            int k=contarFormasGanadoras('x', head1->raya, n);
            int k1 = contarFormasGanadoras('o', head1->raya, n);
            if (name){
                head1->minmax_v= k - k1;
                /*cout << k << '\n';
                cout << k1 << '\n';*/
                //cout << k - k1 << '\n';
            }
            else {
                head1->minmax_v =k1-k;
                /*out << k << '\n';*/
                //cout << k1-k << '\n';
            }
            
            return;
        }
        else {
            vector<vector<int>> pos = encontrarPosicionesVacias(head1->raya);
            //imprimirMatriz(head1->raya);
            //cout << pos.size() << '\n';
            if (pos.size()) {

                for (size_t i = 0; i < pos.size(); i++) {
                    vector<vector<char>> temp = copiar(head1->raya);

                    if (!head1->name) {
                        temp[pos[i][0]][pos[i][1]] = 'x';
                    }
                    else {
                        temp[pos[i][0]][pos[i][1]] = 'o';
                    }
                    //imprimirMatriz(temp);
                    //cout << head1->name << '\n';

                    nodo* temp1 = new nodo(!head1->name, !head1->minmax, head1->nivel + 1, temp);
                    temp1->pos[0] = pos[i][0];
                    temp1->pos[1] = pos[i][1];
                    head1->next.push_back(temp1);
                    cont++;
                    //imprimirMatriz(temp);
                    arma_re(cont,temp1);


                }
                if (head1->next.size() > 0) {
                    vector<int> p;
                    //cout << "w" << '\n';
                    for (size_t i = 0; i < head1->next.size(); i++) {
                        p.push_back(head1->next[i]->minmax_v);
                        //cout << head1->next[i]->minmax_v << ' ';
                    }
                    //cout <<'\n';
                    if (head1->minmax) {
                        auto maximo = max_element(p.begin(), p.end());
                        int max_value = (maximo != p.end()) ? *maximo : INT_MIN;
                        head1->minmax_v = max_value;

                    }
                    else {
                       // cout << "entre" << '\n';
                        auto minimo = min_element(p.begin(), p.end());
                        int min_value = (minimo != p.end()) ? *minimo : INT_MAX;
                        head1->minmax_v = min_value;
                        //cout<< '\n'<< min_value << '\n';
                    }
                    
                }

            }
            else {
                return;
            }

        }

    }

    void armar(vector<vector<char>> p1,int nivel1,int nivel_A1) {
        if (nivel1 + nivel_A1 > (n * n)) {
            this->nivel = nivel1 - ((nivel1 + nivel_A1) - (n * n));
            cout << nivel<<'\n';
        }
        else {
            this->nivel = nivel1;
        }
        vector<vector<char>> p3=copiar(p1);
        head = new nodo(!name, 1, 0, p3);

        
        int cont = 0;
        //nodo(bool name, bool minmax1, int nivel, vector<vector<char>> raya1) 
        arma_re(cont,head);
        

    }
    void imprimirArbol() {
        if (!head) {
            cout << "Árbol vacío." << endl;
            return;
        }

        queue<nodo*> cola;
        cola.push(head);

        while (!cola.empty()) {
            int numNodosNivel = cola.size();
            while (numNodosNivel > 0) {
                nodo* temp = cola.front();
                cola.pop();

                imprimirMatriz(temp->raya);

                for (nodo* hijo : temp->next) {
                    cola.push(hijo);
                }

                numNodosNivel--;
            }
            cout << endl; 
        }
    }

    void imprimirMatriz(vector<vector<char>> matriz) {
        for (int i = 0; i < matriz.size(); ++i) {
            for (int j = 0; j < matriz[i].size(); ++j) {
                cout << matriz[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;



    };
    int contarFormasGanadoras(char jugador, vector<vector<char>> tablero, int n) {
        int formasGanadoras = 0;


        for (int i = 0; i < n; ++i) {
            bool x1 = false;
            bool x2 = true;
            for (int j = 0; j < n; ++j) {
                if (tablero[i][j] == jugador || tablero[i][j] == EMPTY) {
                    x1 = true;
                }
                else if (tablero[i][j] != EMPTY) {
                    x2 = false;
                    break;
                }
            }
            if (x2 && x1) formasGanadoras++;
        }


        for (int j = 0; j < n; ++j) {
            bool x1 = false;
            bool x2 = true;
            for (int i = 0; i < n; ++i) {
                if (tablero[i][j] == jugador || tablero[i][j] == EMPTY) {
                    x1 = true;
                }
                else if (tablero[i][j] != EMPTY) {
                    x2 = false;
                    break;
                }
            }
            if (x2 && x1) formasGanadoras++;
        }


        bool x1 = false;
        bool x2 = true;
        for (int i = 0; i < n; ++i) {
            if (tablero[i][i] == jugador || tablero[i][i] == EMPTY) {
                x1 = true;

            }
            else if (tablero[i][i] != EMPTY) {
                x2 = false;
                break;
            }
        }
        if (x2 && x1) formasGanadoras++;
        x1 = false;
        x2 = true;
        for (int i = 0; i < n; ++i) {
            if (tablero[i][n - 1 - i] == jugador || tablero[i][n - 1 - i] == EMPTY) {
                x1 = true;

            }
            else if (tablero[i][n - 1 - i] != EMPTY) {
                x1 = false;
                break;

            }
        }
        if (x2 && x1) formasGanadoras++;

        return formasGanadoras;
    }
    vector<int> busca(nodo *&xs) {
            cout << xs->next.size() << '\n';
            for (size_t t = 0; t < xs->next.size(); t++) {
                if (xs->next[t]->minmax_v == xs->minmax_v) {
                    xs = xs->next[t];
                    return vector<int>{xs->pos[0], xs->pos[1]};
                }
            }
        }
};


int main() {
    int nxn, profundidad, primero;
    char caracter;
    cout << "--------- N en raya -----------" << endl << "Ingrese el valor de n: ";
    cin >> nxn;

    cout << endl;

    cout << "Ingrese la profundidad del arbol: ";
    cin >> profundidad;

    cout << endl;

    cout << "Quien jugara primero (0: empieza la ia, 1 : empieza el jugador) ";
    cin >> primero;

    cout << "Con qué quieres jugar ('x'' u 'o') ";
    cin >> caracter;

    char currentPlayer;

    if (primero == 1) {
        currentPlayer = caracter;
    }
    else {
        if (caracter == 'x') {
            currentPlayer = 'o';
        }
        else if (caracter == 'o') {
            currentPlayer = 'x';
        }
    }
    bool caracter_arbol = 0;
    if (caracter == 'x') {
        caracter_arbol = 1;
    }
    int position;
    bool gameOver = false;

    int turno = 0;

    Board tablero(nxn);
    //arbol(int n1, bool name1) {
    arbol arbol_nxn (nxn, !caracter_arbol);

    // Si la ia comienza primero
    if (primero == 0) {
        //logica del turno de la ia
        //movimientoIA(tablero.grid);

        // cambio de jugador despues del movimiento
        

        arbol_nxn.armar(tablero.grid, profundidad, turno);
        nodo* p = arbol_nxn.head;
        vector<int> resultado1 = arbol_nxn.busca(p);
        tablero.makePlay1(currentPlayer, resultado1[0], resultado1[1]);
        arbol_nxn.head->limpiarArbol();
        delete arbol_nxn.head;
        arbol_nxn.head = NULL;
        turno++;
        currentPlayer = (currentPlayer == 'x') ? 'o' : 'x';
    }

    while (!gameOver) {

        cout << endl << "-----------------------" << endl << endl;
        cout << "Current board:" << endl << endl;
        tablero.print();
         if (tablero.empate()) {
            cout << "Empate" << '\n';
            break;
        }

        if (turno % 2 == 0 && primero == 1) { // turno del jugador
            cout << "Player " << currentPlayer << ", enter the position to make your play (from 1 to " << nxn * nxn << "): ";
            cin >> position;
            tablero.makePlay(currentPlayer, position);
        }
        else if (turno % 2 == 1 && primero == 0) { // turno del jugador
            cout << "Player " << currentPlayer << ", enter the position to make your play (from 1 to " << nxn * nxn << "): ";
            cin >> position;
            tablero.makePlay(currentPlayer, position);
        }
        else {
            //logica del turno de la ia
            //movimientoIA(tablero.grid);
            arbol_nxn.armar(tablero.grid, profundidad, turno);
            nodo* p = arbol_nxn.head;
            vector<int> resultado1 = arbol_nxn.busca(p);
            tablero.makePlay1(currentPlayer, resultado1[0], resultado1[1]);

            arbol_nxn.head->limpiarArbol();
            delete arbol_nxn.head;
            arbol_nxn.head = NULL;

        }

        if (tablero.checkWin(currentPlayer)) {
            cout << "Player " << currentPlayer << " wins!" << endl;
            tablero.print();
            gameOver = true;

        }
     

        else {
            // Cambiar al siguiente jugador
            currentPlayer = (currentPlayer == 'x') ? 'o' : 'x';
        }
        
        turno++;
    }

    return 0;
}
