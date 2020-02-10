@REM +++Date last modified: 05-Jul-1997

make allobj cmm=s amm=SMALL -fmakefile.sc > slss9707.log
rm *.obj
make allobj cmm=c amm=COMPACT -fmakefile.sc > slsc9707.log
rm *.obj
make allobj cmm=m amm=MEDIUM -fmakefile.sc > slsm9707.log
rm *.obj
make allobj cmm=l amm=LARGE -fmakefile.sc > slsl9707.log
rm *.obj
