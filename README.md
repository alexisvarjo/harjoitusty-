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

Jos haluat ajaa projektin Valgrindilla, lisää make-komentoon vipu ```SANITIZE=1```. Tällöin ```make test``` ajaa samalla myös testit Valgrindilla.

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

# Dokumentaatio

[Määrittelydokumentti](dokumentaatio/Määrittelydokumentti.md)

[Testausdokumentti](dokumentaatio/Testausdokumentti.md)

[Toteutusdokumentti](dokumentaatio/Toteutusdokumentti.md)

[Käyttöohje](dokumentaatio/Käyttöohje.md)


# Viikkoraportit

[Viikkoraportti 1](Viikkoraportit/Viikkoraportti1.md)

[Viikkoraportti 2](Viikkoraportit/Viikkoraportti2.md)

[Viikkoraportti 3](Viikkoraportit/Viikkoraportti3.md)

[Viikkoraportti 4](Viikkoraportit/Viikkoraportti4.md)

[Viikkoraportti 5](Viikkoraportit/Viikkoraportti5.md)

[Viikkoraportti 6](Viikkoraportit/Viikkoraportti6.md)
