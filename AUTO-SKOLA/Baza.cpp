#include <iostream>
#include <string>
#include "Baza.h"
using namespace std;
Baza::Baza(string putanjaBaze) {
	sqlite3_open(putanjaBaze.c_str(), &db);
	sqlite3_exec(db, "PRAGMA foreign_keys=ON;", nullptr, nullptr, nullptr);
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
		"Polozio INTEGER,"
		"FOREIGN KEY (KandidatID) REFERENCES Kandidati(ID) ON DELETE RESTRICT,"
		"FOREIGN KEY (InstruktorID) REFERENCES Instruktori(ID) ON DELETE RESTRICT,"
		"FOREIGN KEY (AutomobilID) REFERENCES Automobili(ID) ON DELETE RESTRICT);";
	IzvrsiUpit(KreirajPolaganja, "GRESKA PRI KREIRANJU TABELE POLAGANJA");
		
}
void Baza::DodajAutomobil(string Marka, string Registracija) {
	sqlite3_stmt* stmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Automobili WHERE RegistarskaOznaka=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjera, 1, Registracija.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera, 0);
	}
	sqlite3_finalize(stmtProvjera);
	if (count > 0) {
		cout << "GRESKA PRI DODAVANJU AUTOMOBILA-AUTOMOBIL VEC POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"INSERT INTO Automobili(MarkaAutomobila, RegistarskaOznaka) VALUES(?, ?);";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI DODAVANJA AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, Marka.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, Registracija.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI DODAVANJU AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "AUTOMOBIL USPJESNO DODAN" << endl;
}
void Baza::UkloniAutomobil(string Registracija) {
	sqlite3_stmt* stmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT (*) FROM Automobili WHERE RegistarskaOznaka=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjera, 1, Registracija.c_str(),-1,SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera, 0);
	}
	sqlite3_finalize(stmtProvjera);
	if (count == 0) {
		cout << "GRESKA PRI UKLANJANU AUTOMOBILA-TRAZENI AUTOMOBIL NE POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"DELETE FROM Automobili WHERE RegistarskaOznaka=?;";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UKLANJANJA AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, Registracija.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI UKLANJANJU AUTOMOBILA" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "AUTOMOBIL USPJESO UKLONJEN" << endl;
}
void Baza::PrikaziSveAutomobile() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT * FROM Automobili;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt, 0);
		string Marka = (const char*)sqlite3_column_text(stmt, 1);
		string Registracija = (const char*)sqlite3_column_text(stmt, 2);

		cout << "ID:" << ID << endl;
		cout << "Marka Automobila:" << Marka << endl;
		cout << "Registarska Oznaka:" << Registracija << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
int Baza::brojAutomobila() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT COUNT (*) FROM Automobili;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	int count = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return count;
}
void Baza::DodajInstruktora(string ImePrezime, string JMBG) {
	sqlite3_stmt* stmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Instruktori WHERE JMBG=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjera, 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera, 0);
	}
	sqlite3_finalize(stmtProvjera);
	if (count > 0) {
		cout << "GRESKA PRI DODAVANJU INSTRUKTORA-INSTRUKTOR VEC POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"INSERT INTO Instruktori(ImePrezime, JMBG) VALUES(?, ?);";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI DODAVANJA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, ImePrezime.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI DODAVANJU INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "INSTRUKTOR USPJESNO DODAN" << endl;
}
void Baza::UkloniInstruktora(string JMBG) {
	sqlite3_stmt* stmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT (*) FROM Instruktori WHERE JMBG=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjera, 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera, 0);
	}
	sqlite3_finalize(stmtProvjera);
	if (count == 0) {
		cout << "GRESKA PRI UKLANJANU INSTRUKTORA-TRAZENI INSTRUKTOR NE POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"DELETE FROM Instruktori WHERE JMBG=?;";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UKLANJANJA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI UKLANJANJU INSTRUKTORA" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "INSTRUKTOR USPJESO UKLONJEN" << endl;
}
void Baza::PrikaziSveInstruktore() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT * FROM Instruktori;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt, 0);
		string Ime = (const char*)sqlite3_column_text(stmt, 1);
		string JMBG = (const char*)sqlite3_column_text(stmt, 2);

		cout << "ID:" << ID << endl;
		cout << "Ime I Prezime Instruktora:" << Ime << endl;
		cout << "JMBG:" << JMBG << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
