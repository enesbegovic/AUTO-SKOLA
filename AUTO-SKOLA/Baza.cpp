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
	sqlite3_stmt* rawStmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Automobili WHERE RegistarskaOznaka=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &rawStmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmtProvjera(rawStmtProvjera);
	sqlite3_bind_text(stmtProvjera.get(), 1, Registracija.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera.get()) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera.get(), 0);
	}
	if (count > 0) {
		cout << "GRESKA PRI DODAVANJU AUTOMOBILA-AUTOMOBIL VEC POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* rawStmt;
	string sql =
		"INSERT INTO Automobili(MarkaAutomobila, RegistarskaOznaka) VALUES(?, ?);";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI DODAVANJA AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	sqlite3_bind_text(stmt.get(), 1, Marka.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt.get(), 2, Registracija.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt.get());
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI DODAVANJU AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "AUTOMOBIL USPJESNO DODAN" << endl;
}
void Baza::UkloniAutomobil(string Registracija) {
	sqlite3_stmt* rawStmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT (*) FROM Automobili WHERE RegistarskaOznaka=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &rawStmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmtProvjera(rawStmtProvjera);
	sqlite3_bind_text(stmtProvjera.get(), 1, Registracija.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera.get()) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera.get(), 0);
	}
	if (count == 0) {
		cout << "GRESKA PRI UKLANJANU AUTOMOBILA-TRAZENI AUTOMOBIL NE POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* rawStmt;
	string sql =
		"DELETE FROM Automobili WHERE RegistarskaOznaka=?;";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UKLANJANJA AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	sqlite3_bind_text(stmt.get(), 1, Registracija.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt.get());
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI UKLANJANJU AUTOMOBILA" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "AUTOMOBIL USPJESO UKLONJEN" << endl;
}
void Baza::PrikaziSveAutomobile() {
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT * FROM Automobili;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt.get(), 0);
		string Marka = (const char*)sqlite3_column_text(stmt.get(), 1);
		string Registracija = (const char*)sqlite3_column_text(stmt.get(), 2);

		cout << "ID:" << ID << endl;
		cout << "Marka Automobila:" << Marka << endl;
		cout << "Registarska Oznaka:" << Registracija << endl;
		cout << "---" << endl;
	}
}
int Baza::brojAutomobila() {
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT COUNT (*) FROM Automobili;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	StmtPtr stmt(rawStmt);
	int count = 0;
	if (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt.get(), 0);
	}
	return count;
}
void Baza::DodajInstruktora(string ImePrezime, string JMBG) {
	sqlite3_stmt* rawStmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT(*) FROM Instruktori WHERE JMBG=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &rawStmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmtProvjera(rawStmtProvjera);
	sqlite3_bind_text(stmtProvjera.get(), 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera.get()) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera.get(), 0);
	}
	if (count > 0) {
		cout << "GRESKA PRI DODAVANJU INSTRUKTORA-INSTRUKTOR VEC POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* rawStmt;
	string sql =
		"INSERT INTO Instruktori(ImePrezime, JMBG) VALUES(?, ?);";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI DODAVANJA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	sqlite3_bind_text(stmt.get(), 1, ImePrezime.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt.get(), 2, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt.get());
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI DODAVANJU INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "INSTRUKTOR USPJESNO DODAN" << endl;
}
void Baza::UkloniInstruktora(string JMBG) {
	sqlite3_stmt* rawStmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT (*) FROM Instruktori WHERE JMBG=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &rawStmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmtProvjera(rawStmtProvjera);
	sqlite3_bind_text(stmtProvjera.get(), 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera.get()) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera.get(), 0);
	}
	if (count == 0) {
		cout << "GRESKA PRI UKLANJANU INSTRUKTORA-TRAZENI INSTRUKTOR NE POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* rawStmt;
	string sql =
		"DELETE FROM Instruktori WHERE JMBG=?;";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UKLANJANJA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	sqlite3_bind_text(stmt.get(), 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt.get());
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI UKLANJANJU INSTRUKTORA" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "INSTRUKTOR USPJESO UKLONJEN" << endl;
}
void Baza::PrikaziSveInstruktore() {
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT * FROM Instruktori;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt.get(), 0);
		string Ime = (const char*)sqlite3_column_text(stmt.get(), 1);
		string JMBG = (const char*)sqlite3_column_text(stmt.get(), 2);

		cout << "ID:" << ID << endl;
		cout << "Ime I Prezime Instruktora:" << Ime << endl;
		cout << "JMBG:" << JMBG << endl;
		cout << "---" << endl;
	}
}
int Baza::brojInstruktora() {
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT COUNT (*) FROM Instruktori;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	StmtPtr stmt(rawStmt);
	int count = 0;
	if (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt.get(), 0);
	}
	return count;
}
void Baza::DodajKandidata(string ImePrezime, string DatumRodjenja, string JMBG) {
	sqlite3_stmt* rawStmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT (*) FROM Kandidati WHERE JMBG=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &rawStmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmtProvjera(rawStmtProvjera);
	sqlite3_bind_text(stmtProvjera.get(), 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera.get()) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera.get(), 0);
	}
	if (count > 0) {
		cout << "GRESKA PRI DODAVANJU KANDIDATA-KANDIDAT VEC POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* rawStmt;
	string sql =
		"INSERT INTO Kandidati(ImePrezime, DatumRodjenja, JMBG) VALUES(?, ?, ?);";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI DODAVANJA KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	sqlite3_bind_text(stmt.get(), 1, ImePrezime.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt.get(), 2, DatumRodjenja.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt.get(), 3, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt.get());
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI DODAVANJU KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "KANDIDAT USPJESNO DODAN" << endl;
}
void Baza::UkloniKandidata(string JMBG) {
	sqlite3_stmt* rawStmtProvjera;
	int count = 0;
	string Provjera =
		"SELECT COUNT (*) FROM Kandidati WHERE JMBG=?;";
	int rc = sqlite3_prepare_v2(db, Provjera.c_str(), -1, &rawStmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmtProvjera(rawStmtProvjera);
	sqlite3_bind_text(stmtProvjera.get(), 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera.get()) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera.get(), 0);
	}
	if (count == 0) {
		cout << "GRESKA PRI UKLANJANU KANDIDATA-TRAZENI KANDIDAT NE POSTOJI!!!" << endl;
		return;
	}
	sqlite3_stmt* rawStmt;
	string sql =
		"DELETE FROM Kandidati WHERE JMBG=?;";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UKLANJANJA KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	sqlite3_bind_text(stmt.get(), 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	rc1 = sqlite3_step(stmt.get());
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI UKLANJANJU KANDIDATA" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "KANDIDAT USPJESNO UKLONJEN" << endl;
}
void Baza::PrikaziSveKandidate() {
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT * FROM Kandidati;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt.get(), 0);
		string Ime = (const char*)sqlite3_column_text(stmt.get(), 1);
		string Datum = (const char*)sqlite3_column_text(stmt.get(), 2);
		string JMBG = (const char*)sqlite3_column_text(stmt.get(), 3);

		cout << "ID:" << ID << endl;
		cout << "Ime I Prezime Kandidata:" << Ime << endl;
		cout << "Datum Rodjenja:" << Datum << endl;
		cout << "JMBG:" << JMBG << endl;
		cout << "---" << endl;
	}
}
int Baza::brojKandidata() {
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT COUNT (*) FROM Kandidati;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	StmtPtr stmt(rawStmt);
	int count = 0;
	if (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt.get(), 0);
	}
	return count;
}
int Baza::ZakaziPolaganje(string Kandidat, string Instruktor, string Automobil, string Kategorija, string DatumPolaganja) {
	sqlite3_stmt* rawStmtKandidat;
	int KandidatID = 0;
	string ProvjeraKandidata =
		"SELECT ID FROM Kandidati WHERE JMBG=?;";
	int rc = sqlite3_prepare_v2(db, ProvjeraKandidata.c_str(), -1, &rawStmtKandidat, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	StmtPtr stmtKandidat(rawStmtKandidat);
	sqlite3_bind_text(stmtKandidat.get(), 1, Kandidat.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtKandidat.get()) == SQLITE_ROW) {
		KandidatID = sqlite3_column_int(stmtKandidat.get(), 0);
	}
	sqlite3_stmt* rawStmtInstruktor;
	int InstruktorID = 0;
	string ProvjeraInstruktora =
		"SELECT ID FROM Instruktori WHERE JMBG=?;";
	int rc1 = sqlite3_prepare_v2(db, ProvjeraInstruktora.c_str(), -1, &rawStmtInstruktor, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	StmtPtr stmtInstruktor(rawStmtInstruktor);
	sqlite3_bind_text(stmtInstruktor.get(), 1, Instruktor.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtInstruktor.get()) == SQLITE_ROW) {
		InstruktorID = sqlite3_column_int(stmtInstruktor.get(), 0);
	}
	sqlite3_stmt* rawStmtAutomobil;
	int AutomobilID = 0;
	string ProvjeraAutomobila =
		"SELECT ID FROM Automobili WHERE RegistarskaOznaka=?;";
	int rc2 = sqlite3_prepare_v2(db, ProvjeraAutomobila.c_str(), -1, &rawStmtAutomobil, nullptr);
	if (rc2 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	StmtPtr stmtAutomobil(rawStmtAutomobil);
	sqlite3_bind_text(stmtAutomobil.get(), 1, Automobil.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtAutomobil.get()) == SQLITE_ROW) {
		AutomobilID = sqlite3_column_int(stmtAutomobil.get(), 0);
	}
	if (KandidatID == 0 || InstruktorID == 0 || AutomobilID == 0) {
		cout << "GRESKA PRI ZAKAZIVANJU POLAGANJA-NEPOSTOJECI KANDIDAT ILI NEPOSTOJECI INSTRUKTOR ILI NEPOSTOJECI AUTOMOBIL" << endl;
		return 0;
	}
	sqlite3_stmt* rawStmtProvjeraDuplikata;
	int count = 0;
	string Duplikati =
		"SELECT COUNT (*) FROM Polaganja WHERE KandidatID=? AND DatumPolaganja=?;";
	int rc3 = sqlite3_prepare_v2(db, Duplikati.c_str(), -1, &rawStmtProvjeraDuplikata, nullptr);
	if (rc3 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	StmtPtr stmtProvjeraDuplikata(rawStmtProvjeraDuplikata);
	sqlite3_bind_int(stmtProvjeraDuplikata.get(), 1, KandidatID);
	sqlite3_bind_text(stmtProvjeraDuplikata.get(), 2, DatumPolaganja.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjeraDuplikata.get()) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjeraDuplikata.get(), 0);
	}
	if (count > 0) {
		cout << "GRESKA PRI ZAKAZIVANJU POLAGANJA-POLAGANJE VEC POSTOJI" << endl;
		return 0;
	}
	sqlite3_stmt* rawStmt;
	string sql =
		"INSERT INTO Polaganja(KandidatID, InstruktorID, AutomobilID, Kategorija, DatumPolaganja, Polozio) VALUES(?, ?, ?, ?, ?, ?);";
	int rc4 = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc4 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI ZAKAZIVANJA POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	StmtPtr stmt(rawStmt);
	sqlite3_bind_int(stmt.get(), 1, KandidatID);
	sqlite3_bind_int(stmt.get(), 2, InstruktorID);
	sqlite3_bind_int(stmt.get(), 3, AutomobilID);
	sqlite3_bind_text(stmt.get(), 4, Kategorija.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt.get(), 5, DatumPolaganja.c_str(), -1, SQLITE_TRANSIENT);
	rc4 = sqlite3_step(stmt.get());
	if (rc4 != SQLITE_DONE) {
		cout << "GRESKA PRI ZAKAZIVANJU POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	int PolaganjeID = sqlite3_last_insert_rowid(db);
	cout << "POLAGANJE USPJESNO ZAKAZANO" << endl;
	return PolaganjeID;
}
void Baza::RezultatiPolaganja(int PolaganjeID, int Polozio) {
	sqlite3_stmt* rawStmt;
	string sql =
		"UPDATE Polaganja SET Polozio=? WHERE ID=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UNOSA REZULTATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	sqlite3_bind_int(stmt.get(), 1, Polozio);
	sqlite3_bind_int(stmt.get(), 2, PolaganjeID);
	rc = sqlite3_step(stmt.get());
	if (rc != SQLITE_DONE) {
		cout << "GRESKA PRI UNOSENJU REZULTATA:" << sqlite3_errmsg(db) << endl;
		return;
	}
}
void Baza::UnesiRezlutatePolaganja(string DatumPolaganja) {
	sqlite3_stmt* rawStmtPolaganje;
	string sql =
		"SELECT Polaganja.ID, Kandidati.ImePrezime "
		"FROM Polaganja "
		"JOIN Kandidati ON Polaganja.KandidatID=Kandidati.ID "
		"WHERE Polaganja.DatumPolaganja=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmtPolaganje, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UZIMANJA SVIH KANDIDATA ZA TRAZENO POLAGANJE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmtPolaganje(rawStmtPolaganje);
	sqlite3_bind_text(stmtPolaganje.get(), 1, DatumPolaganja.c_str(), -1, SQLITE_TRANSIENT);

	
	int brojKandidata=0;
	IzvrsiUpit("BEGIN TRANSACTION;", "GRESKA PRI POKRETANJU TRANSAKCIJE");
	while (sqlite3_step(stmtPolaganje.get()) == SQLITE_ROW) {
		brojKandidata++;

		int PolaganjeID = sqlite3_column_int(stmtPolaganje.get(), 0);
		string ImePrezime = (const char*)sqlite3_column_text(stmtPolaganje.get(), 1);
		string Odgovor;
		cout << "Da Li Je Kandidata "<<ImePrezime<<" Polozio(DA / NE) : ";
		cin >> Odgovor;
		int Polozio = (Odgovor == "DA" || Odgovor == "Da") ? 1 : 0;
		cout << (Polozio ? "Kandidat Je Polozio" : "Kandidat Nije Polozio") << endl;
		RezultatiPolaganja(PolaganjeID, Polozio);
	}
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
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT Kandidati.ImePrezime, Kandidati.JMBG, Instruktori.ImePrezime, Automobili.MarkaAutomobila, Automobili.RegistarskaOznaka, Polaganja.Kategorija, Polaganja.DatumPolaganja, "
		"CASE WHEN Polozio IS NULL THEN 'KANDIDAT CEKA POLAGANJE' "
		"WHEN Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Polozio "
		"FROM Polaganja "
		"JOIN Kandidati ON Polaganja.KandidatID=Kandidati.ID "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"JOIN Automobili ON Polaganja.AutomobilID=Automobili.ID;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		string Kandidat = (const char*)sqlite3_column_text(stmt.get(), 0);
		string JMBG = (const char*)sqlite3_column_text(stmt.get(), 1);
		string Instruktor = (const char*)sqlite3_column_text(stmt.get(), 2);
		string Marka = (const char*)sqlite3_column_text(stmt.get(), 3);
		string Registracija = (const char*)sqlite3_column_text(stmt.get(), 4);
		string Kategorija = (const char*)sqlite3_column_text(stmt.get(), 5);
		string DatumPolaganja = (const char*)sqlite3_column_text(stmt.get(), 6);
		string Polozio = (const char*)sqlite3_column_text(stmt.get(), 7);

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
}
int Baza::brojPolaganja() {
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT COUNT (*) FROM Polaganja;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	StmtPtr stmt(rawStmt);
	int count = 0;
	if (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt.get(), 0);
	}
	return count;
}
int Baza::StatusPolaganja(int PolaganjeID) {
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT Polozio "
		"FROM Polaganja "
		"WHERE ID=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA ZAVRSENIH POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	StmtPtr stmt(rawStmt);
	sqlite3_bind_int(stmt.get(), 1, PolaganjeID);
	int status = -1;
	if (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		status = sqlite3_column_int(stmt.get(), 0);
	}
	return status;
}
void Baza::PrikaziAktivnaPolaganja() {
	sqlite3_stmt* rawStmt;
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
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		string Kandidat = (const char*)sqlite3_column_text(stmt.get(), 0);
		string JMBG = (const char*)sqlite3_column_text(stmt.get(), 1);
		string Instruktor = (const char*)sqlite3_column_text(stmt.get(), 2);
		string Marka = (const char*)sqlite3_column_text(stmt.get(), 3);
		string Registracija = (const char*)sqlite3_column_text(stmt.get(), 4);
		string Kategorija = (const char*)sqlite3_column_text(stmt.get(), 5);
		string DatumPolaganja = (const char*)sqlite3_column_text(stmt.get(), 6);
		string Polozio = (const char*)sqlite3_column_text(stmt.get(), 7);

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
	
}
void Baza::PrikaziSvaZavrsenaPolaganja() {
	sqlite3_stmt* rawStmt;
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
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		string Kandidat = (const char*)sqlite3_column_text(stmt.get(), 0);
		string JMBG = (const char*)sqlite3_column_text(stmt.get(), 1);
		string Instruktor = (const char*)sqlite3_column_text(stmt.get(), 2);
		string Marka = (const char*)sqlite3_column_text(stmt.get(), 3);
		string Registracija = (const char*)sqlite3_column_text(stmt.get(), 4);
		string Kategorija = (const char*)sqlite3_column_text(stmt.get(), 5);
		string DatumPolaganja = (const char*)sqlite3_column_text(stmt.get(), 6);
		string Polozio = (const char*)sqlite3_column_text(stmt.get(), 7);

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
}
void Baza::PrikaziNajmladjegKandidata() {
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT Kandidati.ImePrezime, Kandidati.DatumRodjenja, Kandidati.JMBG, julianday('now') - julianday(Kandidati.DatumRodjenja) AS Starost "
		"FROM Kandidati "
		"ORDER BY Starost ASC "
		"LIMIT 1;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		string Ime = (const char*)sqlite3_column_text(stmt.get(), 0);
		string Datum = (const char*)sqlite3_column_text(stmt.get(), 1);
		string JMBG = (const char*)sqlite3_column_text(stmt.get(), 2);

		cout << "Ime I Prezime Kandidata:" << Ime << endl;
		cout << "Datum Rodjenja:" << Datum << endl;
		cout << "JMBG:" << JMBG << endl;
	}
}
void Baza::PrikaziSveKandidateSaNjihovimStatusom() {
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT Kandidati.ImePrezime, Kandidati.DatumRodjenja, Kandidati.JMBG, "
		"CASE WHEN Polaganja.ID IS NULL THEN 'KANDIDAT NEMA ZAKAZANO POLAGANJE' "
		"WHEN Polaganja.Polozio IS NULL THEN 'CEKA IZLAZAK NA POLAGANJE/CEKA REZULTATE' "
		"WHEN Polaganja.Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Status "
		"FROM Kandidati "
		"LEFT JOIN Polaganja ON Kandidati.ID=Polaganja.KandidatID;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		string Ime = (const char*)sqlite3_column_text(stmt.get(), 0);
		string Datum = (const char*)sqlite3_column_text(stmt.get(), 1);
		string JMBG = (const char*)sqlite3_column_text(stmt.get(), 2);
		string Status = (const char*)sqlite3_column_text(stmt.get(), 3);

		cout << "Ime I Prezime Kandidata:" << Ime << endl;
		cout << "Datum Rodjenja:" << Datum << endl;
		cout << "JMBG:" << JMBG << endl;
		cout << "Status Polaganja Kandidata:" << Status << endl;
		cout << "---" << endl;
	}
}
void Baza::PrikaziBrojPolaganjaZaSveInstruktore() {
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT Instruktori.ImePrezime, COUNT (*) AS BrojPolaganja "
		"FROM Polaganja "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"GROUP BY Instruktori.ID;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		string Ime = (const char*)sqlite3_column_text(stmt.get(), 0);
		int BrojPolaganja = sqlite3_column_int(stmt.get(), 1);

		cout << "Ime I Prezime Instruktora:" << Ime << endl;
		cout << "Broj Polaganja:" << BrojPolaganja << endl;
		cout << "---" << endl;
	}
}
void Baza::PrikaziSveInstruktoreKojiImajuViseOdJednogAktivnogPolaganja() {
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT Instruktori.ImePrezime, COUNT(*) AS BrojAktivnihPolaganja "
		"FROM Polaganja "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"WHERE Polozio IS NULL OR Polozio=0 "
		"GROUP BY Instruktori.ID "
		"HAVING COUNT (*) > 1;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		string Ime = (const char*)sqlite3_column_text(stmt.get(), 0);
		int BrojPolaganja = sqlite3_column_int(stmt.get(), 1);

		cout << "Ime I Prezime Instruktora:" << Ime << endl;
		cout << "Broj Aktivnih Polaganja:" << BrojPolaganja << endl;
		cout << "---" << endl;
	}
}
vector<Automobil>Baza::UzmiSveAutomobile() {
	vector<Automobil>rezultat;
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT * FROM Automobili;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UZIMANJA AUTOMOBILA:" << sqlite3_errmsg(db) << endl;
		return rezultat;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		Automobil a;
		a.ID = sqlite3_column_int(stmt.get(), 0);
		a.Marka = (const char*)sqlite3_column_text(stmt.get(), 1);
		a.Registracija = (const char*)sqlite3_column_text(stmt.get(), 2);
		rezultat.push_back(a);
	}
	return rezultat;
}
vector<Instruktor>Baza::UzmiSveInstruktore() {
	vector<Instruktor>rezultat;
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT * FROM Instruktori;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UZIMANJA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return rezultat;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		Instruktor i;
		i.ID = sqlite3_column_int(stmt.get(), 0);
		i.ImePrezime = (const char*)sqlite3_column_text(stmt.get(), 1);
		i.JMBG = (const char*)sqlite3_column_text(stmt.get(), 2);
		rezultat.push_back(i);
	}
	return rezultat;
}
vector<Kandidat>Baza::UzmiSveKandidate() {
	vector<Kandidat>rezultat;
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT * FROM Kandidati;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UZIMANJA KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return rezultat;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		Kandidat k;
		k.ID = sqlite3_column_int(stmt.get(), 0);
		k.ImePrezime = (const char*)sqlite3_column_text(stmt.get(), 1);
		k.DatumRodjenja = (const char*)sqlite3_column_text(stmt.get(), 2);
		k.JMBG = (const char*)sqlite3_column_text(stmt.get(), 3);
		rezultat.push_back(k);
	}
	return rezultat;
}
vector<Polaganje>Baza::UzmiSvaPolaganja() {
	vector<Polaganje>rezultat;
	sqlite3_stmt* rawStmt;
	string sql=
		"SELECT Polaganja.ID, Kandidati.ImePrezime, Kandidati.JMBG, Instruktori.ImePrezime, Automobili.MarkaAutomobila, Automobili.RegistarskaOznaka, Polaganja.Kategorija, Polaganja.DatumPolaganja, "
		"CASE WHEN Polozio IS NULL THEN 'KANDIDAT CEKA POLAGANJE' "
		"WHEN Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Polozio "
		"FROM Polaganja "
		"JOIN Kandidati ON Polaganja.KandidatID=Kandidati.ID "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"JOIN Automobili ON Polaganja.AutomobilID=Automobili.ID;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UZIMANJA POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return rezultat;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		Polaganje p;
		p.ID = sqlite3_column_int(stmt.get(), 0);
		p.ImePrezimeKandidata = (const char*)sqlite3_column_text(stmt.get(), 1);
		p.JMBG = (const char*)sqlite3_column_text(stmt.get(), 2);
		p.ImePrezimeInstruktora = (const char*)sqlite3_column_text(stmt.get(), 3);
		p.MarkaAutomobila = (const char*)sqlite3_column_text(stmt.get(), 4);
		p.RegistarskaOznaka = (const char*)sqlite3_column_text(stmt.get(), 5);
		p.Kategorija = (const char*)sqlite3_column_text(stmt.get(), 6);
		p.DatumPolaganja = (const char*)sqlite3_column_text(stmt.get(), 7);
		p.Polozio = (const char*)sqlite3_column_text(stmt.get(), 8);
		rezultat.push_back(p);
	}
	return rezultat;
}
void Baza::UnesiRezultatePolaganja1(vector<pair<int, int>>Rezultati) {
	IzvrsiUpit("BEGIN TRANSACTION;", "GRESKA PRI POKRETANJU TRANSAKCIJE");
	for (const auto& r : Rezultati) {
		int PolaganjeID = r.first;
		int Polozio = r.second;
		RezultatiPolaganja(PolaganjeID, Polozio);
	}
	IzvrsiUpit("COMMIT;", "GRESKA PRI COMMIT-u");
	cout << "USPJESNO AZURIRANI REZULTATI ZA TRAZENO POLAGANJE" << endl;
}
vector<Polaganje>Baza::UzmiSvaAktivnaPolaganja() {
	vector<Polaganje>rezultat;
	sqlite3_stmt* rawStmt;
	string sql=
		"SELECT Polaganja.ID, Kandidati.ImePrezime, Kandidati.JMBG, Instruktori.ImePrezime, Automobili.MarkaAutomobila, Automobili.RegistarskaOznaka, Polaganja.Kategorija, Polaganja.DatumPolaganja, "
		"CASE WHEN Polozio IS NULL THEN 'KANDIDAT CEKA POLAGANJE' "
		"WHEN Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Polozio "
		"FROM Polaganja "
		"JOIN Kandidati ON Polaganja.KandidatID=Kandidati.ID "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"JOIN Automobili ON Polaganja.AutomobilID=Automobili.ID "
		"WHERE Polozio IS NULL;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UZIMANJA AKTIVNIH POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return rezultat;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		Polaganje p;
		p.ID = sqlite3_column_int(stmt.get(), 0);
		p.ImePrezimeKandidata = (const char*)sqlite3_column_text(stmt.get(), 1);
		p.JMBG = (const char*)sqlite3_column_text(stmt.get(), 2);
		p.ImePrezimeInstruktora = (const char*)sqlite3_column_text(stmt.get(), 3);
		p.MarkaAutomobila = (const char*)sqlite3_column_text(stmt.get(), 4);
		p.RegistarskaOznaka = (const char*)sqlite3_column_text(stmt.get(), 5);
		p.Kategorija = (const char*)sqlite3_column_text(stmt.get(), 6);
		p.DatumPolaganja = (const char*)sqlite3_column_text(stmt.get(), 7);
		p.Polozio = (const char*)sqlite3_column_text(stmt.get(), 8);
		rezultat.push_back(p);
	}
	return rezultat;
}
vector<Polaganje>Baza::UzmiSvaZavrsenaPolaganja() {
	vector<Polaganje>rezultat;
	sqlite3_stmt* rawStmt;
	string sql=
		"SELECT Polaganja.ID, Kandidati.ImePrezime, Kandidati.JMBG, Instruktori.ImePrezime, Automobili.MarkaAutomobila, Automobili.RegistarskaOznaka, Polaganja.Kategorija, Polaganja.DatumPolaganja, "
		"CASE WHEN Polozio IS NULL THEN 'KANDIDAT CEKA POLAGANJE' "
		"WHEN Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Polozio "
		"FROM Polaganja "
		"JOIN Kandidati ON Polaganja.KandidatID=Kandidati.ID "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"JOIN Automobili ON Polaganja.AutomobilID=Automobili.ID "
		"WHERE Polozio=1 OR Polozio=0;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UZIMANJA AKTIVNIH POLAGANJA:" << sqlite3_errmsg(db) << endl;
		return rezultat;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		Polaganje p;
		p.ID = sqlite3_column_int(stmt.get(), 0);
		p.ImePrezimeKandidata = (const char*)sqlite3_column_text(stmt.get(), 1);
		p.JMBG = (const char*)sqlite3_column_text(stmt.get(), 2);
		p.ImePrezimeInstruktora = (const char*)sqlite3_column_text(stmt.get(), 3);
		p.MarkaAutomobila = (const char*)sqlite3_column_text(stmt.get(), 4);
		p.RegistarskaOznaka = (const char*)sqlite3_column_text(stmt.get(), 5);
		p.Kategorija = (const char*)sqlite3_column_text(stmt.get(), 6);
		p.DatumPolaganja = (const char*)sqlite3_column_text(stmt.get(), 7);
		p.Polozio = (const char*)sqlite3_column_text(stmt.get(), 8);
		rezultat.push_back(p);
	}
	return rezultat;
}
Kandidat Baza::UzmiNajmladjegKandidata() {
	Kandidat rezultat;
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT Kandidati.ID, Kandidati.ImePrezime, Kandidati.DatumRodjenja, Kandidati.JMBG, julianday('now') - julianday(Kandidati.DatumRodjenja) AS Starost "
		"FROM Kandidati "
		"ORDER BY Starost ASC "
		"LIMIT 1;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UZIMANJA NAJMLADJEG KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return rezultat;
	}
	StmtPtr stmt(rawStmt);
	if (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		rezultat.ID = sqlite3_column_int(stmt.get(), 0);
		rezultat.ImePrezime = (const char*)sqlite3_column_text(stmt.get(), 1);
		rezultat.DatumRodjenja = (const char*)sqlite3_column_text(stmt.get(), 2);
		rezultat.JMBG = (const char*)sqlite3_column_text(stmt.get(), 3);
	}
	return rezultat;
}
vector<StatusKandidata>Baza::UzmiSveStatuseKandidata() {
	vector<StatusKandidata>rezultat;
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT Kandidati.ID, Kandidati.ImePrezime, Kandidati.DatumRodjenja, Kandidati.JMBG, "
		"CASE WHEN Polaganja.ID IS NULL THEN 'KANDIDAT NEMA ZAKAZANO POLAGANJE' "
		"WHEN Polaganja.Polozio IS NULL THEN 'CEKA IZLAZAK NA POLAGANJE/CEKA REZULTATE' "
		"WHEN Polaganja.Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Status "
		"FROM Kandidati "
		"JOIN Polaganja ON Kandidati.ID=Polaganja.KandidatID;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI UZIMANJA STATUSA KANDIDATA:" << sqlite3_errmsg(db) << endl;
		return rezultat;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		StatusKandidata sk;
		sk.ID = sqlite3_column_int(stmt.get(), 0);
		sk.ImePrezime = (const char*)sqlite3_column_text(stmt.get(), 1);
		sk.DatumRodjenja = (const char*)sqlite3_column_text(stmt.get(), 2);
		sk.JMBG = (const char*)sqlite3_column_text(stmt.get(), 3);
		sk.Status = (const char*)sqlite3_column_text(stmt.get(), 4);
		rezultat.push_back(sk);
	}
	return rezultat;
}
vector<PolaganjaInstruktora> Baza::UzmiBrojPolaganjaZaSveInstruktore() {
	vector<PolaganjaInstruktora> rezultat;
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT Instruktori.ID, Instruktori.ImePrezime, Instruktori.JMBG, COUNT (*) AS BrojPolaganja "
		"FROM Polaganja "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"GROUP BY Instruktori.ID;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return rezultat;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		PolaganjaInstruktora pi;
		pi.ID = sqlite3_column_int(stmt.get(), 0);
		pi.ImePrezime = (const char*)sqlite3_column_text(stmt.get(), 1);
		pi.JMBG = (const char*)sqlite3_column_text(stmt.get(), 2);
		pi.BrojPolaganja = sqlite3_column_int(stmt.get(), 3);
		rezultat.push_back(pi);
	}
	return rezultat;
}
vector<PolaganjaInstruktora> Baza::UzmiSveInstruktoreKojiImajuViseOdJednogAktivnogPolaganja() {
	vector<PolaganjaInstruktora> rezultat;
	sqlite3_stmt* rawStmt;
	string sql =
		"SELECT Instruktori.ID, Instruktori.ImePrezime, Instruktori.JMBG, COUNT(*) AS BrojAktivnihPolaganja "
		"FROM Polaganja "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"WHERE Polozio IS NULL OR Polozio=0 "
		"GROUP BY Instruktori.ID "
		"HAVING COUNT (*) > 1;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &rawStmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA INSTRUKTORA:" << sqlite3_errmsg(db) << endl;
		return rezultat;
	}
	StmtPtr stmt(rawStmt);
	while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
		PolaganjaInstruktora pi;
		pi.ID = sqlite3_column_int(stmt.get(), 0);
		pi.ImePrezime = (const char*)sqlite3_column_text(stmt.get(), 1);
		pi.JMBG = (const char*)sqlite3_column_text(stmt.get(), 2);
		pi.BrojPolaganja = sqlite3_column_int(stmt.get(), 3);
		rezultat.push_back(pi);
	}
	return rezultat;
}