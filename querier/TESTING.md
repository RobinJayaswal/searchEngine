## **TESTING for querier subsystem**
## Robin Jayaswal, Kyle Dotterrer, May 2016

### **Argument Testing**

#### Automated Argument Testing

```querierArgTest.sh```

```
[kad@flume ~/cs50/labs/tse/querier]$ ./querierArgTest.sh ../data/crawl1 ../data/index1
running 'querier ' with no args.

output:
Error: Incorrect number of arguments
Usage: ./querier pageDirectory indexFilename

PASSED: exited with error

running 'querier ../data/crawl1', one too few args.

output:
Error: Incorrect number of arguments
Usage: ./querier pageDirectory indexFilename

PASSED: exited with error

running 'querier ../data/crawl1 extraArg'

output:
Error: Incorrect number of arguments
Usage: ./querier pageDirectory indexFilename

PASSED: exited with error

running 'querier this/is/not/a/pageDir ', bad pageDirectory

output:
Error: this/is/not/a/pageDir is not a proper crawler directory
See querier.c README for crawler directory reqs

PASSED: exited with error

running 'querier ../data/crawl1 filename8That8Does8Exist', bad indexFilename

output:
Error: filename8That8Does8Exist is not a readable file
See querier.c README.md for index filename reqs

PASSED: exited with error

All Tests PASSED
```

### **Functionality Testing**

#### Automated Query Syntax Testing

```queryTest.sh```

```
[kad@flume ~/cs50/labs/tse/querier]$ ./queryTest.sh
query: (empty)
No documents match.
----------------------------------------
Error: bad char '!' in query
Error: bad char '?' in query
Error: bad char '5' in query
query: and 
Error: 'and' cannot be first
----------------------------------------
query: or 
Error: 'or' cannot be first
----------------------------------------
query: and computer 
Error: 'and' cannot be first
----------------------------------------
query: computer and 
Error: 'and' cannot be last
----------------------------------------
query: or computer 
Error: 'or' cannot be first
----------------------------------------
query: computer or 
Error: 'or' cannot be last
----------------------------------------
query: computer and or science 
Error: 'and' and 'or' cannot be adjacent
----------------------------------------
query: computer or and science 
Error: 'or' and 'and' cannot be adjacent
----------------------------------------
query: computer and and science 
Error: 'and' and 'and' cannot be adjacent
----------------------------------------
query: computer or or science 
Error: 'or' and 'or' cannot be adjacent
----------------------------------------

error status at exit is 0
```

#### Manual General Functionality Testing

dartmouth = 147
computer = 189
```
[kad@flume ~/cs50/labs/tse/querier]$ querier ../data/crawl1 ../data/index1
computer and dartmouth
query: computer and dartmouth 
Matches 5 documents (ranked):
score: 147 doc 7: http://old-www.cs.dartmouth.edu/~dfk/papers/index.html
score:   5 doc 4: http://old-www.cs.dartmouth.edu/~dfk/postdoc.html
score:   3 doc 1: http://old-www.cs.dartmouth.edu/~dfk/
score:   3 doc 6: http://old-www.cs.dartmouth.edu/~dfk/people.html
score:   1 doc 9: http://old-www.cs.dartmouth.edu/
----------------------------------------
```

```
kotz
query: kotz 
Matches 9 documents (ranked):
score: 306 doc  7: http://old-www.cs.dartmouth.edu/~dfk/papers/index.html
score:   5 doc  1: http://old-www.cs.dartmouth.edu/~dfk/
score:   3 doc  6: http://old-www.cs.dartmouth.edu/~dfk/people.html
score:   3 doc  5: http://old-www.cs.dartmouth.edu/~dfk/teaching.html
score:   2 doc  8: http://old-www.cs.dartmouth.edu/~dfk/papers/wordle.html
score:   2 doc  4: http://old-www.cs.dartmouth.edu/~dfk/postdoc.html
score:   2 doc  3: http://old-www.cs.dartmouth.edu/~dfk/kids.html
score:   1 doc 10: http://old-www.cs.dartmouth.edu/~dfk/ChampionInternationalProfessor.html
score:   1 doc  2: http://old-www.cs.dartmouth.edu/~dfk/seatpack/
----------------------------------------
[kad@flume ~/cs50/labs/tse/querier]$ querier ../data/crawl1 ../data/index1
computer and dartmouth or kotz
query: computer and dartmouth or kotz 
Matches 10 documents (ranked):
score: 453 doc  7: http://old-www.cs.dartmouth.edu/~dfk/papers/index.html
score:   8 doc  1: http://old-www.cs.dartmouth.edu/~dfk/
score:   7 doc  4: http://old-www.cs.dartmouth.edu/~dfk/postdoc.html
score:   6 doc  6: http://old-www.cs.dartmouth.edu/~dfk/people.html
score:   3 doc  5: http://old-www.cs.dartmouth.edu/~dfk/teaching.html
score:   2 doc  8: http://old-www.cs.dartmouth.edu/~dfk/papers/wordle.html
score:   2 doc  3: http://old-www.cs.dartmouth.edu/~dfk/kids.html
score:   1 doc  9: http://old-www.cs.dartmouth.edu/
score:   1 doc 10: http://old-www.cs.dartmouth.edu/~dfk/ChampionInternationalProfessor.html
score:   1 doc  2: http://old-www.cs.dartmouth.edu/~dfk/seatpack/
----------------------------------------
```

