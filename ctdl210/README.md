# Citadel 2.10

This is purported to be the original source for Citadel as released
into the public domain in 1983. This ran on CP/M 2.22 and was written
in BDS C. Which appears to be a dialect of K&R C. 

I am going to port this code to Linux. 

Why, you may be asking. This code has been ported multiple times. The
latest incarnation is Citadel/UX, which has grown into a complex
groupware product. 

Well, that is a problem, "complex groupware" is not what I am
looking for. A small, simple, BBS interface is what I am looking for.
So I am starting back at first principles.

# About Me

I have been in the IT industry for about 40 years now. I became
interesting ind BBSing back in the early '90s running on an Amiga 500
and later on a PC clone. On the Amiga I ran Tempest, AmiExpress and
Prometheus. On the PC clone, WWIV. 

I eventually acquired the source code to AmiExpress and started to
modify it. This lead to my own software that eventually ran, but was
never released to the public. With the bankruptcy of Commodore and the
hardware failing, I stopped the development. I pulled the hard drive 
and made backups on floppies and set them aside. 

And now, some 30 years later, I pulled this out of storage and to see 
if I could recover anything. The floppies were formatted for AmigaDOS and 
are not readable on a standard PC diskette, the hard drive has developed bad 
sectors. I was able to make a bit copy of the hard drive and pulled as much
of the contents as possible off the drive. Large chunks of the BBS code
has been lost. Which is too bad. I was using curses for screen
management and a ISAM database. This nessitated a port of ncurses 1.81
to the Amiga and adapting MIX Software's C/Database to work on the
Amiga. This was quite a undertaking, I learned C on the Amiga, but I
had been professionly programing for 10 years at this point. 

All of which leads me to here.

## Phase I

Lower case the file names and fix CP/M end-of-file issues. CP/M used
fixed 128 byte sectors for files, end-of-file was indicated by ^Z. If
this happens in the middle of a sector, the remaining space is filled 
with garbage and needs to be stripped off. Also CP/M used \r\n for 
end-of-line. So far this has not been a problem. I also removed the 
compiled .com files.

## Phase II

First pass over the code. Reformat the code to how I like it. Remove
obvious BDS C nemonics and change them to ANSI C. Remove obvious CP/M
semantics. Compile with gcc <file name> to find undefined stuff. Fix
compile errors.

### Note to self

210msgs.c uses a lot of disk based character I/O. 210modem.c will most
likely require a rewrite. There is no modem, we are using the terminal
interface. No provisions for multi-user usage.

## Phase III

Create a Makefile and try to build. Fix problems as they arise.





