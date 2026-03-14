#include <iostream>
#include <string>

#include "nmealib.h"

using namespace std;
using namespace nmealib::nmea0183;
using namespace nmealib::nmea2000;

extern "C" void app_main() {
    /*
    -------------- NMEA 0183 Library Usage Example --------------
    */
    // Creating message from raw sentence
    string nmea_sentence = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
    auto msg = Nmea0183Factory::create(nmea_sentence).get();

    // One could dynamic cast at runtime if enabled, otherwise check the sentence type
    if (msg->getSentenceType() != "GGA") {
        cerr << "Parsed message is not of type GGA." << endl;
        return;
    }
    GGA gga_msg = static_cast<GGA&>(*msg);

    if (gga_msg.validate()) {
        cout << "Parsed GGA message:" << endl;
        cout << "UTC Time: " << gga_msg.getUtcTime() << endl;
        cout << "Latitude: " << gga_msg.getLatitude() << " " << gga_msg.getLatitudeDirection() << endl;
        cout << "Longitude: " << gga_msg.getLongitude() << " " << gga_msg.getLongitudeDirection() << endl;
        cout << "GPS Quality: " << gga_msg.getGpsQuality() << endl;
        cout << "Satellites: " << gga_msg.getSatellites() << endl;
    } else {
        cerr << "Failed to parse GGA message." << endl;
    }

    // Creating message from individual fields
    DBT dbt_msg("II", 1.2, 'M', 0.6, 'M', 0.0, 'M');
    string dbt_raw = dbt_msg.serialize();
    cout << "Serialized DBT message: " << dbt_raw << endl;

    /*
    -------------- NMEA 2000 Library Usage Example --------------
    */
    string pgn = "19FD0201:8C2C010479FAFFFF";
    auto msg2000 = Nmea2000Factory::create(pgn).get();
    if (msg2000->getType() == nmealib::Message::Type::NMEA2000) {
        cout << "Parsed NMEA 2000 message with PGN: " << msg2000->getPgn() << endl;

        if (msg2000->getPgn() == 130306) {
            PGN130306 real_pgn = static_cast<PGN130306&>(*msg2000);
            cout << "Wind Speed: " << real_pgn.getWindSpeed().getValue() << " m/s" << endl;
        }

    } else {
        cerr << "Failed to parse NMEA 2000 message." << endl;
    }
}