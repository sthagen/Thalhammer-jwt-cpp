#include "jwt-cpp/jwt.h"
#include <gtest/gtest.h>

namespace {
	extern std::string google_cert;
	extern std::string google_cert_base64_der;
	extern std::string google_public_key;
} // namespace

TEST(HelperTest, Cert2Pubkey) {
	auto key = jwt::helper::extract_pubkey_from_cert(google_cert);
	ASSERT_EQ(google_public_key, key);
}

TEST(HelperTest, Base64DER2PemCert) {
	auto cert_pem = jwt::helper::convert_base64_der_to_pem(google_cert_base64_der);
	ASSERT_EQ(google_cert, cert_pem);
}

TEST(HelperTest, DER2PemCert) {
	auto decoded =
		jwt::base::decode<jwt::alphabet::base64>(jwt::base::pad<jwt::alphabet::base64>(google_cert_base64_der));
	auto cert_pem = jwt::helper::convert_der_to_pem(decoded);
	ASSERT_EQ(google_cert, cert_pem);
}

TEST(HelperTest, RsaFromComponents) {
	const std::string public_key_expected =
		R"(-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwoB3iVm4RW+6StkR+nut
x1fQevu2+t0Fu6KBcbvhfyHSXy7w0nJOdTT4jWLjStpRkNQBPZwMwHH35i+21gdn
JtDe/xfO8IX9McFmyodlBUcqX8CruIzDv9AXf2OjXPBG+4aq+03XKl5/muATl32+
+301Vw1dXoGYNeoWQqLTsHT3WS3tOOf+ehuzNuZ+rj+ephaD3lMBToEArrtC9R91
KTTN6YSAOK48NxTA8CfOMFK5itxfIqB5+E9OSQTidXyqLyoeA+xxTKMqYfxvypEe
k1oueAhY9u67NCBdmuavxtfyvwp7+o6Sd+NsewxAhmRKFexw13KOYzDhC+9aMJcu
JQIDAQAB
-----END PUBLIC KEY-----
)";
	const std::string modulus =
		R"(AMKAd4lZuEVvukrZEfp7rcdX0Hr7tvrdBbuigXG74X8h0l8u8NJyTnU0-I1i40raUZDUAT2cDMBx9-YvttYHZybQ3v8XzvCF_THBZsqHZQVHKl_Aq7iMw7_QF39jo1zwRvuGqvtN1ypef5rgE5d9vvt9NVcNXV6BmDXqFkKi07B091kt7Tjn_nobszbmfq4_nqYWg95TAU6BAK67QvUfdSk0zemEgDiuPDcUwPAnzjBSuYrcXyKgefhPTkkE4nV8qi8qHgPscUyjKmH8b8qRHpNaLngIWPbuuzQgXZrmr8bX8r8Ke_qOknfjbHsMQIZkShXscNdyjmMw4QvvWjCXLiU)";
	const std::string exponent = R"(AQAB)";

	const auto public_key = jwt::helper::create_public_key_from_rsa_components(modulus, exponent);

	ASSERT_EQ(public_key, public_key_expected);
}

TEST(HelperTest, EcFromComponents) {
	const std::string public_key_expected =
		R"(-----BEGIN PUBLIC KEY-----
MHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE0uQ1+1P/wmhOuYvVtTogHOSBLC05IvK7
L6sTPIX8Dl4Bg9nhC3v/FsgifjnXnijUxVJSyWa9SuxwBonUhg6SiCEv+ixb74hj
DesC4D7OwllVcnkDJmOy/NMx4N7yDPJp
-----END PUBLIC KEY-----
)";
	const std::string curve = R"(P-384)";
	const std::string x = R"(0uQ1-1P_wmhOuYvVtTogHOSBLC05IvK7L6sTPIX8Dl4Bg9nhC3v_FsgifjnXnijU)";
	const std::string y = R"(xVJSyWa9SuxwBonUhg6SiCEv-ixb74hjDesC4D7OwllVcnkDJmOy_NMx4N7yDPJp)";

	const auto public_key = jwt::helper::create_public_key_from_ec_components(curve, x, y);

	ASSERT_EQ(public_key, public_key_expected);
}

