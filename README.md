# IP Prefixes Set

Opis projektu i implementacji klasy C++ do zarządzania zbiorem **prefiksów IPv4**


## Opis przyjętego podejścia

Z racji przyjętego założenia, iż funkcja **check()** ma być maksymalnie zoptymalizowana konieczne było opracowanie odpowiedniej struktury danych. Adres *IPv4* składa się z czterech bajtów, tj. 32 bitów. Mając w świadomości to oraz dążenie do najniższej złożoności obliczeniowej jako strukturę do przechowywania prefiksów przyjęto **strukturę drzewa**, gdzie każdy jej element reprezentuje pojedynczy bit prefiksu oraz przechowuje następujące dane:

- wartość bitu  (**0** lub **1**)
- **isPrefix** (określa czy element jest ostatnim bitem prefiksu)
- **leadsToPrefix** (określa czy element prowadzi do dłuższego prefiksu)
- **maskLength** (przechowuje długość maski prefiksu)
- **nextLeft, nextRight** (wskaźnik do odpowiednio następnego lewego lub prawego elementu)
- **parent** (wskaźnik do elementu nadrzędnego)

Ponieważ prefiks składa się z bitów określających część sieciową oraz długości maski, można przedstawić go jako łańcuch bitów, gdzie każdy bit wskazuje na kolejny (element o wartości **1** jest prawym rozgałęzieniem, a o wartości **0** lewym). Tak właśnie napisana została zastosowana struktura. Każdy element wskazuje na następny w kolei, a ostatni bit przechowuje także długość maski dla danego prefiksu. Dzięki takiemu podejściu oraz maksymalnej ilości 32 bitów dla pojedynczego prefiksu funkcja **check()** potrzebuje maksymalnie 32 kroków aby stwierdzić, czy dany adres IP zawiera sie w jakimś prefiksie. Dodatkową zaletą takiej struktury jest to, że podczas przechodzenia przez drzewo każdy następny element może przechowywać jedynie większą długość maski od poprzedniej, przez co funkcja zwraca długość maksi przy ostatnim napotkanym elemencie z **isPrefix**.

Atrybut **leadsToPrefix** jest konieczny dla funkcji **del()**, aby podczas odłączania usuwanego prefiksu nie usunąć elementu, który znajduje się na drodze do innego prefiksu.

## Krótki opis funkcji

**add(const uint32_t &base, const uint8_t maskLength)**

Funkcja przechodzi po drzewie i jego istniejących węzłach, a w przypadku ich braku dołącza do drzewa kolejne. Po przejściu pętli nadaje ostatniemu elementowi atrybut **isPrefix** oraz ustawia **maskLength**. W przypadku istnienia prefiksu zwraca odpowiedni komunikat. Podczas iteracji funkcja dodatkowo nadaje atrybut **leadsToPrefix** napotkanym elementom.


**del(const  uint32_t  &base,  const  uint8_t  maskLenght)**

Funkcja przechodzi po drzewie. Po przejściu przejściu **maskLength** elementów sprawdza czy 
takowy element istnieje, jeśli tak to:

- ustawia **isPrefix** na *false* jeśli element prowadzi do innego prefiksu (**leadsToPrefix**)
- wywołuje funkcje odłączającą elementy drzewa jeśli element jest liściem (ostatnim elementem gałęźi)

W przeciwnym razie zwraca stosowny komunikat


**detachNodes(IPNode node)**

Przechodzi od liścia i odłącza odpowiednie elementy drzewa

**hasSibling(const  IPNode  node,  bool  nodeBit)**

Sprawdza czy rodzic danego elementu zawiera drugi potomny element

**isDetachable(const  IPNode  node)**

Sprawdza czy element można odłączyć, tj. czy element nie określa prefiksu i czy nie prowadzi do innego

**isPrefixValid(const  uint32_t  &base,  const  uint8_t  maskLenght,  const  char  ipAsString)**

Sprawdza czy podany prefiks jest poprawny

**parseToCIDR(const  uint32_t  &base,  const  uint8_t  maskLenght,  char  outBuffer)**

Konwertuje prefiks podany w formie liczbowej na postać tekstową

**freeTree(IPNode  node)**

Zwalnia pamięć zajmowaną przez drzewo

## Opis struktury projektu

- **include/**
	IPv4PrefixSet.hpp - plik nagłówkowy klasy *IPv4PrefixSet*

- **src/**
		IPv4PrefixSet.cpp - implementacja klasy *IPv4PrefixSet*
	tests.cpp - implementacja testów jednostkowych
	main.cpp - główny program z funkcją **main()** zawierający przykładowe wywołania funkcji  **check(), add() oraz del()** 

	build_and_run.bat - program do kompilacji uruchomienia projektu
	run_test.bat - program do uruchamiania testów po uprzednim zbudowaniu projektu
	CMakeLists.txt - plik z instrukcjami budowania projektu za pomocą CMake

## Kompilacja i uruchamianie

Projekt należy uruchamiać na systemie **Windows**. Dodatkowo konieczne jest oprogramowanie **CMake** w wersji **conajmniej 3.14**.

Sama kompilacja i uruchamianie jest bardzo proste, wystarczy uruchomić program **build_and_run.bat**. W przypadku chęci uruchomienia testów, po uprzednim zbudowaniu projektu, należy uruchomić program **run_tests.bat**.