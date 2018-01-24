/*
Copyright (C) 2015  Zuzanna Dutkiewicz
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include <iostream>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <ctime>

using namespace std;

#define MAX_TAB 100
#define MAX_ELEM 7

int tablicaZadan[MAX_TAB][MAX_ELEM];
int tablicaPrzerw[MAX_TAB][MAX_ELEM];


// [][0] - czas 1 operacji na pierwszej masznie
// [][1] - czas 2 operacji na drugiej masznie
// [][2] - oczekiwany czas zakonczenia
// [][3] - przerwa
// [][4] - czas rozpoczecia zadania
// [][5] - czas zakonczenia zadania
// [][6] - roznica oczekiwany czas zakonczenia - czas zakonczenia zadania

void odczytajZPliku(const char* nazwaPliku, int tablica[MAX_TAB][MAX_ELEM]);
void drukTab(int tablica[MAX_TAB][MAX_ELEM]);
void inicjujTablice(int tablica[MAX_TAB][MAX_ELEM]);
const char* nazwaPlikuZadan = "plikZadan.txt";
const char* nazwaPlikuPrzerw = "plikPrzerw.txt";
void wstawPrzerwy();
void liczRoznice();

void liczCzasy();

void odczytajZPliku(const char * nazwaPliku, int tablica[MAX_TAB][MAX_ELEM])
{
    char buflicz[10];
    int wiersz = 0;
    int kolumna = 0;

    fstream plik;
    string line;
    plik.open(nazwaPliku, ios::in);
    if(plik.good() != true)
    {
        cout << "Nie mozna otworzyc pliku %s" <<  nazwaPliku << endl;
        exit(1);
    }

    while(getline(plik,line))
    {
        const char * buf = line.c_str();
        int i = 0;
        int j = 0;
        //int liczKolumn = 0;
        wiersz++;
        while(true)
        {
            if(buf[i] != '\t' && buf[i] !='\0')
            {
                buflicz[j]=buf[i];
                j++;
            }
            else
            {
                int dana = 0;
                kolumna++;
                buflicz[j] = '\0';
                dana = atoi(buflicz);
                tablica[wiersz-1][kolumna-1] = dana;
                j = 0;
            }
            if(buf[i] == '\0')
                break;
            i++;
        }
        // liczKolumn = kolumna;
        kolumna = 0;
    }
    //liczKolumn == linia;
    //rozmMacierz = wiersz;
    plik.close();
    printf("Odczyt z pliku %s zakonczony powodzeniem.\n", nazwaPliku);
}

void drukTab(int tablica[MAX_TAB][MAX_ELEM])
{
    int i = 0;
    int j = 0;
    for( i = 0; i < MAX_TAB ; i++)
    {
        if (tablica[i][0] == -1)
            break;
            printf(">>%05i<<", i);
            cout << i+1 << "  ";
        for( j=0; j < MAX_ELEM; j++)
        {
            cout << tablica[i][j] << " ";
        }
        cout << endl;
    }
}
void inicjujTablice(int tablica[MAX_TAB][MAX_ELEM])
{
    int i = 0;
    int j = 0;
    for( i = 0; i < 100 ; i++)
    {
        for( j=0; j < 6; j++)
        {
            tablica[i][j] = 0;
            if (j == 0)
            {
                tablica[i][j] = -1;
            }
            if (j == 2)
            {
                //tablicaZadan[i][j] = 9999;
            }


        }
    }
}

void liczCzasy()
{
    int i = 0;
    //cout << "Tablica zadan: "<< endl;
    // drukTab(tablicaZadan);
    while (tablicaZadan[i][0] != -1)
    {
        if (i == 0)
        {
            tablicaZadan[i][4] = 0; // poczatek pierwszego zadania
            tablicaZadan[i][5] = tablicaZadan[i][0] + tablicaZadan[i][1] + tablicaZadan[i][3]; // koniec drugiego zadania
        }

        if (i !=0 ) // wyliczenie roznicy miedzy czasem 2 operacji na wczesniejszej maszynie, a 1 operacja na przegladanej maszynie
        {
            int roznica = 0;
            roznica = tablicaZadan[i-1][1] + tablicaZadan[i-1][3] - tablicaZadan[i][0];
            if (roznica <0)
            {
                roznica = 0;
            }
            tablicaZadan[i][4] = tablicaZadan[i-1][4] + tablicaZadan[i-1][0] + roznica;
            tablicaZadan[i][5] = tablicaZadan[i][0] + tablicaZadan[i][1] + tablicaZadan[i][4] + tablicaZadan[i][3];
        }
        i++;
    }
}

void wstawPrzerwy()
{
    int szukanyCzas = 0;
    int y = 0;
    liczCzasy();
    cout << "Tablica Zadan:" << endl;
    drukTab(tablicaZadan);
    while (tablicaPrzerw[y][0] != -1)
    {
        int i = 0;
        // obliczenie mo¿liwe najkrotszego czasu rozpoczecia przerwy
        szukanyCzas = tablicaPrzerw[i][2] - tablicaPrzerw[i][1];
        int znalezione = 0;
        for (i = MAX_TAB -1; i >= 0; i--)
        {
            if (tablicaZadan[i][0] != -1 && tablicaZadan[i][5] <= szukanyCzas)
            {
                if (tablicaZadan[i][3] == 0)
                {
                    tablicaZadan[i][3] = 2;
                    znalezione = 1;
                    break;
                }
            }

        }
        if (znalezione == 0)
        {
            for (i = 0; i <= MAX_TAB; i++)
            {
                if (tablicaZadan[i][3] == 0)
                {
                    tablicaZadan[i][3] = 2;
                    znalezione = 1;
                    break;
                }
            }
        }
        if (znalezione == 0)
        {
            cout << "Nie znaleziono miejsca na przerwê element z pliku:"
                << tablicaPrzerw[y][0] << " "
                << tablicaPrzerw[y][1] << " "
                << tablicaPrzerw[y][2] << endl;
        }

        liczCzasy();
        y++;
    }

}

void liczRoznice()
{
    int i = 0;
    for (i = 0; i< MAX_TAB && tablicaZadan[i][0] != -1; i++)
    {
        tablicaZadan[i][6] = tablicaZadan[i][2] - tablicaZadan[i][5];
    }
}

void znajdzMaxL()
{
    int i = 0;
    int poprzWart = tablicaZadan[0][6];
    int pozycja = 0;
    for (i = 0; i< MAX_TAB && tablicaZadan[i][0] != -1; i++)
    {
        if (poprzWart >= tablicaZadan[i][6])
        {
            poprzWart = tablicaZadan[i][6];
            pozycja = i;
        }
    }

    cout << "Najwiêksze spoznienie zadania wynosi: " << poprzWart
    << " Na pozycji: " << i << endl;
    drukTab(tablicaZadan);
}

void PosortujTablice(int tablica[MAX_TAB][MAX_ELEM])
{
    int rozmiarTablicy = 0;
    int i = 0;
    char tablicaXX[MAX_TAB][10];
    char str1[5];
    char str2[5];
    char str3[10];

    for (i = 0; i< MAX_TAB; i++)
    {
       if (tablicaZadan[i][0] == -1)
        {
            rozmiarTablicy = i;
            break;
        }

        sprintf(str1, "%05i", i);
        sprintf(str2, "%05i", tablicaZadan[i][2]);

        int y = 0;
        for (y = 0; y < 5; y++)
        {
            str3[y] = str2[y];
        }
            for (y = 0; y < 5; y++)
        {
            str3[y+5] = str1[y];
        }

        for (y = 0; y < 10; y++)
        {
            tablicaXX[i][y] = str3[y];
            cout << "Tablica str:" << tablicaXX[i][y] << endl;
        }
    }

}

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

void quickSortC(int tab[], int tab_size) {


    qsort(tab, tab_size, sizeof(int), compare);
}



int main()
{
    clock_t start=clock();
    inicjujTablice(tablicaZadan);
    odczytajZPliku(nazwaPlikuZadan, tablicaZadan);
    inicjujTablice(tablicaPrzerw);
    odczytajZPliku(nazwaPlikuPrzerw, tablicaPrzerw);
    cout << "Tablica Przerw:" <<endl;
    drukTab(tablicaPrzerw);
    cout << "Tablica Zadan:" <<endl;
    drukTab(tablicaZadan);
    wstawPrzerwy();
    cout << "Tablica Zadan:" <<endl;
    drukTab(tablicaZadan);
    liczRoznice();
    znajdzMaxL();
    cout << "Czas: " << ((1000*(clock()-start))/CLOCKS_PER_SEC) << "sec" << endl;
    PosortujTablice(tablicaZadan);
    return 0;
}
