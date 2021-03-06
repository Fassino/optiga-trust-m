## Usage
Start the `protected_update_data_generator.exe` with following options, in order to generate the data set for protected update data set.
```
    payload_version=%%d         <default: 0>
    trust_anchor_oid=%%s        <default: E0E8> 
                                <note : Reference to the object which holds the public key to validate the protected update>
    target_oid=%%s              <default: E0E2> 
                                <note : Reference to the object which shall be updated>						
    offset=%%d                  <default: 0> 
                                <note : Update object at certain offset>
    write_type=%%d              <default: 1> 
                                <options : Write (1), EraseAndWrite (2)>
				<note : Write or EraseandWrite data object>			
    sign_algo=%%s               <default: ES_256> 							
                                <options : ES_256 , RSA-SSA-PKCS1-V1_5-SHA-256> 
				<note : What is the signature algorithm used to sign the update payload>
    priv_key=%%s                <default: NULL> 		
                                <note : Private key file (pem format)used to sign the payload. Corresponding certificate containing public key should be written to trust_anchor>
    payload=%%s                 <default: NULL>
                                <note : Path to file containing readable content>
```

To parse the resulting data on the host side (MCU) you can refer to [this repository](https://github.com/Infineon/optiga-manifest-parser)

## Example :											

```console
C:\optiga-trust-m\examples\tools\protected_update_data_set\bin> protected_update_data_generator.exe ^
payload_version=3 ^
trust_anchor_oid=E0E8 ^
target_oid=E0E1 ^
offset=01 ^
write_type=1 ^
sign_algo=RSA_SSA_PKCS1_V1_5_SHA_256 ^
priv_key=<pem private key> ^
payload=<text file with readable data>
```

* Generate a manifest data with payload version 3, trust anchor 0xE0E8. The manifest is signed using "priv_key" and RSA_SSA_PKCS1_V1_5_SHA_256 is the signing algorithm
* Fragments of data is generated from text file provided in option "payload"
* The generated fragments is written at target OID 0xE0E1 from offset 1
	
## Sample :

A sample script demonstrating the usage of the tool is available [here](https://github.com/Infineon/optiga-trust-m/blob/master/examples/tools/protected_update_data_set/samples/sample.bat)
	
## Limitations
* Only SHA-256 digest algorithm is supported for hash calculation
* Support for manifest format 1 (CDDL CBOR) only
    	
## Environment
* The available executable is built using microsoft visual studio 2010 in windows 10 (64 bit)
* mbedTLS 2.7.0 is used for crypto operation.
* The following MACRO must be disabled/ enabled when using mbedTLS for this tool.
  * Enable :
    - `MBEDTLS_FS_IO`
    - `MBEDTLS_PEM_PARSE_C`
    - `MBEDTLS_BASE64_C`
    - `MBEDTLS_ECDSA_DETERMINISTIC`
    - `MBEDTLS_HMAC_DRBG_C`
    - `MBEDTLS_ECDSA_DETERMINISTIC_DEBUG`
    - `MBEDTLS_RSA_C`
    - `MBEDTLS_PKCS1_V15`
    - `MBEDTLS_PK_RSA_ALT_SUPPORT`
    - `MBEDTLS_TIMING_C`
			
  * Disable
    - `MBEDTLS_NO_PLATFORM_ENTROPY` in mbedTLS_config.h
