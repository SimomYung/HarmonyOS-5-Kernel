/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Sysevent header file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 9 20:44:58 2024
 */
#ifndef KLIBS_SYSEVENT_DATA_H
#define KLIBS_SYSEVENT_DATA_H

#ifdef __KERNEL__
#define log_error	klog_error
#else
#define log_error	hm_error
#endif /* __KERNEL__ */

#ifdef CONFIG_SYSEVENT

#ifdef __KERNEL__
static inline bool is_alpha(char c)
{
	return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

static inline bool is_alnum(char c)
{
	return ('0' <= c && c <= '9') || is_alpha(c);
}
#endif /* __KERNEL__ */

static inline bool is_valid_string(const char *str, __u32 max_len)
{
	size_t len = 0;
	bool is_valid = false;

	if (str != NULL) {
		len = strlen(str);
#ifdef __KERNEL__
		is_valid = ((0 < len) && (len <= max_len) && is_alpha((int)str[0]));
#else
		is_valid = ((0 < len) && (len <= max_len) && isalpha((int)str[0]));
#endif /* __KERNEL__ */
	}

	for (__u32 i = 1; is_valid && (i < len); i++) {
#ifdef __KERNEL__
		if (!is_alnum(str[i]) && (str[i] != '_')) {
#else
		if (!isalnum(str[i]) && (str[i] != '_')) {
#endif /* __KERNEL__ */
			is_valid = false;
			break;
		}
	}

	return is_valid;
}

static inline int raw_data_update(struct raw_data *data, size_t pos, __u8 *src, size_t len)
{
	int err = E_HM_OK;

	if ((pos + len) <= data->capacity) {
#ifdef __KERNEL__
		mem_cpy_p_to_p(data->buf + pos, data->capacity, src, len);
#else
		NOFAIL(memcpy_s(data->buf + pos, data->capacity, src, len));
#endif /* __KERNEL__ */
		if ((pos + len) > data->pos) {
			data->pos = pos + len;
		}
	} else {
		log_error("Insufficient space, size: %zu, pos: %zu len: %zu\n",
			   data->capacity, pos, len);
		err = E_HM_INVAL;
	}

	return err;
}

static inline int raw_data_append(struct raw_data *data, __u8 *src, size_t len)
{
	return raw_data_update(data, data->pos, src, len);
}

static inline __u8 encode_tag(__u8 type)
{
	return type << (TAG_BYTE_OFFSET + 1);
}

static inline int unsigned_varint_code(struct raw_data *data, enum encode_type type, __u64 val)
{
	__u64 value = val;
	__u8 encoded_val = encode_tag((__u8)type) |
			 ((value < TAG_BYTE_BOUND) ? 0 : TAG_BYTE_BOUND) |
			 (__u8)(value & TAG_BYTE_MASK);
	int err = raw_data_append(data, (__u8 *)&encoded_val, sizeof(__u8));

	value >>= TAG_BYTE_OFFSET;
	while ((err == E_HM_OK) && (value > 0)) {
		encoded_val = ((value < NON_TAG_BYTE_BOUND) ? 0 : NON_TAG_BYTE_BOUND) |
			      (__u8)(value & NON_TAG_BYTE_MASK);
		err = raw_data_append(data, (__u8 *)&encoded_val, sizeof(__u8));
		value >>= NON_TAG_BYTE_OFFSET;
	}

	return err;
}

static inline int str_length_delimited_encode(struct raw_data *data, const char *str)
{
	__u64 length = (__u64)strlen(str);
	int err = unsigned_varint_code(data, LENGTH_DELIMITED, length);
	if (err == E_HM_OK) {
		err = raw_data_append(data, (__u8 *)str, length);
	}

	return err;
}

static inline int key_value_type_encode(struct raw_data *data, __u8 is_array, __u8 type, __u8 count)
{
	struct param_value_type value_type = {};

	value_type.is_array = is_array;
	value_type.value_type = type;
	value_type.value_byte_cnt = count;

	return raw_data_append(data, (__u8 *)&value_type, sizeof(struct param_value_type));
}

static inline int signed_varint_encode(struct raw_data *data, enum encode_type type, __s64 val)
{
	/* zigzag encode */
	__u64 zero = 0;
	int len = ((sizeof(val) << 3) - 1);

	__u64 uval = ((__u64)val >> len);
	if (val < 0) {
		uval |= ~(~zero >> len);
	}
	uval = ((__u64)val << 1) ^ uval;

	return unsigned_varint_code(data, type, uval);
}

static inline int int64_t_varint_encode(struct raw_data *data, __s64 val)
{
	return signed_varint_encode(data, VARINT, val);
}

static inline int fill_key_type_to_data(struct raw_data *data, const char *key, enum value_type type)
{
	int err = str_length_delimited_encode(data, key);
	if (err == E_HM_OK) {
		err = key_value_type_encode(data, (__u8)0, (__u8)type, (__u8)0);
	}

	return err;
}

static inline int fill_integer_to_data(struct raw_data *data, __s64 val)
{
	return int64_t_varint_encode(data, val);
}

static inline int fill_string_to_data(struct raw_data *data, const char *val)
{
	int err = E_HM_OK;

	if ((val == NULL) || strlen(val) > PARAM_STR_MAX_LEN) {
		log_error("Invalid value\n");
		err = E_HM_INVAL;
	} else {
		err = str_length_delimited_encode(data, val);
	}

	return err;
}

static inline int fill_args_to_data(struct raw_data *data, __u32 params_count, va_list args)
{
	int err = E_HM_OK;

	if (params_count > MAX_PARAM_NUMBER) {
		log_error("Invalid number of params: %u\n", params_count);
		err = E_HM_INVAL;
	} else {
		err = raw_data_append(data, (__u8 *)&params_count, sizeof(__u32));
	}

	for (__u32 i = 0; (err == E_HM_OK) && (i < params_count); i++) {
		enum value_type type = va_arg(args, enum value_type);
		const char *key = va_arg(args, const char *);

		if (!is_valid_string(key, MAX_PARAM_NAME_LENGTH)) {
			log_error("Invalid key\n");
			err = E_HM_INVAL;
			break;
		}

		err = fill_key_type_to_data(data, key, type);
		if (err != E_HM_OK) {
			break;
		}

		if (type == INT64) {
			__s64 int_val = va_arg(args, __s64);

			err = fill_integer_to_data(data, int_val);
		} else if (type == STRING) {
			const char *str_val = va_arg(args, const char *);

			err = fill_string_to_data(data, str_val);
		} else {
			log_error("Unknown value_type: %u\n", (unsigned int)type);
			err = E_HM_INVAL;
		}
	}

	return err;
}

static inline int fill_header_to_data(struct raw_data *data, const char *domain,
				      const char *evt_name, enum sysevent_type evt_type)
{
	struct sysevent_header header = {};
	int err = E_HM_OK;

	if (!is_valid_string(domain, MAX_DOMAIN_LENGTH) ||
	    !is_valid_string(evt_name, MAX_EVENT_NAME_LENGTH)) {
		log_error("Invalid domain or evt_name\n");
		err = E_HM_INVAL;
	} else {
		err = SYSEVENT_CURRENT_REALTIME(&(header.timestamp));
	}
	if (err == E_HM_OK) {
#ifdef __KERNEL__
		str_cpy_p_to_a(header.domain, domain);
		str_cpy_p_to_a(header.name, evt_name);
#else
		NOFAIL(strcpy_s(header.domain, MAX_DOMAIN_LENGTH, domain));
		NOFAIL(strcpy_s(header.name, MAX_EVENT_NAME_LENGTH, evt_name));
#endif /* __KERNEL__ */
		header.type = (__u8)((unsigned int)evt_type - 1);
		header.time_zone = (__u8)TIMEZONE_UTC_INDEX;
		header.id = 0;
		header.pid = SYSEVENT_CURRENT_PID();
		header.tid = SYSEVENT_CURRENT_TID();
		header.uid = SYSEVENT_CURRENT_UID();
		header.is_open_trace = 0;
		err = raw_data_append(data, (__u8 *)&header, sizeof(struct sysevent_header));
	}

	return err;
}
#endif /* CONFIG_SYSEVENT */

#endif
