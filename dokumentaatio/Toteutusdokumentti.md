# Toteutusdokumentti

## Ohjelman yleisrakenne
Ohjelman yleisrakenne on sellainen, että main.cpp sisältää käyttäjän ohjaamisen eri polkuja pitkin ohjelman suorituksessa käyttäen funktioita huffman.cpp, lz78.cpp ja utils.cpp -tiedostoista. 

Huffman.cpp ja lz78.cpp sisältää kaikki funktiot, joita Huffmanin koodaukseen ja lz78-algoritmiin tarvitaan. Utils.cpp sisältää funktioita esimerkiksi tiedoston lukemista ja kirjoittamista varten, mitä käytetään eniten main.cpp:ssä. 

Huffman.cpp sisältää Huffmanin puun toteutuksen ja lopulta myös funktiot 'huffman_encode' ja 'huffman_decode, jotka ovat tärkeimmät main.cpp:stä kutsuttavat funktiot.
lz78.cpp sisältää samalla tyylillä lz78_encode ja lz78_decode -funktiot. 

## Saavutetut aika- ja tilavaativuudet

**Aikavaativuudet**

LZ78-algoritmi pakatessa:

LZ78_compress-funktiossa funktio käy n:n merkin läpi tekstissä. Jokaisella iteraatiolla
- tarkistetaan onko sanakirjassa tiettyä alimerkkijonoa (keskimäärin O(1), pahimmillaan O(n)) ja suoritetaan hajautus m pituiselle merkkijonolle (O(m)),
- kasvatetaan sanakirjan avaimeen liittyvän indeksin arvoa,
- ja tehdään joitain alimerkkijonoihin liittyviä operaatioita.
Tyypillisesti, kun alimerkkijonot eivät kasva suureksi joka kerta, aikavaativuus on O(n). Pahimmillaan aikavaatimus voi olla O(n^2), jos jokainen alijono täytyy lisätä ja hajauttaa sanakirjaan.

LZ78-Algoritmi purkaessa:
- Jokaiselle parille (prefixIndex, char) tehdään yksi haku, joka on keskimäärin O(1).
- Sen jälkeen suoritetaan decompressed += sequence; jossa sequence on pahimmillaan O(n).
Koska tämä toistetaan pahimmillaan noin n kertaa, pahimmillaan aikavaativuus on O(n^2). Keskimäärin aikavaativuus on kuitenkin lähempänä O(n).

Huffmanin koodaus pakatessa:
- Iteroidaan koko syötetyn tekstin läpi, aikavaativuus O(n).
- Huffman-puun luonnissa tarvitaan min-heap, jonka aikavaativuus on O(k), jossa k on uniikkien merkkien määrä.
- Kahden pienimmän puun yhdistäminen O(k log k).
- Tyypillisesti k on suhteellisen pieni, eli suurella n, käytännössä O(1).
- Puun läpikäynti koodien luomista varten on O(k).
- Pakkaaminen O(n) sillä pakatessa koodi käy n:n merkin läpi, ja jos merkkien määrä on vakio, niin aikavaativuus on O(n).
- Puun muuttaminen merkkijonoksi on aikavaativuudeltaan O(k).
Kaiken kaikkiaan aikavaativuus on O(n log k) isolla merkistöllä, ja pienemmällä O(n).

Huffmanin koodaus purkaessa:
- O(n) lukiessa pakattua tiedostoa esittävää bittijonoa,
- O(M) lukiessa M:n bitin pituista tiedoston sisältöä esittävää bittiä,
- O(k) puun luonti, jossa k on merkkien määrä ja 
- O(M) tiedoston purkaminen.
Lopullinen aikavaativuus on O(M+k+n) ≈ O(n), jos m ≈ n ja k on pieni.

**Tilavaativuudet**

LZ78-algoritmi pakatessa:
- Pahimmassa tapauksessa sanakirjassa on n jäsentä.
- Koska jokainen avain sanakirjassa on alimerkkijono, pahimmassa tapauksessa tilavaativuus voi olla O(n^2), jos jokainen uusi sanakirjan jäsen saattaa sisältää uuden kopion aikaisemmasta tekstistä.
- Valmis tulos itsessään on O(n).
Pahimmillaan tilavaativuus on O(n^2).

LZ78-Algoritmi purkaessa:
- Sanakirja sisältää pahimmillaan O(n) merkkijonoa, joissa jokaisen pituus on pahimmillaan O(n), eli sanakirja on pahimmillaan O(n^2).
- Lopullinen purettu merkkijono on O(n).
Pahimmillaan tilavaativuus on O(n^2).

