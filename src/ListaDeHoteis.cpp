#include "../include/ListaDeHoteis.h"
#include <ostream>
#include <string>

using namespace std;

ListaDeHoteis::ListaDeHoteis()
{
}

void ListaDeHoteis::Inicializar()
{
    arquivo_.open("data/Hoteis.txt", ios::in);
    if (!arquivo_.is_open())
    {
        // TODO: throw excessão
    }
    string buffer;
    Hotel aux_leitura;
    string aux_data = "";
    while (getline(arquivo_, buffer))
    {
        aux_leitura.nome = buffer;
        getline(arquivo_, buffer);
        aux_leitura.local = buffer;
        getline(arquivo_, buffer);
        aux_leitura.estrelas = stoi(buffer);
        getline(arquivo_, buffer);
        aux_leitura.avaliacoes = stof(buffer);
        getline(arquivo_, buffer);
        aux_leitura.cafe = stoi(buffer);
        getline(arquivo_, buffer);
        aux_leitura.almoco = stoi(buffer);
        getline(arquivo_, buffer);
        aux_leitura.jantar = stoi(buffer);
        getline(arquivo_, buffer);
        aux_leitura.piscina = stoi(buffer);

        // a partir daqui tem que alterar
        for (int i = 0; i < 12; i++)
        {
            getline(arquivo_, buffer);
            for (int j = 0; j < buffer.size(); j++)
            {
                if (buffer[j] == ' ') // aqui
                {
                    aux_leitura.quartos2[i].push_back(stoi(aux_data));
                    aux_data = "";
                }
                else
                {
                    aux_data.append(1, buffer[j]);
                }
            }
        }
        for (int i = 0; i < 12; i++)
        {
            getline(arquivo_, buffer);
            for (int j = 0; j < buffer.size(); j++)
            {
                if (buffer[j] == ' ')
                {
                    aux_leitura.quartos3[i].push_back(stoi(aux_data));
                    aux_data = "";
                }
                else
                {
                    aux_data.append(1, buffer[j]);
                }
            }
        }

        for (int i = 0; i < 12; i++)
        {
            getline(arquivo_, buffer);
            for (int j = 0; j < buffer.size(); j++)
            {
                if (buffer[j] == ' ')
                {
                    aux_leitura.quartos4[i].push_back(stoi(aux_data));
                    aux_data = "";
                }
                else
                {
                    aux_data.append(1, buffer[j]);
                }
            }
        }
        getline(arquivo_, buffer);
        aux_leitura.preco2 = stof(buffer);
        getline(arquivo_, buffer);
        aux_leitura.preco3 = stof(buffer);
        getline(arquivo_, buffer);
        aux_leitura.preco4 = stof(buffer);
        getline(arquivo_, buffer); // para tirar a linha em branco

        lista_.push_back(aux_leitura);
    }
    arquivo_.close();
}

list<Hotel>::iterator ListaDeHoteis::Buscar(FiltrosHotel filtros)
{
    list<Hotel>::iterator it;
    for (it = lista_.begin(); it != lista_.end(); it++)
    {
        if ((it->local == filtros.local) && (it->estrelas == filtros.estrelas) &&
            (it->cafe == filtros.cafe)) // talvez pensar melhor nos filtros kkkkkkj
            return it;
    }
    if (it == lista_.end())
    {
        // TODO: throw excessão
    }
}

// Estou mudando a interfaçe dos métodos ConsultarCalendario e Reservar para que eles
// recebam a data no formato de inteiro. A conersão de string para inteiro será feita
// na função main no momento que o usuario digita a data.

bool ListaDeHoteis::ConsultarCalendario(Data d, list<Hotel>::iterator it, int pessoas)
{
    if ((pessoas == 2) && (it->quartos2[d.mes - 1][d.dia - 1] != 0))
        return true;
    if ((pessoas == 3) && (it->quartos3[d.mes - 1][d.dia - 1] != 0))
        return true;
    if ((pessoas == 4) && (it->quartos4[d.mes - 1][d.dia - 1] != 0))
        return true;
    return false;
}

