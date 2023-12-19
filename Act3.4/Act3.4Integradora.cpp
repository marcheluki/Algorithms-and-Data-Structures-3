#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
using namespace std;

struct Node {
  int port;  //key
  vector<string> ipAdd; //valor (IP addresses)
  int count; // cant (number of accesses)
  Node *left, *right; //punteros a los hijos
  Node(int& p) : port(p), count(1), left(nullptr), right(nullptr) {}
};

class BST {
  private:
      Node *Root; // raíz del árbol
      void Insert(int&, string&, Node*&);
      Node* Search(Node*, int); 

    public:
      BST(): Root(NULL) {} //Constructor
      //~BST(){ //Destructor
        //DeleteBST(Root);
        //cout << "\nDestructor: BST eliminado\n";
      //}
    void Insert(int &port, string &ipAddress) {Insert(port, ipAddress, Root);}; 
    void PrintNode(Node*);  
    void PrintBST();
    Node* Search(int); 
    //void DeleteBST(Node*&);
};

//Insert
void BST::Insert(int &port, string &ipAddress, Node*& currentNode) {
  if(currentNode == NULL){ //si el árbol está vacío
    currentNode = new Node (port); //crear nuevo nodo
    currentNode -> count = 1; //inicializar el contador en 1
    currentNode -> ipAdd.push_back(ipAddress); //agregar la ip al vector

  } else {
    if(port < currentNode->port){ //si el puerto es menor al nodo actual
      Insert(port, ipAddress, currentNode->left); //insertar en el subárbol izquierdo
    } else if(port > currentNode->port){ //si el puerto es mayor al nodo actual
      Insert(port, ipAddress, currentNode->right); //insertar en el subárbol derecho
    } else {
      currentNode -> count += 1; //si el puerto es igual al nodo actual, aumentar el contador
      currentNode -> ipAdd.push_back(ipAddress); //agregar la ip al vector
    }
  }
}

void BST::PrintNode(Node* currentNode) { //Print Node
  if(currentNode != NULL){ 
    int i = 0;
    cout << "Port: " << currentNode->port << endl; //
    cout << "Number of accesses: " << currentNode->count << endl;
    cout << "IP Addresses: ";
    while(i < currentNode->ipAdd.size()){
      cout << currentNode->ipAdd[i] << " ";
      i++;
      if(i == currentNode->ipAdd.size()){ //si se llega al final del vector
        cout << endl; //salto de línea
      }
    }
  } else {
    cout << "El nodo ingresado no existe." << endl; 
  }
}

//Print BST
void BST::PrintBST() { 
  if(Root == NULL){
    cout << "No se encontraron datos" <<endl; //si el árbol está vacío
    return; //terminar la función
  }

  queue<Node*> Q; //cola para recorrer el árbol
  Q.push(Root); //agregar la raíz a la cola

  Node *Aux;
  while(!Q.empty()){ //mientras la cola no esté vacía
    Q.push(NULL); //agregar un NULL para indicar el final de cada nivel
    Aux = Q.front(); //nodo auxiliar para recorrer el árbol

    while(Aux != NULL){
      PrintNode(Aux); //imprimir el nodo actual

      if(Aux->left != NULL){
        Q.push(Aux->left);  //agregar el hijo izquierdo a la cola
      }

      if(Aux->right != NULL){
        Q.push(Aux->right); //agregar el hijo derecho a la cola
      }

      Q.pop(); //eliminar el nodo actual
      Aux = Q.front(); //actualizar el nodo auxiliar
    }

    Q.pop(); //eliminar el NULL
    cout << endl; //salto de línea
  }
}

//Comparar el número de accesos de cada puerto
bool CompareCount(pair<int, Node*>& a, pair<int, Node*>& b){ 
  return a.second->count > b.second->count; //ordenar de mayor a menor
}

//Search
Node* BST::Search(int portNum) {
    return Search(Root, portNum); //llamar a la función recursiva
}

Node* BST::Search(Node* currentNode, int portNum) {
    if (currentNode == nullptr || currentNode->port == portNum) { //si el nodo es nulo o el puerto es igual al nodo actual
        return currentNode; //regresar el nodo actual
    }

    if (portNum < currentNode->port) {
        return Search(currentNode->left, portNum); //buscar en el subárbol izquierdo
    } else {
        return Search(currentNode->right, portNum); //buscar en el subárbol derecho
    }
}


int main() {
  //Abrir archivo
  ifstream inputFile("ordenadas.txt");
  if (!inputFile.is_open()) {
    cerr << "Error: No se pudo abrir el archivo." << endl;
    return 1;
  }

  //Crear el árbol
  BST Tree; 
  map<int, Node*> portMap; //mapa para almacenar los puertos y sus nodos

  string mes, fecha, ipAddress, razon; //variables para almacenar los datos del archivo
  int dia, portNum; //variables para almacenar los datos del archivo

  while (inputFile >> mes >> dia >> fecha >> ipAddress) { 
    getline(inputFile, razon);  
    razon = razon.substr(1);
    portNum = stoi(ipAddress.substr(ipAddress.find(":") + 1)); //obtener el puerto
    Tree.Insert(portNum, ipAddress); //insertar el puerto en el árbol
        if (portMap.find(portNum) == portMap.end()) {
            // If it doesn't exist, create a new entry
            portMap[portNum] = Tree.Search(portNum);
        }
  }

  //Tree.PrintBST(); //imprimir el árbol
  //cout << endl;

  cout << "Top 5 Ports with the Most Accesses (including their IPs):" << endl;
  //Five ports with most accesses
  vector<pair<int, Node*>> topPorts(portMap.begin(), portMap.end());
  sort(topPorts.begin(), topPorts.end(), &CompareCount);

  for (int i = 0; i < 5 && i < topPorts.size(); ++i) {
    pair<int, Node*> topPort = topPorts[i];
    cout<<"\n"<<i+1<<") ";
    cout << "Port: " << topPort.first << endl<<"\nIPs: "<<endl;
    
    for (const string& ip : topPort.second->ipAdd) {
      cout << ip << " "<< endl;
    }
    cout << "\nTotal Accesses: " << topPort.second->count << endl;
  }


// Summarized output of found ports
cout << "\nTop 5 Ports with the Most Accesses (Summarized):" << endl;

for (int i = 0; i < 5 && i < topPorts.size(); ++i) {
    pair<int, Node*> topPort = topPorts[i];
    cout << i + 1 << ") ";
    cout << "Port: " << topPort.first;
    cout << ", Total Accesses: " << topPort.second->count << endl;
}


  inputFile.close();
  return 0;
}