int Baza::brojInstruktora() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT COUNT (*) FROM Instruktori;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	int count = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return count;
}
void Baza::DodajKandidata(string ImePrezime, string DatumRodjenja, string JMBG) {
	sqlite3_stmt* stmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT (*) FROM Kandidati WHERE JMBG=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjera, 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera, 0);
	}
	sqlite3_finalize(stmtProvjera);
	if (count > 0) {
		cout << "GRESKA PRI DODAVANJU KANDIDATA-KANDIDAT VEC POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"INSERT INTO Kandidati(ImePrezime, DatumRodjenja, JMBG) VALUES(?, ?, ?);";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI DODAVANJA KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, ImePrezime.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, DatumRodjenja.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI DODAVANJU KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "KANDIDAT USPJESNO DODAN" << endl;
}
void Baza::UkloniKandidata(string JMBG) {
	sqlite3_stmt* stmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT (*) FROM Kandidati WHERE JMBG=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjera, 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera, 0);
	}
	sqlite3_finalize(stmtProvjera);
	if (count == 0) {
		cout << "GRESKA PRI UKLANJANU KANDIDATA-TRAZENI KANDIDAT NE POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"DELETE FROM Kandidati WHERE JMBG=?;";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UKLANJANJA KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI UKLANJANJU KANDIDATA" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "KANDIDAT USPJESNO UKLONJEN" << endl;
}
void Baza::PrikaziSveKandidate() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT * FROM Kandidati;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt, 0);
		string Ime = (const char*)sqlite3_column_text(stmt, 1);
		string Datum = (const char*)sqlite3_column_text(stmt, 2);
		string JMBG = (const char*)sqlite3_column_text(stmt, 3);

		cout << "ID:" << ID << endl;
		cout << "Ime I Prezime Kandidata:" << Ime << endl;
		cout << "Datum Rodjenja:" << Datum << endl;
		cout << "JMBG:" << JMBG << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
