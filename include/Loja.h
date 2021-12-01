class Loja
{
public:
    int _id;
    int _estoque;
    int _x, _y;    

    Loja(int id, int estoque, int x, int y);
    ~Loja();
};

Loja::Loja(int id, int estoque, int x, int y)
{
    _id = id;
    _estoque = estoque;
    _x = x;
    _y = y;
}

Loja::~Loja()
{
}
