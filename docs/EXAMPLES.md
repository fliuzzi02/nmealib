# Examples

This page shows practical C++ and Python examples for the full nmealib workflow:
- parsing NMEA 0183 and NMEA 2000 from wire format,
- dispatching to typed message classes,
- creating new messages from fields,
- serializing and validating messages,
- handling parse and protocol errors.

## C++ Examples

### Parse NMEA 0183 and dispatch to typed class

~~~cpp
#include <iostream>
#include <memory>
#include <nmealib.h>

int main() {
	const std::string raw =
		"$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n";

	auto msg = nmealib::nmea0183::Nmea0183Factory::create(raw);

	std::cout << "Protocol: "
			  << nmealib::Message::typeToString(msg->getType())
			  << "\n";
	std::cout << "Valid: " << std::boolalpha << msg->validate() << "\n";

	if (auto* rmc = dynamic_cast<nmealib::nmea0183::RMC*>(msg.get())) {
		std::cout << "RMC lat/lon: "
				  << rmc->getLatitude() << " " << rmc->getLatitudeDirection()
				  << ", "
				  << rmc->getLongitude() << " " << rmc->getLongitudeDirection()
				  << "\n";
		std::cout << "SOG(kn): " << rmc->getSpeedOverGround() << "\n";
	}

	std::cout << "Verbose dump:\n" << msg->getStringContent(true);
}
~~~

### Parse NMEA 2000 and inspect CAN/PGN fields

~~~cpp
#include <iostream>
#include <nmealib.h>

int main() {
	const std::string raw = "01F80200:01010040F4010000"; // PGN 129026
	auto msg = nmealib::nmea2000::Nmea2000Factory::create(raw);

	std::cout << "PGN: " << msg->getPgn() << "\n";
	std::cout << "Priority: " << static_cast<int>(msg->getPriority()) << "\n";
	std::cout << "Source address: " << static_cast<int>(msg->getSourceAddress()) << "\n";
	std::cout << "Destination address: " << static_cast<int>(msg->getDestinationAddress()) << "\n";

	if (auto* cogSog = dynamic_cast<nmealib::nmea2000::PGN129026*>(msg.get())) {
		std::cout << "COG(deg): " << cogSog->getCogDegrees() << "\n";
		std::cout << "SOG(kn): " << cogSog->getSogKnots() << "\n";
	}

	std::cout << "Verbose dump:\n" << msg->getStringContent(true);
}
~~~

### Create messages from fields and round-trip serialize/parse

~~~cpp
#include <cassert>
#include <memory>
#include <nmealib.h>

int main() {
	// NMEA 0183: create VTG from field values
	nmealib::nmea0183::VTG vtg("GP", 220.86, 218.90, 2.55, 4.724, 'A', false);
	std::string vtgWire = vtg.serialize();
	auto vtgParsedBase = nmealib::nmea0183::Nmea0183Factory::create(vtgWire);
	auto* vtgParsed = dynamic_cast<nmealib::nmea0183::VTG*>(vtgParsedBase.get());
	assert(vtgParsed != nullptr);
	assert(vtgParsed->validate());

	// NMEA 2000: create PGN 129026 from field values
	nmealib::nmea2000::PGN129026 pgn(7, 1, 1.5707f, 5.0f);
	std::string pgnWire = pgn.serialize();
	auto pgnParsedBase = nmealib::nmea2000::Nmea2000Factory::create(pgnWire);
	auto* pgnParsed = dynamic_cast<nmealib::nmea2000::PGN129026*>(pgnParsedBase.get());
	assert(pgnParsed != nullptr);
	assert(pgnParsed->validate());
}
~~~

### Handle errors cleanly

~~~cpp
#include <iostream>
#include <nmealib.h>

int main() {
	try {
		// Invalid start character for NMEA 0183
		auto msg = nmealib::nmea0183::Nmea0183Factory::create("GPRMC,123519,A\r\n");
		(void)msg;
	} catch (const nmealib::nmea0183::InvalidStartCharacterException& ex) {
		std::cerr << "Invalid 0183 sentence: " << ex.what() << "\n";
	} catch (const nmealib::NmeaException& ex) {
		std::cerr << "nmealib error: " << ex.what() << "\n";
	}
}
~~~

