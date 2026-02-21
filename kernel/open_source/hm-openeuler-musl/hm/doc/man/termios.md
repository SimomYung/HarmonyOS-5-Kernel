# termios()
General terminal line discipline

## Synopsis
```c
#include <termios.h>
```

## Arguments
N/A

## Library
libc

## Description
It describes a general terminal interface that is provided to control asynchronous communications ports.

Many terminal operation interfaces include a *tio* parameter pointing to the termios structure, which contains the following members:
```c
	tcflag_t c_iflag;	/* input modes */
	tcflag_t c_oflag;	/* output modes */
	tcflag_t c_cflag;	/* control modes */
	tcflag_t c_lflag;	/* local modes */
	cc_t c_line;		/* line discipline */
	cc_t c_cc[NCCS];	/* control chars */
	speed_t __c_ispeed;	/* input speed */
	speed_t __c_ospeed;	/* output speed */
```

- c\_iflag constants:

    **INLCR** Translate NL to CR on input.

    **ICRNL** Translate CR to NL on input.

    **IGNCR** Ignore CR on input.

    **IXON**  Enable XON/XOFF flow control on output.

    **IXANY** Typing any character will restart stopped output.

- c\_oflag constants:

    **ONLCR** Map NL to CR-NL on output.

    **OCRNL** Map CR to NL on output.

- c\_cflag constants:

    **CSIZE** Character size mask. Values are CS5, CS6, CS7, or CS8. (Currently only 1210 platforms are supported)

- c\_lflag constants:

    **ECHO** Echo input characters.

    **ECHOE** If ICANON is also set, the ERASE character erases the preceding input character, the WERASE character erases the whitespace-delimited words.

    **ECHOK** If ICANON is also set, the KILL character erases the current line.

    **ECHONL** If ICANON is also set, echo the NL character even if ECHO is not set.

    **ECHOCTL** If ECHO is also set, terminal special characters other than TAB, NL, START, and STOP are echoed as
                ^X, where X is the character with ASCII code 0x40 greater than the special character. For example,
                character 0x08 (BS) is echoed as ^H.

    **IEXTEN** Enable implementation-defined input processing. This flag, as well as ICANON must be enabled for
               the special characters EOL2, LNEXT, REPRINT, WERASE to be interpreted, and for the IUCLC flag to be
               effective.

    **ISIG** When any of the characters INTR, QUIT, SUSP, or DSUSP are received, generate the corresponding signal.

- NCSS:
size of the array *c\_cc* for control characters, The following subscript names for the array *c\_cc* are defined:

    **VEOF** End-of-file character, the default is Ctrl-D.

    **VINTR** Interrupt character, send a SIGINT signal to foreground process, the default is Ctrl-C.

    **VMIN** Minimum number of characters for noncanonical read.

    **VQUIT** (Ctrl-\) Quit character (QUIT). Send SIGQUIT signal. Recognized when ISIG is set.

    **VSUSP** (Ctrl-Z) Suspend character (SUSP). Send SIGTSTP signal. Recognized when ISIG is set.

    **VSTART** (Ctrl-Q) Start character (START). Restarts output stopped by the Stop character. Recognized when IXON is set.

    **VSTOP**  (Ctrl-S) Stop character (STOP). Stop output until Start character typed. Recognized when IXON is set.

    **VERASE** (DEL, BS, Ctrl-H) Erase character (ERASE). Recognized when ICANON is set, and then not passed as input.

    **VKILL**  (Ctrl-U) Kill character (KILL). Recognized when ICANON is set, and then not passed as input.

    **VWERASE** (not in POSIX; Ctrl-W) Word erase (WERASE). Recognized when ICANON and IEXTEN are set, and then not passed as input.

- Baud Rate Selection:
The input and output baud rates are stored in the termios structure. These are the valid values for objects of type *speed\_t*. The following values are defined:

    B50
    B300
    B1200
    B2400
    B4800
    B9600
    B19200
    B38400
    B57600
    B115200

## Returns
N/A

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
