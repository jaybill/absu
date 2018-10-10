dosbox.exe -noconsole -userconf -conf dosbox-slow.conf \
-c "F:" \
-c "cd dist" \
-c "echo ----------------------- >> out.txt" \
-c "time >> out.txt" -c "molewarp.exe >> out.txt" \
-c "pause" \
-c "exit" 