#### Automated General Functionality Testing

```querierFuzzTest.sh```

```
[kad@flume ~/cs50/labs/tse/querier]$ ./querierFuzzTest.sh ../data/crawl1 ../data/index1
generating 100 queries

query: (empty)
No documents match.
----------------------------------------
Error: bad char '-' in query
query: (empty)
No documents match.
----------------------------------------
Error: bad char '-' in query
query: guls 
No documents match.
----------------------------------------
query: and sxs 
Error: 'and' cannot be first
----------------------------------------
query: ufc justen and bardel profs and fieldon deicoon and engrafts and and and counterexpostulation 
Error: 'and' and 'and' cannot be adjacent
----------------------------------------
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
query: and pouted pious uptakes and infuscate cynoscephalae primrosed improvisors bakingly lawcourt 
Error: 'and' cannot be first
----------------------------------------
Error: bad char '-' in query
query: (empty)
No documents match.
----------------------------------------
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
query: and and turcize jesup spiritually lokiec alumna sparklike and maryn binaries and dustcover triarchies amula hypocarpium 
Error: 'and' cannot be first
----------------------------------------
query: saccopharyngidae convincement teleseism epitheme mcbain assisa pirozzo and and laxism jocularly and dobie and stittville locale and persicaria alamannian 
Error: 'and' and 'and' cannot be adjacent
----------------------------------------
query: and cheltenham doleritic 
Error: 'and' cannot be first
----------------------------------------
Error: bad char '-' in query
query: sullenness demagogueries and derecho and and kokand and isoclinally and vomits and 
Error: 'and' cannot be last
----------------------------------------
Error: bad char '-' in query
Error: bad char ''' in query
query: transvaluing dilatement and angularization uncoincidental stretti and unoffender and and muses and and hypopharyngeal presaged whitings and extroversively plovery and and 
Error: 'and' cannot be last
----------------------------------------
query: unware mollusklike nonrecuperatiness and rhymester drobman cpm contortion and cardiagram squared intrarelation fren ungraven 
No documents match.
----------------------------------------
query: amortissement and santonic capnodium tiepin rogerian thinnest and and diopters acridness duikerbuck tetarcone colpoperineorrhaphy counterdemonstrator kinoos 
Error: 'and' and 'and' cannot be adjacent
----------------------------------------
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
query: huddlement and brand ubbenite annexion and infarctions and neuropsychiatric periodogram and unaxiomatic eagar jaculated and quos 
No documents match.
----------------------------------------
Error: bad char '-' in query
query: antiburglar syndicate 
No documents match.
----------------------------------------
query: prado and and and jalopy and and and isotherm and pubofemoral undust 
Error: 'and' and 'and' cannot be adjacent
----------------------------------------
Error: bad char '-' in query
query: sejant aiwain obedientiar and and basiotripsy 
Error: 'and' and 'and' cannot be adjacent
----------------------------------------
Error: bad char '-' in query
query: whiteheart deutoplastic ornithic 
No documents match.
----------------------------------------
query: and unsoluble ammonolyses zodiophilous and pantomania and and wallasey 
Error: 'and' cannot be first
----------------------------------------
query: phi 
Matches 1 documents (ranked):
score: 1 doc 1: http://old-www.cs.dartmouth.edu/~dfk/
----------------------------------------
Error: bad char '-' in query
Error: bad char '-' in query
query: sulphion ostariophysous and 
Error: 'and' cannot be last
----------------------------------------
query: (empty)
No documents match.
----------------------------------------
Error: bad char '-' in query
Error: bad char '-' in query
query: celiosalpingotomy and inosin 
No documents match.
----------------------------------------
query: chowed and and airers exsurgent burner mallowwort bowfront tollhouse 
Error: 'and' and 'and' cannot be adjacent
----------------------------------------
query: and and dodecane 
Error: 'and' cannot be first
----------------------------------------
Error: bad char '-' in query
query: chicomecoatl hypotremata 
No documents match.
----------------------------------------
Error: bad char '-' in query
query: guebre denize nourishable recapturing subcavate variations and ostealgia fris and bribeworthy and and eurasian and restabilizing 
Error: 'and' and 'and' cannot be adjacent
----------------------------------------
query: and gruffy and falkner reorientated and pennated immechanical laureen deliveries prodigy and overripeness and prosecuting rhodic padeye and and contemner and 
Error: 'and' cannot be first
----------------------------------------
Error: bad char '-' in query
query: and 
Error: 'and' cannot be first
----------------------------------------
query: and olfactible jestwise myriologue 
Error: 'and' cannot be first
----------------------------------------
query: caseinate intemperances and hoicking 
No documents match.
----------------------------------------
query: bletia dayfly abrupter and stabiliment and and and boychick willowstreet netmen kyklops tipcats interrepulsion ferrigno and histon adumbrellar snobonomer 
Error: 'and' and 'and' cannot be adjacent
----------------------------------------
query: and arteries and pacchionian cece dentinoma watchout manna lomein and glagolitsa haymish resigning ingravidate unpolitic hardboiled and 
Error: 'and' cannot be first
----------------------------------------
Error: bad char '-' in query
query: pilotfishes balebos and and and glyceridic and reviewing 
Error: 'and' and 'and' cannot be adjacent
----------------------------------------
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
query: (empty)
No documents match.
----------------------------------------
query: hydrocarbonous dissonate recollet sr and and praam and and majolicas and and and wordages and antietam and tampioned whistler corporales 
Error: 'and' and 'and' cannot be adjacent
----------------------------------------
query: ascendence and and whitebelly stercoreous unquiet sativa and pewdom and luger formably and and myrmidons and housekept skippy russism aggregata ultraisms 
Error: 'and' and 'and' cannot be adjacent
----------------------------------------
query: and gerger and and unquote masgat 
Error: 'and' cannot be first
----------------------------------------
query: and palinuridae and dextrocardial and and and floorage 
Error: 'and' cannot be first
----------------------------------------
query: rheums evocating prevaccinated joss hornstein and millennially arar unspeakable hydriodide and subjectivoidealistic and hooklets and and 
Error: 'and' cannot be last
----------------------------------------
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
query: cockishly kanarese lyman webfeet dynasts locally chromatopathic theatricalisation 
No documents match.
----------------------------------------
query: and 
Error: 'and' cannot be first
----------------------------------------
query: linearizes joeann physicophysiological centerville quelme mnemonic and ensafe oleraceae novelwright and ridgingly foxiness and earth halimot and paludinal dissceptered daberath benthos 
No documents match.
----------------------------------------
Error: bad char '-' in query
query: and unfatherly and 
Error: 'and' cannot be first
----------------------------------------
query: and unglad polypose pithecanthropoid 
Error: 'and' cannot be first
----------------------------------------
query: and lecherously ignazio hibernicism noninferable and wastebin and emulators and 
Error: 'and' cannot be first
----------------------------------------
query: and squarewise gross and and laserjet salaciousness disturbers 
Error: 'and' cannot be first
----------------------------------------
query: (empty)
No documents match.
----------------------------------------
Error: bad char '-' in query
query: cissy and epulo peba and superselection 
No documents match.
----------------------------------------
query: snivel and spirity drukpa gertrude haplophyte and 
Error: 'and' cannot be last
----------------------------------------
query: and and protrusility preaudit rapiers kermises meticais and and mined automatograph 
Error: 'and' cannot be first
----------------------------------------
Error: bad char '-' in query
Error: bad char '-' in query
Error: bad char '-' in query
query: wasteness and cushiest 
No documents match.
----------------------------------------
Error: bad char '-' in query
query: outbarking emery antipoles deactivate affrica 
No documents match.
----------------------------------------
Queries were successful
```

