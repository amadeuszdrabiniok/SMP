#include <iomanip>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <winsock.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<string>
#include <cstdlib>


using namespace std;

class Multiplayer
{
public:
	WSADATA wsaData;
	SOCKET mainSocket;
	SOCKET acceptSocket = SOCKET_ERROR;
	sockaddr_in service;

	Multiplayer(/*char * adres_ip*/)
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NOERROR)
		{
			cout << "Blad inicjalizacji!" << endl;
			_getch();
			exit(0);
		}

		mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		memset(&service, 0, sizeof(service));
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = inet_addr("000");
		service.sin_port = htons(27015);
	}

	void inicjalizacja(char * adres_ip)//Inicjalizuje sockety (w przypadku uzycia konstruktora domyslnego).
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NOERROR)
		{
			cout << "Blad inicjalizacji!" << endl;
			_getch();
			exit(0);
		}

		mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		memset(&service, 0, sizeof(service));
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = inet_addr(adres_ip);
		service.sin_port = htons(27015);
	}


	void serwer_inicjalizacja()//Inicjalizuje hosta gry.
	{
		bind(mainSocket, (SOCKADDR *)& service, sizeof(service));
		listen(mainSocket, 1);
		cout << "Oczekiwanie na polaczenie klienta..." << endl;
		while (acceptSocket == SOCKET_ERROR)
		{
			acceptSocket = accept(mainSocket, NULL, NULL);
		}
		mainSocket = acceptSocket;
	}

	void klient_inicjalizacja()//Inicjalizuje klienta gry.
	{
		while (connect(mainSocket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
		{
			cout << "Blad polaczenia, sprobuje ponownie za 1s..." << endl;
			Sleep(1000);
		}
	}

	char komunikacja_klient(string sender)//Wysyla dane i czeka na odpowiedz.
	{

		int bytesSent;
		int bytesRecv = SOCKET_ERROR;
		char sendbuf[32] = "";
		for (int i = 0; i < sender.length(); i++)
		{
			sendbuf[i] = sender[i];
		}
		char recvbuf[32] = "";

		bytesSent = send(mainSocket, sendbuf, strlen(sendbuf), 0);

		while (bytesRecv == SOCKET_ERROR)
		{
			bytesRecv = recv(mainSocket, recvbuf, 32, 0);
		}

		int i = 0;
		string reciever = "";
		do
		{
			reciever[i] = recvbuf[i];
			if (reciever[i] != NULL)
			{
				break;
			}
			i++;
		} while (1);

		char returner = reciever[i];

		return returner;
	}

	char komunikacja_serwer(string sender)//Czeka na dane, po czym wysyla wlasne.
	{
		int bytesSent;
		int bytesRecv = SOCKET_ERROR;
		char sendbuf[32] = "";

		for (int i = 0; i < sender.length(); i++)
		{
			sendbuf[i] = sender[i];
		}
		char recvbuf[32] = "";

		while (bytesRecv == SOCKET_ERROR)
		{
			bytesRecv = recv(mainSocket, recvbuf, 32, 0);
		}

		int i = 0;
		string reciever = "";
		do
		{
			reciever[i] = recvbuf[i];
			if (reciever[i] != NULL)
			{
				break;
			}
			i++;
		} while (1);

		bytesSent = send(mainSocket, sendbuf, strlen(sendbuf), 0);
		char returner = reciever[i];
		return returner;


		/*int bytesSent;
		int bytesRecv = SOCKET_ERROR;
		bytesRecv = recv(mainSocket, recvbuf, 1, 0);
		bytesSent = send(mainSocket, sendbuf, strlen(sendbuf), 0);*/
	}
};

class Mapa
{
public:
	const int wysokosc = 16;
	const int szerokosc = 40;
};

class Gracz : public Mapa
{
public:
	int x = 10;
	int y = wysokosc / 2;


	char kierunek;
	char znak = 'd';

	void sterowanie()
	{
		if (_kbhit())
		{
			znak = _getch();

			switch (znak)
			{
			case 'w': kierunek = 1;
				break;
			case 's': kierunek = 2;
				break;
			case 'a': kierunek = 3;
				break;
			case 'd': kierunek = 4;
				break;
			}
		}

		switch (kierunek)
		{
		case 1: y--;
			break;
		case 2: y++;
			break;
		case 3: x--;
			break;
		case 4: x++;
			break;
		}
	}


};

class Ogon_gracza : public Gracz
{
public:
	int wynik = 1;
	int ogonX[100];
	int ogonY[100];
	int dlugosc = 2;

	void ogonek()
	{
		ogonX[0] = x;
		ogonY[0] = y;
		for (int i = dlugosc; i > 0; i--)
		{
			ogonX[i] = ogonX[i - 1];
			ogonY[i] = ogonY[i - 1];
		}
	}
	void zjadanie_ogona(int &wynik_przeciwnika)
	{
		for (int i = 0; i < dlugosc; i++)
		{
			if ((x == ogonX[i]) && (y == ogonY[i]))
			{
				dlugosc = i - dlugosc;
				wynik = 1;

			}
		}
	}
};

class Owoc : public Ogon_gracza
{
public:

	int owoX = rand() % (szerokosc - 1) + 2;
	int owoY = rand() % (wysokosc - 1) + 2;

	int owo2X = rand() % (szerokosc - 1) + 2;
	int owo2Y = rand() % (wysokosc - 1) + 2;

	void logika_owocu()
	{
		if ((x == owoX) && (y == owoY))
		{
			wynik += 1;
			owoX = rand() % (szerokosc - 2) + 1;
			owoY = rand() % (wysokosc - 2) + 1;
			owo2X = rand() % (szerokosc - 2) + 1;
			owo2Y = rand() % (wysokosc - 2) + 1;
			dlugosc += 5;
		}
		else if ((x == owo2X) && (y == owo2Y))
		{
			owo2X = rand() % (szerokosc - 2) + 1;
			owo2Y = rand() % (wysokosc - 2) + 1;
			owoX = rand() % (szerokosc - 2) + 1;
			owoY = rand() % (wysokosc - 2) + 1;
			dlugosc += 5;
		}
	}
};

class Grafika : public Owoc
{
public:
	void kolizje()
	{
		if (x >= szerokosc)
		{
			x = 0;
		}
		else if (x <= 0)
		{
			x = szerokosc - 1;
		}
		if (y > wysokosc)
		{
			y = 0;
		}
		else if (y <= -1)
		{
			y = wysokosc - 1;
		}

	}

	void rys()
	{
		COORD start;
		start.X = 0;
		start.Y = 0;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), start);

		HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// wy��czenie kursora
		CONSOLE_CURSOR_INFO     cursorInfo;

		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(hConsole, &cursorInfo);

		SetConsoleTextAttribute(hConsole, 480);
		cout << "\311";
		SetConsoleTextAttribute(hConsole, 482);

		for (int i = 0; i < szerokosc - 2; i++)
		{
			SetConsoleTextAttribute(hConsole, 480);
			cout << "\315";
			SetConsoleTextAttribute(hConsole, 482);
		}
		SetConsoleTextAttribute(hConsole, 480);
		cout << "\273";
		SetConsoleTextAttribute(hConsole, 482);
		cout << endl;
		for (int i = 0; i < wysokosc; i++)
		{
			for (int j = 0; j < szerokosc; j++)
			{
				if ((j == 0) || (j == szerokosc - 1))
				{
					SetConsoleTextAttribute(hConsole, 480);
					cout << "\272";
					SetConsoleTextAttribute(hConsole, 482);
				}
				else if ((i == y) && (j == x))
				{
					cout << "\333"; //waz
				}
				else if ((owoX == j) && (owoY == i))
				{
					SetConsoleTextAttribute(hConsole, 484);
					cout << "\317"; //owoc
					SetConsoleTextAttribute(hConsole, 482);
				}
				else if ((owo2X == j) && (owo2Y == i))
				{
					SetConsoleTextAttribute(hConsole, 484);
					cout << "\317"; //owoc
					SetConsoleTextAttribute(hConsole, 482);
				}
				else
				{
					bool pomoc = false;
					int k = 0;
					while (k < dlugosc)
					{
						if ((ogonX[k] == j) && (ogonY[k] == i))
						{
							cout << "\333"; //waz
							pomoc = true;
						}
						k++;
					}
					if (!pomoc)
					{
						cout << " ";
					}
				}

			}
			cout << endl;
		}

		SetConsoleTextAttribute(hConsole, 480);
		cout << "\310";
		SetConsoleTextAttribute(hConsole, 482);

		for (int i = 0; i < szerokosc - 2; i++)
		{
			SetConsoleTextAttribute(hConsole, 480);
			cout << "\315";
			SetConsoleTextAttribute(hConsole, 482);
		}
		SetConsoleTextAttribute(hConsole, 480);
		cout << "\274";
		SetConsoleTextAttribute(hConsole, 482);
		SetConsoleTextAttribute(hConsole, 480);
		if (wynik < 10) cout << "\nWynik: " << wynik << "                                ";
		if (wynik >= 10) cout << "\nWynik: " << wynik << "                               ";
	}
};



