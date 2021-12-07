# MemCON
**A simulation of a memory controller for ECE 485/585**

**---| How to Use |---**

You can load a file by starting MemCon and selecting a file to load from the current directory.

Then you can simulate the file (it will ask for an output file name).  

You can also change the program's settings which can also be toggled via command line arguments.

Finally you can quit when done.

**---| Command Line Arguments |---**


"-f 'File_Name'" = Will select the given file name for simulating.

"-d" = Enable debug mode.

Will show status of queue failing to enqueue new requests due to being full.

Will show status of queue when all requests from trace file have been processed
or are in the queue.

Will print out a page miss/hit/empty for each request processed.

"-t" = Enable file type checking.

Only .txt files will show as loadable files.

"-l" = Label output. First line of output file will label each column.

"-s" = Simple output.

Output will show request commands with the final data column being the column bits in all cases.
Each request always takes 10 CPU cycles to complete.
