# MemCON
**A simulation of a memory controller for ECE 485/585**

**---| How to Use |---**

You can load a file by starting MemCon and selecting a file to load from the current directory.

Then you can simulate the file (it will ask for an output file name).  

Finally you can quit when done.

**---| Command Line Arguments |---**

"-d" = Enable debug mode.

"-t" = Enable file type checking. Only .txt files will show as loadable files.

"-f 'File_Name'" = Will load the file whose name you give.

**---| Debug Mode |---**

Will display each request loaded from a save file that was loaded when you simulate.
Each request will show the timing, the command, and the full address given.
