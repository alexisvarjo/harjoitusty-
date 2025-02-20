
# Kääntöohjeet
## MacOS

**Ohjelmaa varten**
Aja projektikansion juuressa ```make```


**Testejä varten**
Aja komentokehotteessa ```brew install googletest```

Sen jälkeen projektikansion juuressa ```make test``` testejä varten.

Jos haluat ajaa projektin ASanilla, lisää make-komentoon vipu ```SANITIZE=1``` ja aja ohjelma normaalisti.

## Linux (Debian/Ubuntu tai pkg-config asennettuna)

**Ohjelmaa varten**
Aja ensin ```sudo apt-get install pkgconfig``` ja sen jälkeen aja projektin juuressa ```make```.

**Testejä varten**
Aja komentokehotteessa ```sudo apt-get install pkgconfig``` ja ```sudo apt install libgtest-dev```

Sen jälkeen projektikansion juuressa ```make``` ja ```make test``` testejä varten.

Jos haluat ajaa projektin Valgrindilla, lisää make-komentoon vipu ```SANITIZE=1```. Tällöin ```make test``` ajaa samalla myös testit Valgrindilla.

Jos tämä ei toimi, Makefilessä on kommentoituna lisää ohjeita, joista saattaa olla hyötyä.


# Käyttöohjeet
Valmista projektia voi käyttää parilla eri tavalla. Jos käänsit projektin makefilellä, niin käyttö on seuraavaa. Kaikki komennot ajetaan projektin juuressa

**Pakatessa:**
```build/main <tiedoston tai kansion nimi> <algoritmi>```
Ohjelma hyväksyy syötteeksi ainoastaan tiedostoja, joissa päätteenä .txt, tai kansion. Jos kansiossa on sekä .txt-tiedostoja että muita, ja kansion ensimmäinen tiedosto on .txt-tiedosto, ohjelma pakkaa kaikki .txt-tiedostot eikä koske muihin tiedostoihin.
Vipu <algoritmi> sisältää vaihtoehdot 'h' ja 'lz'. Vaihtoehto 'h' valitsee Huffmanin koodauksen ja vaihtoehto 'lz' valitsee LZ78-algoritmin.

**Purkaessa:**
```build/main <tiedoston tai kansion nimi>```
Ohjelma hyväksyy syötteeksi ainoastaan .bin-tiedostoja (joita pakkaaminen tuottaa) tai kansion. Jos syötteenä on kansio, kansion ensimmäisenä tiedostona täytyy olla .bin-tiedosto, jotta ohjelma purkaa tiedostoja. Tällöin kaikki .txt-tiedostot hypätään yli, ja kaikki .bin-tiedostot puretaan.
Ohjelmalle ei nyt tarvitse antaa algoritmia, sillä ohjelma päättelee jokaisen tiedoston kohdalla sen binäärin alkuun asetetun bitin perusteella, kummalla algoritmilla tiedosto on pakattu ennen kuin se ryhtyy purkamaan tiedostoa.

**Vertaillessa tehokkuutta:**
```build/main <tiedoston tai kansion nimi> p```
Tämä suorittaa nopeusvertailun joko yhdelle tiedostolle tai kummallekin tiedostolle. Lisäksi pakattujen tiedostojen kokoja vertaillaan. Ohjelma myös tarkistaa että lopputuloksena on sama tiedosto kuin syötteenä.
Testi menee seuraavasti: luetaan tiedosto, pakataan se, kirjoitetaan pakattu tiedosto, luetaan pakattu tiedosto, puretaan se, ja kirjoitetaan pakattu tiedosto. Sen jälkeen tarkistetaan, että saatu tiedosto on sama kuin syötetty tiedosto.
Tämä testi tehdään molemmille algoritmeille peräkkäin, ja ajastetaan pakkaaminen ja purkaminen (ilman tiedostonkirjoitusta tai lukua).
Ohjelma tulostaa ajanoton tulokset, ja myös molemmilla algoritmeilla kuinka paljon pakkaamisella säästetään tilaa (pakatun tiedoston koko jaettuna syötetyn tiedoston koolla).

Kaikkien näiden testien jälkeen ohjelma poistaa kaikki saadut tiedostot, paitsi alkuperäisen .txt-tiedoston.
