dosbox.exe -noconsole -userconf -conf dosbox.conf "F:" \
-c "make clean" \
-c "make all" \
-c "cd bin" \
-c "echo ----------------------- >> out.txt" \
-c "time >> out.txt" -c "molewarp.exe >> out.txt" -c "pause" -c "exit" 
