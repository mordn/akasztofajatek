# Kétszemélyes akasztófa játék

## A feladat rövid leírása

A feladat megvalósít egy szerver és egy kliens programot C programozási nyelven. A szerver és a kliens közötti kommunikáció socketekkel történik. A program lehetővé teszi, hogy két ember a kliens program egy-egy példányát futtatva akasztófázzon egymással a szerveren keresztül. A feladat a [Debreceni Egyetem](https://www.inf.unideb.hu/) Hálózati Architektúrák és Protokollok kurzusrára készült.

## Játékszabályok

Az akasztófa játékszabályait illetően a klasszikus (hétköznapi) szabályrendszert kell alkalmazni, mely [ezen a linken](http://jatek.gyujtemeny.com/jatekszabaly/732.php) online elérhető.

## A programok működése

Indítás után a szerver két játékos jelentkezésére vár. Az első játékos bejelentkezéskor üzenetet kap, miszerint a szerver a másik játékosra várakozik. A második játékos bejelentkezése után a játék automatikusan elindul. Az elsőnek jelentkező játékos kezd, meg kell adnia a kitalálandó szót. A szerver a megadott szó betűinek darabszámával megegyező számú aláhúzásjelet továbbít a kliensek felé. Ezután a második játékos elkezdi a tippelést, vagyis megad egy betűt, mely szerinte szerepel a szóban. Ekkor a szerver feladata, hogy illessze a betűt az első játékos által megadott szóra, és annak eredményét továbbítsa mindkét kliens felé. A szerver nyilvántartja a játék mindenkori állását, és érzékeli a végállás létrejöttét (nyertes szituáció), melyról mindkét játékost tájékoztatja. Ha az akasztófa felépül (az egyszerűség kedvéért előre definiált számú '|' karakter), akkor a második játékos (a kitaláló) vesztett. Ha viszont úgy érzi, hogy tudjha a megoldást, akkor a megfejtés bármikor bemondható: ha kitalálja, megúszta a "felakasztást", ha nem, elvesztette a játékot.
