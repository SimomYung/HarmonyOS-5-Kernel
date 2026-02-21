/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @file obj.h
 * Description: obj功能
 * Create: 2022-09-22
 */

#ifndef BSL_OBJ_H
#define BSL_OBJ_H

#include <stdbool.h>
#include <stdint.h>

// The new OID must be added to the corresponding module.
// If new symmetric encryption algo is added then g_oidTable array
// should be modified accordingly
typedef enum {
    BSL_CID_UNKNOWN = 0, // Unknown alg id
    
    /* Algorithm cids from symmetric algorithm */
    // rc2
    BSL_CID_RC2_ECB = 1,   /* identifies RC2 algorithm in ECB mode */
    BSL_CID_RC2_CBC,   /* identifies RC2 algorithm in CBC mode */
    BSL_CID_RC2_OFB,   /* identifies RC2 algorithm in OFB mode */
    BSL_CID_RC2_CFB,   /* identifies RC2 algorithm in CFB mode */
    // rc4
    BSL_CID_RC4 = 16,          /* identifies the RC4 algorithm */

    // BF
    BSL_CID_BF_ECB = 31,            /* Identifies the Blowfish algorithm in ECB mode */
    BSL_CID_BF_CBC,            /* Identifies the Blowfish algorithm in CBC mode */
    BSL_CID_BF_CFB,            /* Identifies the Blowfish algorithm in CFB mode */
    BSL_CID_BF_OFB,            /* Identifies the Blowfish algorithm in OFB mode */

    // des
    BSL_CID_DES_ECB = 46,      /* identifies DES algorithm in ECB mode */
    BSL_CID_DES_CBC,      /* identifies DES algorithm in CBC mode */
    BSL_CID_DES_OFB,      /* identifies DES algorithm in OFB mode */
    BSL_CID_DES_CFB,      /* identifies DES algorithm in CFB mode */
    BSL_CID_DES_EDE_ECB,  /* identifies 2 key triple DES algorithm in ECB mode */
    BSL_CID_DES_EDE_CBC,  /* identifies 2 key triple DES algorithm in CBC mode */
    BSL_CID_DES_EDE_OFB,  /* identifies 2 key triple DES algorithm in OFB mode */
    BSL_CID_DES_EDE_CFB,  /* identifies 2 key triple DES algorithm in CFB mode */

    // chacha
    BSL_CID_CHACHA20_POLY1305 = 61,

    // scb2
    BSL_CID_SCB2_128_ECB = 76, /* identifies SCB2-128 algorithm in ECB mode */
    BSL_CID_SCB2_128_CBC, /* identifies SCB2-128 algorithm in CBC mode */
    BSL_CID_SCB2_256_ECB, /* identifies SCB2-256 algorithm in ECB mode */
    BSL_CID_SCB2_256_CBC, /* identifies SCB2-256 algorithm in CBC mode */

    // KASUMI
    BSL_CID_KASUMI_ECB = 91,   /* identifies Kasumi algorithm in ECB mode */
    BSL_CID_KASUMI_CBC,   /* identifies Kasumi algorithm in CBC mode */
    BSL_CID_KASUMI_OFB,   /* identifies Kasumi algorithm in OFB mode */
    BSL_CID_KASUMI_CFB,   /* identifies Kasumi algorithm in CFB mode */

    // 3des
    BSL_CID_DES_EDE3_ECB = 141, /* identifies 3 key triple DES algorithm in ECB mode */
    BSL_CID_DES_EDE3_CBC, /* identifies 3 key triple DES algorithm in CBC mode */
    BSL_CID_DES_EDE3_OFB, /* identifies 3 key triple DES algorithm in OFB mode */
    BSL_CID_DES_EDE3_CFB, /* identifies 3 key triple DES algorithm in CFB mode */

    // aes
    BSL_CID_AES128_ECB = 191,   /* identifies AES-128 algorithm in ECB mode */
    BSL_CID_AES128_CBC,   /* identifies AES-128 algorithm in CBC mode */
    BSL_CID_AES128_OFB,   /* identifies AES-128 algorithm in OFB mode */
    BSL_CID_AES128_CFB,   /* identifies AES-128 algorithm in CFB mode */
    BSL_CID_AES192_ECB,   /* identifies AES-192 algorithm in ECB mode */
    BSL_CID_AES192_CBC,   /* identifies AES-192 algorithm in CBC mode */
    BSL_CID_AES192_OFB,   /* identifies AES-192 algorithm in OFB mode */
    BSL_CID_AES192_CFB,   /* identifies AES-192 algorithm in CFB mode */
    BSL_CID_AES256_ECB,   /* identifies AES-256 algorithm in ECB mode */
    BSL_CID_AES256_CBC,   /* identifies AES-256 algorithm in CBC mode */
    BSL_CID_AES256_OFB,   /* identifies AES-256 algorithm in OFB mode */
    BSL_CID_AES256_CFB,   /* identifies AES-256 algorithm in CFB mode */
    BSL_CID_AES128_XTS,
    BSL_CID_AES256_XTS,
    BSL_CID_AES128_GCM,   /* Identifies the AES128 algorithm in GCM mode */
    BSL_CID_AES192_GCM,   /* Identifies the AES128 algorithm in GCM mode */
    BSL_CID_AES256_GCM,   /* Identifies the AES256 algorithm in GCM mode */
    BSL_CID_AES128_CTR,   /* Identifies the AES128 algorithm in CTR mode */
    BSL_CID_AES192_CTR,   /* Identifies the AES128 algorithm in CTR mode */
    BSL_CID_AES256_CTR,   /* Identifies the AES128 algorithm in CTR mode */
    BSL_CID_AES128_CCM,
    BSL_CID_AES192_CCM,
    BSL_CID_AES256_CCM,

    BSL_CID_AES128_WRAP_NOPAD,
    BSL_CID_AES192_WRAP_NOPAD,
    BSL_CID_AES256_WRAP_NOPAD,
    BSL_CID_AES128_WRAP_PAD,
    BSL_CID_AES192_WRAP_PAD,
    BSL_CID_AES256_WRAP_PAD,

    // sm4
    BSL_CID_SM4 = 291,
    BSL_CID_SM4_XTS,
    BSL_CID_SM4_CBC,
    BSL_CID_SM4_ECB,
    BSL_CID_SM4_CTR,
    BSL_CID_SM4_GCM,
    BSL_CID_SM4_CFB,
    BSL_CID_SM4_OFB,

    /* asymmetrical algorithm */
    // rsa
    BSL_CID_RSA = 5001,                     /* identifies the RSA algorithm */
    BSL_CID_RSAES_OAEP,              /* identifies RSAES_OAEP */
    BSL_CID_SHA224WITHRSAENCRYPTION, /* identifies signature using SHA224 and RSA */
    BSL_CID_SHA256WITHRSAENCRYPTION, /* identifies signature using SHA256 and RSA */
    BSL_CID_SHA384WITHRSAENCRYPTION, /* identifies signature using SHA384 and RSA */
    BSL_CID_SHA512WITHRSAENCRYPTION, /* identifies signature using SHA512 and RSA */
    BSL_CID_SM3WITHRSAENCRYPTION,    /* identifies SM3 WITH RSAENCRYPTION */
    BSL_CID_RSASSAPSS,               /* Identifies RSASSAPSS algorithm */
    BSL_CID_MD5WITHRSA,              /* identifies signature using MD5 and RSA */
    BSL_CID_SHA1WITHRSA,             /* identifies signature using SHA1 and RSA */
    BSL_CID_SHA1WITHRSAOLD,          /* identifies signature using SHA1 and RSA (corresponds to old Oid) */

    // dsa
    BSL_CID_DSA = 5051,                     /* identifies the DSA algorithm */
    BSL_CID_DSAWITHSHA1,             /* identifies signature using SHA1 and DSA */
    BSL_CID_DSAWITHSHA1_2,           /* identifies signature using SHA1 and DSA */
    BSL_CID_DSAWITHSHA224,           /* identifies signature using SHA224 and DSA */
    BSL_CID_DSAWITHSHA256,           /* identifies signature using SHA256 and DSA */
    BSL_CID_DSAWITHSHA384,           /* identifies signature using SHA384 and DSA */
    BSL_CID_DSAWITHSHA512,           /* identifies signature using SHA512 and DSA */

    // ecdsa
    BSL_CID_ECDSA = 5101,              /* identifies the ECDSA algorithm */
    BSL_CID_ECDSAWITHSHA1,      /* identifies signature using SHA1 and ECDSA */
    BSL_CID_ECDSAWITHSHA224,    /* identifies signature using SHA224 and ECDSA */
    BSL_CID_ECDSAWITHSHA256,    /* identifies signature using SHA256 and ECDSA */
    BSL_CID_ECDSAWITHSHA384,    /* identifies signature using SHA384 and ECDSA */
    BSL_CID_ECDSAWITHSHA512,    /* identifies signature using SHA512 and ECDSA */
    BSL_CID_ECDSA192,           /* identifies the ECDSA192 algorithm */
    BSL_CID_ECDSA192WITHSHA256, /* identifies signature using SHA256 and ECDSA-192 bit */

    // sm2
    BSL_CID_SM2DSAWITHSM3 = 5151,     /* identifies BSL_CID_SM2DSAWITHSM3 */
    BSL_CID_SM2DSAWITHSHA1,    /* identifies BSL_CID_SM2DSAWITHSHA1 */
    BSL_CID_SM2DSAWITHSHA256,  /* identifies BSL_CID_SM2DSAWITHSHA256 */
    BSL_CID_SM2DSA,            /* identifies SM2 DSA */
    BSL_CID_SM2KEP,            /* BSL_CID_SM2KEP */
    BSL_CID_SM2PKEA,           /* BSL_CID_SM2PKEA */

    // sm9
    BSL_CID_SM9 = 5201,

    // dh
    BSL_CID_DH = 5251,           /* identifies the Diffie-Hellman algorithm */

    // ecdh
    BSL_CID_ECDH = 5266,         /* identifies the EC Diffie-Hellman algorithm */

    // X448
    BSL_CID_X448 = 5281,
    
    // ed448
    BSL_CID_ED448 = 5296,

    // ed25519
    BSL_CID_ED25519 = 5311,   /* Identifies ED25519 algorithm */

    // x25519
    BSL_CID_X25519 = 5326,    /* Identifies X25519 algorithm */

    /* hash algorithm */
    BSL_CID_MD4 = 10001,          /* identifies MD4 hash algorithm */
    BSL_CID_MD5,          /* identifies the MD5 hash algorithm */
    BSL_CID_SHA1,         /* identifies the SHA1 hash algorithm */
    BSL_CID_MD5_SHA1,     /* added for ssl special case in which hash info is not added in the signature */
    BSL_CID_SHA224,       /* identifies the SHA224 hash algorithm */
    BSL_CID_SHA256,       /* identifies the SHA256 hash algorithm */
    BSL_CID_SHA384,       /* identifies the SHA384 hash algorithm */
    BSL_CID_SHA512,       /* identifies the SHA512 hash algorithm */
    BSL_CID_SHA3_224,
    BSL_CID_SHA3_256,
    BSL_CID_SHA3_384,
    BSL_CID_SHA3_512,
    BSL_CID_SHAKE128,
    BSL_CID_SHAKE256,
    BSL_CID_SM3,          /* identifies SM3 hash algorithm */

    /* Message Authentication Code Algorithm */
    // hmac
    BSL_CID_HMAC_MD4 = 10501,  /* identifies hmac with MD4 */
    BSL_CID_HMAC_MD5,          /* identifies hmac with MD5 */
    BSL_CID_HMAC_SHA1,         /* identifies hmac with SHA1 */
    BSL_CID_HMAC_MD5_SHA1,
    BSL_CID_HMAC_SHA224,       /* identifies hmac with SHA224 */
    BSL_CID_HMAC_SHA256,       /* identifies hmac with SHA256 */
    BSL_CID_HMAC_SHA384,       /* identifies hmac with SHA384 */
    BSL_CID_HMAC_SHA512,       /* identifies hmac with SHA512 */
    BSL_CID_HMAC_SHA3_224,     /* identifies hmac with SHA3_224 */
    BSL_CID_HMAC_SHA3_256,     /* identifies hmac with SHA3_256 */
    BSL_CID_HMAC_SHA3_384,     /* identifies hmac with SHA3_384 */
    BSL_CID_HMAC_SHA3_512,     /* identifies hmac with SHA3_512 */
    BSL_CID_HMAC_SM3,          /* identifies hmac with SM3 */
    BSL_CID_HMAC_SHA1_DIGEST,
    BSL_CID_HMAC,

    // cmac
    BSL_CID_CMAC_TDES = 10551, /* identifies CMAC-Triple DES */
    BSL_CID_CMAC_AES,          /* identifies CMAC-AES */
    BSL_CID_CMAC_AES128,
    BSL_CID_CMAC_AES192,
    BSL_CID_CMAC_AES256,
    BSL_CID_CMAC_SM4,

    // gmac
    BSL_CID_GMAC_AES128 = 10601,
    BSL_CID_GMAC_AES192,
    BSL_CID_GMAC_AES256,

    // SIPHASH
    BSL_CID_SIPHASH64 = 10651,
    BSL_CID_SIPHASH128,
  
    // cbc-mac
    BSL_CID_CBC_MAC_SM4 = 10701,

    /* Random number algorithm */
    BSL_CID_RNG_SW = 11001,    /* identifies PRNG */
    // DRBG
    BSL_CID_RAND_SHA1,
    BSL_CID_RAND_SHA224,
    BSL_CID_RAND_SHA256,
    BSL_CID_RAND_SHA384,
    BSL_CID_RAND_SHA512,
    BSL_CID_RAND_HMAC_SHA1,
    BSL_CID_RAND_HMAC_SHA224,
    BSL_CID_RAND_HMAC_SHA256,
    BSL_CID_RAND_HMAC_SHA384,
    BSL_CID_RAND_HMAC_SHA512,
    BSL_CID_RAND_AES128_CTR,
    BSL_CID_RAND_AES192_CTR,
    BSL_CID_RAND_AES256_CTR,
    BSL_CID_RAND_AES128_CTR_DF,
    BSL_CID_RAND_AES192_CTR_DF,
    BSL_CID_RAND_AES256_CTR_DF,
    BSL_CID_RAND_SM3,
    BSL_CID_RAND_SM4_CTR_DF,

    /* Key derivation algorithm */
    BSL_CID_SCRYPT = 11501,   /* Identifieds Scrypt KDF algorithm */
    BSL_CID_KDF2 = 11502,     /* Identifieds KDF2 KDF algorithm */
    BSL_CID_DOT16KDF = 11503, /* Identifieds dot16 KDF algorithm */
    BSL_CID_KDFTLS12 = 11504,

    // hkdf
    BSL_CID_HKDF = 11505,

    /* PKCS series standards */
    BSL_CID_PKCS = 12001,   /* identifies PKCS */
    /* PKCS 1 */
    BSL_CID_PKCS1 = 12002,      /* identifies PKCS1 */
    BSL_CID_MGF1,               /* Identifies MaskGen algorithm */

    /* PKCS 5 */
    BSL_CID_PKCS5 = 12201,      /* identifies PKCS5 */
    BSL_CID_RSADSI,             /* identifies RSADSI */
    BSL_CID_PBKDF2,             /* identifies PBKDF2 */
    BSL_CID_PBMAC1,              /* identifies PBMAC1 */
    BSL_CID_DIGESTALGORITHM,     /* identifies DIGESTALGORITHM */
    BSL_CID_ENCRYPTIONALGORITHM, /* identifies ENCRYPTIONALGORITHM */
    BSL_CID_RC5_CBC_PAD,         /* identifies RC5_CBC_PAD */
    BSL_CID_PBES1,               /* Identifieds PBES1 KDF algorithm */
    BSL_CID_PBES2,                         /* identifies PBES2 */
    BSL_CID_PBE_MD2WITHDESCBC,             /* identifies PBE_MD5WITHDESCBC */
    BSL_CID_PBE_MD2WITHRC2CBC,             /* identifies BSL_CID_PBE_MD2WITHRC2CBC */
    BSL_CID_PBE_MD5WITHDESCBC,             /* identifies PBE_MD5WITHDESCBC */
    BSL_CID_PBE_MD5WITHRC2CBC,             /* identifies PBE_MD5WITHRC2CBC */
    BSL_CID_PBE_SHA1WITHDESCBC,            /* identifies PBE_SHA1WITHDESCBC */
    BSL_CID_PBE_SHA1WITHRC2CBC,            /* identifies PBE_SHA1WITHDESCBC */
    BSL_CID_PBE_SHAWITH128BITRC4,          /* identifies PBE Algo (SHAWITH128BITRC4) */
    BSL_CID_PBE_SHAWITH40BITRC4,           /* identifies PBE Algo (SHAWITH40BITRC4) */
    BSL_CID_PBE_SHAWITH3KEY_TRIPLE_DESCBC, /* identifies PBE Algo (SHAWITH3KEY_TRIPLE_DESCBC) */
    BSL_CID_PBE_SHAWITH2KEY_TRIPLE_DESCBC, /* identifies PBE Algo (SHAWITH2KEY_TRIPLE_DESCBC) */
    BSL_CID_PBE_SHAWITH128BIT_RC2CBC,      /* identifies PBE Algo (SHAWITH128BIT_RC2CBC) */
    BSL_CID_PBE_SHAWITH40BIT_RC2CBC,       /* identifies PBE Algo (SHAWITH40BIT_RC2CBC) */
    BSL_CID_PBE_HMACSHA512WITHAES256_CBC,

    // PKCS 7
    BSL_CID_PKCS7 = 12701,                      /* identifies PKCS7 */
    BSL_CID_PKCS7_SIMPLEDATA,           /* identifies PKCS7 Simple Data */
    BSL_CID_PKCS7_SIGNEDDATA,           /* identifies PKCS7 Signed Data */
    BSL_CID_PKCS7_ENVELOPEDDATA,        /* identifies PKCS7 Enveloped Data */
    BSL_CID_PKCS7_SIGNED_ENVELOPEDDATA, /* identifies PKCS7 Signed Enveloped Data */
    BSL_CID_PKCS7_DIGESTEDDATA,         /* identifies PKCS7 Degested Data */
    BSL_CID_PKCS7_ENCRYPTEDDATA,        /* identifies PKCS7 Encrypted Data */

    // PKCS 9
    BSL_CID_PKCS9 = 12901,                      /* identifies PKCS9 */
    BSL_CID_PKCS9_AT_CONTENTTYPE,       /* identifies PKCS9 Content Type */
    BSL_CID_PKCS9_AT_MESSAGEDIGEST,     /* identifies PKCS9 Message Digest */
    BSL_CID_PKCS9_AT_SIGNINGTIME,       /* identifies PKCS9 Signing time */
    BSL_CID_PKCS9_AT_COUNTERSIGNATURE,  /* identifies PKCS9 Counter Signature */
    BSL_CID_PKCS9_AT_RANDOMNONCE,       /* identifies PKCS9 Signed Enveloped Data */
    BSL_CID_PKCS9_AT_SEQUENCENUMBER,    /* identifies PKCS9 Sequence number */
    BSL_CID_UNSTRUCTURED_NAME,          /* identifies unstructuredName */
    BSL_CID_UNSTRUCTURED_ADDR,          /* identifies unstructuredAddress */

    // PKCS 10
    BSL_CID_CHALLENGE_PWD_ATTR = 13001, /* identifies Challenge PWD Attr */
    BSL_CID_EXTENSIONREQUEST,   /* identifies EXTENSIONREQUEST */

    // PKCS 12
    BSL_CID_PKCS12 = 13201,               /* identifies PKCS12 */
    BSL_CID_PKCS12PBEIDS,            /* identifies PKCS12 PBE */
    BSL_CID_BAGTYPES,                /* identifies Bag Types */
    BSL_CID_KEYBAG,                  /* identifies Key Bag */
    BSL_CID_PKCS8SHROUDEDKEYBAG,     /* identifies Bag Types */
    BSL_CID_CERTBAG,                 /* identifies CERT Bag */
    BSL_CID_CRLBAG,                  /* identifies CRL Bag */
    BSL_CID_SECRETBAG,               /* identifies Secret Bag */
    BSL_CID_SAFECONTENTSBAG,         /* identifies Safe Content Bag */
    BSL_CID_X509CERTIFICATE,         /* identifies x509 Certificate */
    BSL_CID_SDSICERTIFICATE,         /* identifies SDSI Certificate */
    BSL_CID_FRIENDLYNAME,            /* identifies Freidnly Name */
    BSL_CID_LOCALKEYID,              /* identifies Local Key ID */

    // RFC 3279
    BSL_CID_KEYEXCHANGEALGORITHM = 13301,     /* identifies Key exchange algorithm */
    BSL_CID_ANSI_X9_62,               /* identifies ANSI_X9_62 */
    BSL_CID_ECSIGTYPE,                /* identifies ECSIGTYPE */
    BSL_CID_FIELDTYPE,                /* identifies Field Type */
    BSL_CID_PRIME_FIELD,              /* identifies PRIME Field */
    BSL_CID_CHARACTERISTIC_TWO_FIELD, /* identifies Characterstic Two field */
    BSL_CID_CHARACTERISTIC_TWO_BASIS, /* identifies Characterstic Two Basis */
    BSL_CID_GNBASIS,                  /* identifies GNBASIS */
    BSL_CID_TPBASIS,                  /* identifies TPBASIS */
    BSL_CID_PPBASIS,                  /* identifies PPBASIS */
    BSL_CID_PUBLICKEYTYPE,            /* identifies PUBLICKEYTYPE */
    BSL_CID_ELLIPTICCURVE,            /* identifies ELLIPTICCURVE */
    BSL_CID_C_TWOCURVE,               /* identifies C_TWOCURVE */
    BSL_CID_EC_PUBLICKEY,             /* identifies EC_PUBLICKEY */

    // SCEP -- RFC 8894
    BSL_CID_VERISIGN = 13401,       /* identifies VERISIGN */
    BSL_CID_PKI,            /* identifies PKI */
    BSL_CID_ATTRIBUTES,     /* identifies ATTRIBUTES */
    BSL_CID_MESSAGETYPE,    /* identifies MESSAGETYPE */
    BSL_CID_PKISTATUS,      /* identifies PKISTATUS */
    BSL_CID_FAILINFO,       /* identifies FAILINFO */
    BSL_CID_SENDERNONCE,    /* identifies SENDERNONCE */
    BSL_CID_RECIPIENTNONCE, /* identifies RECIPIENTNONCE */
    BSL_CID_TRANSID,        /* identifies TRANSID */
    BSL_CID_EXTENSIONREQ,   /* identifies EXTENSIONREQ */

    // OCSP -- RFC 6960
    BSL_CID_PKIX_OCSP_BASIC = 13501,           /* identifies OCSP_BASIC */
    BSL_CID_PKIX_OCSP_NONCE,           /* identifies OCSP_NONCE */
    BSL_CID_PKIX_OCSP_CRL,             /* identifies OCSP_CRL */
    BSL_CID_PKIX_OCSP_RESPONSE,        /* identifies OCSP_RESPONSE */
    BSL_CID_PKIX_OCSP_NOCHECK,         /* identifies OCSP_NOCHECK */
    BSL_CID_PKIX_OCSP_ARCHIVE_CUTOFF,  /* identifies OCSP_ARCHIVE_CUTOFF */
    BSL_CID_PKIX_OCSP_SERVICE_LOCATOR, /* identifies OCSP_SERVICE_LOCATOR */

    // RFC 5280
    // PKIXEXPLICIT
    BSL_CID_PKIX = 13601,                      /* identifies PKIX */
    BSL_CID_PE,                        /* identifies PE */
    BSL_CID_QT,                        /* identifies QT */
    BSL_CID_KP,                        /* identifies KP */
    BSL_CID_AD,                        /* identifies AD */
    BSL_CID_QT_CPS,                    /* identifies CPS */
    BSL_CID_QT_UNOTICE,                /* identifies UNOTICE */
    BSL_CID_AD_OCSP,                   /* identifies OCSP */
    BSL_CID_AD_CAISSUERS,              /* identifies CAISSUERS */
    BSL_CID_AD_TIMESTAMPING,           /* identifies TIMESTAMPING */
    BSL_CID_AD_CAREPOSITORY,           /* identifies CAREPOSITORY */
    BSL_CID_AT,                        /* identifies AT */
    BSL_CID_AT_NAME,                   /* identifies NAME */
    BSL_CID_AT_SURNAME,                /* identifies SURNAME */
    BSL_CID_AT_GIVENNAME,              /* identifies GIVENNAME */
    BSL_CID_AT_INITIALS,               /* identifies INITIALS */
    BSL_CID_AT_GENERATIONQUALIFIER,    /* identifies GENERATIONQUALIFIER */
    BSL_CID_AT_COMMONNAME,             /* identifies COMMONNAME */
    BSL_CID_AT_LOCALITYNAME,           /* identifies LOCALITYNAME */
    BSL_CID_AT_STATEORPROVINCENAME,    /* identifies STATEORPROVINCENAME */
    BSL_CID_AT_ORGANIZATIONNAME,       /* identifies ORGANIZATIONNAME */
    BSL_CID_AT_ORGANIZATIONALUNITNAME, /* identifies ORGANIZATIONALUNITNAME */
    BSL_CID_AT_TITLE,                  /* identifies TITLE */
    BSL_CID_AT_DNQUALIFIER,            /* identifies DNQUALIFIER */
    BSL_CID_AT_COUNTRYNAME,            /* identifies COUNTRYNAME */
    BSL_CID_AT_SERIALNUMBER,           /* identifies SERIALNUMBER */
    BSL_CID_AT_PSEUDONYM,              /* identifies PSEUDONYM */
    BSL_CID_DOMAINCOMPONENT,           /* identifies DOMAINCOMPONENT */
    BSL_CID_EMAILADDRESS,              /* identifies EMAILADDRESS */
    BSL_CID_AT_DESCRIPTION,            /* identifies description */

    // PKIXIMPLICIT
    BSL_CID_CE = 13701,                            /* identifies CE */
    BSL_CID_CE_AUTHORITYKEYIDENTIFIER,     /* identifies AUTHORITYKEYIDENTIFIER */
    BSL_CID_CE_SUBJECTKEYIDENTIFIER,       /* identifies SUBJECTKEYIDENTIFIER */
    BSL_CID_CE_KEYUSAGE,                   /* identifies KEYUSAGE */
    BSL_CID_CE_PRIVATEKEYUSAGEPERIOD,      /* identifies PRIVATEKEYUSAGEPERIOD */
    BSL_CID_CE_CERTIFICATEPOLICIES,        /* identifies CERTIFICATEPOLICIES */
    BSL_CID_ANYPOLICY,                     /* identifies ANYPOLICY */
    BSL_CID_CE_POLICYMAPPINGS,             /* identifies POLICYMAPPINGS */
    BSL_CID_CE_SUBJECTALTNAME,             /* identifies SUBJECTALTNAME */
    BSL_CID_CE_ISSUERALTNAME,              /* identifies ISSUERALTNAME */
    BSL_CID_CE_SUBJECTDIRECTORYATTRIBUTES, /* identifies SUBJECTDIRECTORYATTRIBUTES */
    BSL_CID_CE_BASICCONSTRAINTS,           /* identifies BASICCONSTRAINTS */
    BSL_CID_CE_NAMECONSTRAINTS,            /* identifies NAMECONSTRAINTS */
    BSL_CID_CE_POLICYCONSTRAINTS,          /* identifies POLICYCONSTRAINTS */
    BSL_CID_CE_CRLDISTRIBUTIONPOINTS,      /* identifies CRLDISTRIBUTIONPOINTS */
    BSL_CID_CE_EXTKEYUSAGE,                /* identifies EXTKEYUSAGE */
    BSL_CID_ANYEXTENDEDKEYUSAGE,           /* identifies ANYEXTENDEDKEYUSAGE */
    BSL_CID_KP_SERVERAUTH,                 /* identifies SERVERAUTH */
    BSL_CID_KP_CLIENTAUTH,                 /* identifies CLIENTAUTH */
    BSL_CID_KP_CODESIGNING,                /* identifies CODESIGNING */
    BSL_CID_KP_EMAILPROTECTION,            /* identifies EMAILPROTECTION */
    BSL_CID_KP_TIMESTAMPING,               /* identifies TIMESTAMPING */
    BSL_CID_KP_OCSPSIGNING,                /* identifies OCSPSIGNING */
    BSL_CID_KP_IPSECIKE,                   /* identifies IPSECIKE */
    BSL_CID_CE_INHIBITANYPOLICY,           /* identifies INHIBITANYPOLICY */
    BSL_CID_CE_FRESHESTCRL,                /* identifies FRESHESTCRL */
    BSL_CID_PE_AUTHORITYINFOACCESS,        /* identifies AUTHORITYINFOACCESS */
    BSL_CID_PE_SUBJECTINFOACCESS,          /* identifies SUBJECTINFOACCESS */
    BSL_CID_CE_CRLNUMBER,                  /* identifies CRLNUMBER */
    BSL_CID_CE_ISSUINGDISTRIBUTIONPOINT,   /* identifies ISSUINGDISTRIBUTIONPOINT */
    BSL_CID_CE_DELTACRLINDICATOR,          /* identifies DELTACRLINDICATOR */
    BSL_CID_CE_CRLREASONS,                 /* identifies CRLREASONS */
    BSL_CID_CE_CERTIFICATEISSUER,          /* identifies CERTIFICATEISSUER */
    BSL_CID_CE_HOLDINSTRUCTIONCODE,        /* identifies HOLDINSTRUCTIONCODE */
    BSL_CID_HOLDINSTRUCTION,               /* identifies HOLDINSTRUCTION */
    BSL_CID_HOLDINSTRUCTION_NONE,          /* identifies HOLDINSTRUCTION_NONE */
    BSL_CID_HOLDINSTRUCTION_CALLISSUER,    /* identifies HOLDINSTRUCTION_CALLISSUER */
    BSL_CID_HOLDINSTRUCTION_REJECT,        /* identifies HOLDINSTRUCTION_REJECT */
    BSL_CID_CE_INVALIDITYDATE,             /* identifies INVALIDITYDATE */
    BSL_CID_PDA_DATEOFBIRTH,               /* identifies DATEOFBIRTH */
    BSL_CID_PDA_PLACEOFBIRTH,              /* identifies PLACEOFBIRTH */
    BSL_CID_PDA_GENDER,                    /* identifies GENDER */
    BSL_CID_PDA_COUNTRYOFCITIZENSHIP,      /* identifies COUNTRYOFCITIZENSHIP */
    BSL_CID_PDA_COUNTRYOFRESIDENCE,        /* identifies COUNTRYOFRESIDENCE */
    BSL_CID_PDA,                           /* identifies PDA */
    BSL_CID_ON_PERMANENTIDENTIFIER,        /* identifies PERMANENTIDENTIFIER */
    BSL_CID_ON,                            /* identifies ON */
    BSL_CID_CE_DOMAININFO,                 /* identifies DOMAININFO */

    // CMP -- RFC 6712
    BSL_CID_PASSWORDBASEDMAC = 13801, /* identifies PWD Based MAC */
    BSL_CID_DHBASEDMAC,       /* identifies DH Based MAC */
    BSL_CID_IT,               /* identifies IT */
    BSL_CID_CAPROTENCCERT,    /* identifies CAPROTENCCERT */
    BSL_CID_SIGNKEYPAIRTYPES, /* identifies Sign KeyPair Types */
    BSL_CID_ENCKEYPAIRTYPES,  /* identifies KeyPair Types */
    BSL_CID_PREFERREDSYMMALG, /* identifies Preferred Symmetric Algo */
    BSL_CID_CAKEYUPDATEINFO,  /* identifies CA Key Update Info */
    BSL_CID_CURRENTCRL,       /* identifies Current CRL */
    BSL_CID_CONFIRMWAITTIME,  /* identifies ConfirmWaitTime */

    // CRMF -- RFC 4211
    BSL_CID_PKIP = 13901,                       /* identifies PKIP */
    BSL_CID_REGCTRL,                    /* identifies REGCTRL */
    BSL_CID_REGCTRL_REGTOKEN,           /* identifies REGTOKEN */
    BSL_CID_REGCTRL_AUTHENTICATOR,      /* identifies AUTHENTICATOR */
    BSL_CID_REGCTRL_PKIPUBLICATIONINFO, /* identifies PKIPUBLICATIONINFO */
    BSL_CID_REGCTRL_PKIARCHIVEOPTIONS,  /* identifies PKIARCHIVEOPTIONS */
    BSL_CID_REGCTRL_OLDCERTID,          /* identifies OLDCERTID */
    BSL_CID_REGCTRL_PROTOCOLENCRKEY,    /* identifies PROTOCOLENCRKEY */
    BSL_CID_REGINFO,                    /* identifies REGINFO */
    BSL_CID_REGINFO_UTF8PAIRS,          /* identifies UTF8PAIRS */
    BSL_CID_REGINFO_CERTREQ,            /* identifies CERTREQ */

    BSL_CID_CERTIFICATEREVOCATIONLIST,      /* Identifies CID_CERTIFICATEREVOCATIONLIST */

    // EV certs
    BSL_CID_AT_STREETADDRESS = 14001,       /* Identifies the streetAddress in EV certs */
    BSL_CID_AT_BUSINESSCATEGORY,    /* Identifies the businessCategory in EV certs */
    BSL_CID_AT_POSTALCODE,          /* Identifies the postalCode in EV certs */
    BSL_CID_JD_LOCALITYNAME,        /* Identifies the streetAddress in EV certs */
    BSL_CID_JD_STATEORPROVINCENAME, /* Identifies the jurisdictionLocalityName in EV certs */
    BSL_CID_JD_COUNTRYNAME,         /* Identifies the jurisdictionCountryName in EV certs */

    // TCG
    BSL_CID_CE_SKAE = 14101,         /* Identifies SKAE extension */

    // netscape
    BSL_CID_NETSCAPE = 14201,
    BSL_CID_NS_CERTEXT,
    BSL_CID_NS_DATATYPE,
    BSL_CID_NS_CERTTYPE,
    BSL_CID_NS_BASEURL,
    BSL_CID_NS_REVOCATIOPNURL,
    BSL_CID_NS_CAREVOCATIONURL,
    BSL_CID_NS_RENEWALURL,
    BSL_CID_NS_CAPOLICYURL,
    BSL_CID_NS_SSLSERVERNAME,
    BSL_CID_NS_COMMENT,
    BSL_CID_NS_CERTSEQUENCE,
    BSL_CID_NS_SGC,

    BSL_CID_ML_KEM = 15501,        // Module-Lattice-Based Key-Encapsulation Mechanism
  
    /* standard constant international curve */
    // BRAINPOOL
    BSL_CID_ECC_BRAINPOOLP256R1 = 16501,
    BSL_CID_ECC_BRAINPOOLP384R1,
    BSL_CID_ECC_BRAINPOOLP512R1,

    // SECP
    BSL_CID_SECP384R1,         /* identifies NIST prime curve 384 */
    BSL_CID_SECP521R1,         /* identifies NIST prime curve 521 */

    // RFC 3279
    BSL_CID_C2PNB163V1,               /* identifies RFC 3279 C2PNB163V1 */
    BSL_CID_C2PNB163V2,               /* identifies RFC 3279 C2PNB163V2 */
    BSL_CID_C2PNB163V3,               /* identifies RFC 3279 C2PNB163V3 */
    BSL_CID_C2PNB176W1,               /* identifies RFC 3279 C2PNB176W1 */
    BSL_CID_C2TNB191V1,               /* identifies RFC 3279 C2TNB191V1 */
    BSL_CID_C2TNB191V2,               /* identifies RFC 3279 C2TNB191V2 */
    BSL_CID_C2TNB191V3,               /* identifies RFC 3279 C2TNB191V3 */
    BSL_CID_C2ONB191V4,               /* identifies RFC 3279 C2ONB191V4 */
    BSL_CID_C2ONB191V5,               /* identifies RFC 3279 C2ONB191V5 */
    BSL_CID_C2PNB208W1,               /* identifies RFC 3279 C2PNB208W1 */
    BSL_CID_C2TNB239V1,               /* identifies RFC 3279 C2TNB239V1 */
    BSL_CID_C2TNB239V2,               /* identifies RFC 3279 C2TNB239V2 */
    BSL_CID_C2TNB239V3,               /* identifies RFC 3279 C2TNB239V3 */
    BSL_CID_C2ONB239V4,               /* identifies RFC 3279 C2ONB239V4 */
    BSL_CID_C2ONB239V5,               /* identifies RFC 3279 C2ONB239V5 */
    BSL_CID_C2PNB272W1,               /* identifies RFC 3279 C2PNB272W1 */
    BSL_CID_C2PNB304W1,               /* identifies RFC 3279 C2PNB304W1 */
    BSL_CID_C2TNB359V1,               /* identifies RFC 3279 C2TNB359V1 */
    BSL_CID_C2PNB368W1,               /* identifies RFC 3279 C2PNB368W1 */
    BSL_CID_C2TNB431R1,               /* identifies RFC 3279 C2TNB431R1 */
    BSL_CID_PRIMECURVE,               /* identifies RFC 3279 PRIMECURVE */
    BSL_CID_PRIME192V1,               /* identifies RFC 3279 PRIME192V1 */
    BSL_CID_PRIME192V2,               /* identifies RFC 3279 PRIME192V2 */
    BSL_CID_PRIME192V3,               /* identifies RFC 3279 PRIME192V3 */
    BSL_CID_PRIME239V1,               /* identifies RFC 3279 PRIME239V1 */
    BSL_CID_PRIME239V2,               /* identifies RFC 3279 PRIME239V2 */
    BSL_CID_PRIME239V3,               /* identifies RFC 3279 PRIME239V3 */
    BSL_CID_PRIME256V1,               /* identifies RFC 3279 PRIME256V1 */

    BSL_CID_NIST_PRIME224,  /* NIST Curve P-224 */
    BSL_CID_NIST_C2PNB163K, /* NIST Binary Curve 163K */
    BSL_CID_NIST_C2PNB163B, /* NIST Binary Curve 163B */
    BSL_CID_NIST_C2TNB233K, /* NIST Binary Curve 233K */
    BSL_CID_NIST_C2TNB233B, /* NIST Binary Curve 233B */
    BSL_CID_NIST_C2PNB283K, /* NIST Binary Curve 283K */
    BSL_CID_NIST_C2PNB283B, /* NIST Binary Curve 283B */
    BSL_CID_NIST_C2TNB409K, /* NIST Binary Curve 409K */
    BSL_CID_NIST_C2TNB409B, /* NIST Binary Curve 409B */
    BSL_CID_NIST_C2PNB571K, /* NIST Binary Curve 571K */
    BSL_CID_NIST_C2PNB571B, /* NIST Binary Curve 571B */

    // standard constant sm series curve
    BSL_CID_SM2PRIME256,       /* identifies BSL_CID_PRIME256SM2 */
    BSL_CID_ECC_SM9,
    BSL_CID_EC192WAPI,

    /* standard constant prime */
    BSL_CID_DH_RFC2409_768 = 17001,
    BSL_CID_DH_RFC2409_1024,
    BSL_CID_DH_RFC3526_1536,
    BSL_CID_DH_RFC3526_2048,
    BSL_CID_DH_RFC3526_3072,
    BSL_CID_DH_RFC3526_4096,
    BSL_CID_DH_RFC3526_6144,
    BSL_CID_DH_RFC3526_8192,
    BSL_CID_DH_RFC7919_2048,
    BSL_CID_DH_RFC7919_3072,
    BSL_CID_DH_RFC7919_4096,
    BSL_CID_DH_RFC7919_6144,
    BSL_CID_DH_RFC7919_8192,

    BSL_CID_HARDWAREMODULENAME = 17101,

    BSL_CID_MAX,
    BSL_CID_EXTEND = 0x60000000,
} BslCid;