void ListaDeHoteis::Reservar(Data inicio, Data fim, list<Hotel>::iterator it, Usuario &user, int pessoas)
{
    if (((pessoas == 2) && (user.creditos() < it->preco2) || ((pessoas == 3) && (user.creditos() < it->preco3)) ||
         ((pessoas == 4) && (user.creditos() < it->preco4))))
    {
        // TODO: créditos insuficientes
    }

    if (inicio.mes == fim.mes)
    {
        Data aux;
        aux.mes = inicio.mes;

        // verifica a disponibilidade
        for (int i = (inicio.dia - 1); i < fim.dia; i++)
        {
            aux.dia = i;
            if (!ConsultarCalendario(aux, it, pessoas))
            {
                // TODO: não é possivel reservar pois não há vagas
            }
        }
        if (pessoas == 2)
        {
            for (int i = (inicio.dia - 1); i < fim.dia; i++)
            {
                it->quartos2[(inicio.mes) - 1][i]--;
            }
            user.RegistrarReservaHotel(*(it), inicio, fim, it->preco2, pessoas);
        }
        if (pessoas == 3)
        {
            for (int i = (inicio.dia - 1); i < fim.dia; i++)
            {
                it->quartos3[(inicio.mes) - 1][i]--;
            }
            user.RegistrarReservaHotel(*(it), inicio, fim, it->preco3, pessoas);
        }
        if (pessoas == 4)
        {
            for (int i = (inicio.dia - 1); i < fim.dia; i++)
            {
                it->quartos4[(inicio.mes) - 1][i]--;
            }
            user.RegistrarReservaHotel(*(it), inicio, fim, it->preco4, pessoas);
        }
    }
    else
    {
        Data aux;
        aux.mes = inicio.mes;
        // poderia ser qualquer quartox, o que importa é o mês
        for (int i = (inicio.dia - 1); i < it->quartos2[aux.mes].size(); i++)
        {
            aux.dia = i;
            if (!ConsultarCalendario(aux, it, pessoas))
            {
                // TODO: não há vagas
            }
        }
        aux.mes++;
        while (aux.mes < fim.mes)
        {
            for (int i = 0; i < it->quartos2[aux.mes].size(); i++)
            {
                aux.dia = i;
                if (!ConsultarCalendario(aux, it, pessoas))
                {
                    // TODO: não há vagas
                }
            }
            aux.mes++;
        }
        for (int i = 0; i < fim.dia; i++)
        {
            aux.dia = i;
            if (!ConsultarCalendario(aux, it, pessoas))
            {
                // TODO: não há vagas
            }
        }
        if (pessoas == 2)
        {
            aux.mes = inicio.mes;
            for (int i = (inicio.dia - 1); i < it->quartos2[inicio.mes].size(); i++)
            {
                it->quartos2[(inicio.mes) - 1][i]--;
            }
            aux.mes++;
            while (aux.mes < fim.mes)
            {
                for (int i = 0; i < it->quartos2[aux.mes].size(); i++)
                {
                    it->quartos2[(aux.mes) - 1][i]--;
                }
                aux.mes++;
            }
            for (int i = 0; i < fim.dia; i++)
            {
                it->quartos2[(aux.mes) - 1][i]--;
            }
            user.RegistrarReservaHotel(*(it), inicio, fim, it->preco2, pessoas);
        }
        if (pessoas == 3)
        {
            aux.mes = inicio.mes;
            for (int i = (inicio.dia - 1); i < it->quartos3[inicio.mes].size(); i++)
            {
                it->quartos3[(inicio.mes) - 1][i]--;
            }
            aux.mes++;
            while (aux.mes < fim.mes)
            {
                for (int i = 0; i < it->quartos3[aux.mes].size(); i++)
                {
                    it->quartos3[(aux.mes) - 1][i]--;
                }
                aux.mes++;
            }
            for (int i = 0; i < fim.dia; i++)
            {
                it->quartos3[(aux.mes) - 1][i]--;
            }
            user.RegistrarReservaHotel(*(it), inicio, fim, it->preco3, pessoas);
        }
        if (pessoas == 4)
        {
            aux.mes = inicio.mes;
            for (int i = (inicio.dia - 1); i < it->quartos4[inicio.mes].size(); i++)
            {
                it->quartos4[(inicio.mes) - 1][i]--;
            }
            aux.mes++;
            while (aux.mes < fim.mes)
            {
                for (int i = 0; i < it->quartos4[aux.mes].size(); i++)
                {
                    it->quartos4[(aux.mes) - 1][i]--;
                }
                aux.mes++;
            }
            for (int i = 0; i < fim.dia; i++)
            {
                it->quartos4[(aux.mes) - 1][i]--;
            }
            user.RegistrarReservaHotel(*(it), inicio, fim, it->preco4, pessoas);
        }
    }
}

