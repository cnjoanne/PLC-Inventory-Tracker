# PLC-Inventory-Tracker

## Project Overview
This project validates and parses stock inventory data obtained from Stardew Valley, processes the information, and exports it as a LaTeX file for easy-to-read reports. It is implemented in C in accordance with the project guidelines and to demonstrate proficiency in the C programming language.


## Input CSV Format
The CSV input file has 3 header colums:   
|item         |quantity     |expiry-date  |  
|-------------|-------------|-------------|  
|Cauliflower  |6            |07/05/2027   |  
|Sunflower    |17           |10/12/2025   |  

These files can be found in ```./data```.  

## Validating Input Contents  
When the user provides the path to the CSV file, the parser first verifies that is it a valid file path. It then reads the file line by line, ensuring that each line contains exactly three columns: item, quantity, and expiry-date.   
For each line, the parser performs the following validations:   
1.	The item field must contain a valid item name.  
2.	The quantity must be a valid number.  
3.	The expiry-date must be a properly formatted and actual date.  

Once the items are valid, we store them in a binary cache. This binary cache will be referenced to process data (further eleaborated in Process Features).      

## Process Features  
The system performs the following actions:
- Stock Warnings: Flags low stock and soon-to-expire items.
- Data Sorting and Filtering: User can sort and filter the data based on their preferences  
Users can choose from the following options:  
    1.	Sort by name in alphabetical order (A-Z).  
    2.	Sort by name in reverse alphabetical order (Z-A).  
    3.	Sort by quantity in ascending order.  
    4.	Sort by quantity in descending order.  
    5.	Sort by expiry date, from earliest to latest.  
    6.	Sort by expiry date, from latest to earliest.  
    7.	Filter by expiry date within a user-specified range.  
    8.	Filter by quantity based on user-defined threshold.  

Once the user is satisfied with the sorted/filtered data, they can export it.  

## LaTeX Report  
The generated LaTeX file can be found in the ```./output``` directory, where it is stored as a unique ```.tex``` file. To view the report, open the file in a LaTeX compiler such as Overleaf (an online LaTeX editor).

## Running the System  
To run this system, in ```./PLC-Inventory-Tracker``` run:  
```
> make 
> ./bin/main.exe
```

## Cleaning up  
To remove object files, the executable, and binary cache, run: 
```
> make clean
```

To remove generated LaTeX reports, run:   
```
make clean-reports
```


## File Structure
File structure of the overall system    
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

## References  
Dataset obtained from:   
- https://www.kaggle.com/datasets/shinomikel/stardew-valley-spring-crop-info     
- https://docs.google.com/spreadsheets/d/1Hd7M6ITTDcdGbqGiTtgrp0vgXuJqLaCP2xJ3LaV8gbU/edit?gid=142621552#gid=142621552  