typedef struct BslOidString BslOidString;
typedef struct BslPrvOidInfo BslPrvOidInfo;

/**
  * @ingroup bsl_obj
  * @brief 获取给定算法OID的CID
  *
  * @param oidStr [IN] 算法oid结构体
  *
  * @retval  算法oid值 成功
  * @retval  BSL_CID_UNKNOWN 失败
  */
BslCid BSL_OBJ_GetCID(const BslOidString *oidStr);

/**
  * @ingroup bsl_obj
  * @brief 获取给定算法CID的OID结构
  *
  * @param ulCID [IN] 算法cid
  *
  * @retval  算法oid结构体 成功
  * @retval  失败返回NULL 失败
  */
BslOidString *BSL_OBJ_GetOID(BslCid ulCID);

/**
  * @ingroup bsl_obj
  * @brief 此函数比较两个BslOidString结构，即检查两个结构是否相同。
  *
  * @param pstDestOID [IN] 算法OID结构体
  * @param pstSrcOID [IN] 算法OID结构体
  *
  * @retval  true 成功
  * @retval  false 失败
  */
bool BSL_OBJ_CompareOid(const BslOidString *pstDestOID, const BslOidString *pstSrcOID);

/**
  * @ingroup bsl_obj
  * @brief 拷贝oid 八进制
  *
  * @param pstDestOID [IN] 算法OID结构体
    @param pstSrcOID [IN] 算法OID结构体
  *
  * @retval  BSL_SUCCESS 成功
  * @retval  BSL_INVALID_ARG 参数不合法
  * @retval  BSL_MALLOC_FAIL 内存分配失败
  */
