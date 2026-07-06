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
void Baza::DodajAutomobil(string Marka, string Registracija) {
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Automobili WHERE RegistarskaOznaka='" + Registracija + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &count, nullptr);
	if (count > 0)
		cout << "GRESKA PRI DODAVANJU AUTOMOBILA-AUTOMOBIL VEC POSTOJI!!!" << endl;
	string sql =
		"INSERT INTO Automobili(MarkaAutomobila, RegistarskaOznaka) VALUES('"
		+ Marka + "', '" + Registracija + "');";
	IzvrsiUpit(sql, "GRESKA PRI DODAVANJU AUTOMOBILA");

}
void Baza::UkloniAutomobil(string Registracija) {
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Automobili WHERE RegistarskaOznaka='" + Registracija + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &count, nullptr);
	if (count == 0)
		cout << "GRESKA PRI UKLANJANU AUTOMOBILA-TRAZENI AUTOMOBIL NE POSTOJI!!!" << endl;
	string sql =
		"DELETE FROM Automobili WHERE RegistarskaOznaka='" + Registracija + "';";
	IzvrsiUpit(sql, "GRESKA PRI UKLANJANJU AUTOMOBILA");
}
void Baza::PrikaziSveAutomobile() {
	string sql =
		"SELECT * FROM Automobili;";
	sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << podaci[i] << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, nullptr);
}
void Baza::DodajInstruktora(string ImePrezime, string JMBG) {
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Instruktori WHERE JMBG='" + JMBG + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &count, nullptr);
	if (count > 0)
		cout << "GRESKA PRI DODAVANJU INSTRUKTORA-INSTRUKTOR VEC POSTOJI!!!" << endl;
	string sql =
		"INSERT INTO Instruktori(ImePrezime, JMBG) VALUES('"
		+ ImePrezime + "', '" + JMBG + "');";
	IzvrsiUpit(sql, "GRESKA PRI DODAVANJU INSTRUKTORA");
}
void Baza::UkloniInstruktora(string JMBG) {
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Instruktori WHERE JMBG='" + JMBG + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &count, nullptr);
	if (count == 0)
		cout << "GRESKA PRI BRISANJU INSTRUKTORA-TRAZENI INSTRUKTOR NE POSTOJI!!!" << endl;
	string sql =
		"DELETE FROM Instruktori WHERE JMBG='" + JMBG + "';";
	IzvrsiUpit(sql, "GRESKA PRI BRISANJU INSTRUKTORA");
}
void Baza::PrikaziSveInstruktore() {
	string sql =
		"SELECT * FROM Instruktori;";
	sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << podaci[i] << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, nullptr);
}
void Baza::DodajKandidata(string ImePrezime, string DatumRodjenja, string JMBG) {
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Kandidati WHERE JMBG='" + JMBG + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &count, nullptr);
	if (count > 0)
		cout << "GRESKA PRI DODAVANJU KANDIDATA-KANDIDAT VEC POSTOJI" << endl;
	string sql =
		"INSERT INTO Kandidati(ImePrezime, DatumRodjenja, JMBG) VALUES('"
		+ ImePrezime + "', '" + DatumRodjenja + "', '" + JMBG + "');";
	IzvrsiUpit(sql, "GRESKA PRI DODAVANJU KANDIDATA");
}
void Baza::UkloniKandidata(string JMBG) {
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Kandidati WHERE JMBG='" + JMBG + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &count, nullptr);
	if (count == 0)
		cout << "GRESKA PRI BRISANJU KANDIDATA-NE POSTOJI TRAZENI KANDIDAT" << endl;
	string sql =
		"DELETE FROM Kandidati WHERE JMBG='" + JMBG + "';";
	IzvrsiUpit(sql, "GRESKA PRI BRISANJU KANDIDATA");
}
void Baza::PrikaziSveKandidate() {
	string sql =
		"SELECT * FROM Kandidati;";
	sqlite3_exec(db, sql.c_str(),
		[](void* data, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << podaci[i] << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, nullptr);
}