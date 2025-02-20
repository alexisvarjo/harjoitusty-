# Testausdokumentti
## Yksikkötestauksen kattavuusraportti.
**Huffman.cpp:**
Kaikilla funktioilla on jokaisella pari eri testiä. HuffmanTree-luokalla on myös jokaisella
metodilla testit. Alussa myös satunnaisilla syötteillä tehtävä testi, jossa testataan, että
syöte on sama kuin pakkauksen ja purkamisen jälkeinen tulos. Myös testit
erityistapauksille pakkauksen ja purkamisen jälkeisen tuloksen vertailussa, kuten yhden
merkin syöte, useampi saman merkin syöte, tyhjä syöte ja pari muuta edustavaa testiä.

**LZ78.cpp:**
Kaikilla funktioilla on ainakin muutama testiä. On myös pari yleistä testiä, joista
ensimmäisessä testissä vertaillaan satunnaisilla syötteillä pakkauksen ja purun jälkeisiä
tuloksia, ja tyhjän sekä yhden merkin syötteiden tapauksilla sama. Kaikissa testeissä pari
perustapausta ja rajatapaukset.

**utils.cpp**
Kaikilla funktioilla on ainakin pari testiä.

**main.cpp**
Ei ole yksikkötestejä, koska mainissa ei oikeastaan ole funktioita testattavana.

### Mitä on testattu, miten tämä tehtiin?
Tein pitkälti testit hyödyntämällä Googlen testauskirjastoa. Kaikki testit on myös ajettu Valgrindilla muistivuotojen varmistamiseksi. 
### Minkälaisilla syötteillä testaus tehtiin?
Yleisesti perustapaukset, ja myös parit rajatapaukset. Laajemmat koko
ohjelman testit satunnaisilla syötteillä.
### Miten testit voidaan toistaa?
Satunnaisten merkkien testejä ei pysty toistamaan täsmälleen samana, mutta muut testit
voidaan toistaa samana ajamalla testiohjelma uudestaan
### Ohjelman toiminnan mahdollisen empiirisen testauksen tulosten esittäminen graafisessa muodossa. (Mikäli sopii aiheeseen)
Ajattelin tehdä tämän jonkinlaiseen diaesitykseen, mihin kokoan kuvaajia erilaisilla syötteillä.
