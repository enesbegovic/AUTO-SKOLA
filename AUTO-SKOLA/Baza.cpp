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
		"Polozio INTEGER);";
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
	if (count > 0) {
		cout << "GRESKA PRI DODAVANJU AUTOMOBILA-AUTOMOBIL VEC POSTOJI!!!" << endl;
		return;
	}
	string sql =
		"INSERT INTO Automobili(MarkaAutomobila, RegistarskaOznaka) VALUES('"
		+ Marka + "', '" + Registracija + "');";
	IzvrsiUpit(sql, "GRESKA PRI DODAVANJU AUTOMOBILA");

}
void Baza::UkloniAutomobil(string Registracija) {
	int AutomobilID = 0;
	string Provjera =
		"SELECT ID FROM Automobili WHERE RegistarskaOznaka='" + Registracija + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &AutomobilID, nullptr);
	if (AutomobilID == 0) {
		cout << "GRESKA PRI UKLANJANU AUTOMOBILA-TRAZENI AUTOMOBIL NE POSTOJI!!!" << endl;
		return;
	}
	int ImaPolaganja = 0;
	string ProvjeraPolaganja =
		"SELECT COUNT(*) FROM Polaganja WHERE AutomobilID=" + to_string(AutomobilID) + " AND (Polozio IS NULL OR Polozio=0);";
	sqlite3_exec(db, ProvjeraPolaganja.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &ImaPolaganja, nullptr);
	if (ImaPolaganja > 0) {
		cout << "GRESKA PRI BRISANJU-AUTOMOBIL IMA AKTIVNO POLAGANJE" << endl;
		return;
	}
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
	if (count > 0) {
		cout << "GRESKA PRI DODAVANJU INSTRUKTORA-INSTRUKTOR VEC POSTOJI!!!" << endl;
		return;
	}
	string sql =
		"INSERT INTO Instruktori(ImePrezime, JMBG) VALUES('"
		+ ImePrezime + "', '" + JMBG + "');";
	IzvrsiUpit(sql, "GRESKA PRI DODAVANJU INSTRUKTORA");
}
void Baza::UkloniInstruktora(string JMBG) {
	int InstruktorID = 0;
	string Provjera =
		"SELECT ID FROM Instruktori WHERE JMBG='" + JMBG + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &InstruktorID, nullptr);
	if (InstruktorID == 0) {
		cout << "GRESKA PRI BRISANJU INSTRUKTORA-NE POSTOJI TRAZENI INSTRUKTOR" << endl;
		return;
	}
	int imaPolaganje = 0;
	string ProvjeraPolaganja =
		"SELECT COUNT(*) FROM Polaganja WHERE InstruktorID=" + to_string(InstruktorID) + " AND (Polozio IS NULL OR Polozio=0);";
	sqlite3_exec(db, ProvjeraPolaganja.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &imaPolaganje, nullptr);
	if (imaPolaganje > 0) {
		cout << "GRESKA PRI BRISANJU-INSTRUKTOR IMA AKTIVNO POLAGANJE" << endl;
		return;
	}
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
	if (count > 0) {
		cout << "GRESKA PRI DODAVANJU KANDIDATA-KANDIDAT VEC POSTOJI" << endl;
		return;
	}
	string sql =
		"INSERT INTO Kandidati(ImePrezime, DatumRodjenja, JMBG) VALUES('"
		+ ImePrezime + "', '" + DatumRodjenja + "', '" + JMBG + "');";
	IzvrsiUpit(sql, "GRESKA PRI DODAVANJU KANDIDATA");
}
void Baza::UkloniKandidata(string JMBG) {
	int KandidatID = 0;
	string Provjera =
		"SELECT ID FROM Kandidati WHERE JMBG='" + JMBG + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &KandidatID, nullptr);
	if (KandidatID == 0) {
		cout << "GRESKA PRI BRISANJU KANDIDATA-NE POSTOJI TRAZENI KANDIDAT" << endl;
		return;
	}
	int imaPolaganje = 0;
	string ProvjeraPolaganja =
		"SELECT COUNT(*) FROM Polaganja WHERE KandidatID=" + to_string(KandidatID) + " AND (Polozio IS NULL OR Polozio=0);";
	sqlite3_exec(db, ProvjeraPolaganja.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &imaPolaganje, nullptr);
	if (imaPolaganje > 0) {
		cout << "GRESKA PRI BRISANJU-KANDIDAT IMA AKTIVNO POLAGANJE" << endl;
		return;
	}
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
void Baza::ZakaziPolaganje(string Kandidat, string Instruktor, string Automobil, string Kategorija, string DatumPolaganja) {
	int KandidatID = 0;
	string ProvjeraKandidata =
		"SELECT ID FROM Kandidati WHERE JMBG='" + Kandidat + "';";
	sqlite3_exec(db, ProvjeraKandidata.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &KandidatID, nullptr);
	int InstruktorID = 0;
	string ProvjeraInstruktora =
		"SELECT ID FROM Instruktori WHERE JMBG='" + Instruktor + "';";
	sqlite3_exec(db, ProvjeraInstruktora.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &InstruktorID, nullptr);
	int AutomobilID = 0;
	string ProvjeraAutomobila =
		"SELECT ID FROM Automobili WHERE RegistarskaOznaka='" + Automobil + "';";
	sqlite3_exec(db, ProvjeraAutomobila.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &AutomobilID, nullptr);
	if (KandidatID == 0 || InstruktorID == 0 || AutomobilID == 0) {
		cout << "GRESKA PRI ZAKAZIVANJU POLAGANJA-NEPOSTOJECI KANDIDAT ILI NEPOSTOJECI INSTRUKTOR ILI NEPOSTOJECI AUTOMOBIL" << endl;
		return;
	}
	string sql =
		"INSERT INTO Polaganja(KandidatID, InstruktorID, AutomobilID, Kategorija, DatumPolaganja) VALUES("
		+ to_string(KandidatID) + ", " + to_string(InstruktorID) + ", " + to_string(AutomobilID) + ", '" + Kategorija + "', '" + DatumPolaganja + "');";
	IzvrsiUpit(sql, "GRESKA PRI ZAKAZIVANJU POLAGANJA");
}
void Baza::UnesiRezlutatePolaganja(string Kandidat, string DatumPolaganja, bool Polozio) {
	int KandidatID = 0;
	string Provjera =
		"SELECT ID FROM Kandidati WHERE JMBG='" + Kandidat + "';";
	sqlite3_exec(db, Provjera.c_str(),
		[](void* data, int, char** podaci, char**) {
			*(int*)data = atoi(podaci[0]);
			return 0;
		}, &KandidatID, nullptr);
	if (KandidatID == 0)
		cout << "GRESKA-NE POSTOJI TRAZENI KANDIDAT" << endl;
	string sql =
		"UPDATE Polaganja SET Polozio=" + to_string(Polozio) + " WHERE KandidatID=" + to_string(KandidatID) + " AND DatumPolaganja='" + DatumPolaganja + "';";
	IzvrsiUpit(sql, "GRESKA PRI UNOSU REZULTATA");
}
void Baza::PrikaziSvaPolaganja() {
	string sql =
		"SELECT Kandidati.ImePrezime, Kandidati.JMBG, Instruktori.ImePrezime, Automobili.MarkaAutomobila, Automobili.RegistarskaOznaka, Polaganja.Kategorija, Polaganja.DatumPolaganja, "
		"CASE WHEN Polozio IS NULL THEN '---' "
		"WHEN Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Polozio "
		"FROM Polaganja "
		"JOIN Kandidati ON Polaganja.KandidatID=Kandidati.ID "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"JOIN Automobili ON Polaganja.AutomobilID=Automobili.ID;";
	char* errMsg = nullptr;
	int rc=sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << (podaci[i] ? podaci[i] : "NULL") << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		cout << "Greska u prikazu: " << errMsg << endl;
		sqlite3_free(errMsg);
	}
}
void Baza::PrikaziAktivnaPolaganja() {
	string sql =
		"SELECT Kandidati.ImePrezime, Kandidati.JMBG, Instruktori.ImePrezime, Automobili.MarkaAutomobila, Automobili.RegistarskaOznaka, Polaganja.Kategorija, Polaganja.DatumPolaganja, "
		"CASE WHEN Polozio IS NULL THEN '---' "
		"WHEN Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Polozio "
		"FROM Polaganja "
		"JOIN Kandidati ON Polaganja.KandidatID=Kandidati.ID "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"JOIN Automobili ON Polaganja.AutomobilID=Automobili.ID "
		"WHERE Polozio IS NULL;";
	char* errMsg = nullptr;
	int rc=sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << podaci[i] << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		cout << "GRESKA U PRIKAZU AKTIVNIH POLAGANJA:" << errMsg << endl;
		sqlite3_free(errMsg);
	}
}
void Baza::PrikaziSvaZavrsenaPolaganja() {
	string sql =
		"SELECT Kandidati.ImePrezime, Kandidati.JMBG, Instruktori.ImePrezime, Automobili.MarkaAutomobila, Automobili.RegistarskaOznaka, Polaganja.Kategorija, Polaganja.DatumPolaganja, "
		"CASE WHEN Polozio IS NULL THEN '---' "
		"WHEN Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Polozio "
		"FROM Polaganja "
		"JOIN Kandidati ON Polaganja.KandidatID=Kandidati.ID "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"JOIN Automobili ON Polaganja.AutomobilID=Automobili.ID "
		"WHERE Polozio=1 OR Polozio=0;";
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << podaci[i] << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		cout << "GRESKA U PRIKAZU ZAVRSENIH POLAGANJA:" << errMsg << endl;
		sqlite3_free(errMsg);
	}
}
void Baza::PrikaziNajmladjegKandidata() {
	string sql =
		"WITH Split1 AS("
		"SELECT ImePrezime, JMBG, DatumRodjenja, "
		"instr(DatumRodjenja, '.') AS P1, "
		"substr(DatumRodjenja, instr(DatumRodjenja, '.')+1) AS Rest "
		"FROM Kandidati"
		"), "
		"Split2 AS ("
		"SELECT ImePrezime, JMBG, DatumRodjenja, "
		"substr(DatumRodjenja, 1, P1-1)AS Dan, "
		"substr(Rest, 1, instr(Rest, '.')-1) AS Mjesec, "
		"substr(Rest, instr(Rest, '.')+1) AS Godina "
		"FROM Split1"
		") "
		"SELECT ImePrezime, JMBG, DatumRodjenja FROM Split2 "
		"ORDER BY CAST(Godina AS INTEGER) DESC, CAST(Mjesec AS INTEGER) DESC, CAST(Dan AS INTEGER) DESC "
		"LIMIT 1;";
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << (podaci[i] ? podaci[i] : "NULL") << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		cout << "GRESKA U PRIKAZU NAJMLADJEG KANDIDATA:" << errMsg << endl;
		sqlite3_free(errMsg);
	}
}
void Baza::PrikaziNajstarijegKandidata() {
	string sql =
		"WITH Split1 AS("
		"SELECT ImePrezime, JMBG, DatumRodjenja, "
		"instr(DatumRodjenja, '.')AS P1, "
		"substr(DatumRodjenja, instr(DatumRodjenja, '.')+1)AS Rest "
		"FROM Kandidati"
		"), "
		"Split2 AS("
		"SELECT ImePrezime, JMBG, DatumRodjenja, "
		"substr(DatumRodjenja, 1, P1-1)AS Dan, "
		"substr(Rest, 1, instr(Rest, '.')-1)AS Mjesec, "
		"substr(Rest, instr(Rest, '.')+1)AS Godina "
		"FROM Split1"
		") "
		"SELECT ImePrezime, JMBG, DatumRodjenja FROM Split2 "
		"ORDER BY CAST(Godina AS INTEGER) ASC, CAST(Mjesec AS INTEGER) ASC, CAST(Dan AS INTEGER) ASC "
		"LIMIT 1;";
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << (podaci[i] ? podaci[i] : "NULL") << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIKAZU NAJSTARIJEG KANDIDATA:" << errMsg << endl;
		sqlite3_free(errMsg);
	}
}
void Baza::PrikaziSveKandidateSaNjihovimStatusom() {
	string sql =
		"SELECT Kandidati.ImePrezime, Kandidati.DatumRodjenja, Kandidati.JMBG, "
		"Polaganja.DatumPolaganja, "
		"CASE WHEN Polaganja.ID IS NULL THEN 'KANDIDAT NEMA ZAKAZANO POLAGANJE' "
		"WHEN Polaganja.Polozio IS NULL THEN 'CEKA IZLAZAK NA POLAGANJE/CEKA REZULTATE' "
		"WHEN Polaganja.Polozio=1 THEN 'KANDIDAT JE POLOZIO' "
		"ELSE 'KANDIDAT NIJE POLOZIO' END AS Status "
		"FROM Kandidati "
		"LEFT JOIN Polaganja ON Kandidati.ID=Polaganja.KandidatID;";
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(),
		[](void* data, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << (podaci[i] ? podaci[i] : "-") << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		cout << "GRSKA PRI PRIKAZU STATUSA KANDIDATA:" << errMsg << endl;
		sqlite3_free(errMsg);
	}
}
void Baza::PrikaziBrojPolaganjaZaSveInstruktore() {
	string sql =
		"SELECT Instruktori.ImePrezime, COUNT(*) AS BrojPolaganja "
		"FROM Polaganja "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"GROUP BY Instruktori.ID;";
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << (podaci[i] ? podaci[i] : "-") << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIKAZU:" << errMsg << endl;
		sqlite3_free(errMsg);
	}
}
void Baza::PrikaziSveInstruktoreKojiImajuViseOdJednogAktivnogPolaganja() {
	string sql =
		"SELECT Instruktori.ImePrezime, COUNT(*) AS BrojAktivnihPolaganja "
		"FROM Polaganja "
		"JOIN Instruktori ON Polaganja.InstruktorID=Instruktori.ID "
		"WHERE Polozio IS NULL OR Polozio=0 "
		"GROUP BY Instruktori.ID "
		"HAVING COUNT(*) > 1;";
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(),
		[](void*, int kolone, char** podaci, char** nazivKolone) {
			for (int i = 0; i < kolone; i++)
				cout << nazivKolone[i] << ": " << (podaci[i] ? podaci[i] : "-") << endl;
			cout << "---" << endl;
			return 0;
		}, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIKAZU:" << errMsg << endl;
		sqlite3_free(errMsg);
	}
}