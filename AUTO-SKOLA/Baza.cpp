#include <iostream>
#include <string>
#include "Baza.h"
using namespace std;
Baza::Baza() {
	sqlite3_open("AUTO-SKOLA.db", &db);
}
Baza::~Baza() {
	sqlite3_close(db);
}
void Baza::IzvrsiUpit(string sql, string PorukaGreske) {
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		cout << PorukaGreske << ": " << errMsg << endl;
		sqlite3_free(errMsg);
	}
}
void Baza::KreirajTabelu() {
	string KreirajAutomobile =
		"CREATE TABLE IF NOT EXISTS Automobili("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"MarkaAutomobila TEXT NOT NULL,"
		"RegistarskaOznaka TEXT NOT NULL UNIQUE);";
	IzvrsiUpit(KreirajAutomobile, "GRESKA PRI KREIRANJU TABELE AUTOMOBILI");

	string KreirajInstruktore =
		"CREATE TABLE IF NOT EXISTS Instruktori("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"ImePrezime TEXT NOT NULL,"
		"JMBG TEXT NOT NULL UNIQUE);";
	IzvrsiUpit(KreirajInstruktore, "GRESKA PRI KREIRANJU TABELE INSTRUKTORI");

	string KreirajKandidate =
		"CREATE TABLE IF NOT EXISTS Kandidati("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"ImePrezime TEXT NOT NULL,"
		"DatumRodjenja TEXT NOT NULL,"
		"JMBG TEXT NOT NULL UNIQUE);";
	IzvrsiUpit(KreirajKandidate, "GRESKA PRI KREIRANJU TABELE KANDIDATI");

	string KreirajPolaganja =
		"CREATE TABLE IF NOT EXISTS Polaganja("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"KandidatID INTEGER NOT NULL,"
		"InstruktorID INTEGER NOT NULL,"
		"AutomobilID INTEGER NOT NULL,"
		"Kategorija TEXT NOT NULL,"
		"DatumPolaganja TEXT NOT NULL,"
		"Polozio INTEGER DEFAULT 0);";
	IzvrsiUpit(KreirajPolaganja, "GRESKA PRI KREIRANJU TABELE POLAGANJA");
		
}