void get_IP(string &user_provided_ip, char * adresip)
{
	for (int i = 0; i < user_provided_ip.length(); i++)
	{
		adresip[i] = user_provided_ip[i];
	}

}


void WinCheck(int wynik_przeciwnika, int wynik_gracza)
{

	if (wynik_przeciwnika == 15)
	{
		cout << "WYGRAL PRZECIWNIK!";
		system("pause");
		exit(0);
	}
	else if (wynik_gracza == 15)
	{
		cout << "WYGRALES!";
		system("pause");
		exit(0);
	}
}

int main()
{
	srand(time(NULL));
	Multiplayer polaczenie;
	Grafika gra;
	string user_provided_ip; //IP ktore podaje uzytkownik w wygodnej formie stringa
	int wynik_przeciwnika = 0;
	int wybor;
	cout << "Wybierz tryb:" << endl << "1.Host" << endl << "2.Klient" << endl;
	cin >> wybor;

	if (wybor == 1)
	{
		system("cls");
		cout << "Podaj TWOJE IP: ";
		cin >> user_provided_ip;
		char pojemnik_na_adres[15] = "";
		char *adresip = pojemnik_na_adres;
		get_IP(user_provided_ip, adresip);
		polaczenie.inicjalizacja(adresip);
		polaczenie.serwer_inicjalizacja();
	}
	else if (wybor == 2)
	{
		system("cls");
		cout << "Podaj IP HOSTA: ";
		cin >> user_provided_ip;
		char pojemnik_na_adres[15] = "";
		char *adresip = pojemnik_na_adres;
		get_IP(user_provided_ip, adresip);
		polaczenie.inicjalizacja(adresip);
		polaczenie.klient_inicjalizacja();
	}


	while (1)
	{
		string wiadomosc;
		string odbior;
		gra.rys();
		gra.sterowanie();
		gra.kolizje();
		gra.zjadanie_ogona(gra.wynik);
		gra.logika_owocu();
		gra.ogonek();
		wiadomosc = gra.wynik + '0';

		if (wybor == 1)
		{
			odbior = polaczenie.komunikacja_serwer(wiadomosc);
		}
		else if (wybor == 2)
		{
			odbior = polaczenie.komunikacja_klient(wiadomosc);
		}
		wynik_przeciwnika = odbior[0];
		WinCheck(wynik_przeciwnika, gra.wynik);
		cout << endl << endl << "Wynik przeciwnika: " << odbior[0];
		Sleep(100);
	}
}