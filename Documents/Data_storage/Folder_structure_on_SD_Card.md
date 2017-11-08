# Data storing on sd card
Folder structure used to save the logged data on the SD-Card

## VID
* Each VID(17_Byte) is mapped to a 1-Byte value (MVID). These maps are stored in the root dir in maps.txt
* Each vehicle (VID) has its own folder.
* The VID folders are containing a file for each date

## <Date>.hex
* The format of the data-file is hex
* The data is stored in 12 Byte segemnts

### Data structure
|Bytes|1.  |2. - 5.  |6. - 8.|9. - 12.|
|-----|----|---------|-------|--------|
|Value|MVID|Date (ms)|DataID |Value   |



# Visualization

-VID01 (DIR)
    |
    Date01.hex (File)
    |
    Data02.hex
-VID02 (DIR)
    |
    Date01.hex (File)
    |
    Data02.hex