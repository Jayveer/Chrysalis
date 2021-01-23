
# Chrysalis


Chrysalis is a free open source tool designed to be used with the game Metal Gear Solid: Peace Walker. It allows the user to extract files from the SLOT.DAT and STAGEDAT.PDT files packaged with the game. The default usage is for the PSP version of the game but the PS3 version is supported in this tool as well.

This project uses [Zlib](https://github.com/madler/zlib) by [Mark Adler](https://github.com/madler)


### Latest Changes
 - Added support to extrtact dar and qar archives

### To Do
 - Find out how slot lookup is hashed
 - Add multithreaded extract
 - Create GUI variant
 - Clean up the code

##  Usage

Currently only a CLI version of the application exists. The filename of the file to extract is mandatory whilst all other inputs are optional and will revert to defaults if not set. If extracting from the PSP version of the game it is possible to simply drag the file on the executable to extract it, this will extract all files. It is possible to extract a specfic page using its numbered index when extracting slot files, as well as extracting a specific stage by providing its name when extracting stage files.

If extracting SLOT.DAT it is required to have SLOT.KEY alongside it in the same directory.

```
Chrysalis.exe "path\to\STAGEDAT.PDT"
```
The above instruction will extract all files from STAGEDAT.PDT to the current directory.

```
Chrysalis.exe -ps3 "path\to\STAGEDAT.PDT"
```
If you want to extract files from the PS3 version of the game use the -ps3 option

```
Chrysalis.exe -ps3 "path\to\SLOT.DAT"
```
The same applies to SLOT.DAT. If extracting from SLOT.DAT, SLOT.KEY must reside alonside it in the same directory. This will extract all files from SLOT.DAT

```
Chrysalis.exe -ps3 "path\to\SLOT.DAT" 23
```
The numbered index of the SLOT page to extract can be added in order to only extract files for a given index.

```
Chrysalis.exe -psp "path\to\SLOT.DAT" 23
```
The -psp option can also be used if extracting from the PSP version. However this is the default and therefore will be used regardless if the platform is omitted.

```
Chrysalis.exe -ps3 "path\to\STAGEDAT.PDT" "init"
```
For the Stage archive the name of the stage can be added after the file name in order to extract the specified stage only

```
Chrysalis.exe -ps3 "path\to\STAGEDAT.PDT" "init" "path\to\output"
```
Adding an output path at the end will extract the files to a specified folder of your choice.

```
Chrysalis.exe "path\to\cache.dar" "path\to\output"
```
It's also possible to extract from dar and qar archives. The Platform option is unnecessary here.

```
Chrysalis.exe "path\to\cache.qar" "path\to\output"
```
Extracting from Qar.

## License
[MIT](LICENSE.md)
This project falls under the MIT license.
