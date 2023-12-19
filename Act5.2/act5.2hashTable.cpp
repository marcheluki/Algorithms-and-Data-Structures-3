#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <math.h>
#include <list>
#include <random>
using namespace std;

// El formato del archivo “bitacora.txt” es el siguiente:
//Mes en formato de 3 letras
//Día
//Hora en formato hh:mm:ss
//Dirección IP origen en formato ###.##.###.###:####
//Razón de la falla
//Ejemplo:
//Aug 4 03:18:56 960.96.3.29:5268 Failed password for admin

struct Node {
  int port;  //key
  vector<string> ipAdd; //valor (IP addresses)
  int count; // cant (number of accesses)
  Node *left, *right; //punteros a los hijos
  Node(int& p) : port(p), count(1), left(nullptr), right(nullptr) {}
};

class HashTable {
  private:
    int capacity;
    vector<int> *table;
  public:
    HashTable(int V) {
      int size = getPrime(V);
      this->capacity = size;
      table = new vector<int>[capacity];
    }
    ~HashTable() {
      delete[] table;
      cout << "\nDestructor: HashTable deleted. " << endl;
    }
    bool checkPrime(int); // O(sqrt(n))
    int getPrime(int); // O(sqrt(n))
    void insert(int); // O(1 + alpha)
    void deleteItem(int); // O(1 + alpha)
    int hashFunction(int); // O(1 + alpha)
    void displayHash(); // O(n)
    void quadratic(int); // O(1 + alpha)
    void chain(int); // O(1 + alpha)
    void collitions(); // O(n)
    void clearHash(); // O(n)
    void top5portsWIthMostAccesses(); // O(n)
    void top5portsWIthMostAccessesAndIPs(); // O(n)
};

bool HashTable::checkPrime(int n) {
  if (n == 1 || n == 0)
    return false;

  int sqr_root = sqrt(n);
  for (int i = 2; i <= sqr_root; i++)
    if (n % i == 0)
      return false;
  return true;
} // O(sqrt(n))

int HashTable::getPrime(int n) {
  if (n % 2 == 0)
    n++;
  while (!checkPrime(n))
    n += 2;
  return n;
} // O(sqrt(n))

int HashTable::hashFunction(int key) {
  int p = getPrime(capacity);
  int a = rand() % p;
  int b = rand() % p;

  return ((a * key + b) % p) % capacity;
} // O(sqrt(n))

void HashTable::insert(int key) {
  int index = hashFunction(key);
  table[index].push_back(key);
} // O(1 + alpha)

void HashTable::deleteItem(int key) {
  int index = hashFunction(key);
  vector<int>::iterator i;
  for (i = table[index].begin(); i != table[index].end(); i++) {
    if (*i == key)
      break;
  }
  if (i != table[index].end())
    table[index].erase(i);
} // O(1 + alpha)

void HashTable::displayHash() {
  for (int i = 0; i < capacity; i++) {
    cout << i;
    for (auto x : table[i])
      cout << " --> " << x;
    cout << endl;
  }
} // O(n)

void HashTable::quadratic(int key) {
    int index = hashFunction(key);
    int i = 0;
    while (table[index].size() != 0) {
        index = (index + i * i) % capacity;
        i++;
    }
    table[index].push_back(key);
} // O(1 + alpha)

void HashTable::chain(int key) {
  int index = hashFunction(key);
  table[index].push_back(key);
} // O(1 + alpha)

void HashTable::collitions() {
  int count = 0;
  for (int i = 0; i < capacity; i++) {
    if (table[i].size() > 1)
      count++;
  }
  cout << "Number of collitions: " << count << endl;
} // O(n)

void HashTable::clearHash() {
  for (int i = 0; i < capacity; i++)
    table[i].clear();
} // O(n)

//Comparar el número de accesos de cada puerto
bool CompareCount(pair<int, Node*>& a, pair<int, Node*>& b){ 
  return a.second->count > b.second->count; //ordenar de mayor a menor
}
void HashTable::top5portsWIthMostAccesses() {
  map<int, int> ports;
  for (int i = 0; i < capacity; i++) {
    for (auto x : table[i]) {
      ports[x]++;
    }
  }
  priority_queue<pair<int, int>> pq;
  for (auto x : ports) {
    pq.push(make_pair(x.second, x.first));
  }
  cout << "Top 5 ports with most accesses: " << endl;
  for (int i = 0; i < 5; i++) {
    cout << pq.top().second << " with " << pq.top().first << " accesses" << endl;
    pq.pop();
  }
} // O(n)

void HashTable::top5portsWIthMostAccessesAndIPs() {
  map<int, Node*> ports;
  for (int i = 0; i < capacity; i++) {
    for (auto x : table[i]) {
      if (ports.find(x) == ports.end()) {
        ports[x] = new Node(x);
      }
      ports[x]->count++;
    }
  }
  ifstream inputFile("bitacora.txt");
  if (!inputFile.is_open()) {
    cerr << "Error: No se pudo abrir el archivo." << endl;
    return;
  }
  string mes, fecha, ipAddress, razon; //variables para almacenar los datos del archivo
  int dia, portNum; //variables para almacenar los datos del archivo
  while (inputFile >> mes >> dia >> fecha >> ipAddress) {
    getline(inputFile, razon);
    razon = razon.substr(1);
    portNum = stoi(ipAddress.substr(ipAddress.find(":") + 1)); //obtener el puerto
    if (ports.find(portNum) == ports.end()) {
      ports[portNum] = new Node(portNum);
    }
    ports[portNum]->ipAdd.push_back(ipAddress);
  }
  inputFile.close();
  vector<pair<int, Node*>> topPorts(ports.begin(), ports.end());
  sort(topPorts.begin(), topPorts.end(), &CompareCount);
  for (int i = 0; i < 5 && i < topPorts.size(); ++i) {
    cout << topPorts[i].first << " with " << (topPorts[i].second->count) - 1<< " accesses" << endl;
    cout << "IP addresses: " << endl;
    for (int j = 0; j < topPorts[i].second->ipAdd.size(); j++) {
      cout << topPorts[i].second->ipAdd[j] << endl;
    }
    cout << endl;
  }
}

int main(){
  //Abrir archivo
  ifstream inputFile("bitacora.txt");
  if (!inputFile.is_open()) {
    cerr << "Error: No se pudo abrir el archivo." << endl;
    return 1;
  }
  cout << "Hash Table "<<endl;
  HashTable hash(100);
  string mes, fecha, ipAddress, razon; //variables para almacenar los datos del archivo
  int dia, portNum; //variables para almacenar los datos del archivo
  while (inputFile >> mes >> dia >> fecha >> ipAddress) {
    getline(inputFile, razon);
    razon = razon.substr(1);
    portNum = stoi(ipAddress.substr(ipAddress.find(":") + 1)); //obtener el puerto
    hash.insert(portNum);
  }
  cout<<"------------------------"<<endl;
  cout<<"Summarized:"<<endl;
  hash.top5portsWIthMostAccesses();
  cout<<"------------------------"<<endl;
  cout << "Top 5 Ports with the Most Accesses (including their IPs):" << endl;
  hash.top5portsWIthMostAccessesAndIPs();
  cout<<"------------------------"<<endl;
  inputFile.close();
  return 0;
};