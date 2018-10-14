dosbox.exe -noconsole -userconf -conf dosbox-slow.conf \
-c "F:" \
-c "cd bin" \
-c "echo ----------------------- >> out.txt" \
-c "time >> out.txt" \
-c "redir -o out.txt -eo test.exe" \
-c "pause" \
-c "exit" 