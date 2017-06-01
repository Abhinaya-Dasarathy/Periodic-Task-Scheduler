# Periodic-Task-Scheduler
Implemented a periodic task scheduler which performs task once in every 30 seconds and stores the values in a persistent storage (SQLIte)

The tasks performed are-
memory.cpp: prints and stores VM size of the process in database every 30s
tcp_connect.cpp: stores time to estabish tcp connection to google.com every 30s

Database used : SQLite

Compiling the program: cmake . &&make ( The binary gets generated in src/pts as pts)

Running the program: ./src/pts/pts

Folders: 
 1. /src - contains the source cpp files
 2. /src/pts - contains the implementation of periodic task scheduler.
 3. /src/task - contains the implementation of the tasks
 4. /include - contains the header files required
 5. /datastore - contains the SQLite header files 
