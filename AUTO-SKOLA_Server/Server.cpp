#include<httplib.h>
#include<nlohmann/json.hpp>
#include "Baza.h"
using json = nlohmann::json;
int main() {
	Baza baza("C:\\Users\\enesb\\source\\repos\\AUTO-SKOLA\\AUTO-SKOLA\\AUTO-SKOLA.db");
	baza.KreirajTabelu();
	httplib::Server server;
	server.Get("/", [](const httplib::Request& req, httplib::Response& res) {
		res.set_content("Server Radi!!!", "text/plain");
		});
	server.Get("/Automobili", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Automobil>automobili = baza.UzmiSveAutomobile();
		json rezultat = json::array();
		for (const auto& a : automobili) {
			json ja;
			ja["ID"] = a.ID;
			ja["Marka Automobila"] = a.Marka;
			ja["Registarska Oznaka"] = a.Registracija;
			rezultat.push_back(ja);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Post("/Automobili", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string Marka = tijelo.at("marka");
			string Registracija = tijelo.at("registracija");
			baza.DodajAutomobil(Marka, Registracija);
			json Poruka;
			Poruka["Poruka"] = "Automobil uspijesno dodan";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Delete("/Automobili/:RegistarskaOznaka", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string Registracija = req.path_params.at("RegistarskaOznaka");
			baza.UkloniAutomobil(Registracija);
			json Poruka;
			Poruka["Poruka"] = "Automobil uspjesno uklonjen";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Get("/Instruktori", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Instruktor>instruktori = baza.UzmiSveInstruktore();
		json rezultat = json::array();
		for (const auto& i : instruktori) {
			json ji;
			ji["ID"] = i.ID;
			ji["Ime i Prezime"] = i.ImePrezime;
			ji["JMBG:"] = i.JMBG;
			rezultat.push_back(ji);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Post("/Instruktori", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string ImePrezime = tijelo.at("imeprezime");
			string JMBG = tijelo.at("JMBG");
			baza.DodajInstruktora(ImePrezime, JMBG);
			json Poruka;
			Poruka["Poruka"] = "Instruktor uspijesno dodan";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Delete("/Instruktori/:JMBG", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string JMBG = req.path_params.at("JMBG");
			baza.UkloniInstruktora(JMBG);
			json Poruka;
			Poruka["Poruka"] = "Instruktor uspijesno uklonjen";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Get("/Kandidati", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Kandidat>kandidati = baza.UzmiSveKandidate();
		json rezultat = json::array();
		for (const auto& k : kandidati) {
			json jk;
			jk["ID"] = k.ID;
			jk["Ime i Prezime"] = k.ImePrezime;
			jk["Datum Rodjenja"] = k.DatumRodjenja;
			jk["JMBG"] = k.JMBG;
			rezultat.push_back(jk);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Post("/Kandidati", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string ImePrezime = tijelo.at("imeprezime");
			string datumRodjenja = tijelo.at("datumrodjenja");
			string JMBG = tijelo.at("JMBG");
			baza.DodajKandidata(ImePrezime, datumRodjenja, JMBG);
			json Poruka;
			Poruka["Poruka"] = "Kandidat je uspijesno dodan";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Delete("/Kandidati/:JMBG", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			string JMBG = req.path_params.at("JMBG");
			baza.UkloniKandidata(JMBG);
			json Poruka;
			Poruka["Poruka"] = "Kandidat uspijesno uklonjen";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Get("/Polaganja", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Polaganje>polaganja = baza.UzmiSvaPolaganja();
		json rezultat = json::array();
		for (const auto& p : polaganja) {
			json jp;
			jp["ID"] = p.ID;
			jp["Ime i Prezime Kandidata"] = p.ImePrezimeKandidata;
			jp["JMBG Kandidata"] = p.JMBG;
			jp["Ime i Prezime Instruktora"] = p.ImePrezimeInstruktora;
			jp["Automobil Na Kojem Kandidat Polaze"] = p.MarkaAutomobila;
			jp["Registarska Oznaka Automobila"] = p.RegistarskaOznaka;
			jp["Kategorija Koju Kandidat Polaze"] = p.Kategorija;
			jp["Datum Polaganja"] = p.DatumPolaganja;
			jp["Rezultat Polaganja"] = p.Polozio;
			rezultat.push_back(jp);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Post("/Polaganja", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string JMBGKandidata = tijelo.at("JMBGKandidata");
			string JMBGInstruktora = tijelo.at("JMBGInstruktora");
			string RegistarskaOznaka = tijelo.at("Registracija");
			string Kategorija = tijelo.at("Kategorija");
			string DatumPolaganja = tijelo.at("DatumPolaganja");
			int PolaganjeID=baza.ZakaziPolaganje(JMBGKandidata, JMBGInstruktora, RegistarskaOznaka, Kategorija, DatumPolaganja);
			json Poruka;
			Poruka["Poruka"] = "Polaganje uspijesno zakazano";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch(const std::exception&e){
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Post("/Polaganja/Rezultati", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			vector<pair<int, int>>rezultati;
			for (const auto& r : tijelo.at("rezultati")) {
				int PolaganjeID = r.at("polaganjeid");
				int Polozio = r.at("polozio");
				rezultati.push_back({ PolaganjeID,Polozio });
			}
			baza.UnesiRezultatePolaganja1(rezultati);
			json Poruka;
			Poruka["Poruka"] = "Uspijesni azurirani rezultati";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch(const std::exception&e){
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Get("/Polaganja/Aktivna", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Polaganje>polaganja = baza.UzmiSvaAktivnaPolaganja();
		json rezultat = json::array();
		for (const auto& p : polaganja) {
			json jp;
			jp["ID"] = p.ID;
			jp["Ime i Prezime Kandidata"] = p.ImePrezimeKandidata;
			jp["JMBG Kandidata"] = p.JMBG;
			jp["Ime i Prezime Instruktora"] = p.ImePrezimeInstruktora;
			jp["Automobil Na Kojem Kandidat Polaze"] = p.MarkaAutomobila;
			jp["Registarska Oznaka Automobila"] = p.RegistarskaOznaka;
			jp["Kategorija Koju Kandidat Polaze"] = p.Kategorija;
			jp["Datum Polaganja"] = p.DatumPolaganja;
			jp["Rezultat Polaganja"] = p.Polozio;
			rezultat.push_back(jp);
		}
		res.set_content(rezultat.dump(4), "application/json");
		
		});
	server.Get("/Polaganja/Zavrsena", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Polaganje>polaganja = baza.UzmiSvaZavrsenaPolaganja();
		json rezultat = json::array();
		for (const auto& p : polaganja) {
			json jp;
			jp["ID"] = p.ID;
			jp["Ime i Prezime Kandidata"] = p.ImePrezimeKandidata;
			jp["JMBG Kandidata"] = p.JMBG;
			jp["Ime i Prezime Instruktora"] = p.ImePrezimeInstruktora;
			jp["Automobil Na Kojem Kandidat Polaze"] = p.MarkaAutomobila;
			jp["Registarska Oznaka Automobila"] = p.RegistarskaOznaka;
			jp["Kategorija Koju Kandidat Polaze"] = p.Kategorija;
			jp["Datum Polaganja"] = p.DatumPolaganja;
			jp["Rezultat Polaganja"] = p.Polozio;
			rezultat.push_back(jp);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/Kandidati/Najmladji", [&baza](const httplib::Request& req, httplib::Response& res) {
		Kandidat k = baza.UzmiNajmladjegKandidata();
		json rezultat;
		rezultat["ID"] = k.ID;
		rezultat["Ime i Prezime"] = k.ImePrezime;
		rezultat["Datum Rodjenja"] = k.DatumRodjenja;
		rezultat["JMBG"] = k.JMBG;
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/Kandidati/Status", [&baza](const httplib::Request& req, httplib::Response& res) {
		vector<StatusKandidata>statusi = baza.UzmiSveStatuseKandidata();
		json rezultat = json::array();
		for (const auto& s : statusi) {
			json js;
			js["ID"] = s.ID;
			js["Ime i Prezime"] = s.ImePrezime;
			js["Datum Rodjenja"] = s.DatumRodjenja;
			js["JMBG"] = s.JMBG;
			js["Status Polaganja"] = s.Status;
			rezultat.push_back(js);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/Instruktori/BrojPolaganja", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<PolaganjaInstruktora>brpolagnja = baza.UzmiBrojPolaganjaZaSveInstruktore();
		json rezultat = json::array();
		for (const auto& bp : brpolagnja) {
			json jbp;
			jbp["ID"] = bp.ID;
			jbp["Ime i Prezime"] = bp.ImePrezime;
			jbp["JMBG"] = bp.JMBG;
			jbp["Broj Polaganja"] = bp.BrojPolaganja;
			rezultat.push_back(jbp);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/Instruktori/ZauzetiInstruktori", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<PolaganjaInstruktora> polaganja = baza.UzmiSveInstruktoreKojiImajuViseOdJednogAktivnogPolaganja();
		json rezultat = json::array();
		for (const auto& x : polaganja) {
			json jpi;
			jpi["ID"] = x.ID;
			jpi["Ime i Prezime"] = x.ImePrezime;
			jpi["JMBG"] = x.JMBG;
			jpi["Broj Polaganja"] = x.BrojPolaganja;
			rezultat.push_back(jpi);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	std::cout << "Server Pokrenut Na http://localhost:8080" << std::endl;
	server.listen("localhost", 8080);
	return 0;
}