int Baza::brojKandidata() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT COUNT (*) FROM Kandidati;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	int count = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return count;
}
int Baza::ZakaziPolaganje(string Kandidat, string Instruktor, string Automobil, string Kategorija, string DatumPolaganja) {
	sqlite3_stmt* stmtKandidat;
	int KandidatID = 0;
	string ProvjeraKandidata =
		"SELECT ID FROM Kandidati WHERE JMBG=?;";
	int rc = sqlite3_prepare_v2(db, ProvjeraKandidata.c_str(), -1, &stmtKandidat, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	sqlite3_bind_text(stmtKandidat, 1, Kandidat.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtKandidat) == SQLITE_ROW) {
		KandidatID = sqlite3_column_int(stmtKandidat, 0);
	}
	sqlite3_finalize(stmtKandidat);
	sqlite3_stmt* stmtInstruktor;
	int InstruktorID = 0;
	string ProvjeraInstruktora =
		"SELECT ID FROM Instruktori WHERE JMBG=?;";
	int rc1 = sqlite3_prepare_v2(db, ProvjeraInstruktora.c_str(), -1, &stmtInstruktor, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	sqlite3_bind_text(stmtInstruktor, 1, Instruktor.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtInstruktor) == SQLITE_ROW) {
		InstruktorID = sqlite3_column_int(stmtInstruktor, 0);
	}
	sqlite3_finalize(stmtInstruktor);
	sqlite3_stmt* stmtAutomobil;
	int AutomobilID = 0;
	string ProvjeraAutomobila =
		"SELECT ID FROM Automobili WHERE RegistarskaOznaka=?;";
	int rc2 = sqlite3_prepare_v2(db, ProvjeraAutomobila.c_str(), -1, &stmtAutomobil, nullptr);
	if (rc2 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	sqlite3_bind_text(stmtAutomobil, 1, Automobil.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtAutomobil) == SQLITE_ROW) {
		AutomobilID = sqlite3_column_int(stmtAutomobil, 0);
	}
	sqlite3_finalize(stmtAutomobil);
	if (KandidatID == 0 || InstruktorID == 0 || AutomobilID == 0) {
		cout << "GRESKA PRI ZAKAZIVANJU POLAGANJA-NEPOSTOJECI KANDIDAT ILI NEPOSTOJECI INSTRUKTOR ILI NEPOSTOJECI AUTOMOBIL" << endl;
		return 0;
	}
	sqlite3_stmt* stmtProvjeraDuplikata;
	int count = 0;
	string Duplikati =
		"SELECT COUNT (*) FROM Polaganja WHERE KandidatID=? AND DatumPolaganja=?;";
	int rc3 = sqlite3_prepare_v2(db, Duplikati.c_str(), -1, &stmtProvjeraDuplikata, nullptr);
	if (rc3 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	sqlite3_bind_int(stmtProvjeraDuplikata, 1, KandidatID);
	sqlite3_bind_text(stmtProvjeraDuplikata, 2, DatumPolaganja.c_str(),-1,SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjeraDuplikata) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjeraDuplikata, 0);
	}
	sqlite3_finalize(stmtProvjeraDuplikata);
	if (count > 0) {
		cout << "GRESKA PRI ZAKAZIVANJU POLAGANJA-POLAGANJE VEC POSTOJI" << endl;
		return 0;
	}
	sqlite3_stmt* stmt;
	string sql =
		"INSERT INTO Polaganja(KandidatID, InstruktorID, AutomobilID, Kategorija, DatumPolaganja, Polozio) VALUES(?, ?, ?, ?, ?, ?);";
	int rc4 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc4 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI ZAKAZIVANJA POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	sqlite3_bind_int(stmt, 1, KandidatID);
	sqlite3_bind_int(stmt, 2, InstruktorID);
	sqlite3_bind_int(stmt, 3, AutomobilID);
	sqlite3_bind_text(stmt, 4, Kategorija.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 5, DatumPolaganja.c_str(), -1, SQLITE_TRANSIENT);
	rc4 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc4 != SQLITE_DONE) {
		cout << "GRESKA PRI ZAKAZIVANJU POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	int PolaganjeID = sqlite3_last_insert_rowid(db);
	cout << "POLAGANJE USPJESNO ZAKAZANO" << endl;
	return PolaganjeID;
}
void Baza::RezultatiPolaganja(int PolaganjeID, int Polozio) {
	sqlite3_stmt* stmt;
	string sql =
		"UPDATE Polaganja SET Polozio=? WHERE ID=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UNOSA REZULTATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_int(stmt, 1, Polozio);
	sqlite3_bind_int(stmt, 2, PolaganjeID);
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc != SQLITE_DONE) {
		cout << "GRESKA PRI UNOSENJU REZULTATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
}
void Baza::UnesiRezlutatePolaganja(string DatumPolaganja) {
	sqlite3_stmt* stmtPolaganje;
	string sql =
		"SELECT Polaganja.ID, Kandidati.ImePrezime "
		"FROM Polaganja "
		"JOIN Kandidati ON Polaganja.KandidatID=Kandidati.ID "
		"WHERE Polaganja.DatumPolaganja=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmtPolaganje, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UZIMANJA SVIH KANDIDATA ZA TRAZENO POLAGANJE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtPolaganje, 1, DatumPolaganja.c_str(), -1, SQLITE_TRANSIENT);

	
	int brojKandidata=0;
	IzvrsiUpit("BEGIN TRANSACTION;", "GRESKA PRI POKRETANJU TRANSAKCIJE");
	while (sqlite3_step(stmtPolaganje) == SQLITE_ROW) {
		brojKandidata++;

		int PolaganjeID = sqlite3_column_int(stmtPolaganje, 0);
		string ImePrezime = (const char*)sqlite3_column_text(stmtPolaganje, 1);
		string Odgovor;
		cout << "Da Li Je Kandidata "<<ImePrezime<<" Polozio(DA / NE) : ";
		cin >> Odgovor;
		int Polozio = (Odgovor == "DA" || Odgovor == "Da") ? 1 : 0;
		cout << (Polozio ? "Kandidat Je Polozio" : "Kandidat Nije Polozio") << endl;
		RezultatiPolaganja(PolaganjeID, Polozio);
	}
	sqlite3_finalize(stmtPolaganje);
	IzvrsiUpit("COMMIT;", "GRESKA PRI COMMIT-u");
	if (brojKandidata == 0) {
		cout << "GRESKA PRILIKOM UNOSA DATUMA POLAGANJA" << endl;
		return;
	}
	else {
		cout << "USPJESNO AZURIRANI REZULTATI ZA TRAZENO POLAGANJE" << endl;
	}
}
void Baza::PrikaziSvaPolaganja() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Kandidati.ImePrezime, Kandidati.JMBG, Instruktori.ImePrezime, Automobili.MarkaAutomobila, Automobili.RegistarskaOznaka, Polaganja.Kategorija, Polaganja.DatumPolaganja, "
		"CASE WHEN Polozio IS NULL THEN 'KANDIDAT CEKA POLAGANJE' "
		"WHEN Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Polozio "
		"FROM Polaganja "
		"JOIN Kandidati ON Polaganja.KandidatID=Kandidati.ID "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"JOIN Automobili ON Polaganja.AutomobilID=Automobili.ID;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		string Kandidat = (const char*)sqlite3_column_text(stmt, 0);
		string JMBG = (const char*)sqlite3_column_text(stmt, 1);
		string Instruktor = (const char*)sqlite3_column_text(stmt, 2);
		string Marka = (const char*)sqlite3_column_text(stmt, 3);
		string Registracija = (const char*)sqlite3_column_text(stmt, 4);
		string Kategorija = (const char*)sqlite3_column_text(stmt, 5);
		string DatumPolaganja = (const char*)sqlite3_column_text(stmt, 6);
		string Polozio = (const char*)sqlite3_column_text(stmt, 7);

		cout << "Ime I Prezime Kandidata:" << Kandidat << endl;
		cout << "JMBG Kandidata:" << JMBG << endl;
		cout << "Ime I Prezime Instruktor:" << Instruktor << endl;
		cout << "Marka Automobila:" << Marka << endl;
		cout << "Registarska Oznaka:" << Registracija << endl;
		cout << "Kategorija:" << Kategorija << endl;
		cout << "Datum Polaganja:" << DatumPolaganja << endl;
		cout << "Rezultat:" << Polozio << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
