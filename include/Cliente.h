#include <string>

using namespace std;

class Cliente
{

public:

    int _id;
    int _idade;
    string _estado;
    string _pagamento_frequente;
    int _localizacao[2];

    Cliente(int id,
            int idade,
            string estado,
            string pagamento,
            int x,
            int y);

    ~Cliente();
};

Cliente::Cliente(int id,
                int idade,
                string estado,
                string pagamento,
                int x,
                int y)
{
    _id = id;
    _idade = idade;
    _estado = estado;
    _pagamento_frequente = pagamento;
    _localizacao[0] = x;   
    _localizacao[1] = y;
}

Cliente::~Cliente()
{
}
