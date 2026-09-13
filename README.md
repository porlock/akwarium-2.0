# Akwarium 2.0

Sterownik akwarium oparty na **Arduino Nano (ATmega328)**. Mierzy temperaturę i pH wody, pokazuje odczyty na wyświetlaczu Nokia 5110 i pozwala zmieniać ustawienia za pomocą joysticka.

Projekt powstał z myślą o zebraniu podstawowej kontroli akwarium w jednym urządzeniu: podglądzie parametrów wody, sterowaniu grzałką oraz automatyzacji obniżania pH i uzupełniania wody. Działa lokalnie, bez połączenia z internetem. Część automatyki wymaga jeszcze dokończenia — szczegóły poniżej.

## Co robi

- **Pomiar temperatury** — odczytuje pierwszy czujnik na magistrali OneWire przez bibliotekę DallasTemperature.
- **Pomiar pH** — przelicza napięcie z analogowego modułu sondy na pH; średnia ruchoma z 20 próbek wygładza odczyt.
- **Podgląd na ekranie** — pokazuje temperaturę, pH lub naprzemiennie oba parametry, a także ikony stanów sterowania i błędu.
- **Sterowanie grzałką** — przełącza wyjście przekaźnika w zależności od temperatury.
- **Konfiguracja joystickiem** — udostępnia progi temperatury i pH, czas impulsu dozowania oraz odstęp między impulsami.
- **Kalibracja pH** — zawiera procedurę dwupunktową dla buforów pH 7 i pH 4.
- **Pamięć ustawień** — zapisuje konfigurację w EEPROM i odczytuje ją po restarcie. Gdy znacznik danych jest niezgodny, korzysta z ustawień domyślnych.

## Stan automatyki

To projekt rozwojowy. Obecny kod ma następujące ograniczenia:

| Obszar | Obecne zachowanie |
| --- | --- |
| Grzałka | Włącza się poniżej dolnego progu i wyłącza powyżej tego samego progu. Górny próg jest dostępny w menu, ale nie jest wykorzystywany w sterowaniu — nie ma jeszcze histerezy między dwoma progami. |
| Obniżanie pH | Logika wyjścia nazwanego `HCL` przewiduje cykliczne dozowanie po przekroczeniu górnego progu, aż pH spadnie poniżej dolnego. W `RelayType::setClockOn()` znajduje się jednak deklaracja `void setOn();` zamiast wywołania `setOn();`, więc impuls nie załącza wyjścia. |
| Dolewka | Jest wyjście przekaźnika i logika sterowania, ale `readWaterLevel()` zawsze ustawia `false`. Odczyt czujnika poziomu wody nie został zaimplementowany. |
| Kalibracja | W czasie wyświetlania ekranu kalibracji główna pętla pomiarów i sterowania jest pomijana. Nie oznacza to automatycznego wyłączenia wcześniej aktywnych wyjść. |
| Błąd temperatury | Brak czujnika przy starcie sygnalizuje błąd i ustawia odczyt na 0°C. Sterowanie grzałką nie blokuje się z tego powodu. |

Przed użyciem automatyki w działającym akwarium należy poprawić te ograniczenia i sprawdzić zachowanie wyjść oraz obsługę awarii na sprzęcie.

## Sprzęt

- Arduino Nano z ATmega328, zgodnie z konfiguracją `nanoatmega328`.
- Wyświetlacz Nokia 5110 ze sterownikiem PCD8544.
- Czujnik temperatury zgodny z OneWire i DallasTemperature, np. DS18B20.
- Sonda pH z modułem udostępniającym analogowy sygnał napięciowy.
- Joystick analogowy z przyciskiem oraz sygnalizator dźwiękowy.
- Odpowiednie moduły wykonawcze dla grzałki, dozownika i planowanej dolewki.

Przypisanie sygnałów w kodzie:

| Sygnał | Pin Nano |
| --- | --- |
| Pomiar temperatury OneWire | D4 |
| Wejście analogowe pH | A6 |
| Joystick X / Y | A0 / A1 |
| Przycisk joysticka | D12 |
| Sygnalizator dźwiękowy | D6 |
| Wyjście `HCL` | D2 |
| Wyjście grzałki | D3, logika aktywna stanem niskim |
| Wyjście dolewki | D5 |

Wyświetlacz jest inicjalizowany w `src/screen.cpp` przez `Adafruit_PCD8544(11, 10, 9, 8)`. Przed podłączeniem należy sprawdzić znaczenie argumentów tego konstruktora w używanej wersji biblioteki. Powyższa tabela opisuje sygnały programu, a nie kompletny schemat połączeń i zasilania.

## Budowanie i wgrywanie

Projekt korzysta z **PlatformIO** i frameworka Arduino. Zależności oraz ich wersje znajdują się w `platformio.ini`: Adafruit GFX, Adafruit PCD8544, OneWire i DallasTemperature.

1. Otwórz katalog projektu w PlatformIO, np. w Visual Studio Code z rozszerzeniem PlatformIO IDE.
2. Podłącz Arduino Nano przez USB.
3. Zbuduj i wgraj program z interfejsu PlatformIO lub terminala:

   ```sh
   pio run -e nanoatmega328
   pio run -e nanoatmega328 --target upload
   ```

4. Opcjonalnie uruchom monitor portu szeregowego:

   ```sh
   pio device monitor --baud 9600
   ```

## Obsługa

Na ekranie głównym ruch joysticka w lewo lub w prawo przełącza widoki odczytów. Naciśnięcie przycisku otwiera ustawienia.

W ustawieniach:

- **Lewo / prawo** — poprzednia lub następna pozycja.
- **Góra / dół** — zmiana wartości.
- **Przycisk** — wykonanie wybranej akcji: zapis, reset lub kalibracja; na pozycji liczbowej powrót do ekranu głównego.

Zmiany ustawień działają w pamięci bieżącej sesji. Aby zachować je po odłączeniu zasilania, wybierz **„Zapis ustawien”**. Dotyczy to również konfiguracji po kalibracji i przywróceniu wartości domyślnych.

Kalibracja prowadzi kolejno przez pomiary buforów pH 7 i pH 4, czekając na stabilizację napięcia. Kod oczekuje zakresów 2,9–3,1 V dla pH 7 oraz 3,3–3,5 V dla pH 4, więc procedura jest dopasowana do konkretnego toru pomiarowego. Wartość `vRef` w `include/SettingsType.h` również wymaga dopasowania do sprzętu.

## Ustawienia domyślne

Są to wartości zapisane w programie, a nie zalecenia dla konkretnej obsady akwarium.

| Parametr | Wartość |
| --- | --- |
| Dolny / górny próg pH | 6,5 / 6,7 |
| Czas impulsu `HCL` | 3 s |
| Okres impulsów `HCL` | 30 min |
| Dolny / górny próg temperatury | 25 / 27°C |
| Napięcie odniesienia do przeliczeń ADC (`vRef`) | 4,49 V |

## Organizacja kodu

- `src/main.cpp` — inicjalizacja, główna pętla i logika sterowania.
- `src/probing.cpp` — odczyt czujników i przeliczanie pH.
- `src/RelayType.cpp` — obsługa wyjść przekaźników.
- `src/MainScreen.cpp`, `src/OptionsScreen.cpp`, `src/PhCalibrationScreen.cpp` — ekrany i ich obsługa.
- `src/screen.cpp` — rysowanie interfejsu na LCD.
- `include/SettingsType.h` — struktura konfiguracji i wartości domyślne.
- `include/GlobalEnums.h` — przypisanie pinów i stany interfejsu.
- `platformio.ini` — płytka docelowa, framework i biblioteki.
