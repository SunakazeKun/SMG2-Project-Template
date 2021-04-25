CW_PATH=../CodeWarrior
CPPFILES="kamekLoader smg"

CC=$CW_PATH/mwcceppc
CFLAGS="-i . -I- -i ../include -Cpp_exceptions off -enum int -Os -use_lmw_stmw on -fp hard -rostr -sdata 0 -sdata2 0 -DUSA"

for i in $CPPFILES
do
	echo Compiling $i.cpp...
	$CC $CFLAGS -c -o $i.o $i.cpp
done

echo Linking...
mkdir -p out

../Kamek/Kamek.exe kamekLoader.o smg.o -static=0x80001800 -output-riiv=out/riivo_USA.xml -input-dol=../dols/USA.dol -output-dol=out/USA.dol

sleep 100