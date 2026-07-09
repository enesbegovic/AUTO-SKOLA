#include <iostream>
#include <string>
#include "Baza.h"
using namespace std;
int main() {
	Baza baza;
	baza.KreirajTabelu();
	int izbor;
	do {
		cout << "MENI:" << endl;
		cout << "1.Dodaj kandidata" << endl;
		cout << "2.Ukloni kandidata" << endl;
		cout << "3.Prikazi sve kandidate" << endl;
		cout << "4.Dodaj instruktora" << endl;
		cout << "5.Ukloni instruktora" << endl;
		cout << "6.Prikazi sve instruktore" << endl;
		cout << "7.Dodaj automobil" << endl;
		cout << "8.Ukloni automobil" << endl;
		cout << "9.Prikazi sve automobile" << endl;
		cout << "10.Zakazi polaganje" << endl;
		cout << "11.Unesi rezultate polaganja" << endl;
		cout << "12.Prikazi sva polaganja" << endl;
		cout << "13.Prikazi sva aktivna polaganja" << endl;
		cout << "14.Prikazi sva zavrsena polaganja" << endl;
		cout << "15.Prikazi najmlajdeg kandidata" << endl;
		cout << "16.Prikazi najstarijeg kandidata" << endl;
		cout << "17.Prikazi sve kandidate i njihove statuse" << endl;
		cout << "18.Prikazi broj polaganja za svakog instruktora" << endl;
		cout << "19.Prikazi sve zauzete instruktore(instruktori koji imaju vise od 1 aktivnog polaganja)" << endl;
		cout << "20.Izlaz" << endl;
		cin >> izbor;
		cin.ignore();
		switch (izbor) {
		case 1:
		{
			string ImePrezime;
			cout << "Unesite ime i prezime kandidata: ";
			getline(cin, ImePrezime);
			int dan, mjesec, godina;
			cout << "Unesite dan rodjenja kandidata: ";
			cin >> dan;
			cout << "Unesite mjesec rodjenja kandidata: ";
			cin >> mjesec;
			cout << "Unesite godinu rodjenja kandidata: ";
			cin >> godina;
			cin.ignore();
			string DatumStr = to_string(dan) + "." + to_string(mjesec) + "." + to_string(godina);
			string JMBG;
			cout << "Unesite JMBG kandidata: ";
			getline(cin, JMBG);
			baza.DodajKandidata(ImePrezime, DatumStr, JMBG);
			break;
		}
		case 2:
		{
			string JMBG;
			cout << "Unesite JMBG kandidata kojeg zelite ukloniti: ";
			getline(cin, JMBG);
			baza.UkloniKandidata(JMBG);
			break;
		}
		case 3:
		{
			baza.PrikaziSveKandidate();
			break;
		}
		case 4:
		{
			string ImePrezime;
			cout << "Unesite ime i prezime instruktora: ";
			getline(cin, ImePrezime);
			string JMBG;
			cout << "Unesite JMBG: ";
			getline(cin, JMBG);
			baza.DodajInstruktora(ImePrezime, JMBG);
			break;
		}
		case 5:
		{
			string JMBG;
			cout << "Unesite JMBG instruktora kojeg zelite ukloniti: ";
			getline(cin, JMBG);
			baza.UkloniInstruktora(JMBG);
			break;
		}
		case 6:
		{
			baza.PrikaziSveInstruktore();
			break;
		}
		case 7:
		{
			string Naziv;
			cout << "Unesite Marku i model automobila: ";
			getline(cin, Naziv);
			string Registracija;
			cout << "Unesite registarsku oznaku automobila: ";
			getline(cin, Registracija);
			baza.DodajAutomobil(Naziv, Registracija);
			break;
		}
		case 8:
		{
			string Registracija;
			cout << "Unesite registarsku oznaku automobila kojeg zelite ukloniti: ";
			getline(cin, Registracija);
			baza.UkloniAutomobil(Registracija);
			break;
		}
		case 9:
		{
			baza.PrikaziSveAutomobile();
			break;
		}
		case 10:
		{
			string JMBGKandidata;
			cout << "Unesite JMBG kandidata kojeg prijavljujete za polaganje: ";
			getline(cin, JMBGKandidata);
			string JMBGInstruktora;
			cout << "Unesite JMBG instruktor: ";
			getline(cin, JMBGInstruktora);
			string Automobil;
			cout << "Unesite registarsku oznaku automobila na kojem kandidat polaze: ";
			getline(cin, Automobil);
			string Kategorija;
			cout << "Unesite oznaku kategorije koju kandidat polaze: ";
			getline(cin, Kategorija);
			int dan, mjesec, godina;
			cout << "Unesite dan polaganja: ";
			cin >> dan;
			cout << "Unesite mjesec polaganja: ";
			cin >> mjesec;
			cout << "Unesite godinu polaganja: ";
			cin >> godina;
			cin.ignore();
			string DatumPolaganjaStr = to_string(dan) + "." + to_string(mjesec) + "." + to_string(godina);
			baza.ZakaziPolaganje(JMBGKandidata, JMBGInstruktora, Automobil, Kategorija, DatumPolaganjaStr);
			break;
		}
		case 11:
		{
			string JMBGKandidata;
			cout << "Unesite JMBG kandidata za kojeg unosite rezultate: ";
			getline(cin, JMBGKandidata);
			int dan, mjesec, godina;
			cout << "Unesite dan na koji je polagao kandidat: ";
			cin >> dan;
			cout << "Unesite mjesec na koji je polagao kandidat: ";
			cin >> mjesec;
			cout << "Unesite godinu na koju je polagao kandidat: ";
			cin >> godina;
			cin.ignore();
			string DatumStr = to_string(dan) + "." + to_string(mjesec) + "." + to_string(godina);
			string Odgovor;
			cout << "Da li je student polozio(DA/NE)" << endl;
			cin >> Odgovor;
			bool Polozio = (Odgovor == "Da" || Odgovor == "DA");
			if (Polozio)
				cout << "KANDIDAT JE POLOZIO" << endl;
			else
				cout << "KANDIDAT NIJE POLOZIO" << endl;
			baza.UnesiRezlutatePolaganja(JMBGKandidata, DatumStr, Polozio);
			break;
		}
		case 12:
		{
			baza.PrikaziSvaPolaganja();
			break;
		}
		case 13:
		{
			baza.PrikaziAktivnaPolaganja();
			break;
		}
		case 14:
		{
			baza.PrikaziSvaZavrsenaPolaganja();
			break;
		}
		case 15:
		{
			baza.PrikaziNajmladjegKandidata();
			break;
		}
		case 16:
		{
			baza.PrikaziNajstarijegKandidata();
			break;
		}
		case 17:
		{
			baza.PrikaziSveKandidateSaNjihovimStatusom();
			break;
		}
		case 18:
		{
			baza.PrikaziBrojPolaganjaZaSveInstruktore();
			break;
		}
		case 19:
		{
			baza.PrikaziSveInstruktoreKojiImajuViseOdJednogAktivnogPolaganja();
			break;
		}
		case 20:
		{
			cout << "DOVIDJENJA" << endl;
			break;
		}
		default:
		{
			cout << "POGRESAN IZBOR!!!" << endl;
			break;
		}
		}

	}while (izbor != 20);
	
}