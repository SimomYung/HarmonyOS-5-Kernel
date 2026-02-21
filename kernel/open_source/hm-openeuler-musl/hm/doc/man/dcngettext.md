# dcngettext()
Translate message and choose plural form

## Synopsis
```c
#include <libintl.h>

char *dcngettext(const char *domainname,
                 const char *msgid, const char *msgid_plural,
                 unsigned long int n, int category);

```

## Arguments
- **domainname**
Specify the domain to be used while translating the message.
&nbsp;
- **msgid**
The message to be translated.
&nbsp;
- **msgid_plural**
The plural form of message.
&nbsp;
- **n**
Choose the form of message to be translate. If *n* is 1 *msgid* is used, otherwise *msgid_plural* is used.
&nbsp;
- **category**
Specify the category to use.

## Library
- libc

## Description
Translate a string text into the user's native language, by looking up the appropriate plural form of the translation in a message catalog. The message catalog is a set of message located at *dirname/locale/category/domainname.mo*.
&nbsp;
If *domainname* is NULL, "messages" is used as default.

## Returns

If the message is found in the domain data file, the translated message will be returned, otherwise *msgid* or *msgid_plural* will be returned.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety

TODO