TEST(HelperTest, ErrorCodeMessages) {
	ASSERT_EQ(std::error_code(jwt::error::rsa_error::ok).message(), "no error");
	ASSERT_EQ(std::error_code(static_cast<jwt::error::rsa_error>(-1)).message(), "unknown RSA error");
	ASSERT_EQ(std::error_code(jwt::error::rsa_error::ok).category().name(), std::string("rsa_error"));

	ASSERT_EQ(std::error_code(jwt::error::ecdsa_error::ok).message(), "no error");
	ASSERT_EQ(std::error_code(static_cast<jwt::error::ecdsa_error>(-1)).message(), "unknown ECDSA error");
	ASSERT_EQ(std::error_code(jwt::error::ecdsa_error::ok).category().name(), std::string("ecdsa_error"));

	ASSERT_EQ(std::error_code(jwt::error::signature_verification_error::ok).message(), "no error");
	ASSERT_EQ(std::error_code(static_cast<jwt::error::signature_verification_error>(-1)).message(),
			  "unknown signature verification error");
	ASSERT_EQ(std::error_code(jwt::error::signature_verification_error::ok).category().name(),
			  std::string("signature_verification_error"));

	ASSERT_EQ(std::error_code(jwt::error::signature_generation_error::ok).message(), "no error");
	ASSERT_EQ(std::error_code(static_cast<jwt::error::signature_generation_error>(-1)).message(),
			  "unknown signature generation error");
	ASSERT_EQ(std::error_code(jwt::error::signature_generation_error::ok).category().name(),
			  std::string("signature_generation_error"));

	ASSERT_EQ(std::error_code(jwt::error::token_verification_error::ok).message(), "no error");
	ASSERT_EQ(std::error_code(static_cast<jwt::error::token_verification_error>(-1)).message(),
			  "unknown token verification error");
	ASSERT_EQ(std::error_code(jwt::error::token_verification_error::ok).category().name(),
			  std::string("token_verification_error"));

	int i = 10;
	for (i = 10; i < 21; i++) {
		ASSERT_NE(std::error_code(static_cast<jwt::error::rsa_error>(i)).message(),
				  std::error_code(static_cast<jwt::error::rsa_error>(-1)).message());
	}
	ASSERT_EQ(std::error_code(static_cast<jwt::error::rsa_error>(i)).message(),
			  std::error_code(static_cast<jwt::error::rsa_error>(-1)).message());

	for (i = 10; i < 24; i++) {
		ASSERT_NE(std::error_code(static_cast<jwt::error::ecdsa_error>(i)).message(),
				  std::error_code(static_cast<jwt::error::ecdsa_error>(-1)).message());
	}
	ASSERT_EQ(std::error_code(static_cast<jwt::error::ecdsa_error>(i)).message(),
			  std::error_code(static_cast<jwt::error::ecdsa_error>(-1)).message());

	for (i = 10; i < 18; i++) {
		ASSERT_NE(std::error_code(static_cast<jwt::error::signature_verification_error>(i)).message(),
				  std::error_code(static_cast<jwt::error::signature_verification_error>(-1)).message());
	}
	ASSERT_EQ(std::error_code(static_cast<jwt::error::signature_verification_error>(i)).message(),
			  std::error_code(static_cast<jwt::error::signature_verification_error>(-1)).message());

	for (i = 10; i < 24; i++) {
		ASSERT_NE(std::error_code(static_cast<jwt::error::signature_generation_error>(i)).message(),
				  std::error_code(static_cast<jwt::error::signature_generation_error>(-1)).message());
	}
	ASSERT_EQ(std::error_code(static_cast<jwt::error::signature_generation_error>(i)).message(),
			  std::error_code(static_cast<jwt::error::signature_generation_error>(-1)).message());

	for (i = 10; i < 16; i++) {
		ASSERT_NE(std::error_code(static_cast<jwt::error::token_verification_error>(i)).message(),
				  std::error_code(static_cast<jwt::error::token_verification_error>(-1)).message());
	}
	ASSERT_EQ(std::error_code(static_cast<jwt::error::token_verification_error>(i)).message(),
			  std::error_code(static_cast<jwt::error::token_verification_error>(-1)).message());
}

