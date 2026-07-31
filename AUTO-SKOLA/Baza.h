#pragma once
#include <iostream>
#include <string>
#include "sqlite3.h"
using namespace std;
class Baza {
	sqlite3* db;
public:
	Baza();
	~Baza();
	void IzvrsiUpit(string sql, string PorukaGreske);
	void KreirajTabelu();
	void DodajAutomobil(string Marka, string Registracija);
	void UkloniAutomobil(string Registracija);
	void PrikaziSveAutomobile();
	void DodajInstruktora(string ImePrezime, string JMBG);
	void UkloniInstruktora(string JMBG);
	void PrikaziSveInstruktore();
	void DodajKandidata(string ImePrezime, string DatumRodjenja, string JMBG);
	void UkloniKandidata(string JMBG);
	void PrikaziSveKandidate();
	void ZakaziPolaganje(string Kandidat, string Instruktor, string Automobil, string Kategorija, string DatumPolaganja);
	void UnesiRezlutatePolaganja(string DatumPolaganja);
	void PrikaziSvaPolaganja();
	void PrikaziAktivnaPolaganja();
	void PrikaziSvaZavrsenaPolaganja();
	void PrikaziNajmladjegKandidata();
	void PrikaziSveKandidateSaNjihovimStatusom();
	void PrikaziBrojPolaganjaZaSveInstruktore();
	void PrikaziSveInstruktoreKojiImajuViseOdJednogAktivnogPolaganja();
};