int Baza::brojPolaganja() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT COUNT (*) FROM Polaganja;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	int count = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return count;
}
int Baza::StatusPolaganja(int PolaganjeID) {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Polozio "
		"FROM Polaganja "
		"WHERE ID=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA ZAVRSENIH POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	sqlite3_bind_int(stmt, 1, PolaganjeID);
	int status = -1;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		status = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return status;
}
void Baza::PrikaziAktivnaPolaganja() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Kandidati.ImePrezime, Kandidati.JMBG, Instruktori.ImePrezime, Automobili.MarkaAutomobila, Automobili.RegistarskaOznaka, Polaganja.Kategorija, Polaganja.DatumPolaganja, "
		"CASE WHEN Polozio IS NULL THEN 'KANDIDAT CEKA POLAGANJE' "
		"WHEN Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Polozio "
		"FROM Polaganja "
		"JOIN Kandidati ON Polaganja.KandidatID=Kandidati.ID "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"JOIN Automobili ON Polaganja.AutomobilID=Automobili.ID "
		"WHERE Polozio IS NULL;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		string Kandidat = (const char*)sqlite3_column_text(stmt, 0);
		string JMBG = (const char*)sqlite3_column_text(stmt, 1);
		string Instruktor = (const char*)sqlite3_column_text(stmt, 2);
		string Marka = (const char*)sqlite3_column_text(stmt, 3);
		string Registracija = (const char*)sqlite3_column_text(stmt, 4);
		string Kategorija = (const char*)sqlite3_column_text(stmt, 5);
		string DatumPolaganja = (const char*)sqlite3_column_text(stmt, 6);
		string Polozio = (const char*)sqlite3_column_text(stmt, 7);

		cout << "Ime I Prezime Kandidata:" << Kandidat << endl;
		cout << "JMBG Kandidata:" << JMBG << endl;
		cout << "Ime I Prezime Instruktor:" << Instruktor << endl;
		cout << "Marka Automobila:" << Marka << endl;
		cout << "Registarska Oznaka:" << Registracija << endl;
		cout << "Kategorija:" << Kategorija << endl;
		cout << "Datum Polaganja:" << DatumPolaganja << endl;
		cout << "Rezultat:" << Polozio << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
	
}
void Baza::PrikaziSvaZavrsenaPolaganja() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Kandidati.ImePrezime, Kandidati.JMBG, Instruktori.ImePrezime, Automobili.MarkaAutomobila, Automobili.RegistarskaOznaka, Polaganja.Kategorija, Polaganja.DatumPolaganja, "
		"CASE WHEN Polozio IS NULL THEN 'KANDIDAT CEKA POLAGANJE' "
		"WHEN Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Polozio "
		"FROM Polaganja "
		"JOIN Kandidati ON Polaganja.KandidatID=Kandidati.ID "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"JOIN Automobili ON Polaganja.AutomobilID=Automobili.ID "
		"WHERE Polozio=1 OR Polozio=0;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		string Kandidat = (const char*)sqlite3_column_text(stmt, 0);
		string JMBG = (const char*)sqlite3_column_text(stmt, 1);
		string Instruktor = (const char*)sqlite3_column_text(stmt, 2);
		string Marka = (const char*)sqlite3_column_text(stmt, 3);
		string Registracija = (const char*)sqlite3_column_text(stmt, 4);
		string Kategorija = (const char*)sqlite3_column_text(stmt, 5);
		string DatumPolaganja = (const char*)sqlite3_column_text(stmt, 6);
		string Polozio = (const char*)sqlite3_column_text(stmt, 7);

		cout << "Ime I Prezime Kandidata:" << Kandidat << endl;
		cout << "JMBG Kandidata:" << JMBG << endl;
		cout << "Ime I Prezime Instruktor:" << Instruktor << endl;
		cout << "Marka Automobila:" << Marka << endl;
		cout << "Registarska Oznaka:" << Registracija << endl;
		cout << "Kategorija:" << Kategorija << endl;
		cout << "Datum Polaganja:" << DatumPolaganja << endl;
		cout << "Rezultat:" << Polozio << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
