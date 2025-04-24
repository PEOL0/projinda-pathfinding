# Pathfinding

Detta projekt har som syfte att underlätta vägval för orienterare och andra kartanvändare. Användaren får välja 2 punkter på en karta och då kommer vårt program hitta bästa vägen mellan punkterna som undviker höjdkurvor, stup och byggnader, och prioriterar bla. stigar och vägar över tät skog. 

## Funktioner

#### createGrid / freeGrid
- Kommer skapa ett grid / ta bort ett grid

#### setImpassible
- Gör en punkt i gridet till opasserbar och därmed kommer vägen inte genereras genom den

#### findPath
- Hitta kortaste vägen mellan 2 punkter på kartan

#### calculateEstimatedCost
- Räkna ut den heuristiska kostnaden mellan 2 punkter

#### getLowestCostNode
- Välj ut den punkten/noden som har minst heuristisk kostnad

#### processNeighbors
- Kolla vilka grannar som är bäst

#### reconstructPath / freePath
- Skapa väg / ta bort väg

#### getPathLength
- Få längd på path

#### getNode
- Få tillbaka noden vid koordinaterna X och Y

#### isValid
- Kolla om koordinaterna är valid på kartan




## Hur man använder programmet

1. Öppna upp index.html i en browser
2. Välj område att visa
3. TODO NEDÅT
4. Klicka på start och slutpunkt på kartan som visas
5. Klicka på "generera väg"




### Authors: 
- PEOL0
- SofiaTheFruit