int32_t BSL_OBJ_CpyOidOcts(BslOidString *pstDestOID, const BslOidString *pstSrcOID);

/**
  * @ingroup bsl_obj
  * @brief 创建并设置可在X509证书中使用的私有OID信息
  *
  * @param aPrvOidInfo [IN] 私有OID信息
    @param uiCount [IN] 私有OID信息数量
  *
  * @retval  BSL_SUCCESS 成功
  * @retval  BSL_INVALID_ARG 参数不合法
  * @retval  BSL_MALLOC_FAIL 内存分配失败
  */
int32_t BSL_OBJ_NewPrivateOID(BslPrvOidInfo *aPrvOidInfo, uint32_t uiCount);

/**
  * @ingroup bsl_obj
  * @brief 获取X509证书中使用的私有OID信息
  *
  * @param void
  *
  * @retval  BslPrvOidInfo
  * @retval  NULL
  */
const BslPrvOidInfo *BSL_OBJ_GetPrivateOID(void);

/**
  * @ingroup bsl_obj
  * @brief 释放所有私有OID信息表
  *
  * @param void
  *
  * @retval  BslPrvOidInfo
  * @retval  NULL
  */
void BSL_OBJ_FreePrivateOID(void);

/**
 * @ingroup bsl_obj
 * @brief 通过签名ID获取摘要ID
 *
 * @param signAlg [IN] 签名ID
 *
 * @retval 摘要ID
 */
BslCid BSL_OBJ_GetHashIdFromSignId(BslCid signAlg);

/**
 * @ingroup bsl_obj
 * @brief 通过签名ID获取算法ID
 *
 * @param signAlg [IN] 签名ID
 *
 * @retval 算法ID
 */
BslCid BSL_OBJ_GetAsymAlgIdFromSignId(BslCid signAlg);

/**
 * @ingroup bsl_obj
 * @brief 通过非对称算法ID和摘要ID获取签名ID
 *
 * @param asymAlg [IN] 非对称算法ID
   @param hashAlg [IN] 摘要ID
 *
 * @retval 签名ID
 */
BslCid BSL_OBJ_GetSignIdFromHashAndAsymId(BslCid asymAlg, BslCid hashAlg);

/**
 * @ingroup bsl_obj
 * @brief 通过hmac算法ID获取hashID
 *
 * @param ulAlgType [IN] hmac算法ID
 *
 * @retval hashID
 */
BslCid BSL_OBJ_GetHashIdFromMacId(BslCid ulAlgType);

#endif
