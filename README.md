# Pakkaus- ja purkuohjelma kurssille Algoritmit ja Tekoäly Harjoitustyö

Tätä ohjelmaa voi käyttää tiedostojen pakkaamiseen ja purkamiseen tai Huffmanin koodauksen ja Lempel-Ziv78 algoritmin tehojen vertailuun.

# Riippuvuudet:
- [icu4c](https://github.com/unicode-org/icu)
- [googletest](https://github.com/google/googletest)

# Kääntöohjeet
## MacOS
Aja komentokehotteessa ```brew install icu4c@75 && brew install googletest```

Sen jälkeen projektikansion juuressa ```make``` itse ohjelmaa varten ja ```make test``` testejä varten.

## Linux (Debian/Ubuntu tai pkg-config asennettuna)
Aja komentokehotteessa ```sudo apt-get install libicu-dev && sudo apt install libgtest-dev```

Sen jälkeen projektikansion juuressa ```make``` ja ```make test``` testejä varten.

Jos tämä ei toimi, Makefilessä on kommentoituna lisää ohjeita, joista saattaa olla hyötyä.


# Käyttöohjeet
Valmista projektia voi käyttää kahdella eri tavalla:

```build/main <tiedoston tai kansion nimi> <algoritmi> <käyttötila>```

tai 

```build/main <tiedoston tai kansion nimi> p```

Vipu ```<algoritmi>``` valitsee algoritmin, vaihtoehdolla ```lz``` se käyttää Lempel-Ziv algoritmia, ja vaihtoehdolla ```h``` se käyttää Huffmanin koodausta.

Vipu ```<käyttötila>``` tarkoittaa puretaanko pakattu tiedosto, vai pakataanko tiedosto. ```e``` pakkaa tiedoston, ja ```d``` purkaa pakatun tiedoston.

Toinen käyttötapa suorittaa testin, jossa ohjelma ajaa molemmat algoritmit, ensin pakkaa, sitten purkaa ottaa tästä ajan ja tarkistaa
tulosten oikeellisuuden. Ohjelma myös kertoo kuinka paljon tilaa pakattu tiedosto vie.

Ohjelman on tarkoitus tulevaisuudessa pystyä käsittelemään sekä kansioita, että yksittäisiä tiedostoja.

## [Määrittelydokumentti](dokumentaatio/Määrittelydokumentti.pdf)

## [Testausdokumentti](dokumentaatio/Testausdokumentti.pdf)

## [Toteutusdokumentti](dokumentaatio/Toteutusdokumentti.pdf)

## [Käyttöohje](dokumentaatio/Käyttöohje.pdf)


## Viikkoraportit

[Viikkoraportti 1](Viikkoraportit/Viikkoraportti1.pdf)

[Viikkoraportti 2](Viikkoraportit/Viikkoraportti2.pdf)

[Viikkoraportti 3](Viikkoraportit/Viikkoraportti3.pdf)

[Viikkoraportti 4](Viikkoraportit/Viikkoraportti4.pdf)

[Viikkoraportti 5](Viikkoraportit/Viikkoraportti5.pdf)

[Viikkoraportti 6](Viikkoraportit/Viikkoraportti6.pdf)