### **Memory Testing**

#### Manual Memory Allocation/Deallocation Testing

```
[kad@flume ~/cs50/labs/tse/querier]$ querier ../data/crawl1 ../data/index1
computer            
query: computer 
Matches 6 documents (ranked):
score: 187 doc 7: http://old-www.cs.dartmouth.edu/~dfk/papers/index.html
score:  14 doc 5: http://old-www.cs.dartmouth.edu/~dfk/teaching.html
score:   5 doc 4: http://old-www.cs.dartmouth.edu/~dfk/postdoc.html
score:   3 doc 6: http://old-www.cs.dartmouth.edu/~dfk/people.html
score:   3 doc 1: http://old-www.cs.dartmouth.edu/~dfk/
score:   1 doc 9: http://old-www.cs.dartmouth.edu/
----------------------------------------
computer and science 
query: computer and science 
Matches 6 documents (ranked):
score: 134 doc 7: http://old-www.cs.dartmouth.edu/~dfk/papers/index.html
score:   4 doc 5: http://old-www.cs.dartmouth.edu/~dfk/teaching.html
score:   3 doc 4: http://old-www.cs.dartmouth.edu/~dfk/postdoc.html
score:   3 doc 6: http://old-www.cs.dartmouth.edu/~dfk/people.html
score:   2 doc 1: http://old-www.cs.dartmouth.edu/~dfk/
score:   1 doc 9: http://old-www.cs.dartmouth.edu/
----------------------------------------
computer and science or dartmouth and kotz
query: computer and science or dartmouth and kotz 
Matches 7 documents (ranked):
score: 281 doc  7: http://old-www.cs.dartmouth.edu/~dfk/papers/index.html
score:   6 doc  1: http://old-www.cs.dartmouth.edu/~dfk/
score:   6 doc  6: http://old-www.cs.dartmouth.edu/~dfk/people.html
score:   5 doc  4: http://old-www.cs.dartmouth.edu/~dfk/postdoc.html
score:   4 doc  5: http://old-www.cs.dartmouth.edu/~dfk/teaching.html
score:   1 doc  9: http://old-www.cs.dartmouth.edu/
score:   1 doc 10: http://old-www.cs.dartmouth.edu/~dfk/ChampionInternationalProfessor.html
----------------------------------------

query: (empty)
No documents match.
----------------------------------------
Memory Allocation Report: 71 malloc, 71 free, 0 free(NULL), 0 net
```

