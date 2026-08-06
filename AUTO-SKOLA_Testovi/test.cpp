#include "pch.h"
#include "Baza.h"

TEST(BazaTest, DodajAutomobilProvjeriBrojAutomobila) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajAutomobil("A1", "123456");
	EXPECT_EQ(testBaza.brojAutomobila(),1);
}
TEST(BazaTest, DodajDupliAutomobilProvjeriBrojAutomobila) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajAutomobil("A1", "123456");
	EXPECT_EQ(testBaza.brojAutomobila(), 1);
	testBaza.DodajAutomobil("A1", "123456");
	EXPECT_EQ(testBaza.brojAutomobila(), 1);
}
TEST(BazaTest, UkloniAutomobilProvjeriBrojAutomobila) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajAutomobil("A1", "123456");
	EXPECT_EQ(testBaza.brojAutomobila(), 1);
	testBaza.UkloniAutomobil("123456");
	EXPECT_EQ(testBaza.brojAutomobila(), 0);
}
TEST(BazaTest, UkloniNepostojeciAutomobilProvjeriBrojAutomobila) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajAutomobil("A1", "123456");
	EXPECT_EQ(testBaza.brojAutomobila(), 1);
	testBaza.UkloniAutomobil("000000");
	EXPECT_EQ(testBaza.brojAutomobila(), 1);
}
TEST(BazaTest, DodajInstruktoraProvjeriBrojInstruktora) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajInstruktora("Instruktor1", "123456");
	EXPECT_EQ(testBaza.brojInstruktora(), 1);
}
TEST(BazaTest, DodajDuplogInstruktoraProvjeriBrojInstruktora) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajInstruktora("Instruktor1", "123456");
	EXPECT_EQ(testBaza.brojInstruktora(), 1);
	testBaza.DodajInstruktora("Instruktor12", "123456");
	EXPECT_EQ(testBaza.brojInstruktora(), 1);
}
TEST(BazaTest, ObrisiInstruktoraProvjeriBrojInstruktora) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajInstruktora("Instruktor1", "123456");
	EXPECT_EQ(testBaza.brojInstruktora(), 1);
	testBaza.UkloniInstruktora("123456");
	EXPECT_EQ(testBaza.brojInstruktora(), 0);
}
TEST(BazaTest, ObrisiNepostojecegInstruktoraProvjeriBrojInstruktora) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajInstruktora("Instruktor1", "123456");
	EXPECT_EQ(testBaza.brojInstruktora(), 1);
	testBaza.UkloniInstruktora("000000");
	EXPECT_EQ(testBaza.brojInstruktora(), 1);
}
TEST(BazaTest, DodajKandidataProvjeriBrojKandidata) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajKandidata("Kandidat1", "2000-01-01", "123456");
	EXPECT_EQ(testBaza.brojKandidata(), 1);
}
TEST(BazaTest, DodajDuplogKandidataProvjeriBrojKandidata) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajKandidata("Kandidat1", "2000-01-01", "123456");
	EXPECT_EQ(testBaza.brojKandidata(), 1);
	testBaza.DodajKandidata("KANDIDAT", "2000-05-05", "123456");
	EXPECT_EQ(testBaza.brojKandidata(), 1);
}
TEST(BazaTest, ObrisiKandidataProvjeriBrojKandidata) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajKandidata("Kandidat1", "2000-01-01", "123456");
	EXPECT_EQ(testBaza.brojKandidata(), 1);
	testBaza.UkloniKandidata("123456");
	EXPECT_EQ(testBaza.brojKandidata(), 0);
}
TEST(BazaTest, ObrisiNepostojecegKandidataProvjeriBrojKandidata) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajKandidata("Kandidat1", "2000-01-01", "123456");
	EXPECT_EQ(testBaza.brojKandidata(), 1);
	testBaza.UkloniKandidata("000000");
	EXPECT_EQ(testBaza.brojKandidata(), 1);
}
TEST(BazaTest, DodajPolaganjeProvjeriBrojPolaganja) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajAutomobil("A1", "111-222");
	testBaza.DodajInstruktora("Instruktor1", "111111");
	testBaza.DodajKandidata("Kandidat1","2000-01-01", "123456");
	testBaza.ZakaziPolaganje("123456", "111111", "111-222", "B", "2026-08-08");
	EXPECT_EQ(testBaza.brojPolaganja(), 1);
}
TEST(BazaTest, DodajPolaganjeNepostojeciKandidatProvjeriBrojPolaganja) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajAutomobil("A1", "111-222");
	testBaza.DodajInstruktora("Instruktor1", "111111");
	testBaza.DodajKandidata("Kandidat1", "2000-01-01", "123456");
	testBaza.ZakaziPolaganje("000000", "111111", "111-222", "B", "2026-08-08");
	EXPECT_EQ(testBaza.brojPolaganja(), 0);
}
TEST(BazaTest, DodajPolaganjeNepostojeciInstruktorProvjeriBrojPolaganja) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajAutomobil("A1", "111-222");
	testBaza.DodajInstruktora("Instruktor1", "111111");
	testBaza.DodajKandidata("Kandidat1", "2000-01-01", "123456");
	testBaza.ZakaziPolaganje("123456", "000000", "111-222", "B", "2026-08-08");
	EXPECT_EQ(testBaza.brojPolaganja(), 0);
}
TEST(BazaTest, DodajPolaganjeNepostojeciAutomobilProvjeriBrojPolaganja) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajAutomobil("A1", "111-222");
	testBaza.DodajInstruktora("Instruktor1", "111111");
	testBaza.DodajKandidata("Kandidat1", "2000-01-01", "123456");
	testBaza.ZakaziPolaganje("123456", "111111", "000-222", "B", "2026-08-08");
	EXPECT_EQ(testBaza.brojPolaganja(), 0);
}
TEST(BazaTest, DodajDuploPolaganjeProvjeriBrojPolaganja) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajAutomobil("A1", "111-222");
	testBaza.DodajInstruktora("Instruktor1", "111111");
	testBaza.DodajKandidata("Kandidat1", "2000-01-01", "123456");
	testBaza.ZakaziPolaganje("123456", "111111", "111-222", "B", "2026-08-08");
	EXPECT_EQ(testBaza.brojPolaganja(), 1);
	testBaza.DodajAutomobil("B1", "222-333");
	testBaza.DodajInstruktora("Instruktor2", "222222");
	testBaza.ZakaziPolaganje("123456", "222222", "222-333", "B", "2026-08-08");
	EXPECT_EQ(testBaza.brojPolaganja(), 1);
}
TEST(BazaTest, RezultatiPolaganjaKandidatPolozio) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajAutomobil("A1", "111-222");
	testBaza.DodajInstruktora("Instruktor1", "111111");
	testBaza.DodajKandidata("Kandidat1", "2000-01-01", "123456");
	int PolaganjeID=testBaza.ZakaziPolaganje("123456", "111111", "111-222", "B", "2026-08-08");
	testBaza.RezultatiPolaganja(PolaganjeID, 1);
	EXPECT_EQ(testBaza.StatusPolaganja(PolaganjeID),1);
}
TEST(BazaTest, RezultatiPolaganjaKandidatNijePolozio) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajAutomobil("A1", "111-222");
	testBaza.DodajInstruktora("Instruktor1", "111111");
	testBaza.DodajKandidata("Kandidat1", "2000-01-01", "123456");
	int PolaganjeID = testBaza.ZakaziPolaganje("123456", "111111", "111-222", "B", "2026-08-08");
	testBaza.RezultatiPolaganja(PolaganjeID, 0);
	EXPECT_EQ(testBaza.StatusPolaganja(PolaganjeID),0);
}