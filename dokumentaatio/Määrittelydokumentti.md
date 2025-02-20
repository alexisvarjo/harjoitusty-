# **Määrittelydokumentti**

### **Alexis Varjo**

- **Kieli:** C++

- **Opinto-ohjelma:** TKT

---
## **Projektin toteutus**
Käytän projektissa **C++:aa**, koska haluan oppia kyseistä kieltä enemmän, joten pyrin tekemään kaiken koodauksen tällä hetkellä sillä kielellä.
C++ tarjoaa useita etuja:
- **Valmiit tietorakenteet**
- **Hyvä mahdollisuus bittitason tiedonkäsittelyyn**
- **Se on lähellä laitteistoa oleva kieli**

Muita riittävästi hallitsemiani kieliä on esimerkiksi Python ja C, joilla tehtyjä töitä pystyn
arvioimaan.

---

## **Toteutetut algoritmit:**
- **Huffmanin koodaus**
- **LZ78**

### **Ratkaistava ongelma**
Ratkaistu ongelma on rajallisen tiedostotilan säästäminen.
Ohjelma saa syötteenä tiedoston, ja argumenttina ajanmittauksen tai käytetyn algoritmin.

---
## **Algoritmien analyysi**
### **Huffmanin koodaus**
Huffmanin koodauksessa aikavaativuuden analyysi ei ole merkittävässä osassa O-
analyysinä, koska siinä algoritmissa n olisi tekstin sisältämien merkkien lukumäärä, joka
pysyy suomen- ja englanninkielisissä teksteissä suhteellisen vakiona. Huffmanin
koodaus vie tilaa noin 2 – 320 tavua, jos oletuksena on 8-bittiset aakkoset ja puu
kootaan rekursiivisesti.

### **LZ78**
LZ78-algoritmissa muodostetaan lista tupleja, joka sisältää alimerkkijonon ja missä indeksissä se esiintyy. Mitä enemmän alijonoja tekstissä on, sitä enemmän tilaa tällä pystytään säästämään

---

## **Harjoitustyön ydin**
Harjoitustyöni tärkein osa on pakkausalgoritmit. Tärkeintä on, että pakkaus toimii
häviöttömästi ja sitä on helppo testata ja mitata eri syötteillä. Myös esimerkiksi
muistinhallinta ja yleisesti ohjelman pitää toimia joka ikisellä kerralla. Ytimessä on
LZ77/LZ78, ja Huffmanin koodaus. Toinen yhtä tärkeä osa ohjelmaa on pakatun tiedoston purku.

**Viitteet:**

1: [Efficient way of storing Huffman tree](https://stackoverflow.com/questions/759707/efficient-way-of-storing-huffman-tree)

2: [Huffman coding - Wikipedia](https://en.wikipedia.org/wiki/Huffman_coding)

3: [LZ77 and LZ78 - Wikipedia](https://en.wikipedia.org/wiki/LZ77_and_LZ78)