namespace {
	std::string google_cert =
// This is to handle the different subject alternate name ordering
// see https://github.com/wolfSSL/wolfssl/issues/4397
#if defined(LIBWOLFSSL_VERSION_HEX) && LIBWOLFSSL_VERSION_HEX < 0x05007000
		R"(-----BEGIN CERTIFICATE-----
MIIFfTCCBOagAwIBAgIKYFOB9QABAACIvTANBgkqhkiG9w0BAQUFADBGMQswCQYD
VQQGEwJVUzETMBEGA1UEChMKR29vZ2xlIEluYzEiMCAGA1UEAxMZR29vZ2xlIElu
dGVybmV0IEF1dGhvcml0eTAeFw0xMzA1MjIxNTQ5MDRaFw0xMzEwMzEyMzU5NTla
MGYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1N
b3VudGFpbiBWaWV3MRMwEQYDVQQKEwpHb29nbGUgSW5jMRUwEwYDVQQDDAwqLmdv
b2dsZS5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARmSpIUbCqhUBq1UwnR
Ai7/TNSk6W8JmasR+I0r/NLDYv5yApbAz8HXXN8hDdurMRP6Jy1Q0UIKmyls8HPH
exoCo4IDlzCCA5MwggLDBgNVHREEggK6MIICtoIMKi5nb29nbGUuY29tgg0qLmFu
ZHJvaWQuY29tghYqLmFwcGVuZ2luZS5nb29nbGUuY29tghIqLmNsb3VkLmdvb2ds
ZS5jb22CFiouZ29vZ2xlLWFuYWx5dGljcy5jb22CCyouZ29vZ2xlLmNhggsqLmdv
b2dsZS5jbIIOKi5nb29nbGUuY28uaW6CDiouZ29vZ2xlLmNvLmpwgg4qLmdvb2ds
ZS5jby51a4IPKi5nb29nbGUuY29tLmFygg8qLmdvb2dsZS5jb20uYXWCDyouZ29v
Z2xlLmNvbS5icoIPKi5nb29nbGUuY29tLmNvgg8qLmdvb2dsZS5jb20ubXiCDyou
Z29vZ2xlLmNvbS50coIPKi5nb29nbGUuY29tLnZuggsqLmdvb2dsZS5kZYILKi5n
b29nbGUuZXOCCyouZ29vZ2xlLmZyggsqLmdvb2dsZS5odYILKi5nb29nbGUuaXSC
CyouZ29vZ2xlLm5sggsqLmdvb2dsZS5wbIILKi5nb29nbGUucHSCDyouZ29vZ2xl
YXBpcy5jboIUKi5nb29nbGVjb21tZXJjZS5jb22CDSouZ3N0YXRpYy5jb22CDCou
dXJjaGluLmNvbYIQKi51cmwuZ29vZ2xlLmNvbYIWKi55b3V0dWJlLW5vY29va2ll
LmNvbYINKi55b3V0dWJlLmNvbYIWKi55b3V0dWJlZWR1Y2F0aW9uLmNvbYILKi55
dGltZy5jb22CC2FuZHJvaWQuY29tggRnLmNvggZnb28uZ2yCFGdvb2dsZS1hbmFs
eXRpY3MuY29tggpnb29nbGUuY29tghJnb29nbGVjb21tZXJjZS5jb22CCnVyY2hp
bi5jb22CCHlvdXR1LmJlggt5b3V0dWJlLmNvbYIUeW91dHViZWVkdWNhdGlvbi5j
b20wHQYDVR0OBBYEFFN409DVTUYFOWYp0Rxq5cqBhJ6GMB8GA1UdIwQYMBaAFL/A
MOv1QxE+Z7qekfv8atrjaxIkMFsGA1UdHwRUMFIwUKBOoEyGSmh0dHA6Ly93d3cu
Z3N0YXRpYy5jb20vR29vZ2xlSW50ZXJuZXRBdXRob3JpdHkvR29vZ2xlSW50ZXJu
ZXRBdXRob3JpdHkuY3JsMA4GA1UdDwEB/wQEAwIHgDAdBgNVHSUEFjAUBggrBgEF
BQcDAQYIKwYBBQUHAwIwDQYJKoZIhvcNAQEFBQADgYEAAyfQrePfKEL5f67KHp4A
FrYSrvGJjn+ZQHzzoivX2/GW5Iw0oPqf+Jjy8eO2ufQGG5bz++YnK50WQhwQNRgT
iK9AyWx8gvtI4bDo569UuabTZqJEdaPlPbQOz5I6m55hQbc0Fwc//kjOFRghAlsb
OVBpRpkExpB4LxmyUw0tYjY=
-----END CERTIFICATE-----
)";
#else
		R"(-----BEGIN CERTIFICATE-----
