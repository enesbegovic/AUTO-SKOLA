# AUTO-SKOLA

Konzolna C++ aplikacija za upravljanje auto-školom (kandidati, instruktori, automobili, polaganja), sa SQLite bazom podataka. Sav rad sa bazom ide preko prepared statements.

## Tehnologije

- C++ (Visual Studio, konzolna aplikacija)
- SQLite3
- Git / GitHub za verzionisanje

## Struktura baze

### Automobili
- `ID` (PRIMARY KEY AUTOINCREMENT)
- `MarkaAutomobila`
- `RegistarskaOznaka` (UNIQUE)

### Instruktori
- `ID` (PRIMARY KEY AUTOINCREMENT)
- `ImePrezime`
- `JMBG` (UNIQUE)

### Kandidati
- `ID` (PRIMARY KEY AUTOINCREMENT)
- `ImePrezime`
- `DatumRodjenja`
- `JMBG` (UNIQUE)

### Polaganja
- `ID` (PRIMARY KEY AUTOINCREMENT)
- `KandidatID` (FOREIGN KEY → Kandidati, ON DELETE RESTRICT)
- `InstruktorID` (FOREIGN KEY → Instruktori, ON DELETE RESTRICT)
- `AutomobilID` (FOREIGN KEY → Automobili, ON DELETE RESTRICT)
- `Kategorija`
- `DatumPolaganja`
- `Polozio` (NULL = ceka polaganje, 0 = nije polozio, 1 = polozio)

## Funkcionalnosti

**Kandidati**
- Dodaj / ukloni / prikazi sve kandidate

**Instruktori**
- Dodaj / ukloni / prikazi sve instruktore

**Automobili**
- Dodaj / ukloni / prikazi sve automobile

**Polaganja**
- Zakazi polaganje (kandidat + instruktor + automobil + kategorija + datum), provjera duplikata
- Unesi rezultate polaganja — pronalazi sve kandidate zakazane za dati datum i unosi rezultat za svakog (transakcija)
- Prikazi sva polaganja / samo aktivna (ceka rezultat) / samo zavrsena (polozio ili nije)

**Izvjestaji**
- Prikazi najmladjeg kandidata (racuna starost preko julianday())
- Prikazi sve kandidate sa njihovim statusom (nema zakazano / ceka rezultate / polozio / nije polozio)
- Prikazi broj polaganja po instruktoru
- Prikazi instruktore sa vise od jednog aktivnog polaganja

## Kljucni koncepti koristeni u projektu

- Prepared statements za sve upite (bind/step/reset umjesto sqlite3_exec)
- FOREIGN KEY constraint-i sa ON DELETE RESTRICT
- Provjera duplikata i postojanja zapisa prije INSERT/DELETE (SELECT COUNT)
- Transakcije (BEGIN/COMMIT) za unos vise rezultata odjednom
- JOIN upiti kroz Polaganja → Kandidati/Instruktori/Automobili
- CASE WHEN u SQL-u za citljiv prikaz statusa (umjesto ternarnog izraza u C++)
- julianday() za racunanje razlike/starosti na osnovu datuma

## Pokretanje

1. Otvoriti `.slnx` fajl u Visual Studio-u
2. Build (Debug/x64)
3. Pokrenuti — baza (`AUTO-SKOLA.db`) se automatski kreira pri prvom pokretanju ako ne postoji
