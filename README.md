# Pakkaus- ja purkuohjelma kurssille Algoritmit ja Tekoäly Harjoitustyö

Tätä ohjelmaa voi käyttää tiedostojen pakkaamiseen ja purkamiseen tai Huffmanin koodauksen ja Lempel-Ziv78 algoritmin tehojen vertailuun.

# Riippuvuudet:
- [googletest](https://github.com/google/googletest)

# Kääntöohjeet
## MacOS
Aja komentokehotteessa ```brew install googletest```

Sen jälkeen projektikansion juuressa ```make``` itse ohjelmaa varten ja ```make test``` testejä varten.

Jos haluat ajaa projektin ASanilla, lisää make-komentoon vipu ```SANITIZE=1``` ja aja ohjelma normaalisti.

## Linux (Debian/Ubuntu tai pkg-config asennettuna)
Aja komentokehotteessa ```sudo apt install libgtest-dev```

Sen jälkeen projektikansion juuressa ```make``` ja ```make test``` testejä varten.

Jos haluat ajaa projektin Valgrindilla, lisää make-komentoon vipu ```SANITIZE=1``` ja aja ohjelma normaalisti.

Jos tämä ei toimi, Makefilessä on kommentoituna lisää ohjeita, joista saattaa olla hyötyä.


# Käyttöohjeet
Valmista projektia voi käyttää kahdella eri tavalla:
Pakatessa:
```build/main <tiedoston tai kansion nimi> <algoritmi>```

Purkaessa:
```build/main <tiedoston tai kansion nimi>```

Vertaillessa tehokkuutta:
```build/main <tiedoston tai kansion nimi> p```

Vipujen merkitys:
'p', jolloin ohjelma suorittaa testin, jossa ohjelma ajaa molemmat algoritmit, ensin pakkaa, sitten purkaa ottaa tästä ajan ja tarkistaa
tulosten oikeellisuuden. Ohjelma myös kertoo kuinka paljon tilaa pakattu tiedosto vie.
'h' jolloin ohjelma purkaa tai pakkaa tiedoston Huffmanin koodauksella.
'lz' jolloin ohjelma purkaa tai pakkaa tiedoston Lempel-Ziv78-algoritmilla.

Ohjelman on tarkoitus pystyä käsittelemään sekä kansioita että yksittäisiä tiedostoja.

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

# To do

- Linuxilla projekti ei tällä hetkellä käänny, vaan tulee erroreita
- Toteutusdokumentti
- Testausdokumentin päivitys
- Käyttöohjedokumentti
