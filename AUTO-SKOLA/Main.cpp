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
		cout << "16.Prikazi sve kandidate i njihove statuse" << endl;
		cout << "17.Prikazi broj polaganja za svakog instruktora" << endl;
		cout << "18.Prikazi sve zauzete instruktore(instruktori koji imaju vise od 1 aktivnog polaganja)" << endl;
		cout << "19.Izlaz" << endl;
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
			string DatumISO = to_string(godina) + "-" + (mjesec < 10 ? "0" : "") +  to_string(mjesec) + "-" + (dan < 10 ? "0" : "") + to_string(dan);
			string JMBG;
			cout << "Unesite JMBG kandidata: ";
			getline(cin, JMBG);
			baza.DodajKandidata(ImePrezime, DatumISO, JMBG);
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
			string DatumPolaganjaISO = to_string(godina) + "-" + (mjesec < 10 ? "0" : "") + to_string(mjesec) + "-" + (dan < 10 ? "0" : "") + to_string(dan);
			baza.ZakaziPolaganje(JMBGKandidata, JMBGInstruktora, Automobil, Kategorija, DatumPolaganjaISO);
			break;
		}
		case 11:
		{
			int dan, mjesec, godina;
			cout << "Unesite dan polaganja za koje zelite unijeti rezultate: ";
			cin >> dan;
			cout << "Unesite mjesec polaganja za koje zelite unijeti rezultate: ";
			cin >> mjesec;
			cout << "Unesite godinu polaganja za koje zelite unijeti rezultate: ";
			cin >> godina;
			cin.ignore();
			string DatumISO = to_string(godina) + "-" + (mjesec < 10 ? "0" : "") + to_string(mjesec) + "-" + (dan < 10 ? "0" : "") + to_string(dan);
			baza.UnesiRezlutatePolaganja(DatumISO);
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
			baza.PrikaziSveKandidateSaNjihovimStatusom();
			break;
		}
		case 17:
		{
			baza.PrikaziBrojPolaganjaZaSveInstruktore();
			break;
		}
		case 18:
		{
			baza.PrikaziSveInstruktoreKojiImajuViseOdJednogAktivnogPolaganja();
			break;
		}
		case 19:
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

	}while (izbor != 19);
	
}