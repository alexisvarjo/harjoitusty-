# Viikkoraportti 2

Käytin tällä viikolla aikaa noin kymmenen tuntia projektiin.
## 1. Mitä olen tehnyt tällä viikolla?
- Tein tällä viikolla Huffmanin koodauksen toteutuksen sillä tasolla, että
ohjelmalle voi syöttää syötteenä pakattavan merkkijonon ja se palauttaa
pakatun merkkijonon, tai pakatun merkkijonon voi syöttää funktiolle ja se
palauttaa puretun merkkijonon.
## 2. Miten ohjelma on edistynyt?
- Puolet algoritmisesta osuudesta on tehty. Kirjoitin yhden testin, joka testaa
kokonaisuudessaan Huffmanin-koodauksen algoritmia satunnaisilla
syötteillä.
## 3. Mitä opin tällä viikolla / tänään?
- Huffmanin koodauksen toimintaperiaatteen, ja että C++:lla ääkkösten
käsittely saattaa aiheuttaa harmaita hiuksia. Myös joitain testaamisen
perusteita on tullut luettua
## 4. Mikä jäi epäselväksi tai tuottanut vaikeuksia?
- En ollut varma tarviiko ohjelmani toimia myös suomenkielisillä syötteillä, vai
riittääkö vain englanninkielisillä syötteillä toimiminen, niin toteutin
varmuuden vuoksi siten, että ohjelma toimii molemmilla.
- Testaus: tuntuu vähän ärsyttävältä kirjoittaa testejä joka ikiseen funktioon,
nyt kirjoitin yhden ylätason testin joka testaa koko Huffmanin koodauksen
purkamisen ja pakkauksen kerralla. Toki testaaminen on arvokasta, ja
helpompi löytää bugeja, jos testaa pieniä palasia kerrallaan. Mutta
kokonaistestin ei pitäisi toimia, jos mikäli yksikin pienempi funktio ei toimisi.
## 5. Mitä teen seuraavaksi?
- Toisen pakkausalgoritmin ohjelmaani, jokin Lempel-Ziv algoritmeista.
Toteutan tämän siten, että voin toteuttaa lopullisen tavoitteen missä
ohjelma pystyy lukemaan tiedoston, pakkaamaan sen, ja lukemaan pakatun
tiedoston. Tämä siten, että on mahdollista verrata algoritmien nopeutta eri
syötteillä.
