## Pass simple condition (level00)

This [binary](source.c) takes an input through `scanf` which is supposed to be
a password. It is simply an int that we can find hardcoded in the program
(0x149c which is 5276 in decimal). When the password matches, we get a shell.

```shell
# execute the binary and feed it the right value
(echo 5276; cat) | ./level00
***********************************
* 	     -Level00 -		  *
***********************************
Password:
Authenticated!
# the shell is yours
whoami
level01
cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```
