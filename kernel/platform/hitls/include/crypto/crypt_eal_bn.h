/**
 * @defgroup    crypt_eal_bn    crypt_eal_bn.h
 * @ingroup     crypto
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief       BN external interface
 */

#ifndef CRYPT_EAL_BN_H
#define CRYPT_EAL_BN_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct BigNum CRYPT_BN_BigNum;

typedef struct BnOptimizer CRYPT_BN_Optimizer;

/**
 * @ingroup bn
 * @brief   Big number creation
 *
 * @param   bits [IN] Number of bits
 *
 * @retval Not NULL, success.
 * @retval NULL, failure.
 */
CRYPT_BN_BigNum *CRYPT_EAL_BN_Create(uint32_t bits);

/**
 * @ingroup bn
 * @brief   Big number destruction
 *
 * @param   a [IN] Big number
 *
 * @retval None
 */
void CRYPT_EAL_BN_Destroy(CRYPT_BN_BigNum *a);

/**
 * @ingroup bn
 * @brief   Binary to big number
 *
 * @param   r [OUT] Big number
 * @param   bin [IN] Data stream to be converted
 * @param   binLen [IN] Data stream length
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_MEM_ALLOC_FAIL, memory allocation failure.
 */
int32_t CRYPT_EAL_BN_Bin2Bn(CRYPT_BN_BigNum *r, const uint8_t *bin, uint32_t binLen);

/**
 * @ingroup bn
 * @brief   Convert big number to binary numbers
 *
 * @param   a [IN] Big number
 * @param   bin [IN/OUT] Data stream to be converted -- The input pointer cannot be null.
 * @param   binLen [IN/OUT] Data stream length -- The length of the bin buffer.
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_SECUREC_FAIL, copying error.
 */
int32_t CRYPT_EAL_BN_Bn2Bin(const CRYPT_BN_BigNum *a, uint8_t *bin, uint32_t *binLen);

/**
 * @ingroup bn
 * @brief   Hexadecimal to a big number
 *
 * @param   r [OUT] Big number
 * @param   r [IN] Data stream to be converted
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_MEM_ALLOC_FAIL, memory allocation failure.
 * @retval  CRYPT_BN_CONVERT_INPUT_INVALID, invalid string.
 */
int32_t CRYPT_EAL_BN_Hex2Bn(CRYPT_BN_BigNum **r, const char *str);

/**
 * @ingroup bn
 * @brief   Convert big number to hexadecimal numbers
 *
 * @param   a [IN] Big number
 * @param   char [OUT] Converts a hexadecimal character string.
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_BN_INPUT_INVALID, the input parameter of the big number is invalid.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_MEM_ALLOC_FAIL, memory allocation failure.
 */
char *CRYPT_EAL_BN_Bn2Hex(const CRYPT_BN_BigNum *a);

/**
 * @ingroup bn
 * @brief   Decimal to big number
 *
 * @param   r [OUT] Big number
 * @param   str [IN] Decimal string to be converted
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_MEM_ALLOC_FAIL, memory allocation failure.
 * @retval  CRYPT_BN_CONVERT_INPUT_INVALID, invalid string.
 */
int32_t CRYPT_EAL_BN_Dec2Bn(CRYPT_BN_BigNum **r, const char *str);

/**
 * @ingroup bn
 * @brief   Decimal to big number
 *
 * @param   r [OUT] Big number
 * @param   str [IN] Decimal string to be converted
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_BN_INPUT_INVALID, the input parameter of the big number is invalid.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_MEM_ALLOC_FAIL, memory allocation failure.
 */
char *CRYPT_EAL_BN_Bn2Dec(const CRYPT_BN_BigNum *a);

/**
 * @ingroup bn
 * @brief   Big number optimizer creation
 *
 * @param   None
 *
 * @retval Not NULL, success.
 * @retval NULL, failure.
 */
CRYPT_BN_Optimizer *CRYPT_EAL_BN_OptimizerCreate(void);

/**
 * @ingroup bn
 * @brief   Destroy the big number optimizer.
 *
 * @param   opt [IN] Big number optimizer
 *
 * @retval None
 */
void CRYPT_EAL_BN_OptimizerDestroy(CRYPT_BN_Optimizer *opt);

/**
 * @ingroup bn
 * @brief   Set the symbol.
 *
 * @param   a [IN] Big number
 * @param   sign [IN] Symbol. The value true indicates a negative number
 *                    and the value false indicates a positive number.
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_BN_NO_NEGATOR_ZERO, 0 cannot be set to a negative sign.
 */
int32_t CRYPT_EAL_BN_SetSign(CRYPT_BN_BigNum *a, bool sign);

/**
 * @ingroup bn
 * @brief   Check whether the value of a big number is 1.
 *
 * @attention The big number input parameter cannot be a null pointer.
 * @param   a [IN] Big number
 *
 * @retval  true, the value of a big number is 1.
 * @retval  false, the value of a big number is not 1.
 *
 */
bool CRYPT_EAL_BN_IsOne(const CRYPT_BN_BigNum *a);

/**
 * @ingroup crypt_eal_bn
 * @brief   Check whether a big number is a negative number.
 *
 * @attention The input parameter cannot be a null pointer.
 * @param   a [IN] Big number
 *
 * @retval  true, the value of a big number is negative.
 * @retval  false, the value of a big number is not negative.
 *
 */
bool CRYPT_EAL_BN_IsNegative(const CRYPT_BN_BigNum *a);

