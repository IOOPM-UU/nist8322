# Inlupp 1: Hash Table & Frequency Counter

Detta repository innehåller en generisk implementation av en hashtabell och en länkad lista i C, komplett med iteratorer. Det inkluderar även programmet `freq_count` som använder hashtabellen för att läsa in och räkna ord-frekvenser i en eller flera textfiler.

## Författare
* **Nils Strandell**
* **Wilmer Säfsten**

## Kompilering

Projektet kompileras smidigast med den medföljande Makefilen:

* **Bygg huvudprogrammet (freq_count):**
  ```bash
  make freq_count
  ```

* **Bygg alla enhetstester:**
  ```bash
  make test
  ```

* **Rensa bort kompilerade körbara filer:**
  ```bash
  make clean
  ```

## Körning

### Frekvensräknare (freq_count)

Programmet tar en eller flera textfiler som argument och skriver ut hur många gånger varje ord förekommer, sorterat i fallande ordning.

```bash
./freq_count fil1.txt [fil2.txt ...]
```

### Enhetstester

För att bygga och köra CUnit-testerna (för både hashtabellen och den länkade listan) i ett svep:
```bash
make test
```

För att bygga och köra testerna med Valgrind för att verifiera att inga minnesläckor finns:
```bash
make memtest
```

## Designbeslut (Design Decisions)

* **Generisk data (`elem_t`):** Datastrukturerna är typ-agnostiska och lagrar all data (både nycklar och värden) via unionen `elem_t`. För att hantera datatyper säkert används macros som `int_elem()` och `ptr_elem()`.
* **Polymorfism via funktionspekare:** Hashtabellen är inte låst till en specifik datatyp (t.ex. strängar eller heltal). Istället skickar användaren in funktionspekare för hashning och jämförelse (`ioopm_hash_function`, `ioopm_eq_function`) när tabellen instansieras.
* **Minnesägandeskap (Ownership):** Hashtabellen tar **inte** ägandeskap över minnet för dess nycklar och värden. Om dynamiskt allokerade strängar (t.ex. via `strdup`) sätts in i tabellen, är det anroparens ansvar att frigöra detta minne innan tabellen förstörs.
* **Felhantering:** Grova användarfel (t.ex. att skicka in `NULL`-pekare istället för en instansierad hashtabell) fångas upp via `assert()` för att programmet ska avbrytas (fail-fast) snarare än att orsaka okontrollerade minnesfel. Mjukare fel, som uppslagning av nycklar som inte existerar, hanteras genom att funktionerna returnerar booleska värden (`false`).

## Kodtäckning (Code Coverage)

Verktyget `gcov` (med flaggan `-b` för branch coverage) användes för att mäta kodtäckning. Processen är automatiserad i Makefilen. För att kompilera med coverage-flaggor, köra testerna och generera rapporterna, körs kommandot:

```bash
make coverage
```

**Resultat per fil:**
* `hash_table.c`: 91.86% line coverage, 88.24% branch coverage
* `hash_table_iterator.c`: 100.00% line coverage, 100.00% branch coverage
* `linked_list.c`: 91.76% line coverage, 100.00% branch coverage
* `list_iterator.c`: 100.00% line coverage, 100.00% branch coverage

# Initial Profiling Results

**Profileringsmetod:** Kompilerades med flaggan `-pg` och analyserades med verktyget `gprof`. Eftersom programmets exekvering gick så snabbt att ingen tid hann ackumuleras, baseras analysen på antalet funktionsanrop.

**Topp 3 funktioner:**
1. `string_eq`
2. `find_previous_entry`
3. `string_hash`

**Analys:**
* **Egen kod vs. Bibliotek:** Alla tre är funktioner skrivna i min egen kod, inte inbyggda standardfunktioner från C-biblioteket.
* **Konsistens & Trender:** Dessa tre funktioner ligger konsekvent i topp oavsett vilken indatafil som används. Den tydligaste trenden är att när datamängden växer, ökar det absoluta antalet anrop till dessa specifika funktioner kraftigt, men deras inbördes ordning förblir oförändrad.
* **Förväntningar:** Detta stämmer helt överens med förväntningarna. En ordfrekvensräknare baserad på en hashtabell kommer naturligt att lägga majoriteten av sin exekveringstid på att hasha nycklar, vandra genom länkade listor (buckets) och jämföra strängar för att hitta matchningar eller lösa kollisioner.
* **Optimering:** Eftersom `find_previous_entry` och `string_eq` dominerar, lägger programmet uppenbarligen mycket resurser på kollisionshantering. Det mest effektiva sättet att göra programmet snabbare vore att implementera en självanpassande logik (dynamisk resizing) som ökar det totala antalet buckets när belastningsgradenblir för hög. Fler buckets ger kortare listor, vilket skulle minska antalet nödvändiga strängjämförelser.