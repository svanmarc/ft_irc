#connection a IRSSI irssi -c localhost -p 6667 -n "myNick" -w "myPassword"
irssi -c localhost -p 6667 -n $1 -w $2

#connection a netcat
nc localhost 6667

#connection a hexchat
hexchat --connect=localhost:6667

********************************************************************************************************************

#liberer le port 6667
sudo lsof -i :6667

#kill le processus
kill -9 <PID>

********************************************************************************************************************

#cmds IRSSI & Hexchat (avec / selon a verif)
connect localhost
nick <pseudo>
msg <pseudo> <message>
join #<channel>
part #<channel>
quit

#cmds Netcat
nc localhost 6667
NICK <pseudo>
USER <pseudo>
JOIN #<channel>
PART #<channel>
QUIT

********************************************************************************************************************