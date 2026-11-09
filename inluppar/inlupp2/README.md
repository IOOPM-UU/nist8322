cat << 'EOF' > README.md
# IOOPM Inlupp 2: Webstore

## Bygga och köra tester
För att kompilera och köra hela testsviten (CUnit), kör:
```bash
make test
```

För att köra testerna med Valgrind (minneshantering), kör:
```bash
make memtest
```

## Kodtäckning (Code Coverage)
Följande kodtäckning per `.c`-fil har uppmätts med hjälp av verktygen `gcov` och `lcov`:

* `db_backend.c`: Linjetäckning 77.3 %, Grenartäckning 55.1 %
* `db_tests.c`: Linjetäckning 96.2 %, Grenartäckning 50.0 %
* `hash_table.c`: Linjetäckning 74.4 %, Grenartäckning 58.8 %
* `hash_table_iterator.c`: Linjetäckning 100.0 %, Grenartäckning 77.8 %
* `linked_list.c`: Linjetäckning 30.6 %, Grenartäckning 13.3 %
* `list_iterator.c`: Linjetäckning 100.0 %, Grenartäckning 57.1 %
* `utils.c`: Linjetäckning 0.0 %, Grenartäckning 0.0 % (I/O-hantering undantagen)

*Notera: Datastrukturerna (hash_table och linked_list) testades fullständigt i Inlupp 1. Täckningen här representerar endast den del som nyttjas av webbshoppens logik.*

För att generera denna HTML-rapport lokalt, kör:
```bash
make coverage
```
Rapporten hittas sedan i `coverage_report/index.html`.

## Starta programmet
För att bygga och starta det interaktiva gränssnittet i terminalen, kör:
```bash
make run
```

För att köra programmet automatiskt med fördefinierade testkommandon, kör:
```bash
make run_test
```
EOF