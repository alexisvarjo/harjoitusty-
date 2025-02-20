# Toteutusdokumentti

## Ohjelman yleisrakenne
Ohjelman yleisrakenne on sellainen, että main.cpp sisältää käyttäjän ohjaamisen eri polkuja pitkin ohjelman suorituksessa käyttäen funktioita huffman.cpp, lz78.cpp ja utils.cpp -tiedostoista. 

huffman.cpp ja lz78.cpp sisältää kaikki funktiot mitä Huffmanin koodaukseen ja lz78-algoritmiin tarvitaan. Utils.cpp sisältää funktioita esimerkiksi tiedoston lukemista ja kirjoittamista varten, joita käytetään eniten main.cpp:ssä. 

huffman.cpp sisältää Huffmanin puun toteutuksen ja lopulta myös funktiot huffman_encode ja huffman_decode, jotka ovat tärkeimmät main.cpp:stä kutsuttavat funktiot.
lz78.cpp sisältää samalla tyylillä lz78_encode ja lz78_decode -funktiot. 

## Saavutetut aika- ja tilavaativuudet

**Aikavaativuudet**

LZ78-algoritmi pakatessa:

LZ78_compress-funktiossa funktio käy n:n merkin läpi tekstissä. Jokaisella iteraatiolla
- tarkistetaan onko sanakirjassa tiettyä alimerkkijonoa (keskimäärin O(1), pahimmillaan O(n)) ja suoritetaan hajautus m pituiselle merkkijonolle (O(m)). 
- kasvatetaan sanakirjan avaimeen liittyvän indeksin arvoa
- tehdään joitain alimerkkijonoihin liittyviä operaatioita
Tyypillisesti, kun alimerkkijonot eivät kasva suureksi joka kerta, aikavaativuus on O(n). Pahimmillaan aikavaatimus voi olla O(n^2) jos jokainen alijono täytyy lisätä ja hajauttaa sanakirjaan.

LZ78-Algoritmi purkaessa:
- jokaiselle parille (prefixIndex, char) tehdään yksi haku, joka on keskimäärin O(1)
- sen jälkeen decompressed += sequence; jossa sequence on pahimmillaan O(n).
Koska tämä toistetaan pahimmillaan noin n kertaa, niin pahimmillaan aikavaativuus on O(n^2). Keskimäärin aikavaativuus on kuitenkin lähempänä O(n)

Huffmanin koodaus pakatessa:
- iteroidaan koko syötetyn tekstin läpi, aikavaativuus O(n)
- Huffman-puun luonnissa tarvitaan min-heap, jonka aikavaativuus on O(k), jossa k on uniikkien merkkien määrä.
- Kahden pienimmän puun yhdistäminen O(k log k)
- Tyypillisesti k on suhteellisen pieni, eli suurella n, käytännössä O(1).
- Puun läpikäynti koodien luomista varten: O(k)
- Pakkaaminen O(n) sillä pakatessa koodi käy n:n merkin läpi, ja jos merkkien määrä on vakio, niin aikavaativuus on O(n).
- Puun muuttaminen merkkijonoksi on aikavaativuudeltaan O(k)
Kaiken kaikkiaan aikavaativuus on O(n log k) isolla merkistöllä, ja pienemmällä O(n).

Huffmanin koodaus purkaessa:
- O(n) lukiessa pakattua tiedostoa esittävää bittijonoa
- O(M) lukiessa M:n bitin pituista tiedoston sisältöä esittävää bittiä
- O(k) puun luonti, jossa k on merkkien määrä
- O(M) tiedoston purkaminen
Lopullinen aikavaativuus on O(M+k+n) ≈ O(n), jos m ≈ n ja k on pieni.

**Tilavaativuudet**

LZ78-algoritmi pakatessa:
- Pahimmassa tapauksessa sanakirjassa on n jäsentä
- Koska jokainen avain sanakirjassa on alimerkkijono, pahimmassa tapauksessa tilavaativuus voi olla pahimmillaan O(n^2) koska jokainen uusi sanakirjan jäsen saattaa sisältää uuden kopion aikaisemmasta tekstistä
- valmis tulos itsessään on O(n)
Pahimmillaan tilavaativuus on O(n^2).

LZ78-Algoritmi purkaessa:
- Sanakirja sisältää pahimmillaan O(n) merkkijonoa, joissa jokaisen pituus on pahimmillaan O(n), eli sanakirja on pahimmillaan O(n^2)
- lopullinen purettu merkkijono on O(n)
Pahimmillaan tilavaativuus on O(n^2).

Huffmanin koodaus pakatessa:
- Rakentaessa esiintymistiheyksiä O(k) k:lla merkillä
- Huffmanin puu vie tilaa O(k)
- Koodien luominen vie tilaa O(k)
- encode vie tilaa pahimmillaan O(M) jossa M on kaikkien huffman koodien pituuksien summa
- merkkijonoksi muuttaminen vie tilaa O(k)
Kaiken kaikkiaan tilavaativuus on O(n) + O(k) + O(M) ja tyypillisesti k ja m ovat suhteellisen pieniä verrattuna n:ään, joten O(n) + O(k) + O(M) ≈ O(n)

Huffmanin koodaus purkaessa:
- pakatun puun säilöminen O(s)
- bittien erittely O(M) metadatan lukemiseen ja säilömiseen
- Huffmanin puu vie tilaa O(k) jossa k on merkkien määrä
- lopullinen purettu tulos vie O(n) verran tilaa, jossa n on puretun tuloksen merkkien määrä
Lopullinen tilavaativuus on O(M+k+n) ≈ O(n) jos M on lähelle n:ää ja vasenta puolta dominoi n.

## Suorituskykyvertailu




## Työn mahdolliset puutteet ja parannusehdotukset
Ohjelman suorituskykyä voisi parantaa rinnakkaissuorittamisen implementoimisella, joka kyllä vaatisi paljon vaivaa, mutta saattaisi nopeuttaa ohjelmaa merkittävästi. Ohjelma myös vie aika paljon muistia, ja muistin käyttöä voisi optimoida implementoida kaiken 'striimaamisella' eli sillein, että aina kun mahdollista, niin mitään ei missään kohtaa säilytetä kokonaan muistissa.

## Laajojen kielimallien (ChatGPT yms.) käyttö.
Olen käyttänyt laajoja kielimalleja ohjelmantekoprosessissa antamaan palautetta koodista, ja tulkitsemaan kääntäjän antamia virheitä, ja etsiäkseni virheitä. Olen myös generoinut sillä testejä, ja tarkistanut itse, että testien parametrit ovat järkeviä. Sen lisäksi olen kysynyt parannusehdotuksia, ja ehdotuksia siitä mikä olisi fiksu rakenne tiedostojen suhteen. Olen käyttänyt ChatGPT:n malleja 4o, o1, o3-mini ja o3-mini-high


## Viitteet
