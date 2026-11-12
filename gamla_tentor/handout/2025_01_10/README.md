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

Öppna en terminal och skriv direkt `mkdir kodprov250110`. Gå till
denna mapp med `cd kodprov250110`. Ladda hem kodprovet till datorn
med följande kommando:

    curl -L --remote-name http://eliasc.github.io/ioopm/ticket.zip

Nu har du en zipfil med den utdelade koden. Packa upp den med
`unzip ticket.zip` (och ange lösenordet som du får under labben) så
får du *en annan* zipfil som heter `kodprov.zip`. Packar du upp
den med `unzip kodprov.zip` så får du en mapp `handout` som
innehåller uppgiften som ska lösas, samt en `Makefile` som du
använder för att lämna in kodprovet.

## Att lämna in

Gå till mappen `kodprov250110`. Om du är osäker på var du är i
filsystemet kan du skriva `cd; cd kodprov250110`. Skriv `make
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

# Jacquardvävstolar

En av de första programmerbara maskinerna var den så kallade
Jacquardvävstolen från början av 1800-talet. Den kunde väva olika
former av textilier med hjälp av hålkort som styrde hur maskinen
skulle arbeta. I det här kodprovet ska vi implementera en enkel
modell av en programerbar vävstol. Varje rad i väven kommer att
vara en sträng, och en hel väv blir därför en array av strängar.

Varje instruktion till vävstolen representeras av en sträng som
beskriver hur en viss rad ska vävas. En sträng utan siffror
reproduceras som den är: instruktionen "abcd" kommer att vävas som
"abcd". Siffror används för att repetera tecken: instruktionen
"2a3b2c" kommer att vävas som "aabbbcc". För enkelhets skull
tillåts bara *siffror* (alltså ensiffriga tal). En siffra får inte
följas av en annan siffra och får inte stå sist i en instruktion.

## Ett bibliotek för vävstolar

I filen `loom.h` definieras ett gränssnitt för ett bibliotek för
att modellera programmerbara vävstolar. Den enda synliga typen är
`loom_t` som representerar en vävstol. En vävstol har en
maxkapacitet för hur många instruktioner den kan lagra samt en
fast bredd för hur breda rader (strängar) den producerar.

Det finns fyra funktioner i gränssnittet:

- `loom_t *loom_create(size_t instr_size, size_t width)` -- skapa
  en tom vävstol med plats för `instr_size` instruktioner och
  vävbredd `width`. Om `instr_size` eller `width` är 0 returneras
  `NULL`.
- `void loom_destroy(loom_t *loom)` -- frigör minnet för vävstolen
  `loom`, inklusive alla instruktioner som har lagts till.
- `bool loom_add_instruction(loom_t *loom, char *instruction)` --
  försök lägga till instruktionen `instruction` sist bland
  instruktionerna i `loom`. Returnerar `true` om tillägget
  lyckades, annars `false`. Instruktionen måste vara välformad
  (inga två siffror i rad, inga siffror sist) och producera en rad
  av rätt bredd. Det måste också finnas plats för instruktionen.
  Vävstolen måste ta egna kopior på instruktionerna så att de kan
  frigöras. Instruktionen får inte vara `NULL`.
- `char **loom_weave(loom_t *loom)` -- väv en väv av vävstolen
  `loom`s nuvarande instruktioner. Instruktionerna vävs en i taget
  i den ordning de lagts till. Varje rad ska vara allokerad
  separat på heapen och läggas i en array vars sista element är en
  `NULL`-pekare.

En vävstol kan väva sin väv hur många gånger som helst -- en
tillagd instruktion försvinner aldrig. Till din hjälp har du fått
en funktion `loom_check_instruction()` som kollar att en
instruktion är välformad och att den producerar en rad av en viss
bredd. Fler tips för implementationen hittar du sist i
instruktionerna!

Följande exempel visar en interaktion med biblioteket:

```c
  // Create a loom that stores 10 instructions and weaves rows of width 5
  loom_t *loom = loom_create(10, 5);

  // Weaving with no instructions yields a singleton array containing NULL
  char **empty_tapestry = loom_weave(loom);
  assert(empty_tapestry[0] == NULL);
  free(empty_tapestry);

  // Some instructions for weaving rows of width 5
  assert(loom_add_instruction(loom, "  A  "));
  assert(loom_add_instruction(loom, " 3X "));
  assert(loom_add_instruction(loom, " 3X "));
  assert(loom_add_instruction(loom, "5V"));

  char **tapestry = loom_weave(loom);
  for (int i = 0; tapestry[i] != NULL; i++) {
    puts(tapestry[i]);
    free(tapestry[i]);
  }
  // Prints
  //   A
  //  XXX
  //  XXX
  // VVVVV
  free(tapestry);

  // Instructions of the wrong width are rejected
  assert(!loom_add_instruction(loom, ""));
  assert(!loom_add_instruction(loom, "3A2BC"));

  // Instructions cannot be NULL
  assert(!loom_add_instruction(loom, NULL));

  // Free allocated memory
  loom_destroy(loom);
```

**Din uppgift är att implementera nödvändiga funktioner och
strukturer från `loom.h` i filen `loom.c`. Du får inte ändra på
`loom.h`. När du är klar borde du kunna köra testerna genom
Valgrind utan minnesfel eller -läckor.**

## Filer

- `loom.h` -- Gränssnittet som du ska implementera
- `loom.c` -- Du ska skriva din implementation här
- `loom_test.c` -- Tester för din implementation
- `Makefile`  -- En makefil med följande targets
  - `all`     -- Bygg programmet
  - `run`     -- Bygg och kör `main`-funktionen i `loom.c`
  - `test`    -- Bygg och kör testerna
  - `memrun`  -- Bygg och kör `main`-funktionen i `loom.c` med Valgrind
  - `memtest` -- Bygg och kör testerna med Valgrind
  - `clean`   -- Ta bort filer som har byggts

Notera att du kan skriva egna tester i den existerande
`main`-funktionen i `loom.c` och köra den med `make run`. Koden i
`main`-funktionen kommer inte att bedömas.

## Tips

- Kanske kommer du att vilja ha en array i en strukt. Kom ihåg att
  typen `foo_t *arr[]` används för så kallade "flexible array
  members" när den står i en `struct`. Om du inte vet vad det är så
  vill du nog ha typen `foo_t **arr`, alltså en pekare till (en
  array av) `foo_t *`.
- Om du använder en array kommer du att behöva hålla koll på
  vilket index nästa instruktion ska läggas på, utöver storleken
  på arrayen.
- Du slipper hantera vissa jobbiga gränsfall eftersom du vet att
  varje instruktion är välformad. Du vet till exempel att en
  siffra alltid följs av ett tecken som inte är en siffra och att
  ingen instruktion är `NULL`.
- Skriv en hjälpfunktion som bara väver en rad givet en
  instruktion och en väntad bredd (så att du vet hur mycket minne
  som ska allokeras). Du kan skriva din kod så att varje tecken
  alltid skrivs $n$ antal gånger, där $n$ är $1$ om ingen siffra
  har lästs. I funktionen `loom_check_instruction` finns kod som
  gör om en siffra till motsvarande heltal. Testa din
  hjälpfunktion i `main` så att du vet att den fungerar innan du
  använder den i `loom_weave()`!
- Typen `size_t` är bara en icke-negativ `int`.