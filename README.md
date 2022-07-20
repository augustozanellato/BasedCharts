# BasedCharts

BasedCharts è stato sviluppato come progetto per il corso di Programmazione ad Oggetti tenuto presso l'Università degli Studi di Padova nell'Anno Accademico 2021-2022.

È un applicazione *general-purpose* per la creazione di grafici scritta in **C++** usando il **framework Qt**.

Il progetto è rilasciato sotto licenza [GPL (Giant Penis License)](./LICENSE) con l'aggiunta del seguente disclaimer gentilmente ~~rubato~~ preso in prestito da [@Maxelweb](https://github.com/Maxelweb)
> Si consiglia caldamente di NON riutilizzare a fine universitario il codice, in parte o del tutto, visto che potrete andare incontro a sanzioni molto pesanti. Il progetto è stato messo pubblicamente per poter eventualmente trarre ispirazione e comprendere il meccanismo del MV di Qt, non per essere copiato. Procedete a vostro rischio e pericolo, ma qualsiasi richiesta di copiatura NON è a priori autorizzata, e qualora venisse scoperta verrà comunicata a chi di dovere.

Se vedere questo progetto vi ha semplificato la vita nello sviluppo del vostro non esitate ad offrirmi uno spritz, non mi offendo mica 👀

La relazione è disponibile [qui](https://github.com/augustozanellato/BasedCharts/raw/master/docs/relazione.pdf)

## Ambienti testati

|              | Ambiente di sviluppo | Ambiente di test |
|-------------:|----------------------|------------------|
| OS           | Arch Linux (rolling) | Ubuntu 18.04     |
| Compilatore  | Clang 13.0.1         | GCC 7.3.0        |
| Versione Qt  | 5.15.2               | 5.9.5            |
| Build system | cmake+ninja          | Qmake+make       |

## Struttura delle directory

- `assets/`: contiene le icone e i temi usati nel progetto
- `build/`: ignorata da `.gitignore`, contiene file temporanei usati per buildare il progetto e la relativa doc
- `docs/`: contiene i sorgenti in LaTeX della relazione del progetto
- `examples/`: contiene alcuni file `.json` leggibili dal progetto contenenti alcuni grafici.
  - `examples/invalid/`: contiene alcuni file non validi, usati principalmente per testare l'*error handling* della logica di deserializzazione.
- `scripts/`: contiene alcuni script di utilty
- `src/`: contiene i sorgenti del progetto organizzati con la seguente struttura:
  - `src/charts/`: contiene le varie classi che gestiscono i vari tipi di grafico, *factory* compresa
  - `src/models/`: contiene il modello utilizzato per rappresentare i dati contenuti nei grafici
  - `src/utils/`: contiene alcune utility per la gestione dei temi, un file contenente le eccezioni e la struttura di dati custom `Vector2D` usata come storage del modello
  - `src/views/`: contiene la vista principale
  - `src/windows/`: contiene la definizione delle varie finestre che compongono il progetto
  - `src/windows/wizard/`: contiene il wizard di creazione o apertura di un grafico
- `test/`: contiene alcuni unit test usati per verificare la correttezza della struttura di dati e (parzialmente) della logica di serializzazione

## Note varie

- Il progetto è stato sviluppato usando cmake e clang ~~perché sono autistico~~ per usare i sanitizer clang e clang-tidy.
- La relazione è scritta in LaTeX e compila usando [latexmk](https://ctan.org/pkg/latexmk)
- La gerarchia è stata disegnata con draw.io ed è salvata in `.drawio.svg`, viene automaticamente convertita da latexmk in pdf in modo tale da essere inclusa nella relazione
- Potrei aver fatto un trick bastardo per eliminare i warning runtime di Qt ma evidentemente il prof non ci ha fatto caso/non se n'è accorto
- Effettivamente il progetto non si avvia nella VM fornita dal prof facendo doppio click perché ~~Uculu~~ Ubuntu non distingue un eseguibile PIE da una libreria, ma di certo non è un mio problema
- Che schifo le immagini raster, sono diventato clinicamente ritardato per avere tutto vettoriale renderizzato alla risoluzione corretta.

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
