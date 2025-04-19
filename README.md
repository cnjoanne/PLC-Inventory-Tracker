# PLC-Inventory-Tracker

To run, in ```./PLC-Inventory-Tracker```:  
```
> make 
> ./bin/main.exe
```

Afterwards, run to remove *.obj files:  
```
> make clean
```

To remove reports generated, run:   
```
make clean-reports
```



File structure  
```
PLC-INVENTORY-TRACKER
├── bin
│   ├── executable_file_placeholder.txt
│   └── main.exe
├── data
│   ├── bad_inputs.csv
│   ├── cache
│   │   ├── bin_cache.bin
│   │   └── bin_cache_placeholder.txt
│   ├── empty.csv
│   ├── input.csv
│   ├── managing_input_csv.ipynb
│   └── mini.csv
├── include
│   ├── binary_cache.h
│   ├── csv_parser.h
│   ├── filtering.h
│   ├── instruction_printer.h
│   ├── item.h
│   ├── latex_export.h
│   ├── sorting.h
│   └── utils.h
├── Makefile
├── obj
│   ├── binary_cache.o
│   ├── csv_parser.o
│   ├── filtering.o
│   ├── instruction_printer.o
│   ├── latex_export.o
│   ├── main.o
│   ├── sorting.o
│   └── utils.o
├── output
│   ├── latex_placeholder.txt
│   └── stock_report_19042025-471118.tex
├── README.md
└── src
    ├── binary_cache.c
    ├── csv_parser.c
    ├── filtering.c
    ├── instruction_printer.c
    ├── latex_export.c
    ├── main.c
    ├── sorting.c
    └── utils.c
```
Dataset obtained from: https://www.kaggle.com/datasets/shinomikel/stardew-valley-spring-crop-info  
https://docs.google.com/spreadsheets/d/1Hd7M6ITTDcdGbqGiTtgrp0vgXuJqLaCP2xJ3LaV8gbU/edit?gid=142621552#gid=142621552  