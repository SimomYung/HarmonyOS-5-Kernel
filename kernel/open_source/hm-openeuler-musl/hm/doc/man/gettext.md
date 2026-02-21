# gettext(), dgettext(), dcgettext()
Translate a message to the native language according to the locale

## Synopsis
```c
#include <libintl.h>

char *gettext(const char *msgid);
char *dgettext(const char *domainname, const char *msgid);
char *dcgettext(const char *domainname, const char *msgid, int category);

```

## Arguments
- **msgid**
The message identifier to be translated, usually the English version of the message.
&nbsp;
- **domainname**
Specify the domain to be used while translating the message.
&nbsp;
- **category**
Specify the category of the domain while translating the message.

## Library
- libc

## Description
Translate the message identified by *msgid* to the native language according to the locale and the *domain* specified by *domainname*, which is located at *dirname/locale/category/domainname.mo*, is used.
&nbsp;
If *domainname* is NULL, "messages" is used as default, that is what gettext() does.
&nbsp;
If *category* is not specified, like in gettext and dgettext, the category LC_MESSAGES is chosen.

## Returns
If the message is found in the domain data file, the translated file will be returned, otherwise *msgid* will be returned.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety

TODO