#### Automated Memory Allocation/Deallocation Testing

```valgrind --leak-check=full --show-leak-kinds=all```

```
[kad@flume ~/cs50/labs/tse/querier]$ valgrind querier ../data/crawl1 ../data/index1
==9879== Memcheck, a memory error detector
==9879== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==9879== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==9879== Command: querier ../data/crawl1 ../data/index1
==9879== 
computer
query: computer 
Matches 6 documents (ranked):
score: 187 doc 7: http://old-www.cs.dartmouth.edu/~dfk/papers/index.html
score:  14 doc 5: http://old-www.cs.dartmouth.edu/~dfk/teaching.html
score:   5 doc 4: http://old-www.cs.dartmouth.edu/~dfk/postdoc.html
score:   3 doc 6: http://old-www.cs.dartmouth.edu/~dfk/people.html
score:   3 doc 1: http://old-www.cs.dartmouth.edu/~dfk/
score:   1 doc 9: http://old-www.cs.dartmouth.edu/
----------------------------------------
computer and science
query: computer and science 
Matches 6 documents (ranked):
score: 134 doc 7: http://old-www.cs.dartmouth.edu/~dfk/papers/index.html
score:   4 doc 5: http://old-www.cs.dartmouth.edu/~dfk/teaching.html
score:   3 doc 4: http://old-www.cs.dartmouth.edu/~dfk/postdoc.html
score:   3 doc 6: http://old-www.cs.dartmouth.edu/~dfk/people.html
score:   2 doc 1: http://old-www.cs.dartmouth.edu/~dfk/
score:   1 doc 9: http://old-www.cs.dartmouth.edu/
----------------------------------------
computer and science or dartmouth and kotz
query: computer and science or dartmouth and kotz 
Matches 7 documents (ranked):
score: 281 doc  7: http://old-www.cs.dartmouth.edu/~dfk/papers/index.html
score:   6 doc  1: http://old-www.cs.dartmouth.edu/~dfk/
score:   6 doc  6: http://old-www.cs.dartmouth.edu/~dfk/people.html
score:   5 doc  4: http://old-www.cs.dartmouth.edu/~dfk/postdoc.html
score:   4 doc  5: http://old-www.cs.dartmouth.edu/~dfk/teaching.html
score:   1 doc  9: http://old-www.cs.dartmouth.edu/
score:   1 doc 10: http://old-www.cs.dartmouth.edu/~dfk/ChampionInternationalProfessor.html
----------------------------------------

query: (empty)
No documents match.
----------------------------------------
==9879== 
==9879== HEAP SUMMARY:
==9879==     in use at exit: 0 bytes in 0 blocks
==9879==   total heap usage: 12,862 allocs, 12,862 frees, 241,897 bytes allocated
==9879== 
==9879== All heap blocks were freed -- no leaks are possible
==9879== 
==9879== For counts of detected and suppressed errors, rerun with: -v
==9879== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```





