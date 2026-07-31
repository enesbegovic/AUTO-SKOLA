# AUTO-SKOLA

Konzolna C++ aplikacija za upravljanje autoškolom, sa SQLite bazom podataka. Prati kandidate, instruktore, automobile i polaganja vozačkih ispita.

## Tabele
- Kandidati (ImePrezime, DatumRodjenja, JMBG)
- Instruktori (ImePrezime, JMBG)
- Automobili (MarkaAutomobila, RegistarskaOznaka)
- Polaganja (KandidatID, InstruktorID, AutomobilID, Kategorija, DatumPolaganja, Polozio)

## Funkcionalnosti
- Dodavanje, brisanje i prikaz kandidata, instruktora i automobila
- Zakazivanje polaganja (povezuje kandidata, instruktora i automobil)
- Unos rezultata polaganja (položio/nije položio)
- Prikaz svih, aktivnih (zakazanih) i završenih polaganja
- Zaštita od brisanja kandidata/instruktora/automobila koji imaju aktivno (nezavršeno) polaganje

## Tehnologije
- C++
- SQLite3
