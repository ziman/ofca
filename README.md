OFCA Free Command Analyzer
==========================

_v1.0, (c) ZimaN & Tomaj 2k5_

Toto je slobodny softver sireny pod licenciou GNU GPL.
Pre blizsie informacie citaj subor license.txt.

*NOTE: This is historical code, please do not draw any conclusions from it.* :)

Co a vobec - preco?
-------------------

OFCA je programovaci jazyk, ktoreho idea vznikla
jedneho pekneho vecera na IRCI (detaily sa dotiahli
nasledujuci den na matike) a implementacia (z vacsej casti)
nasledujuci vecer - 15.4.2005. Vychadza z nemenej popularneho
vychodoslovenskeho programovacieho jazyka Saral (Saris 
algorithmic language).

Podstatou OFCE je extremne human-like syntax. Co to
vlastne znamena? Na rozdiel od inych programovacich jazykov,
v ktorych sa vyskytuju same specialne znaky a operatory, OFCA
taketo znaky vobec nepozna (okrem uvodzovky). Vsetky elementy
syntaxe a jazyka su vyjadrene slovami, ktore su navyse

1. case insensitive - nezalezi na velkosti pismen
2. grammar insensitive - nezalezi na i/y,

cim sa dosahuje velka developer-priatelskost. Program v OFCI
sa nenazyva "kod" (co je v ostatnych jazykoch uplne opodstatnene),
ale vzhladom na syntax OFCE "sloh". V OFCI su takisto dalsie
vylepsenia, o ktorych sa zmienime neskor.

OFCA je programovaci jazyk tak multiplatformovy, aky
sa nam chcelo nakodit (i.e. vzhladom na prenosny kod v C++
zrejme celkom slusne multiplatformovy), taky rychly, ako sa
nam chcelo optimalizovat (to sa nam velmi nechcelo) a taky
spolahlivy, ako sa nam chcelo pisat kontroly chyb (to je celkom
na urovni).

Co OFCA je a co NIE je
----------------------

OFCA je taky mily ulet, programatorsky vtip. Samozrejme,
ze je to interpretovany jazyk, v ktorom sa daju robit aj celkom
zaujimave slohy. (Planuje sa Dijkstrov algoritmus).

OFCA NIE je ziadny kvalitny programovaci jazyk (ano,
nemame problemy uznat si to). V pripade OFCE ide o recesiu,
a teda kod asi nebude dokonaly, snazili sme sa ho vsak udrzat
aspon na takej urovni, aby nesegfaultoval, ale v pripade chyby
inteligentne vypisal dokonca aj jej pricinu a slusne skoncil.

Od OFCE necakaj, ze ti bude pomahat s vyvojom slohov
v OFCI. Vzhladom na jej syntax moze ktorekolvek slovo znamenat
cokolvek a v pripade chyby v kode sa error obycajne zahlasi az
na konci programu, ked posledne alebo predposledne slovo bude
povazovane za premennu, ktorej uz nebude co priradit.
	
Opis syntaxe
------------

### Syntax OFCE je extremne benevolentna.

Pozostava zo slov oddelenych oddelovacmi. Oddelovac moze byt novy
riadok, medzera alebo tabulator (proste whitespace). Na slova sa
nekladu absolutne ziadne podmienky (mozu obsahovat akekolvek znaky),
samozrejme okrem whitespace znakov.

OFCA rozlisuje dva typy slov:

1. Retazce - slova uvedene v uvodzovkach:
  - napr. "Abeceda zjedla deda.\n"
  - znaky uvedene backslashom budu interpretovane C-ckovskym stylom (\n, \r, \t, \\)
		  
2. Operatory - vsetko ostatne (NIE je to operator v beznom slova zmysle!)
  - napr. konec
	
Pri operatoroch je VELMI dolezite spomenut viacero veci:

- Z operatorov sa automaticky odstranuju znaky: ,.;:!
  To vedie k este viac human-like syntaxi
- Pri operatoroch sa nerozlisuju velke/male pismena, ani i/y!
- Nasledujuce operatory sa v texte priamo mazu bez nahrady (akoby tam ani neboli): toten, ten, toto, to, tota, ta, :), asik

Tento pristup umoznuje obohatit sloh v OFCI o prirodzenejsie prvky jazyka.

Napr:
```
	Napis "Vloz cislo: ", popytaj toto cislo a napis toto cislo!
```
je to iste ako
```
	napis "Vloz cislo: "
	popytaj cislo
	napis cislo
```

### Vyznam operatorov zavisi od ich pozicie v slohu

Lubovolny operator moze mat rozne vyznamy, ktore zavisia
od toho, kde sa nachadza. V pripade, ze operator nie je zaujimavym
slovom, povazuje sa za premennu. Operator mozeme explicitne urcit
ako premennu, ked pred nu napiseme operator "menak".

Priklad:
```
	menak ak je 10
	napis ak
```

vypise premennu s nazvom ak. Ak by sme neuviedli operator "menak",
operator "ak" by bol povazovany za zaujimave slovo, ktore znamena
vetevenie programu na zaklade podmienky. V druhom riadku operator
"ak" nemoze znamenat nic ine ako premennu, a teda sa pred nu nesmie
pisat operator "menak".

Vzhladom na to sa zavadza operator vyraz (viraz), ktory
nam hovori, ze nasledujuce tri operatory tvoria vyraz, ktory pozostava
z laveho operandu, operatora a praveho operandu. Oba operandy vsak
mozu byt tiez vyrazmi, a teda sa vyrazy rekurzivne vyhodnocuju.

```
	vyraz 1 plus 10			= 11
	vyraz a plus b			= a+b
	vyraz a krat vyraz a plus b	= a*(a+b)
```

V poslednom pripade je pravym operandom dalsi vyraz. Tieto
vyrazy sa daju pouzit v priradeni (a je vyraz a plus b), v podmienke
(ak vyraz a je b), ci kdekolvek inde, kde je potrebna nejaka ciselna
hodnota.
	
### Co pozname z inych jazykov a ako sa to povie v OFCI

C/C++		|	Pascal		|	OFCA
----------------|-----------------------|-------------
x = 5		|	x:=5		|	x je 5 ALEBO x bude 5
const x = 5	|	const x = 5	|	x bude furt 5
x = y+z		|	x:=y+z		|	x je vyraz y plus z
printf("bla\n")	|	writeln("bla)	|	napis "bla\n"
printf("\n")	|	writeln;	|	enter
x = p*(q+r)	|	x:=p*(q+r)	|	x je vyraz p krat vyraz q plus r
gets(x)		|	readln(x)	|	popytaj x
break		|	break		|	ubzikni
continue	|	continue	|	nazad
(...)		|			|

Bohuzial, som lenivy ako vos, a tak sa mi nechce pisat viacej. Pozri si prikladove programy, tam to pochopis.

- procedura sa uvadza:
```
	coska meno_procedury
		-prikaz-
		-prikaz-
	konec cehoska
```

- volanie procedury
```
	zrob meno_procedury
```

- procedura dostane kopie vsetkych premennych. akekolvek zmeny sa po navrate
  z procedury stratia, okrem premennej "vysledok" (ta sa zachova).
  vo "vysledku" je teda ulozena navratova hodnota. vstupne parametre procedury
  si vsak musis poriesit sam.
		

Zoznam zaujimavych slov
-----------------------

Plany do buducnosti (alias TODO)
--------------------------------

- KOfca (ikona Kofoly)
- port pod Dev-Cpp
- jazyk je netypovy, pribudnu typy
- funkcie
- polia
- objekty & struktury (?)
- praca so subormi	
