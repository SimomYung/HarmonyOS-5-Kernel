# futex()
Fast user-space locking

## Synopsis
```c
#include <linux/futex.h>
#include <times.h>

int futex(int *uaddr, int futex_op, int val,
          const struct timespec *timeout,  /* or: unit32_t val2 */
          int *uaddr2, int val3);
```

There is no libc wrapper for this system call; call it using syscall().

## Arguments

- **uaddr**
  Point to the futex word which is four-byte integers that must be aligned on a four-byte boundary.
  &nbsp;

- **futex_op**
  Specify the operation to perform on the futex.
  &nbsp;

- **val**
  A value whose meaning and purpose depends on *futex_op*.
  &nbsp;

- **timeout**
  For several blocking operations, the *timeout* argument is a pointer to a *timespec* structure that specifies a timeout for the operation. For some operations, the least significant four bytes are instead used as an integer whose meaning is determined by the operation.
  &nbsp;

- **uaddr2**
  Where it is required, *uaddr2* is a pointer to a second futex word that is employed by the operation.
  &nbsp;

- **val3**
  The interpretation of *val3* depends on the operation.

  &nbsp;

  The arguments *timeout*, *uaddr2*, and *val3* are required only for certain of the futex operations. Where one of these arguments is not required, it is ignored.

## Library
libc

## Description
The futex system call provides a method for waiting until a certain condition becomes true. The *futex_op* consists of two parts: a command that specifies the operation to be performed, bit-wise ORed with zero or options that modify the behavior of the operation. The option that may be included in *futex_op* is as follows:

- **FUTEX_PRIVATE_FLAG**
  This option bit can be employed with all futex operations. It means the futex is process-private and not shared with another process, in other words, it is being used for synchronization only between threads of the same process.

- **FUTEX_BLOCKED_DETECT_FLAG**
  This option bit can be employed only with the FUTEX_WAIT_BITSET, FUTEX_LOCK_PI, and FUTEX_WAIT operations.

  If this option is set, the task will be checked for waiting too long when blocked.

&nbsp;
The following operations specified in *futex_op* are supported:

- **FUTEX_WAIT**
  This operation tests that the value at the futex word pointed to by the address *uaddr* still contains the expected value *val*, and if so, the sleeps waiting for a FUTEX_WAKE operation on the futex word. If the futex value does not match *val*, then the call fails immediately with the error EAGAIN.

  If the *timeout* is not NULL, it specifies a relative timeout for the wait. The timeout is measured according to the CLOCK_MONOTONIC clock. If *timeout* is NULL, the call blocks indefinitely.

  The arguments *uaddr2* and *val3* are ignored.

  &nbsp;

- **FUTEX_WAKE**
  This operation wakes at most *val* if the waiters that are waiting on the futex word at the address *uaddr*. No guarantee is provided about which waiters are awoken.

  The arguments *timeout*, *uaddr2* and *val3* are ignored.

  &nbsp;

- **FUTEX_REQUEUE**
  The operation wakes up a maximum of *val* waiters that are waiting on the futex at *uaddr*. If there are more than *val* waiters, then the remaining waiters are removed from the wait queue of the source futex at *uaddr* and added to the wait queue of the target futex at *uaddr2*. The *val2* specifies an upper limit on the number of waiters that are requeued to the futex *uaddr2*.
  &nbsp;

- **FUTEX_WAKE_OP**
  This operation supports more than one futex must be handled at the same time. The operation is equivalent to executing the following code atomically and totally ordered with respect to other futex operations on any of the two supplied futex words:

  ```c
int oldval = *(int *)uaddr2;
*(int *)uaddr2 = oldval @op @oparg;
futex(uaddr, FUTEX_WAKE, val, 0, 0, 0);
if (oldval @cmp @cmparg)
      futex(uaddr2, FUTEX_WAKE, val2, 0, 0, 0);
  ```

  In other words, FUTEX_WAKE_OP does the following:

  - save the original value of the futex word at *uaddr2* and perform an operation to modify the value of the futex at *uaddr2*
  - wake up a maximum of *val* waiters on the futex for the futex word at *uaddr*
  - dependent on the results of a test of the original value of the futex word at *uaddr2*, wake up a maximum of *val2* waiters on the futex *uaddr2*

  The operation and comparison that are performed are encoded in the bits of the argument *val3*. Pictorially, the encoding is:
  ```
  +---+-----+--------------+--------------+
  | op| cmp |     oparg    |    cmparg    |
  +---+-----+--------------+--------------+
    4    4          12            12       # of bits
  ```
  In the above, *op* and *cmp* are each one of the codes listed below. The *oparg* and *cmparg* components are literal numeric values, except as noted below.

  The *op* component has one of the following values:

  *op* == 0  /* uaddr2 = oparg; */
  *op* == 1  /* uaddr2 += oparg; */
  *op* == 2  /* uaddr2 |= oparg; */
  *op* == 3  /* uaddr2 &= ~oparg; */
  *op* == 4  /* uaddr2 ^= oparg; */

  In addition, bit-wise ORing the following value into *op* causes (1 << *oparg*) to be used as the operand:

  ```c
  #define FUTEX_OP_ARG_SHIFT  8  /* Use (1 << oparg) as operand */
  ```

  The *cmp* field is one of the following:

  *cmp* == 0  /* if (oldval == cmparg) wake */
  *cmp* == 1  /* if (oldval != cmparg) wake */
  *cmp* == 2  /* if (oldval < cmparg) wake */
  *cmp* == 3  /* if (oldval <= cmparg) wake */
  *cmp* == 4  /* if (oldval > cmparg) wake */
  *cmp* == 5  /* if (oldval >= cmparg) wake */

  The return value of FUTEX_WAKE_OP is the sum of the number of waiters woken on the futex *uaddr* plus the number of waiters woken on the futex *uaddr2*.

  &nbsp;

