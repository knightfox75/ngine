@echo off
cls

NGN_FileSystem -idir data -ofile data.pkg -key "0123456789ABCDEF"
pause

exit