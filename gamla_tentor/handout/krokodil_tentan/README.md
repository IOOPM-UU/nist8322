# Instruktioner

## I Windows

Efter att ha loggat in, sök efter "ThinLinc-klient" i sökrutan i
nedre vänstra hörnet och öppna den. Skriv in servern
"thinlinc.student.it.uu.se" och ditt eget användarnamn och
lösenord. Om du får frågan om du litar på denna host, tryck på
"Continue". Det nya fönstret som kommer upp kör nu på
IT-institutionens Linux-maskiner. Du kan göra fönstret helskärm
genom att klicka på rutan i det övre högra hörnet av fönstret.
Undvik att trycka på "x" eftersom detta kommer att avsluta din
anslutning!

## I Linux

Öppna en terminal och skriv direkt `mkdir kodprov251030`. Gå till
denna mapp med `cd kodprov251030`. Ladda hem kodprovet till datorn
med följande kommando:

    curl -L --remote-name http://eliasc.github.io/ioopm/assam.zip

Nu har du en zipfil med den utdelade koden. Packa upp den med
`unzip assam.zip` (och ange lösenordet som du får under labben) så
får du *en annan* zipfil som heter `kodprov.zip`. Packar du upp
den med `unzip kodprov.zip` så får du en mapp `handout` som
innehåller uppgiften som ska lösas, samt en `Makefile` som du
använder för att lämna in kodprovet.

## Att lämna in

Gå till mappen `kodprov251030`. Om du är osäker på var du är i
filsystemet kan du skriva `cd; cd kodprov251030`. Skriv `make
handin` för att lämna in kodprovet. Detta kommer att skapa en
zipfil som innehåller filen du ska lämna in (och inga andra
filer), och denna fil kommer att sparas på en plats där vi kan
rätta den.

## Allmänna regler

- Samma regler gäller som vid en skriftlig salstenta: inga
  mobiltelefoner, inga textmeddelanden, inga samtal med någon
  förutom lärare på kursen, oavsett medium.
- Du måste kunna identifiera dig.
- Du får ta med ett handskrivet papper (max A4). Du får skriva på
  båda sidorna, men får inte ta med ett förstoringsglas.
- Du får ta med en bok i fysisk form, på en läsplatta eller på en
  laptop. Du får inte köra några andra program på denna maskin och
  får inte använda den för något annat än att läsa kurslitteratur.
- Du måste skriva all kod själv, förutom koden som har delats ut.

# Rutschkanor och stegar

Spelet "Rutchkanor och stegar" ("Chutes and Ladders" på engelska)
är ett enkelt brädspel där pjäser rör sig längs med rutor på en
bana. Vissa rutor på banan har rutschkanor som gör att man hamnar
längre bak på banan, medan andra rutor har stegar som leder till
längre fram. Det här kodprovet går ut på att modellera hur pjäser
rör sig på ett sådant bräde.

I nedanstående skiss går banan från `A` till `H`, med en
rutschkana från `G` ner till `B` (visas som `@`) och en stege från
`C` upp till `F` (visas som `#`):

    H <- G <- F <- E
         @    #    |
         @    #    |
    A -> B -> C -> D

Landar man på `C` klättrar man direkt upp till `F`. Landar man på
`G` åker man direkt ner till `B`.

## Ett bibliotek för spelbanor

I filen `board.h` definieras ett gränssnitt för ett bibliotek som
representerar en bana. Typen `tile_t` representerar en ruta på
banan. En ruta innehåller tre saker: en pekare till nästa ruta
(`NULL` om det är den sista rutan), en pekare till en annan ruta
som är kopplad via en rutschkana eller stege (`NULL` om det inte
finns någon sådan), samt en indikator huruvida en pjäs står på
rutan eller inte. Notera att vi inte behöver skilja på stegar
eller rutschkanor -- båda är länkar till en annan ruta. För
enkelhets antar vi att en ruta där en stege eller rutschkana
slutar inte har en egen stege eller rutschkana från sig.

Det finns fem funktioner i gränssnittet:

- `tile_t *tile_create(tile_t *next)` — skapar en ruta utan
  rutschkana/stege och utan pjäs. Pekaren `next` anger nästa ruta
  i banan.

