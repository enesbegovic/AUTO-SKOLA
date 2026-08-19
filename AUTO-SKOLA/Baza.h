#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "sqlite3.h"
#include "StmtPtr.h"
using namespace std;
struct Automobil {
	int ID;
	string Marka;
	string Registracija;
};
struct Instruktor {
	int ID;
	string ImePrezime;
	string JMBG;
};
struct Kandidat {
	int ID;
	string ImePrezime;
	string DatumRodjenja;
	string JMBG;
};
struct Polaganje {
	int ID;
	string ImePrezimeKandidata;
	string JMBG;
	string ImePrezimeInstruktora;
	string MarkaAutomobila;
	string RegistarskaOznaka;
	string Kategorija;
	string DatumPolaganja;
	string Polozio;
};
struct StatusKandidata {
	int ID;
	string ImePrezime;
	string DatumRodjenja;
	string JMBG;
	string Status;
};
struct PolaganjaInstruktora {
	int ID;
	string ImePrezime;
	string JMBG;
	int BrojPolaganja;
};
class Baza {
	sqlite3* db;
public:
	Baza(string putanjaBaze="AUTO-SKOLA.db");
	~Baza();
	void IzvrsiUpit(string sql, string PorukaGreske);
	void KreirajTabelu();
	vector<Automobil>UzmiSveAutomobile();
	vector<Instruktor>UzmiSveInstruktore();
	vector<Kandidat>UzmiSveKandidate();
	vector<Polaganje>UzmiSvaPolaganja();
	vector<Polaganje>UzmiSvaAktivnaPolaganja();
	vector<Polaganje>UzmiSvaZavrsenaPolaganja();
	Kandidat UzmiNajmladjegKandidata();
	vector<StatusKandidata>UzmiSveStatuseKandidata();
	vector<PolaganjaInstruktora> UzmiBrojPolaganjaZaSveInstruktore();
	vector<PolaganjaInstruktora> UzmiSveInstruktoreKojiImajuViseOdJednogAktivnogPolaganja();
	void UnesiRezultatePolaganja1(vector<pair<int, int>>Rezultati);
	void DodajAutomobil(string Marka, string Registracija);
	void UkloniAutomobil(string Registracija);
	void PrikaziSveAutomobile();
	int brojAutomobila();
	void DodajInstruktora(string ImePrezime, string JMBG);
	void UkloniInstruktora(string JMBG);
	void PrikaziSveInstruktore();
	int brojInstruktora();
	void DodajKandidata(string ImePrezime, string DatumRodjenja, string JMBG);
	void UkloniKandidata(string JMBG);
	void PrikaziSveKandidate();
	int brojKandidata();
	int ZakaziPolaganje(string Kandidat, string Instruktor, string Automobil, string Kategorija, string DatumPolaganja);
	void RezultatiPolaganja(int PolaganjeID, int Polozio);
	void UnesiRezlutatePolaganja(string DatumPolaganja);
	void PrikaziSvaPolaganja();
	int brojPolaganja();
	int StatusPolaganja(int  PolaganjeID);
	void PrikaziAktivnaPolaganja();
	void PrikaziSvaZavrsenaPolaganja();
	void PrikaziNajmladjegKandidata();
	void PrikaziSveKandidateSaNjihovimStatusom();
	void PrikaziBrojPolaganjaZaSveInstruktore();
	void PrikaziSveInstruktoreKojiImajuViseOdJednogAktivnogPolaganja();
};