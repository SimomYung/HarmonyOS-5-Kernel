# Readme for list.sh and posix\_api.list

The shell script list.sh generates two lists, api.list and posix\_api\_supported.list.

The api.list is list of all supported libc interfaces, include POSIX and others.

The posix\_api\_supported.list is list of supported POSIX interfaces.

The list.sh generates these files by reading the first line of docs in hm/doc/man. So
make sure that all docs have the same format and there is no redundant file in the
directory.

The posix\_api.list is all interfaces listed by POSIX standard. The list.sh judges if
an interface is POSIX interface by grepping it in this file.
