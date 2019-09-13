#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>

using namespace std;

#define ILOSC_INSTANCJI 1

class Zadanie
{
public:
    int czas_trwania;
    int numer_zadania;
    int laczny_czas;
};

bool porownaj_zadania(Zadanie pierwsze, Zadanie drugie)
{
    return (pierwsze.laczny_czas > drugie.laczny_czas);
}

class instancja
{
public:
    string nazwa;
    int liczba_zadan;
    int liczba_maszyn;
    Zadanie **tablica_klasa;
    Zadanie **pomocnicza;
    Zadanie *laczny_czas_kolumna;
    Zadanie **ZAD;
    int k=0;

    void zadeklaruj_tablice()
    {
        laczny_czas_kolumna = new Zadanie [liczba_zadan];
        tablica_klasa = new Zadanie* [liczba_zadan];
        for(int i=0; i<liczba_zadan; i++)
        {
            tablica_klasa[i] = new Zadanie [liczba_maszyn];
        }

        pomocnicza = new Zadanie* [liczba_zadan];
        for(int i=0; i<liczba_zadan; i++)
        {
            pomocnicza[i] = new Zadanie [liczba_maszyn];
        }

        ZAD = new Zadanie* [1];
        ZAD[0] = new Zadanie [liczba_maszyn];
    }

    void ZadanieKopia(int numer)
    {
        for (int j = 0; j < liczba_maszyn; j++)
        {
            ZAD[0][j].numer_zadania = tablica_klasa[numer][j].numer_zadania;
            ZAD[0][j].czas_trwania = tablica_klasa[numer][j].czas_trwania;
        }
    }

    // Funkcja do wstawiania zadania przed inne zadanie (o danym numerze) w tablicy zadan 'tab'
    void Wstaw_przed(int l_zadan, int l_maszyn, int numer_zadania)
    {
        // Tworzenie miejsca w tablicy na nowe zadanie przez przesuwanie dalszych w prawo (dzieki tablicy statycznej o maksymalnym rozmiarze
        // nie trzeba sie martwic o rozszerzanie tablicy)
        for (int i = l_zadan; i > numer_zadania; i--)
        {
            for (int j = 0; j < l_maszyn; j++)
            {
                pomocnicza[i][j].czas_trwania = pomocnicza[i-1][j].czas_trwania;
                pomocnicza[i][j].numer_zadania = pomocnicza[i-1][j].numer_zadania;
            }
        }
        // Wstawianie
        for (int i = 0; i < l_maszyn; i++)
        {
            pomocnicza[numer_zadania][i].czas_trwania = ZAD[0][i].czas_trwania;
            pomocnicza[numer_zadania][i].numer_zadania = ZAD[0][i].numer_zadania;
        }
    }

    int licz_Cmax(int l_zadan, int l_maszyn)
    {

        Zadanie tab_Cmax[l_zadan+1][l_maszyn+1];

        // Zerowanie tablicy
        for (int i = 0; i <= l_zadan; ++i)
        {
            for (int j = 0; j <= l_maszyn; ++j)
            {
                tab_Cmax[i][j].czas_trwania = 0;
            }
        }
        // Obliczanie Cmax
        for (int i = 1; i <= l_zadan; ++i)
        {
            for (int j = 1; j <= l_maszyn; ++j)
            {

                tab_Cmax[i][j].czas_trwania = max(tab_Cmax[i-1][j].czas_trwania, tab_Cmax[i][j-1].czas_trwania) + pomocnicza[i-1][j-1].czas_trwania;
            }
        }
cout<<endl<<tab_Cmax[l_zadan][l_maszyn].czas_trwania<<endl;
        return tab_Cmax[l_zadan][l_maszyn].czas_trwania;
    }