- `void tile_destroy(tile_t *tile)` — frigör allt minne som går att
  nå från och med rutan `tile`.

- `bool tile_add_link(tile_t *from, tile_t *to)` — lägger till en
  rutschkana eller stege från rutan `from` till rutan `to`.
  Returnerar `false` (och gör ingenting) om det redan finns en
  länk från `from`, annars `true`. Du kan anta att det redan finns
  en väg mellan `from` och `to` (in någon riktning) som inte
  använder några rutschkanor eller stegar.

- `bool tile_has_piece(tile_t *tile)` — kontrollerar om rutan
  innehåller pjäsen just nu.

- `tile_t *tile_move(tile_t *start, size_t steps)` — flyttar en ny
  pjäs `steps` steg framåt från rutan `start`. Om rutan redan har
  en pjäs flyttas den pjäsen istället. När pjäsen slutar på en
  ruta med en rutschkana eller stege så hamnar pjäsen på den
  länkade rutan. Det går inte att färdas längs mer än en
  rutschkana eller stege på ett drag -- tar man en rutschkana
  eller stege till en ruta med en rutschkana eller stege så
  stannar man där. Om pjäsen hamnar utanför banans slut så stannar
  den på den sista rutan. Om pjäsen försöker flytta till en ruta
  som redan har en pjäs så stannar den på sin ursprungliga plats.
  Funktionen returnerar den ruta pjäsen slutligen hamnar på.

Följande exempel visar en interaktion med biblioteket:

```c
// Bygg banan a -> b -> c -> d -> e -> f -> g -> h
tile_t *h = tile_create(NULL);
tile_t *g = tile_create(h);
tile_t *f = tile_create(g);
tile_t *e = tile_create(f);
tile_t *d = tile_create(e);
tile_t *c = tile_create(d);
tile_t *b = tile_create(c);
tile_t *a = tile_create(b);

// Lägg till en stege från c till f och en rutschkana från g till b
tile_add_link(c, f);
tile_add_link(g, b);

// Flytta en pjäs från a tre steg framåt (hamnar på d)
assert(tile_move(a, 3) == d);

// Nu har d en pjäs, men inte a (till exempel)
assert(tile_has_piece(d));
assert(!tile_has_piece(a));

// Flytta en pjäs från a två steg framåt (hamnar på c och tar stegen till f)
assert(tile_move(a, 2) == f);

// Flytta pjäsen från d tre steg framåt (hamnar på g och åker rutschkana ner till b)
assert(tile_move(d, 3) == b);

// Försök flytta pjäsen från b fyra steg framåt (där står redan en pjäs, stå still)
assert(tile_move(b, 4) == b);

// Frigör minnet
tile_destroy(a);
```

**Din uppgift är att implementera nödvändiga funktioner och
strukturer från `board.h` i filen `board.c`. När du är klar borde
du kunna köra testerna genom Valgrind utan minnesfel eller
-läckor.**

## Filer
- `board.h` -- Gränssnittet som du ska implementera
- `board.c` -- Du ska skriva din implementation här
- `board_test.c` -- Tester för din implementation
- `Makefile`  -- En makefil med följande targets
  - `all`     -- Bygg programmet
  - `run`     -- Bygg och kör main-funktionen i board.c
  - `test`    -- Bygg och kör testerna
  - `memrun`  -- Bygg och kör main-funktionen i board.c med Valgrind
  - `memtest` -- Bygg och kör testerna med Valgrind
  - `clean`   -- Ta bort filer som har byggts

Notera att du kan skriva egna tester i den existerande
`main`-funktionen i `board.c` och köra den med `make run`. Koden i
`main`-funktionen kommer inte att bedömas.

## Tips
- Pjäser behöver ingen dynamisk data, utan det räcker med att
  indikera om ett rälssegment har en pjäs på sig eller inte.
- När du flyttar en pjäs vill du förmodligen först räkna ut vilken
  ruta du landar på och sen avgöra ifall pjäsen ska flyttas eller
  inte (beroende på om det redan stod en pjäs där).
- Typen `size_t` är bara en icke-negativ `int`.
