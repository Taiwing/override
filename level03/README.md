## Leet dood (level03)

This [binary](source.c) takes user input through a "%d" scanf call. The value
passed to the program is then substracted from the constant 0x1337d00d. The
result will be the key parameter for the _decrypt_ function if it is between 1
and 21 (inclusive). Then a string is decrypted by applying xoring the key with
each of its characters. If the result is "Congratulations!" then a shell we get
a shell.

The string is "Q}|u\`sfg~sf{}|a3". To find the key all we need is to perform one
operation: `Q xor C` (0x51 xor 0x43), which is equal to 0x12 (18 in decimal).
Then we can check `Q xor 0x12` will be equal to _C_. This is also true for every
other character of string.

Then we take our leet constant, substract the key we want to pass to the decrypt
function, and we should have our password: `0x1337d00d - 0x12` which is equal to
0x1337cffb or 322424827 in decimal. Now we can access the shell:

```shell
# run the program, input the password and there we are
./level03
***********************************
*               level03         **
***********************************
Password:322424827
```
