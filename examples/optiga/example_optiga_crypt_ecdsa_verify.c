/**
* \copyright
* MIT License
*
* Copyright (c) 2019 Infineon Technologies AG
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*
* \endcopyright
*
* \author Infineon Technologies AG
*
* \file example_optiga_crypt_ecdsa_verify.c
*
* \brief   This file provides the example for ECDSA verify operation using #optiga_crypt_ecdsa_verify.
*
* \ingroup grOptigaExamples
*
* @{
*/

#include "optiga/optiga_crypt.h"

#ifdef OPTIGA_CRYPT_ECDSA_VERIFY_ENABLED

extern void example_log_execution_status(const char_t* function, uint8_t status);
extern void example_log_function_name(const char_t* function);

/**
 * Callback when optiga_crypt_xxxx operation is completed asynchronously
 */
static volatile optiga_lib_status_t optiga_lib_status;
//lint --e{818} suppress "argument "context" is not used in the sample provided"
static void optiga_crypt_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
    if (NULL != context)
    {
        // callback to upper layer here
    }
}

static uint8_t public_key [] = 
{
    //BitString Format
    0x03,
        //Remaining length
        0x42, 
            //unused bits
            0x00,
            //Compression format
            0x04,
            //NIST-256 Public Key
            0x8b,0x88,0x9c,0x1d,0xd6,0x07,0x58,0x2e,
            0xd6,0xf8,0x2c,0xc2,0xd9,0xbe,0xd0,0xfe,
            0x64,0xf3,0x24,0x5e,0x94,0x7d,0x54,0xcd,
            0x20,0xdc,0x58,0x98,0xcf,0x51,0x31,0x44,
            0x22,0xea,0x01,0xd4,0x0b,0x23,0xb2,0x45,
            0x7c,0x42,0xdf,0x3c,0xfb,0x0d,0x33,0x10,
            0xb8,0x49,0xb7,0xaa,0x0a,0x85,0xde,0xe7,
            0x6a,0xf1,0xac,0x31,0x31,0x1e,0x8c,0x4b
};

//SHA-256 Digest
static uint8_t digest [] = 
{
    0xE9,0x5F,0xB3,0xB1,0x9F,0xA4,0xDD,0x27,0xFE,0xAE,0xB3,0x33,0x40,0x80,0xCE,0x35,
    0xDF,0x3E,0x08,0xF1,0x6F,0x36,0xF3,0x24,0x0E,0xB0,0xB3,0x2F,0xAB,0xD0,0x90,0xCA,
};

// ECDSA Signature
static uint8_t signature [] = 
{
    //DER INTEGER Format
    0x02,
        //Length
        0x20,
            0x39,0xA4,0x70,0xE9,0x32,0x30,0xF5,0x5F,
            0xA4,0xDF,0x8A,0x07,0x36,0x58,0x65,0xC6,
            0xE6,0x1B,0x07,0x51,0xFB,0xC6,0x16,0x05,
            0xEB,0xDF,0x56,0x6D,0xA9,0x50,0x3B,0x24,
    //DER INTEGER Format
    0x02,
        //Length
        0x1E,
            0x49,0x33,0x6C,0x07,0x2B,0xD0,0x40,0x20,
            0x0F,0xD4,0xE0,0x7E,0x67,0x66,0xC4,0xF5,
            0x7F,0x98,0xEC,0x38,0xB8,0xEF,0x44,0x8F,
            0x6A,0xE1,0xFD,0x1E,0x92,0xB4,
};

/**
 * The below example demonstrates the verification of signature using
 * the public key provided by host.
 *
 * Example for #optiga_crypt_ecdsa_verify
 *
 */
void example_optiga_crypt_ecdsa_verify(void)
{
    uint8_t logging_status = 0;
    public_key_from_host_t public_key_details = {
                                                 public_key,
                                                 sizeof(public_key),
                                                 (uint8_t)OPTIGA_ECC_CURVE_NIST_P_256
                                                };

    optiga_lib_status_t return_status;

    optiga_crypt_t * me = NULL;
    example_log_function_name(__FUNCTION__);
    do
    {
        /**
         * 1. Create OPTIGA Crypt Instance
         */
        me = optiga_crypt_create(0, optiga_crypt_callback, NULL);
        if (NULL == me)
        {
            break;
        }

        /**
         * 2. Verify ECDSA signature using public key from host
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_crypt_ecdsa_verify (me,
                                                   digest,
                                                   sizeof(digest),
                                                   signature,
                                                   sizeof(signature),
                                                   OPTIGA_CRYPT_HOST_DATA,
                                                   &public_key_details);

        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == optiga_lib_status) 
        {
            //Wait until the optiga_crypt_ecdsa_verify operation is completed
        }

        if ((OPTIGA_LIB_SUCCESS != optiga_lib_status))
        {
            //Signature verification failed.
            break;
        }
        logging_status = 1;
    } while (FALSE);

    if (me)
    {
        //Destroy the instance after the completion of usecase if not required.
        return_status = optiga_crypt_destroy(me);
    }
    example_log_execution_status(__FUNCTION__,logging_status);
}

#endif  //OPTIGA_CRYPT_ECDSA_VERIFY_ENABLED
/**
* @}
*/