Huffmanin koodaus pakatessa:
- Rakentaessa esiintymistiheydet vie O(k) k:lla merkillä.
- Huffmanin puu vie tilaa O(k).
- Koodien luominen vie tilaa O(k).
- Pakkaus vie tilaa pahimmillaan O(M) jossa M on kaikkien huffman koodien pituuksien summa.
- Merkkijonoksi muuttaminen vie tilaa O(k).
Kaiken kaikkiaan tilavaativuus on O(n) + O(k) + O(M) ja tyypillisesti k ja m ovat suhteellisen pieniä verrattuna n:ään, joten O(n) + O(k) + O(M) ≈ O(n).

Huffmanin koodaus purkaessa:
- Pakatun puun säilöminen O(s),
- bittien erittely O(M) metadatan lukemiseen ja säilömiseen,
- Huffmanin puu vie tilaa O(k) jossa k on merkkien määrä,
- ja lopullinen purettu tulos vie O(n) verran tilaa, jossa n on puretun tuloksen merkkien määrä.
Lopullinen tilavaativuus on O(M+k+n) ≈ O(n) jos M on lähelle n:ää ja vasenta puolta dominoi n.

## Suorituskykyvertailu

| File | Huffman pakkaus | LZ78 pakkaus | Huffman purku | LZ78 purku | Huffman pakkaussuhde | LZ pakkaussuhde |
|------|-----------------|--------------|---------------|------------|----------------------|-----------------|
|world192.txt | 447 ms | 653 ms | 192 ms | 136 ms | 0.63 | 0.63 |
| E_coli.txt | 697 ms | 1262 ms | 168 ms | 230 ms | 0.28 | 0.53 |
| test2.txt | 0.02 ms | 0.02 ms | 0.002 ms | 0.009 ms | 1.33 | 1.94|
| pi.txt | 163 ms | 285 ms | 53 ms | 83 ms | 0.42 | 0.92 |
| d&d.txt | 45 ms | 72 ms | 22 ms | 20 ms | 0.65 | 0.93 |
| linis.txt | 139 ms | 203 ms | 64 ms | 49 ms | 0.65 | 0.72 |
| bible.txt | 695 ms | 1053 ms | 272 ms | 224 ms | 0.55 | 0.61 |
| asyoulik.txt | 22 ms | 36 ms | 9.9 ms | 10 ms | 0.61 | 1.02 |
| enwik6.txt | 6.33 s | 12.14 s | 2.33 s | 2.30 s | 0.64 | 0.62 |
| lcet10.txt | 78 ms | 119 ms | 33 ms | 30 ms | 0.59 | 0.84 |
| enwik5.txt | 2.13 s | 3.91 s | 765 ms | 958 ms | 0.64 | 0.68 |
| plrabn12.txt | 95 ms | 145 ms | 35 ms | 36 ms | 0.57 | 0.88 |
| C-ohjelmointi.txt | 2.3 ms | 4 ms | 0.96 ms | 1.4 ms | 0.68 | 1.54 |
| alice29.txt | 28 ms | 43 ms | 11 ms | 12 ms | 0.58 | 0.96 |
| japani.txt | 14 ms | 23 ms | 5.9 ms | 7.5 ms | 0.62 | 1.13|
| test.txt | 20 ms | 22 ms | 7.3 ms | 1.7 ms | 0.51 | 0.16 |


## Työn mahdolliset puutteet ja parannusehdotukset
Ohjelman suorituskykyä voisi parantaa rinnakkaissuorittamisen implementoimisella, joka kyllä vaatisi paljon vaivaa, mutta saattaisi nopeuttaa ohjelmaa merkittävästi. Ohjelma myös vie aika paljon muistia, ja muistin käyttöä voisi optimoida implementoida kaiken 'striimaamisella' eli sillein, että aina kun mahdollista, niin mitään ei missään kohtaa säilytetä kokonaan muistissa.

## Laajojen kielimallien (ChatGPT yms.) käyttö.
Olen käyttänyt laajoja kielimalleja ohjelmantekoprosessissa antamaan palautetta koodista, ja tulkitsemaan kääntäjän antamia virheitä, ja etsiäkseni virheitä. Olen myös generoinut sillä testejä, ja tarkistanut itse, että testien parametrit ovat järkeviä. Sen lisäksi olen kysynyt parannusehdotuksia, ja ehdotuksia siitä mikä olisi fiksu rakenne tiedostojen suhteen. Olen käyttänyt ChatGPT:n malleja 4o, o1, o3-mini ja o3-mini-high

## Viitteet
