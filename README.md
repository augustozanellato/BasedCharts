# BasedCharts

BasedCharts è stato sviluppato come progetto per il corso di Programmazione ad Oggetti tenuto presso l'Università degli Studi di Padova nell'Anno Accademico 2021-2022.

È un applicazione *general-purpose* per la creazione di grafici scritta in **C++** usando il **framework Qt**.

Il progetto è rilasciato sotto licenza [GPL (Giant Penis License)](./LICENSE) con l'aggiunta del seguente disclaimer gentilmente ~~rubato~~ preso in prestito da @Maxelweb
> Si consiglia caldamente di NON riutilizzare a fine universitario il codice, in parte o del tutto, visto che potrete andare incontro a sanzioni molto pesanti. Il progetto è stato messo pubblicamente per poter eventualmente trarre ispirazione e comprendere il meccanismo del MV di Qt, non per essere copiato. Procedete a vostro rischio e pericolo, ma qualsiasi richiesta di copiatura NON è a priori autorizzata, e qualora venisse scoperta verrà comunicata a chi di dovere.

## Ambienti testati

|              | Ambiente di sviluppo | Ambiente di test |
|-------------:|----------------------|------------------|
| OS           | Arch Linux (rolling) | Ubuntu 18.04     |
| Compilatore  | Clang 13.0.1         | GCC 7.3.0        |
| Versione Qt  | 5.15.2               | 5.9.5            |
| Build system | cmake+ninja          | Qmake+make       |

## Feedback ricevuto

```plain
Feedback Progetto di Programmazione ad Oggetti
Valutazione del progetto: ottimo/eccellente

Vincoli obbligatori
+ Soddisfatti

Orientamento ad oggetti
+ Incapsulamento
+ Estensibilità ed evolvibilità, polimorfismo
+ Efficienza e robustezza
+ Implementazione di quattro tipi di charts
+ Gerarchia di classi particolarmente sviluppata e curata
- Il progetto presenta accoppiamento fra il modello e la vista. In particolare,
  la classe principale AbstractChart e' una sottoclasse di QWidget. La
  rappresentazione logica dei charts dovrebbe essere indipendente dal framework
  utilizzato per visualizzarli.

Funzionalità
+ Piu' temi disponibili
+ Possibilita' di personalizzare i colori utilizzati
+ La finestra principale supporta piu' grafici aperti contemporaneamente

GUI
+ Visualizza i diversi tipi di dato in maniera opportuna
+ Usabile e intuitiva
+ Robusta, gestisce ridimensionamento
+ Uso adeguato di icone ed elementi grafici e animazioni

Relazione
- Descrizione ridotta dell'utilizzo del polimorfismo
+ Per il resto, adeguata

Suggerimenti non collegati alla valutazione
Nessuno.
```