    int algorytm_NEH()
    {

        int minimum = 0;
        int Najlepsza_opcja = 0;

        for(int licznik=1; licznik<liczba_zadan; licznik++)
        {

            // Pobranie zadania z tablicy
            ZadanieKopia(laczny_czas_kolumna[licznik].numer_zadania-1);

            // Wstawianie skopiowanego zadania na koniec
            Wstaw_przed(licznik,liczba_maszyn,licznik);
            int Cmax = licz_Cmax(licznik+1,liczba_maszyn);
            minimum = Cmax;			// Minimalny Cmax
            Najlepsza_opcja = licznik;    // Po wstawieniu przed tym zadaniem uzyskamy minimalnego Cmaxa

            // Przesuwanie zadania i liczenie aktualnego Cmaxa
            for (int k = licznik; k > 0; k--)
            {
                for(int j = 0; j < liczba_maszyn; j++)
                {
                    swap(pomocnicza[k][j].czas_trwania,pomocnicza[k-1][j].czas_trwania);
                    swap(pomocnicza[k][j].numer_zadania,pomocnicza[k-1][j].numer_zadania);
                }

                      		Cmax = licz_Cmax(licznik+1,liczba_maszyn);

                // Wyliczanie Cmax odbywa sie po kazdej zmianie w tablicy tmp
             /*  int l_zadan=licznik+1;
               int l_maszyn=liczba_maszyn;
                       Zadanie tab_Cmax[l_zadan+1][l_maszyn+1];

                for (int i = 0; i <= l_zadan; ++i)
                {
                    for (int j = 0; j <= l_maszyn; ++j)
                    {
                        tab_Cmax[i][j].czas_trwania = 0;
                    }
                }
                // Obliczanie Cmax
                for (int i = 1; i <= l_zadan; ++i)
                {
                    for (int j = 1; j <= l_maszyn; ++j)
                    {

                        tab_Cmax[i][j].czas_trwania = max(tab_Cmax[i-1][j].czas_trwania, tab_Cmax[i][j-1].czas_trwania) + pomocnicza[i-1][j-1].czas_trwania;
                    }
                }
                //Cmax = licz_Cmax(licznik+1,liczba_maszyn);
                Cmax=tab_Cmax[l_zadan][l_maszyn].czas_trwania;*/
                // Sprawdzenie nowego Cmaxa
                if (Cmax <= minimum)
                {
                    minimum = Cmax;
                    Najlepsza_opcja = k-1;
                }

            }

           // Przesuniecie zadania z poczatku tablicy na wyznaczona optymalna pozycje
            for (int i = 0; i < Najlepsza_opcja; i++)
            {
                // Zrobienie miejsca na zadanie
                for (int j = 0; j < liczba_maszyn; j++)
                {
                    pomocnicza[i][j].czas_trwania = pomocnicza[i+1][j].czas_trwania;
                    pomocnicza[i][j].numer_zadania = pomocnicza[i+1][j].numer_zadania;
                }
            }

            // Wstawienie zadania w powstala luke (lepsze czasowo niz swapowanie zadania do pozycji optymalnej)
            for (int j = 0; j<liczba_maszyn; j++)
            {
                pomocnicza[Najlepsza_opcja][j].czas_trwania = ZAD[0][j].czas_trwania;
                pomocnicza[Najlepsza_opcja][j].numer_zadania = ZAD[0][j].numer_zadania;
            }
        }
        return minimum;
    }
    void oblicz()
    {
        // obliczamy �aczny czas zadania na wszystkich maszynach
        for (int i=0; i < liczba_zadan; i++)
        {
            // Zerowanie
            laczny_czas_kolumna[i].laczny_czas = 0;
            // Liczenie lacznego czasu
            for (int j=0; j<liczba_maszyn; j++)
            {
                laczny_czas_kolumna[i].laczny_czas = laczny_czas_kolumna[i].laczny_czas + tablica_klasa[i][j].czas_trwania;
            }
            laczny_czas_kolumna[i].numer_zadania = tablica_klasa[i][0].numer_zadania;
            laczny_czas_kolumna[i].czas_trwania =  tablica_klasa[i][0].czas_trwania;
        }

        stable_sort(laczny_czas_kolumna,laczny_czas_kolumna+liczba_zadan,porownaj_zadania);

        for(int i=0; i<liczba_maszyn; i++)
        {
            pomocnicza[0][i].czas_trwania = tablica_klasa[laczny_czas_kolumna[0].numer_zadania-1][i].czas_trwania;
            pomocnicza[0][i].numer_zadania = tablica_klasa[laczny_czas_kolumna[0].numer_zadania-1][i].numer_zadania;
        }
    }
};

int main(int argc, char** argv)
{

    fstream in_file;
    fstream out_file;
    fstream out_file_cmax;
    in_file.open("bench_fs.txt",ios::in);
    out_file.open("output.txt",ios::out);
    out_file_cmax.open("NEH_output.txt",ios::out);
    string nazwa;
    int liczba_zadan;
    int liczba_maszyn;
    int Cmax = 0;
    int glowna_inkrementacja = 0;

    instancja *tablica = new instancja [ILOSC_INSTANCJI];

    int smietnik, czas_zadania_na_maszynie;   // zmienne do wczytywania tablicy

    while(glowna_inkrementacja < ILOSC_INSTANCJI)
    {
        in_file >> tablica[glowna_inkrementacja].nazwa;
        in_file >> tablica[glowna_inkrementacja].liczba_zadan;
        in_file >> tablica[glowna_inkrementacja].liczba_maszyn;
      tablica[glowna_inkrementacja].zadeklaruj_tablice();

        for(int i=0; i<tablica[glowna_inkrementacja].liczba_zadan; i++)
        {
            for(int j=0; j<tablica[glowna_inkrementacja].liczba_maszyn; j++)
            {
                in_file >> smietnik;
                in_file >> czas_zadania_na_maszynie;
                tablica[glowna_inkrementacja].tablica_klasa[i][j].czas_trwania = czas_zadania_na_maszynie;
                tablica[glowna_inkrementacja].tablica_klasa[i][j].numer_zadania = i+1;


            }
        }

        tablica[glowna_inkrementacja].oblicz();
        clock_t star=clock();

        Cmax = tablica[glowna_inkrementacja].algorytm_NEH();
        cout<<"czas wykonania programu: "<<clock()-star<<endl;
        cout<<glowna_inkrementacja<< " " << " Cmax = " << Cmax << endl;
        //	cout << endl;
        glowna_inkrementacja++;
        //cout << glowna_inkrementacja;
    }

   glowna_inkrementacja = 0;

    	while(glowna_inkrementacja < ILOSC_INSTANCJI)
    	{
    		tablica[glowna_inkrementacja].oblicz();
    	//	cout << "Instancja nr. "<< glowna_inkrementacja + 1;
    		Cmax = tablica[glowna_inkrementacja].algorytm_NEH();
    		cout << " Cmax = " << Cmax << endl;

    		out_file_cmax << Cmax << " ";

    		for (int i=0; i<tablica[glowna_inkrementacja].liczba_zadan; i++)
    		{
    			out_file << tablica[glowna_inkrementacja].pomocnicza[i][0].numer_zadania << " ";
    		}
    		glowna_inkrementacja++;
    	}

    //in_file.close();
    //out_file.close();
//	out_file_cmax.close();
    return 0;
}
