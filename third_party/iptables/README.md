What is iptables?
iptables is the userspace command line program used to configure the Linux 2.4.x and later packet filtering ruleset. It is targeted towards system administrators.

Since Network Address Translation is also configured from the packet filter ruleset, iptables is used for this, too.

The iptables package also includes ip6tables. ip6tables is used for configuring the IPv6 packet filter.

Dependencies
iptables requires a kernel that features the ip_tables packet filter. This includes all 2.4.x and later kernel releases.

Main Features
listing the contents of the packet filter ruleset
adding/removing/modifying rules in the packet filter ruleset
listing/zeroing per-rule counters of the packet filter ruleset
Git Tree
The current development version of iptables can be accessed at https://git.netfilter.org/iptables/.

Authors
iptables was mostly written by the netfilter core team, but it has received numerous contributions from many individuals.