MIIF8DCCBVmgAwIBAgIKYFOB9QABAACIvTANBgkqhkiG9w0BAQUFADBGMQswCQYD
VQQGEwJVUzETMBEGA1UEChMKR29vZ2xlIEluYzEiMCAGA1UEAxMZR29vZ2xlIElu
dGVybmV0IEF1dGhvcml0eTAeFw0xMzA1MjIxNTQ5MDRaFw0xMzEwMzEyMzU5NTla
MGYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1N
b3VudGFpbiBWaWV3MRMwEQYDVQQKEwpHb29nbGUgSW5jMRUwEwYDVQQDFAwqLmdv
b2dsZS5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARmSpIUbCqhUBq1UwnR
Ai7/TNSk6W8JmasR+I0r/NLDYv5yApbAz8HXXN8hDdurMRP6Jy1Q0UIKmyls8HPH
exoCo4IECjCCBAYwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMAsGA1Ud
DwQEAwIHgDAdBgNVHQ4EFgQUU3jT0NVNRgU5ZinRHGrlyoGEnoYwHwYDVR0jBBgw
FoAUv8Aw6/VDET5nup6R+/xq2uNrEiQwWwYDVR0fBFQwUjBQoE6gTIZKaHR0cDov
L3d3dy5nc3RhdGljLmNvbS9Hb29nbGVJbnRlcm5ldEF1dGhvcml0eS9Hb29nbGVJ
bnRlcm5ldEF1dGhvcml0eS5jcmwwZgYIKwYBBQUHAQEEWjBYMFYGCCsGAQUFBzAC
hkpodHRwOi8vd3d3LmdzdGF0aWMuY29tL0dvb2dsZUludGVybmV0QXV0aG9yaXR5
L0dvb2dsZUludGVybmV0QXV0aG9yaXR5LmNydDAMBgNVHRMBAf8EAjAAMIICwwYD
VR0RBIICujCCAraCDCouZ29vZ2xlLmNvbYINKi5hbmRyb2lkLmNvbYIWKi5hcHBl
bmdpbmUuZ29vZ2xlLmNvbYISKi5jbG91ZC5nb29nbGUuY29tghYqLmdvb2dsZS1h
bmFseXRpY3MuY29tggsqLmdvb2dsZS5jYYILKi5nb29nbGUuY2yCDiouZ29vZ2xl
LmNvLmlugg4qLmdvb2dsZS5jby5qcIIOKi5nb29nbGUuY28udWuCDyouZ29vZ2xl
LmNvbS5hcoIPKi5nb29nbGUuY29tLmF1gg8qLmdvb2dsZS5jb20uYnKCDyouZ29v
Z2xlLmNvbS5jb4IPKi5nb29nbGUuY29tLm14gg8qLmdvb2dsZS5jb20udHKCDyou
Z29vZ2xlLmNvbS52boILKi5nb29nbGUuZGWCCyouZ29vZ2xlLmVzggsqLmdvb2ds
ZS5mcoILKi5nb29nbGUuaHWCCyouZ29vZ2xlLml0ggsqLmdvb2dsZS5ubIILKi5n
b29nbGUucGyCCyouZ29vZ2xlLnB0gg8qLmdvb2dsZWFwaXMuY26CFCouZ29vZ2xl
Y29tbWVyY2UuY29tgg0qLmdzdGF0aWMuY29tggwqLnVyY2hpbi5jb22CECoudXJs
Lmdvb2dsZS5jb22CFioueW91dHViZS1ub2Nvb2tpZS5jb22CDSoueW91dHViZS5j
b22CFioueW91dHViZWVkdWNhdGlvbi5jb22CCyoueXRpbWcuY29tggthbmRyb2lk
LmNvbYIEZy5jb4IGZ29vLmdsghRnb29nbGUtYW5hbHl0aWNzLmNvbYIKZ29vZ2xl
LmNvbYISZ29vZ2xlY29tbWVyY2UuY29tggp1cmNoaW4uY29tggh5b3V0dS5iZYIL
eW91dHViZS5jb22CFHlvdXR1YmVlZHVjYXRpb24uY29tMA0GCSqGSIb3DQEBBQUA
A4GBAAMn0K3j3yhC+X+uyh6eABa2Eq7xiY5/mUB886Ir19vxluSMNKD6n/iY8vHj
trn0BhuW8/vmJyudFkIcEDUYE4ivQMlsfIL7SOGw6OevVLmm02aiRHWj5T20Ds+S
OpueYUG3NBcHP/5IzhUYIQJbGzlQaUaZBMaQeC8ZslMNLWI2
-----END CERTIFICATE-----
)";
#endif

	std::string google_cert_base64_der = "MIIF8DCCBVmgAwIBAgIKYFOB9QABAACIvTANBgkqhkiG9w0BAQUFADBGMQswCQYD"
										 "VQQGEwJVUzETMBEGA1UEChMKR29vZ2xlIEluYzEiMCAGA1UEAxMZR29vZ2xlIElu"
										 "dGVybmV0IEF1dGhvcml0eTAeFw0xMzA1MjIxNTQ5MDRaFw0xMzEwMzEyMzU5NTla"
										 "MGYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1N"
										 "b3VudGFpbiBWaWV3MRMwEQYDVQQKEwpHb29nbGUgSW5jMRUwEwYDVQQDFAwqLmdv"
										 "b2dsZS5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARmSpIUbCqhUBq1UwnR"
										 "Ai7/TNSk6W8JmasR+I0r/NLDYv5yApbAz8HXXN8hDdurMRP6Jy1Q0UIKmyls8HPH"
										 "exoCo4IECjCCBAYwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMAsGA1Ud"
										 "DwQEAwIHgDAdBgNVHQ4EFgQUU3jT0NVNRgU5ZinRHGrlyoGEnoYwHwYDVR0jBBgw"
										 "FoAUv8Aw6/VDET5nup6R+/xq2uNrEiQwWwYDVR0fBFQwUjBQoE6gTIZKaHR0cDov"
										 "L3d3dy5nc3RhdGljLmNvbS9Hb29nbGVJbnRlcm5ldEF1dGhvcml0eS9Hb29nbGVJ"
										 "bnRlcm5ldEF1dGhvcml0eS5jcmwwZgYIKwYBBQUHAQEEWjBYMFYGCCsGAQUFBzAC"
										 "hkpodHRwOi8vd3d3LmdzdGF0aWMuY29tL0dvb2dsZUludGVybmV0QXV0aG9yaXR5"
										 "L0dvb2dsZUludGVybmV0QXV0aG9yaXR5LmNydDAMBgNVHRMBAf8EAjAAMIICwwYD"
										 "VR0RBIICujCCAraCDCouZ29vZ2xlLmNvbYINKi5hbmRyb2lkLmNvbYIWKi5hcHBl"
										 "bmdpbmUuZ29vZ2xlLmNvbYISKi5jbG91ZC5nb29nbGUuY29tghYqLmdvb2dsZS1h"
										 "bmFseXRpY3MuY29tggsqLmdvb2dsZS5jYYILKi5nb29nbGUuY2yCDiouZ29vZ2xl"
										 "LmNvLmlugg4qLmdvb2dsZS5jby5qcIIOKi5nb29nbGUuY28udWuCDyouZ29vZ2xl"
										 "LmNvbS5hcoIPKi5nb29nbGUuY29tLmF1gg8qLmdvb2dsZS5jb20uYnKCDyouZ29v"
										 "Z2xlLmNvbS5jb4IPKi5nb29nbGUuY29tLm14gg8qLmdvb2dsZS5jb20udHKCDyou"
										 "Z29vZ2xlLmNvbS52boILKi5nb29nbGUuZGWCCyouZ29vZ2xlLmVzggsqLmdvb2ds"
										 "ZS5mcoILKi5nb29nbGUuaHWCCyouZ29vZ2xlLml0ggsqLmdvb2dsZS5ubIILKi5n"
										 "b29nbGUucGyCCyouZ29vZ2xlLnB0gg8qLmdvb2dsZWFwaXMuY26CFCouZ29vZ2xl"
										 "Y29tbWVyY2UuY29tgg0qLmdzdGF0aWMuY29tggwqLnVyY2hpbi5jb22CECoudXJs"
										 "Lmdvb2dsZS5jb22CFioueW91dHViZS1ub2Nvb2tpZS5jb22CDSoueW91dHViZS5j"
										 "b22CFioueW91dHViZWVkdWNhdGlvbi5jb22CCyoueXRpbWcuY29tggthbmRyb2lk"
										 "LmNvbYIEZy5jb4IGZ29vLmdsghRnb29nbGUtYW5hbHl0aWNzLmNvbYIKZ29vZ2xl"
										 "LmNvbYISZ29vZ2xlY29tbWVyY2UuY29tggp1cmNoaW4uY29tggh5b3V0dS5iZYIL"
										 "eW91dHViZS5jb22CFHlvdXR1YmVlZHVjYXRpb24uY29tMA0GCSqGSIb3DQEBBQUA"
										 "A4GBAAMn0K3j3yhC+X+uyh6eABa2Eq7xiY5/mUB886Ir19vxluSMNKD6n/iY8vHj"
										 "trn0BhuW8/vmJyudFkIcEDUYE4ivQMlsfIL7SOGw6OevVLmm02aiRHWj5T20Ds+S"
										 "OpueYUG3NBcHP/5IzhUYIQJbGzlQaUaZBMaQeC8ZslMNLWI2";

	std::string google_public_key = R"(-----BEGIN PUBLIC KEY-----
MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEZkqSFGwqoVAatVMJ0QIu/0zUpOlv
CZmrEfiNK/zSw2L+cgKWwM/B11zfIQ3bqzET+ictUNFCCpspbPBzx3saAg==
-----END PUBLIC KEY-----
)";
} // namespace
