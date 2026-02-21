/* SPDX-License-Identifier: GPL-2.0-only */

#include <linux/xz.h>
#include <linux/compiler.h>
#include <linux/errno.h>

/*******************************************************************************
  Function name	: xz_decompress
  Description	: decompress xz format data
  Parameters	:
	in	: xz format data
	out	: buffer for storing the decompressed data
	in_size	: size of xz format data
	out_size: size of buffer for storing the decompressed data
  return 	: decompressed data result size OR error code
*******************************************************************************/
int liblinux_xz_decompress(const void *in, size_t in_size,
		  void *out, size_t out_size)
{
	struct xz_dec *xz_dec = NULL;
	struct xz_buf xz_buf;
	enum xz_ret xz_ret;
	int ret = 0;

	xz_dec = xz_dec_init(XZ_SINGLE, 0);
	if (!xz_dec)
		return -ENOMEM;

	xz_buf.in_size = in_size;
	xz_buf.in = (const uint8_t *)in;
	xz_buf.in_pos = 0;
	xz_buf.out_size = out_size;
	xz_buf.out = (uint8_t *)out;
	xz_buf.out_pos = 0;

	xz_ret = xz_dec_run(xz_dec, &xz_buf);

	xz_dec_end(xz_dec);

	/*
	 * XZ_STREAM_END means xz_dec_run's operation
	 * finished successfully.
	 */
	if (xz_ret == XZ_STREAM_END) {
		ret = xz_buf.out_pos;
	} else {
		/*
		 * XZ_OK just means everything is OK so far.
		 * more input or more output space is required to continue.
		 * XZ_OK is possible only in multi-call mode
		 * (XZ_PREALLOC or XZ_DYNALLOC).
		 * this funtion uses single mode(XZ_SINGLE).
		 */
		if (unlikely(xz_ret == XZ_OK)) {
			ret = -1;
		} else {
			ret = - (int)xz_ret;
		}
	}

	return ret;
}