- **FUTEX_WAIT_BITSET**
  This operation is the same as FUTEX_WAIT except that *val3* is used to provide a 32-bit bit mask. This bit mask, in which at least one bit must be set.

  If the *timeout* is not NULL, it specifies a absolute timeout for the wait. The timeout is measured according to the CLOCK_MONOTONIC clock. If *timeout* is NULL, the call blocks indefinitely.

  The *uaddr2* argument is ignored.

  &nbsp;

- **FUTEX_WAKE_BITSET**
  The operation is the same as FUTEX_WAKE except that the *val3* argument is used to provide a 32-bit bit mask. This bit mask, in which at least one bit must be set, is used to select which waiters should be woken up. The selection is done by a bit-wise AND of the wake bit mask and the bit mask of the waiter(set using FUTEX_WAIT_BITSET). All of the waiters for which the result of the AND is nonzero are woken up; the remaining waiters are left sleeping.

  The constant FUTEX_BITSET_MATCH_ANY, which corresponds to all 32 bits set in the bit mask, can be used as the *val3* argument for FUTEX_WAKT_BITSET and FUTEX_WAKE_BITSET. Other than differences in the handling of the *timeout* argument, the FUTEX_WAIT operation is equivalent to FUTEX_WAIT_BITSET with *val3* specified as FUTEX_BITSET_MATCH_ANY; that is , allow a wake-up by any waker. The FUTEX_WAKE operation is equivalent to FUTEX_WAKE_BITSET with *val3* specified as FUTEX_BITSET_MATCH_ANY; that is, wake up any waiters.

  The *uaddr2* and *timeout* arguments are ignored.

## Returns
When an error occurred, all operations return -1 and set errno to indicate the cause of error.
&nbsp;
The return value on success depends on the operation, as described in the following list:

- **FUTEX_WAIT**
Return 0 if the caller was woken up.

- **FUTEX_WAKE**
  Return the number of waiters that were woken up.

- **FUTEX_REQUEUE**

  Return the number of waiters that were woken up.

- **FUTEX_WAKE_OP**

  Return the total number of waiters that were woken up. This is the sum of the woken waiters on the two futexes for the futex words at *uaddr* and *uaddr2*.

- **FUTEX_WAIT_BITSET**

  Return 0 if the caller was woken up.

- **FUTEX_WAKE_BITSET**

  Return the number of waiters that were woken up.

## Errors
- **EAGAIN**
  (FUTEX_WAIT, FUTEX_WAIT_BITSET) The value pointed to by *uaddr* was not equal to the expected value *val* at the time of the call.
  &nbsp;

- **EFAULT**
  A required pointer argument points outside the accessible address space.
  &nbsp;
- **EINVAL**

  - *timeout* argument is invalid (*tv_sec* was less than zero, or *tv_nsec* was not less than 1, 000, 000, 000).

  - (FUTEX_WAIT_BITSET, FUTEX_WAKE_BITSET) The bit mask supplied in *val3* is zero.

  - Invalid argument.

    &nbsp;

- **ENOSYS**
  Invalid operation specified in *futex_op*
  &nbsp;

- **EINTR**
  A FUTEX_WAIT or FUTEX_WAIT_BITSET operation was interrupted by a signal.
  &nbsp;
- **ETIMEDOUT**

  The operation in *futex_op* employed the timeout specified in *timeout*, and the timeout expired before the operation completed.
## Example
&nbsp;
## Classification
Native Interface

## Function Safety
TODO
