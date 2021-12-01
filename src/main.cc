#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include "Cliente.h"
#include "Loja.h"
#include <algorithm>


using namespace std;

// note a passagem por referencia
// a funcao retorna true se a < b
bool comp1(const pair<int, double> &a, const pair<int, double> &b) {
    // se o ticket for igual, eu olho o id
    if (a.second == b.second) {
        // o menor id ganha
        return b.first < a.first; //ou seja, b é maior nesse caso
    }
    return a.second < b.second;
}

// usado na segunda ordenacao 
bool comp2(const pair<int, double> &a, const pair<int, double> &b) {
    // se o ticket for igual, eu olho o id
    if (a.second == b.second) {
        // o menor id ganha
        return b.first > a.first; //ou seja, b é maior nesse caso
    }
    return a.second < b.second;
}



int main(int argc, char const *argv[])
{
    // Scores 
    map<string, int> score_estado;
    map<string, int> score_pagamento;

    score_estado["MG"] = 0;
    score_estado["PR"] = 10;
    score_estado["SP"] = 20;
    score_estado["SC"] = 30;
    score_estado["RJ"] = 40;
    score_estado["RN"] = 50;
    score_estado["RS"] = 60;

    score_pagamento["DINHEIRO"] = 1;
    score_pagamento["DEBITO"] = 2;
    score_pagamento["CREDITO"] = 3;

    // grid
    int N, M;
    
    cin >> N >> M;

    // numero de lojas
    int m;
    cin >> m;

    // cout << N << ", " << M << endl;
    // cout << m << endl;

    vector<Loja*> Lojas;
    // computando lojas
    for (int i = 0; i < m; i++)
    {
        int id = i;
        int estoque;
        int x, y;

        cin >> estoque >> x >> y;

        Loja *l = new Loja(id, estoque, x, y);
        Lojas.push_back(l);
    } // O(m)
    
    // numero de clientes    
    int n;
    cin >> n;

    vector<Cliente*> Clientes;
    
    for (int i = 0; i < n; i++)
    {
        int id = i;
        int idade;
        string estado;
        string pagamento;
        int x, y;

        cin >> idade;
        cin >> estado;
        cin >> pagamento;
        cin >> x >> y;

        Cliente *c = new Cliente(id,
                                 idade,
                                 estado,
                                 pagamento,
                                 x,y);
        Clientes.push_back(c);
    } //O(n)

    // vector que vamos ordenar
    // id, ticket
    vector< pair<int, double> > ListaPrioridadeLojaTemp; 


    // Lista de prioridade temporaria das lojas
    for (int i = 0; i < n; i++)
    {
        double ticket;
        double num;
        int score_est = score_estado[Clientes[i]->_estado];
        int score_pag = score_pagamento[Clientes[i]->_pagamento_frequente] ;

        num = (double)(abs(60 - Clientes[i]->_idade) + score_est);
        ticket = num / score_pag;

        pair<int, double> item {Clientes[i]->_id, ticket };
        ListaPrioridadeLojaTemp.push_back(item);
    } // O(n)

    // Faz a ordenacao do menor para o maior do ticket e id
    sort(ListaPrioridadeLojaTemp.begin(), ListaPrioridadeLojaTemp.end(), comp1); // O(n log(n))*
    
    // A fila de prioridade
    queue<int> FilaPrioridadeLoja;
    for (int i = n-1; i >= 0; i--)
    {
        FilaPrioridadeLoja.push(ListaPrioridadeLojaTemp[i].first);
        // cout << ListaPrioridadeLojaTemp[i].first << endl;
    }



    // Agora temos que pegar a fila de prioridade de cada cliente
    vector< vector< pair<int, int> > > ListaDasListasDaPrioridadeDosClientes;
    
    // id_loja, distancia   
    
    for (int i = 0; i < n; i++)
    {
        vector< pair<int, int> > ListaPrioridadeClienteTemp;
        int x_cliente = Clientes[i]->_localizacao[0];
        int y_cliente = Clientes[i]->_localizacao[1];

        for (int j = 0; j < m; j++)
        {
            // em cada loja
            int x_loja = Lojas[j]->_x;
            int y_loja = Lojas[j]->_y;
            int id_loja = Lojas[j]->_id;

            // distancia é a prioridade dos clientes
            int dx = abs(x_loja - x_cliente);
            int dy = abs(y_loja - y_cliente);

            int dist;
            // entao dx tem que andar ainda 
            if (dx > dy) {
                dist = dy + (dx - dy) - 1;
            }
            // entao dy tem que andar
            else if (dx < dy) {
                dist = dx + (dy - dx) - 1;
            // sao iguais as distancias
            } else {
                dist = dx - 1;
            }

            pair<int, int> item {id_loja, dist};
            ListaPrioridadeClienteTemp.push_back(item);
        }
        
        // ordenar a lista do cliente
        sort(ListaPrioridadeClienteTemp.begin(), ListaPrioridadeClienteTemp.end(), comp2); // O(mlogm)


        ListaDasListasDaPrioridadeDosClientes.push_back(ListaPrioridadeClienteTemp);
    } // O(n *(m + mlog(m))) = O(n*m + nmlog(m)) = O(nm(1+ log(m)))

    /* 
    Agora temos que fazer o casamento estável com o que temos

    - Lista de prioridade das lojas sao iguais
    - Lista de prioridade dos clientes sao diferentes
    
    Temos que ter
    
    - Loja[id_cliente] = Loja onde o Cliente está ajeitado - inicializa com null
        - é util nas comparacoes entre lojas
        ex: o cliente_id está na loja id_loja, logo, vamos ver se as preferencias do cliente
        para essa loja é maior que a nossa, se nao for, pegamos ele, se for deixamos ele quieto


    - Cliente[id_loja] = [array de clientes na loja (id_loja)] - inicializa como um array vazio

    - devemos manter as lojas "nao visitadas" em uma fila
    */

   vector< map<int, int> > VetorHashDePrioridadeClientes;

   for (int i = 0; i < n; i++)
   {    
       map<int, int> myMap;
       for (int j = 0; j < m; j++)
       {
           myMap[ListaDasListasDaPrioridadeDosClientes[i][j].first] = j;
       }
       VetorHashDePrioridadeClientes.push_back(myMap);
   } // O(n*m)
   
    

    // mapeamentos
    // LojasMap[id_cliente] = Loja que o cliente ta associado
    map<int, int> LojasMap;
    for (int i = 0; i < n; i++)
    {
        LojasMap[i] = -1;
    } // O(n)

    vector< vector<int> > ClientesMap;
    for (int i = 0; i < m; i++)
    {
        vector<int> vet;
        ClientesMap.push_back(vet);
    } // O(m)
    
    

    
    // Filas de prioridade separadas para cada Loja
    vector<queue<int>> ListasDeFilasDePrioridadeDasLojas;
    queue<int> LojasNaoVisitadas;

    for (int j = 0; j < m; j++)
    {   
        queue<int> FilaNova(FilaPrioridadeLoja);
        ListasDeFilasDePrioridadeDasLojas.push_back(FilaNova);            
        LojasNaoVisitadas.push(j);
    } // O(m)

    

    // - M = ClientesMap
    // 
    // - Lojas nao visitadas = LojasNaoVisitadas
    // 
    // - Listas individuais de prioridade de cada loja = ListasDeFilasDePioridadeDasLoja
    // 
    // - Cliente está em alguma loja? => LojasMap[id_cliente] te responde,
    //   lembre de exluir o id_cliente da lista de preferencia da loja
    // 
    // - o Cliente prefere qual loja? => VetorHashDePrioridadeClientes[id_cliente][id_loja],
    //   quanto menor, mais o cliente prefere
    // 
    // - Para fazer a troca? LojasMap[id_cliente] = id_nova_loja e insere novamente o id da 
    //   loja antiga nas nao visitadas. 
    

    // ==================== GALE–SHAPLEY ===================
    
    // enquanto houver lojas ainda nao visitadas o loop itera
    

    while (!LojasNaoVisitadas.empty()) {
        

        int loja_id = LojasNaoVisitadas.front();
        LojasNaoVisitadas.pop(); // remove a loja visitada

        // A loja persiste enquanto houver estoque e pessoas para comprar
        while (Lojas[loja_id]->_estoque && !ListasDeFilasDePrioridadeDasLojas[loja_id].empty()) { // no maximo n vezes

        
            int cliente_id = ListasDeFilasDePrioridadeDasLojas[loja_id].front();
            ListasDeFilasDePrioridadeDasLojas[loja_id].pop(); // remove o cliente consultado

            // nao foi consultado ainda   
            if (LojasMap[cliente_id] == -1) {
                LojasMap[cliente_id] = loja_id;
                ClientesMap[loja_id].push_back(cliente_id);
                
                // diminui o estoque
                Lojas[loja_id]->_estoque--;
            } 

            // Verifico se o cliente prefere uma loja a outra
            else {
                int loja_id_disputa = LojasMap[cliente_id];
                
                 // se prefiro l a l' 
                if (VetorHashDePrioridadeClientes[cliente_id][loja_id] < VetorHashDePrioridadeClientes[cliente_id][loja_id_disputa]) 
                {
                    LojasMap[cliente_id] = loja_id;
                    
                    // Busca o indice do no array para apagar o  elemento
                    vector< int >::iterator it;
                    for (it = ClientesMap[loja_id_disputa].begin(); it != ClientesMap[loja_id_disputa].end(); it++)
                    {
                        if (*it == cliente_id) {
                            break;
                        }                     
                    } // O(estoque)
                    ClientesMap[loja_id_disputa].erase(it); // apaga o elemento
                    Lojas[loja_id_disputa]->_estoque++; // aumenta o estoque
 
                    // insere na lista de clientes da nova loja
                    ClientesMap[loja_id].push_back(cliente_id);
                    Lojas[loja_id]->_estoque--; // diminui o estoque 

                    // coloca novamente na fila o id que "perdeu" um cliente 
                    LojasNaoVisitadas.push(loja_id_disputa);

                } else {
                    continue;
                }
            }
        }
    }


    // Printando as saidas
    for (int j = 0; j < m; j++) {
        cout << j << endl;
        int size = ClientesMap[j].size();
        for (int i = 0; i < size; i++) {
            cout << ClientesMap[j][i] << " " ;
        }
        cout << endl;
    }

     


    return 0;
}



