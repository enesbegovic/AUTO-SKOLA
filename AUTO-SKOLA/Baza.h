#pragma once
#include <iostream>
#include <string>
#include "sqlite3.h"
using namespace std;
class Baza {
	sqlite3* db;
public:
	Baza(string putanjaBaze="AUTO-SKOLA.db");
	~Baza();
	void IzvrsiUpit(string sql, string PorukaGreske);
	void KreirajTabelu();
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