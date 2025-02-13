# Pakkaus- ja purkuohjelma kurssille Algoritmit ja Tekoäly Harjoitustyö

Tätä ohjelmaa voi käyttää tiedostojen pakkaamiseen ja purkamiseen tai Huffmanin koodauksen ja Lempel-Ziv78 algoritmin tehojen vertailuun.

# Kääntöohjeet
- tulevaisuudessa lisää tästä

# Käyttöohjeet
Valmista projektia voi käyttää kahdella eri tavalla:

./exec <tiedoston tai kansion nimi> <algoritmi> <käyttötila>

tai 

./exec <tiedoston tai kansion nimi> p

Vipu <algoritmi> valitsee algoritmin, vaihtoehdolla 'lz' se käyttää Lempel-Ziv algoritmia, ja vaihtoehdolla 'h' se käyttää Huffmanin koodausta.

Vipu <käyttötila> tarkoittaa puretaanko pakattu tiedosto, vai pakataanko tiedosto. 'e' pakkaa tiedoston, ja 'd' purkaa pakatun tiedoston.

Toinen käyttötapa suorittaa testin, jossa ohjelma ajaa molemmat algoritmit, ensin pakkaa, sitten purkaa ottaa tästä ajan ja tarkistaa
tulosten oikeellisuuden. Ohjelma myös kertoo kuinka paljon tilaa pakattu tiedosto vie.