## Python Examples

### Parse NMEA 0183 and use typed accessors

~~~python
import nmealib

raw = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n"
msg = nmealib.nmea0183.Nmea0183Factory.create(raw)

print(type(msg).__name__)  # RMC
print(msg.get_sentence_type())
print(msg.get_string_content(verbose=False))

if isinstance(msg, nmealib.nmea0183.RMC):
	print("Lat/Lon:", msg.get_latitude(), msg.get_longitude())
	print("COG:", msg.get_course_over_ground())
	print("SOG:", msg.get_speed_over_ground())

	# Convert raw NMEA coordinate field to decimal degrees when needed
	lat_dd = nmealib.nmea0183.Message0183.convert_nmea_coordinate_to_decimal_degrees("4807.038")
	print("Converted latitude:", lat_dd)
~~~

### Parse NMEA 2000 and inspect PGN-specific values

~~~python
import nmealib

raw = "01F80200:01010040F4010000"  # PGN 129026
msg = nmealib.nmea2000.Nmea2000Factory.create(raw)

print(type(msg).__name__)
print("PGN:", msg.get_pgn())
print("Source:", msg.get_source_address())
print("Destination:", msg.get_destination_address())
print(msg.get_string_content(verbose=True))

if isinstance(msg, nmealib.nmea2000.PGN129026):
	print("COG(deg):", msg.get_cog_degrees())
	print("SOG(kn):", msg.get_sog_knots())
~~~

### Create messages in Python and serialize

~~~python
import nmealib

# NMEA 0183
vtg = nmealib.nmea0183.VTG("GP", 220.86, 218.90, 2.55, 4.724, "A", False)
vtg_wire = vtg.serialize()
print(vtg_wire)

# NMEA 2000
pgn129026 = nmealib.nmea2000.PGN129026(7, 1, 1.5707, 5.0)
pgn_wire = pgn129026.serialize()
print(pgn_wire)

# Round-trip parse back
parsed_0183 = nmealib.nmea0183.Nmea0183Factory.create(vtg_wire)
parsed_2000 = nmealib.nmea2000.Nmea2000Factory.create(pgn_wire)

print(parsed_0183.validate(), parsed_2000.validate())
~~~

### Handle exceptions in Python

~~~python
import nmealib

bad_raw = "GPRMC,123519,A\r\n"

try:
	nmealib.nmea0183.Nmea0183Factory.create(bad_raw)
except nmealib.InvalidStartCharacterException as ex:
	print("Invalid start character:", ex)
except nmealib.NmeaException as ex:
	print("Generic nmealib exception:", ex)
~~~

## End-to-End Mixed Stream Example

Use this pattern when input lines can be either NMEA 0183 or NMEA 2000.

~~~python
import nmealib

raw_messages = [
	"$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W,A,V*6A\r\n",
	"01F10D00:0101FEBF0060FFFF",
	"$GPVTG,220.86,T,218.90,M,2.550,N,4.724,K*40\r\n",
	"09F11260:342C71FF7FFF7FFD",
]

for raw in raw_messages:
	try:
		if raw.startswith("$") or raw.startswith("!"):
			msg = nmealib.nmea0183.Nmea0183Factory.create(raw)
		else:
			msg = nmealib.nmea2000.Nmea2000Factory.create(raw)

		print(type(msg).__name__, "->", msg.serialize())
	except nmealib.NmeaException as ex:
		print("Skipped invalid message:", ex)
~~~

## Coverage Notes

The examples above cover the core feature set used across all supported message classes:
- Factory parsing and automatic typed dispatch
- Protocol/base introspection through Message, Message0183, and Message2000 APIs
- Type-specific field getters (RMC, VTG, PGN129026 used as concrete patterns)
- Serialization and round-trip validation
- Protocol-specific and generic exception handling

You can apply the same pattern to every supported NMEA 0183 sentence class and NMEA 2000 PGN class listed in docs/nmea0183 and docs/nmea2000.
