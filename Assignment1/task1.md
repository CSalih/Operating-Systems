## Task 1
### Script 1
```bash
#!/bin/bash
# Script 1
#
for FN in "$@" #Argv durchiterieren
do
chmod 0750 "$FN" #Rechte vom Ordner ändern (rwxr-x---)
done
```
### Script 2
```bash
#!/bin/bash
# Eine Funktion
function usage { 
    # $0 = Pfad
    echo "$0: <TODO: fill me in>"
    # Beenden    
    exit 1
}

# shift: Each time shift is executed the first argument will be removed
# || execute only when the right side status is non-zero
ARG1=$1; shift || usage
ARG2=$1; shift || usage
ARG3=$1; shift || usage

# -n: print line number with output lines to $ARG2

#ARG3 Search element
#ARG1 File to search
#ARG2 output file
grep -n "$ARG3" "$ARG1" > "$ARG2"
```