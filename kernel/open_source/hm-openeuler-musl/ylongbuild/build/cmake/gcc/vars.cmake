set(COMMON_CFLAGS "-Werror -g -O2")
set(COMMON_CFLAGS "${COMMON_CFLAGS} -std=gnu11 -Wall -Wextra \
    -Wl,-z,relro -fstack-protector-strong -Wl,-z,noexecstack -Wl,-z,now -D_FORTIFY_SOURCE=2")

set(COMMON_SFLAGS "-Werror -g -O2")
set(COMMON_SFLAGS "${COMMON_SFLAGS} -std=gnu11 -Wall -Wextra \
    -Wl,-z,relro -fstack-protector-strong -Wl,-z,noexecstack -Wl,-z,now -D_FORTIFY_SOURCE=2")