/**
 * @ingroup bn
 * @brief Big number addition
 *
 * @param   r [OUT] Sum
 * @param   a [IN] Addend
 * @param   b [IN] Addend
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_BN_INPUT_INVALID, the input parameter of the big number is invalid.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_MEM_ALLOC_FAIL, memory allocation failure.
 */
int32_t CRYPT_EAL_BN_Add(CRYPT_BN_BigNum *r, const CRYPT_BN_BigNum *a, const CRYPT_BN_BigNum *b);

/**
 * @ingroup bn
 * @brief   Big number subtraction
 *
 * @param   r [OUT] Difference
 * @param   a [IN] Minuend
 * @param   b [IN] Subtrahend
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_BN_INPUT_INVALID, the input parameter of the big number is invalid.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_MEM_ALLOC_FAIL, memory allocation failure.
 */
int32_t CRYPT_EAL_BN_Sub(CRYPT_BN_BigNum *r, const CRYPT_BN_BigNum *a, const CRYPT_BN_BigNum *b);

/**
 * @ingroup bn
 * @brief   Big Number Multiplication
 *
 * @param   r [OUT] Product
 * @param   a [IN] Multiplier
 * @param   b [IN] Multiplier
 * @param   opt [IN] Optimizer
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_BN_INPUT_INVALID, the input parameter of the big number is invalid.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_MEM_ALLOC_FAIL, memory allocation failure.
 * @retval  CRYPT_BN_OPTIMIZER_GET_FAIL, failed to apply for space from the optimizer.
 */
int32_t CRYPT_EAL_BN_Mul(CRYPT_BN_BigNum *r, const CRYPT_BN_BigNum *a,
    const CRYPT_BN_BigNum *b, CRYPT_BN_Optimizer *opt);

/**
 * @ingroup bn
 * @brief   Big Number Division
 *
 * @param   q [OUT] Quotient
 * @param   r [OUT] Remainder
 * @param   x [IN] Dividend
 * @param   y [IN] Divisor
 * @param   opt [IN] Optimizer
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_BN_INPUT_INVALID, the input parameter of the big number is invalid.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_MEM_ALLOC_FAIL, memory allocation failure.
 * @retval  CRYPT_BN_OPTIMIZER_GET_FAIL, failed to apply for space from the optimizer.
 * @retval  CRYPT_BN_ERR_DIVISOR_ZERO, divisor cannot be 0.
 */
int32_t CRYPT_EAL_BN_Div(CRYPT_BN_BigNum *q, CRYPT_BN_BigNum *r, const CRYPT_BN_BigNum *x,
    const CRYPT_BN_BigNum *y, CRYPT_BN_Optimizer *opt);

/**
 * @ingroup bn
 * @brief   To calculate the maximum common divisor
 * @par Description: gcd(a,b) (a、b!=0)
 *
 * @param   r [OUT] Maximum common divisor
 * @param   a [IN] Big number
 * @param   b [IN] Big number
 * @param   opt [IN] Optimizer
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_BN_INPUT_INVALID, the input parameter of the big number is invalid.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_MEM_ALLOC_FAIL, memory allocation failure.
 * @retval  CRYPT_BN_OPTIMIZER_GET_FAIL, failed to apply for space from the optimizer.
 * @retval  CRYPT_BN_ERR_GCD_NO_ZERO, the maximum common divisor cannot be 0.
 */
int32_t CRYPT_EAL_BN_Gcd(CRYPT_BN_BigNum *r, const CRYPT_BN_BigNum *a,
    const CRYPT_BN_BigNum *b, CRYPT_BN_Optimizer *opt);

/**
 * @ingroup bn
 * @brief   Big number modulo inverse
 *
 * @param   r [OUT] Result
 * @param   x [IN] Big number
 * @param   m [IN] Module
 * @param   opt [IN] Optimizer
 *
 * @retval  CRYPT_SUCCESS.
 * @retval  CRYPT_BN_INPUT_INVALID, the input parameter of the big number is invalid.
 * @retval  CRYPT_NULL_INPUT, invalid null pointer.
 * @retval  CRYPT_MEM_ALLOC_FAIL, memory allocation failure.
 * @retval  CRYPT_BN_OPTIMIZER_GET_FAIL, failed to apply for space from the optimizer.
 * @retval  CRYPT_BN_ERR_NO_INVERSE, cannot calculate the modulo inverse.
 */
int32_t CRYPT_EAL_BN_ModInv(CRYPT_BN_BigNum *r, const CRYPT_BN_BigNum *x,
    const CRYPT_BN_BigNum *m, CRYPT_BN_Optimizer *opt);

/**
 * @ingroup bn
 * @brief   Return the big number modulo exponent of the corresponding length.
 *
 * @param   r [OUT] Output result
 * @param   len [IN] Length
 *
 * @retval Not NULL, success.
 * @retval NULL, failure.
 */
CRYPT_BN_BigNum *CRYPT_EAL_BN_GetRfc3526Prime(CRYPT_BN_BigNum *r, uint32_t len);

/**
 * @ingroup bn
 * @brief   Returns the number of security bits provided by a specific algorithm and a specific key size
 *
 * @param   [OUT] Output result
 * @param   pubLen [IN] Size of the public key
 * @param   prvLen [IN] Size of the private key
 *
 * @retval Number of security bits
 */
int32_t CRYPT_EAL_BN_SecBits(int32_t pubLen, int32_t prvLen);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // CRYPT_EAL_MD_H
