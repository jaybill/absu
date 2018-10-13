dosbox.exe -noconsole -userconf -conf dosbox-slow.conf \
-c "F:" \
-c "cd bin" \
-c "echo ----------------------- >> out.txt" \
-c "time >> out.txt" -c "test.exe >> out.txt" \
-c "pause" \
-c "exit" 