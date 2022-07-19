# Snowcrash flashback (level08)

[This](source.c) one is pretty straight forward. What this binary does is take
a filename as a argument. It opens the given file, reads its content and creates
a copy inside the `backups` directory. Since it has the setuid bit on as usual
it can read the next level's .pass file. All we have to do is to give it the
file. The only subtlety here is that if the filename is a path with
subdirectories the program will try to create the file and fail since they dont
exist. The simplest solution then is to create a symlink the the file we want
and to feed it to the program.

```shell
# get your rights back
chmod 755 .
# create the symlink
ln -s /home/users/level09/.pass flag
# run the program to get a copy
./level08 flag
# enjoy your flag
cat backups/flag
```