void Baza::PrikaziNajmladjegKandidata() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Kandidati.ImePrezime, Kandidati.DatumRodjenja, Kandidati.JMBG, julianday('now') - julianday(Kandidati.DatumRodjenja) AS Starost "
		"FROM Kandidati "
		"ORDER BY Starost ASC "
		"LIMIT 1;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		string Ime = (const char*)sqlite3_column_text(stmt, 0);
		string Datum = (const char*)sqlite3_column_text(stmt, 1);
		string JMBG = (const char*)sqlite3_column_text(stmt, 2);

		cout << "Ime I Prezime Kandidata:" << Ime << endl;
		cout << "Datum Rodjenja:" << Datum << endl;
		cout << "JMBG:" << JMBG << endl;
	}
	sqlite3_finalize(stmt);
}
void Baza::PrikaziSveKandidateSaNjihovimStatusom() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Kandidati.ImePrezime, Kandidati.DatumRodjenja, Kandidati.JMBG, "
		"CASE WHEN Polaganja.ID IS NULL THEN 'KANDIDAT NEMA ZAKAZANO POLAGANJE' "
		"WHEN Polaganja.Polozio IS NULL THEN 'CEKA IZLAZAK NA POLAGANJE/CEKA REZULTATE' "
		"WHEN Polaganja.Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Status "
		"FROM Kandidati "
		"JOIN Polaganja ON Kandidati.ID=Polaganja.KandidatID;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		string Ime = (const char*)sqlite3_column_text(stmt, 0);
		string Datum = (const char*)sqlite3_column_text(stmt, 1);
		string JMBG = (const char*)sqlite3_column_text(stmt, 2);
		string Status = (const char*)sqlite3_column_text(stmt, 3);

		cout << "Ime I Prezime Kandidata:" << Ime << endl;
		cout << "Datum Rodjenja:" << Datum << endl;
		cout << "JMBG:" << JMBG << endl;
		cout << "Status Polaganja Kandidata:" << Status << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
void Baza::PrikaziBrojPolaganjaZaSveInstruktore() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Instruktori.ImePrezime, COUNT (*) AS BrojPolaganja "
		"FROM Polaganja "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"GROUP BY Instruktori.ID;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		string Ime = (const char*)sqlite3_column_text(stmt, 0);
		int BrojPolaganja = sqlite3_column_int(stmt, 1);

		cout << "Ime I Prezime Instruktora:" << Ime << endl;
		cout << "Broj Polaganja:" << BrojPolaganja << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
void Baza::PrikaziSveInstruktoreKojiImajuViseOdJednogAktivnogPolaganja() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Instruktori.ImePrezime, COUNT(*) AS BrojAktivnihPolaganja "
		"FROM Polaganja "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"WHERE Polozio IS NULL OR Polozio=0 "
		"GROUP BY Instruktori.ID "
		"HAVING COUNT (*) > 1;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		string Ime = (const char*)sqlite3_column_text(stmt, 0);
		int BrojPolaganja = sqlite3_column_int(stmt, 1);

		cout << "Ime I Prezime Instruktora:" << Ime << endl;
		cout << "Broj Aktivnih Polaganja:" << BrojPolaganja << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}