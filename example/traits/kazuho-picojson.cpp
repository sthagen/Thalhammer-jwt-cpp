#include "jwt-cpp/traits/kazuho-picojson/traits.h"

#include <iostream>
#include <sstream>

int main() {
	using sec = std::chrono::seconds;
	using min = std::chrono::minutes;
	using traits = jwt::traits::kazuho_picojson;
	using claim = jwt::basic_claim<traits>;

	claim from_raw_json;
	std::istringstream iss{R"##({"api":{"array":[1,2,3],"null":null}})##"};
	iss >> from_raw_json;

	claim::set_t list{"once", "twice"};
	std::vector<int64_t> big_numbers{727663072ULL, 770979831ULL, 427239169ULL, 525936436ULL};

	const auto time = jwt::date::clock::now();
	const auto token = jwt::create<traits>()
						   .set_type("JWT")
						   .set_issuer("auth.mydomain.io")
						   .set_audience("mydomain.io")
						   .set_issued_at(time)
						   .set_not_before(time)
						   .set_expires_at(time + min{2} + sec{15})
						   .set_payload_claim("boolean", picojson::value(true))
						   .set_payload_claim("integer", picojson::value(int64_t{12345}))
						   .set_payload_claim("precision", picojson::value(12.345))
						   .set_payload_claim("strings", claim(list))
						   .set_payload_claim("array", claim(big_numbers.begin(), big_numbers.end()))
						   .set_payload_claim("object", from_raw_json)
						   .sign(jwt::algorithm::none{});
	const auto decoded = jwt::decode<traits>(token);

	const auto api_array = decoded.get_payload_claim("object").to_json().get("api").get("array");
	std::cout << "api array = " << api_array << '\n';

	jwt::verify<traits>()
		.allow_algorithm(jwt::algorithm::none{})
		.with_issuer("auth.mydomain.io")
		.with_audience("mydomain.io")
		.with_claim("object", from_raw_json)
		.verify(decoded);

	return 0;
}
