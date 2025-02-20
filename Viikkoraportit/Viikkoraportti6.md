# Viikkoraportti 6

Käytin tällä viikolla noin viisi tuntia projektiin

# 1. Mitä olen tehnyt tällä viikolla?
- Muokkasin mainia siten, että käyttäjän ei tarvitse enää valita purkaako hän vai pakkaako hän, vaan ohjelma päättelee tämän tiedostopäätteen perusteella
- Tein ohjelmaa ylipäätään robustimmaksi
- Pieniä muokkauksia, joilla ohjelma kääntyy myös linuxilla
- Yksi riippuvuus pois (icu4) jota ei enää tarvitse kun ohjelma ei käytä u32string tai char32_t tietotyyppejä
- tiedostot luetaan ja kirjoitetaan isommissa palikoissa, jotta ohjelma toimii myös suurilla tekstitiedostoilla
# 2. Miten ohjelma on edistynyt?
- ihan hyvin. Haluaisin tehdä ohjelmasta nopeamman, mutta en usko, että ilman moniydinsuorittamisen implementoimista olisi mahdollisuuksia suuriin nopeusparannuksiin, ja sen toteuttaminen olisi vaikeaa
# 3. Mitä opin tällä viikolla?
- tutuistuin applen xcode profiler -työkaluun jolla voi analysoida ohjelman kauan vieviä osia ja mitä resursseja ohjelma käyttää
# 4. Mikä jäi epäselväksi tai tuotti vaikeuksia
- Ei oikeastaan mitään
# 5. Mitä teen seuraavaksi?
- Toteutusdokumentti, käyttöohje jne. laatiminen
