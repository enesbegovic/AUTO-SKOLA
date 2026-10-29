#include <sodium.h>
#include <mutex>
#include <map>
#include <chrono>
#include<httplib.h>
#include<nlohmann/json.hpp>
#include "Baza.h"
using namespace std;
using json = nlohmann::json;
int main() {
	if (sodium_init() < 0) {
		cout << "Libsodium nije inicijalizovan" << endl;
		return 1;
	}
	Baza baza("AUTO-SKOLA.db");
	struct Sesija {
		string UserName;
		string Uloga;
		std::chrono::steady_clock::time_point istice;
	};
	std::map<string, Sesija>sesije;
	std::mutex sesijeMutex;
	baza.KreirajTabelu();
	httplib::Server server;
	server.set_pre_routing_handler([&sesije, &sesijeMutex](const httplib::Request& req, httplib::Response& res) {
		if (req.path == "/" || req.path == "/register" || req.path == "/login") {
			return httplib::Server::HandlerResponse::Unhandled;
		}
		string auth = req.get_header_value("Authorization");
		string Prefiks = "Bearer ";
		if (auth.rfind(Prefiks,0) == 0) {
			string token = auth.substr(Prefiks.size());
			std::lock_guard<mutex>lock(sesijeMutex);
			auto it = sesije.find(token);
			if (it != sesije.end() && it->second.istice > std::chrono::steady_clock::now()) {
				bool Mijenja = (req.method == "POST" || req.method == "PUT" || req.method == "DELETE")
				&& req.path != "/logout";
				if (Mijenja && it->second.Uloga != "admin") {
					res.status = 403;
					res.set_content("{\"Greska\":\"Nemate dozvolu za ovu akciju\"}", "application/json");
					return httplib::Server::HandlerResponse::Handled;
				}
				return httplib::Server::HandlerResponse::Unhandled;
			}
		}
		res.status = 401;
		res.set_content("{\"Greska\":\"Nevazeci ili istekao token\"}", "application/json");
		return httplib::Server::HandlerResponse::Handled;
		});
	server.Get("/", [](const httplib::Request& req, httplib::Response& res) {
		res.set_content("Server Radi!!!", "text/plain");
		});
	server.Post("/register", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string UserName = tijelo.at("username");
			string PassWord = tijelo.at("password");
			char hash[crypto_pwhash_STRBYTES];
			if (crypto_pwhash_str(hash, PassWord.c_str(), PassWord.size(), crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE)!=0) {
				res.status = 500;
				res.set_content("{\"Greska\":\"Hashiranje nije uspijelo\"}", "application/json");
				return;
			}
			baza.DodajKorisnika(UserName, hash);
			json Poruka;
			Poruka["Poruka"] = "Korisnik uspijesno dodan";
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
	server.Post("/login", [&baza, &sesije, &sesijeMutex](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string UserName = tijelo.at("username");
			string PassWord = tijelo.at("password");
			string Hash = baza.UzmiHash(UserName);
			if (Hash.empty() || crypto_pwhash_str_verify(Hash.c_str(), PassWord.c_str(), PassWord.size()) != 0) {
				res.status = 401;
				res.set_content("{\"Greska\":\"Pogresno korisnicko ime ili lozinka\"}", "application/json");
				return;
			}
			unsigned char bajtovi[32];
			randombytes_buf(bajtovi, sizeof bajtovi);
			char token[sodium_base64_ENCODED_LEN(sizeof bajtovi, sodium_base64_VARIANT_URLSAFE_NO_PADDING)];
			sodium_bin2base64(token, sizeof token, bajtovi, sizeof bajtovi, sodium_base64_VARIANT_URLSAFE_NO_PADDING);
			string Uloga = baza.UzmiUlogu(UserName);
			{
				std::lock_guard<mutex>lock(sesijeMutex);
				for (auto it = sesije.begin(); it != sesije.end();) {
					if (it->second.istice < std::chrono::steady_clock::now()) {
						it = sesije.erase(it);
					}
					else {
						++it;
					}
				}
				sesije[token] = { UserName,Uloga,std::chrono::steady_clock::now() + std::chrono::hours(1) };
			}
			json Poruka;
			Poruka["Token"] = token;
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
	server.Post("/logout", [&sesije, &sesijeMutex](const httplib::Request& req, httplib::Response& res) {
		string auth = req.get_header_value("Authorization");
		string Prefiks = "Bearer ";
		string token = auth.substr(Prefiks.size());
		{
			std::lock_guard<mutex>lock(sesijeMutex);
			sesije.erase(token);
		}
		res.status = 200;
		res.set_content("{\"Poruka\":\"Odjava uspijesna\"}", "application/json");
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
	server.listen("0.0.0.0", 8080);
	return 0;
}