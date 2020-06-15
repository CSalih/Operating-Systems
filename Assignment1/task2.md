## Task 2
```bash
#!/bin/bash
INFILE=$1	#argv 1
OUTFILE=$2	#argv 2
if [ -e "$INFILE" ]	 # Ist infile lesbar?
then
    cat "$INFILE" >> "$OUTFILE" # füge infile in outfile hinzu
fi
```