void ListaDeHoteis::CancelarReserva(list<Hotel>::iterator it, Data inicio, Data fim, Usuario &user, int pessoas)
{
    if (inicio.mes == fim.mes)
    {
        if (pessoas == 2)
        {
            for (int i = (inicio.dia - 1); i < fim.dia; i++)
            {
                it->quartos2[(inicio.mes) - 1][i]++;
            }
            user.ReembolsarReservaHotel(*(it), inicio, fim, it->preco2, pessoas);
        }
        if (pessoas == 3)
        {
            for (int i = (inicio.dia - 1); i < fim.dia; i++)
            {
                it->quartos3[(inicio.mes) - 1][i]++;
            }
            user.ReembolsarReservaHotel(*(it), inicio, fim, it->preco3, pessoas);
        }
        if (pessoas == 4)
        {
            for (int i = (inicio.dia - 1); i < fim.dia; i++)
            {
                it->quartos4[(inicio.mes) - 1][i]++;
            }
            user.ReembolsarReservaHotel(*(it), inicio, fim, it->preco4, pessoas);
        }
    }
    else
    {
        Data aux;
        if (pessoas == 2)
        {
            aux.mes = inicio.mes;
            for (int i = (inicio.dia - 1); i < it->quartos2[inicio.mes].size(); i++)
            {
                it->quartos2[(inicio.mes) - 1][i]++;
            }
            aux.mes++;
            while (aux.mes < fim.mes)
            {
                for (int i = 0; i < it->quartos2[aux.mes].size(); i++)
                {
                    it->quartos2[(aux.mes) - 1][i]++;
                }
                aux.mes++;
            }
            for (int i = 0; i < fim.dia; i++)
            {
                it->quartos2[(aux.mes) - 1][i]++;
            }
            user.ReembolsarReservaHotel(*(it), inicio, fim, it->preco2, pessoas);
        }
        if (pessoas == 3)
        {
            aux.mes = inicio.mes;
            for (int i = (inicio.dia - 1); i < it->quartos3[inicio.mes].size(); i++)
            {
                it->quartos3[(inicio.mes) - 1][i]++;
            }
            aux.mes++;
            while (aux.mes < fim.mes)
            {
                for (int i = 0; i < it->quartos3[aux.mes].size(); i++)
                {
                    it->quartos3[(aux.mes) - 1][i]++;
                }
                aux.mes++;
            }
            for (int i = 0; i < fim.dia; i++)
            {
                it->quartos3[(aux.mes) - 1][i]++;
            }
            user.ReembolsarReservaHotel(*(it), inicio, fim, it->preco3, pessoas);
        }
        if (pessoas == 4)
        {
            aux.mes = inicio.mes;
            for (int i = (inicio.dia - 1); i < it->quartos4[inicio.mes].size(); i++)
            {
                it->quartos4[(inicio.mes) - 1][i]++;
            }
            aux.mes++;
            while (aux.mes < fim.mes)
            {
                for (int i = 0; i < it->quartos4[aux.mes].size(); i++)
                {
                    it->quartos4[(aux.mes) - 1][i]++;
                }
                aux.mes++;
            }
            for (int i = 0; i < fim.dia; i++)
            {
                it->quartos4[(aux.mes) - 1][i]++;
            }
            user.ReembolsarReservaHotel(*(it), inicio, fim, it->preco4, pessoas);
        }
    }
}

void ListaDeHoteis::Avaliar(list<Hotel>::iterator it, Usuario &user, float avaliacao)
{
    if (avaliacao > 10.0)
    {
        // TODO: digite um valor válido
    }
    float media = ((it->avaliacoes + avaliacao) / 2.0);
    it->avaliacoes = media;
}

void ListaDeHoteis::Fechar()
{
    arquivo_.open("data/Hoteis.txt", ios::out);
    if (!arquivo_.is_open())
    {
        // TODO: throw excessão
    }

    for (Hotel x : lista_)
    {
        arquivo_ << x.nome << endl;
        arquivo_ << x.local << endl;
        arquivo_ << x.estrelas << endl;
        arquivo_ << x.avaliacoes << endl;
        arquivo_ << x.cafe << endl;
        arquivo_ << x.almoco << endl;
        arquivo_ << x.jantar << endl;
        arquivo_ << x.piscina << endl;

        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < x.quartos2[i].size(); i++)
            {
                arquivo_ << x.quartos2[i][j] << " ";
            }
            arquivo_ << endl;
        }
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < x.quartos3[i].size(); i++)
            {
                arquivo_ << x.quartos3[i][j] << " ";
            }
            arquivo_ << endl;
        }
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < x.quartos4[i].size(); i++)
            {
                arquivo_ << x.quartos4[i][j] << " ";
            }
            arquivo_ << endl;
        }
        arquivo_ << x.preco2 << endl;
        arquivo_ << x.preco3 << endl;
        arquivo_ << x.preco4 << endl;
        arquivo_ << endl;
    }
    arquivo_.close();
}
