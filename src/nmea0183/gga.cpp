#include "nmealib/nmea0183/gga.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<GGA> GGA::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "GGA::create";
    if (baseMessage->getSentenceType() != "GGA") {
        throw NotGGAException(context, "Expected sentence type 'GGA', got " + baseMessage->getSentenceType());
    }

    // Parse the payload to extract GGA-specific fields
    std::string payload = baseMessage->getPayload();
    std::istringstream ss(payload);
    std::string token;
    std::vector<std::string> fields;

    while (std::getline(ss, token, ',')) {
        fields.push_back(token);
    }

    // Preserve the final empty field when payload ends with a comma.
    if (!payload.empty() && payload.back() == ',') {
        fields.push_back("");
    }

    // Drop first element which is the sentence type (e.g. "GPGGA")
    if (!fields.empty()) {
        fields.erase(fields.begin());
    }
    size_t messageSize = fields.size();
    if (messageSize != 14 && messageSize != 12) {
        throw NotGGAException(context, "Insufficient fields in GGA payload: expected 14 or 12, got " + std::to_string(fields.size()) + ". Payload: " + payload);
    }

    try {
        double timestamp = fields[0].empty() ? 0.0 : std::stod(fields[0]);
        double latitude = Message0183::convertNmeaCoordinateToDecimalDegrees(fields[1]);
        char latitudeDirection = fields[2].empty() ? '\0' : fields[2][0];
        double longitude = Message0183::convertNmeaCoordinateToDecimalDegrees(fields[3]);
        char longitudeDirection = fields[4].empty() ? '\0' : fields[4][0];
        unsigned int gpsQuality = fields[5].empty() ? 0 : std::stoul(fields[5]);
        unsigned int satellites = fields[6].empty() ? 0 : std::stoul(fields[6]);
        double hdop = fields[7].empty() ? 0.0 : std::stod(fields[7]);
        double altitude = fields[8].empty() ? 0.0 : std::stod(fields[8]);
        char altitudeUnits = fields[9].empty() ? '\0' : fields[9][0];
        double geoidalSeparation = fields[10].empty() ? 0.0 : std::stod(fields[10]);
        char geoidalSeparationUnits = fields[11].empty() ? '\0' : fields[11][0];
        double dgpsAge = -1;
        std::string dgpsReferenceStationId = "";

        if(messageSize == 14) {
            dgpsAge = fields[12].empty() ? 0.0 : std::stod(fields[12]);
            dgpsReferenceStationId = fields[13].empty() ? "" : fields[13];
        }

        return std::unique_ptr<GGA>(new GGA(std::move(*baseMessage),
                                            timestamp,
                                            latitude,
                                            latitudeDirection,
                                            longitude,
                                            longitudeDirection,
                                            gpsQuality,
                                            satellites,
                                            hdop,
                                            altitude,
                                            altitudeUnits,
                                            geoidalSeparation,
                                            geoidalSeparationUnits,
                                            dgpsAge,
                                            dgpsReferenceStationId));
    } catch (const std::exception& e) {
        throw NmeaException(context, "Error parsing GGA fields: " + std::string(e.what()));
    }
}

GGA::GGA(Message0183 baseMessage,
         double timestamp,
         double latitude,
         char latitudeDirection,
         double longitude,
         char longitudeDirection,
         unsigned int gpsQuality,
         unsigned int satellites,
         double hdop,
         double altitude,
         char altitudeUnits,
         double geoidalSeparation,
         char geoidalSeparationUnits,
         double dgpsAge,
         std::string dgpsReferenceStationId) noexcept
    : Message0183(std::move(baseMessage)),
            utcTime_(timestamp),
      latitude_(latitude),
      latitudeDirection_(latitudeDirection),
      longitude_(longitude),
      longitudeDirection_(longitudeDirection),
      gpsQuality_(gpsQuality),
      satellites_(satellites),
      hdop_(hdop),
      altitude_(altitude),
      altitudeUnits_(altitudeUnits),
      geoidalSeparation_(geoidalSeparation),
      geoidalSeparationUnits_(geoidalSeparationUnits),
      dgpsAge_(dgpsAge),
      dgpsReferenceStationId_(std::move(dgpsReferenceStationId)) {}

GGA::GGA(std::string talkerId,
         double timestamp,
         double latitude,
         char latitudeDirection,
         double longitude,
         char longitudeDirection,
         unsigned int gpsQuality,
         unsigned int satellites,
         double hdop,
         double altitude,
         char altitudeUnits,
         double geoidalSeparation,
         char geoidalSeparationUnits,
         double dgpsAge,
         std::string dgpsReferenceStationId)
    : Message0183(*Message0183::create(composeRaw(talkerId,
                                                  timestamp,
                                                  latitude,
                                                  latitudeDirection,
                                                  longitude,
                                                  longitudeDirection,
                                                  gpsQuality,
                                                  satellites,
                                                  hdop,
                                                  altitude,
                                                  altitudeUnits,
                                                  geoidalSeparation,
                                                  geoidalSeparationUnits,
                                                  dgpsAge,
                                                  dgpsReferenceStationId))),
                    utcTime_(timestamp),
      latitude_(latitude),
      latitudeDirection_(latitudeDirection),
      longitude_(longitude),
      longitudeDirection_(longitudeDirection),
      gpsQuality_(gpsQuality),
      satellites_(satellites),
      hdop_(hdop),
      altitude_(altitude),
      altitudeUnits_(altitudeUnits),
      geoidalSeparation_(geoidalSeparation),
      geoidalSeparationUnits_(geoidalSeparationUnits),
      dgpsAge_(dgpsAge),
      dgpsReferenceStationId_(std::move(dgpsReferenceStationId)) {}

std::unique_ptr<nmealib::Message> GGA::clone() const {
    return std::unique_ptr<GGA>(new GGA(*this));
}

std::string GGA::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);
    std::ostringstream latStream;
    latStream << std::setprecision(10) << latitude_;
    const std::string latitudeStr = latStream.str();

    std::ostringstream lonStream;
    lonStream << std::setprecision(10) << longitude_;
    const std::string longitudeStr = lonStream.str();

    if (verbose) {
        ss << "\tTimestamp: " << utcTime_ << "\n";
        ss << "\tLatitude: " << latitudeStr << " " << latitudeDirection_ << "\n";
        ss << "\tLongitude: " << longitudeStr << " " << longitudeDirection_ << "\n";
        ss << "\tGPS Quality: " << gpsQuality_ << "\n";
        ss << "\tSatellites: " << satellites_ << "\n";
        ss << "\tHDOP: " << hdop_ << "\n";
        ss << "\tAltitude: " << altitude_ << " " << altitudeUnits_ << "\n";
        ss << "\tGeoidal Separation: " << geoidalSeparation_ << " " << geoidalSeparationUnits_ << "\n";
        ss << "\tDGPS Age: " << dgpsAge_ << "\n";
        ss << "\tDGPS Ref: " << dgpsReferenceStationId_;
        ss << "\n";
    } else {
        ss << "Time=" << utcTime_
           << ", Lat=" << latitudeStr << latitudeDirection_
           << ", Lon=" << longitudeStr << longitudeDirection_
           << ", Qual=" << gpsQuality_
           << ", Sats=" << satellites_
           << ", HDOP=" << hdop_
           << ", Alt=" << altitude_ << altitudeUnits_
           << ", GeoSep=" << geoidalSeparation_ << geoidalSeparationUnits_
           << ", DGPSAge=" << dgpsAge_
           << ", DGPSRef=" << dgpsReferenceStationId_;
    }
    return ss.str();
}

std::string GGA::composeRaw(const std::string& talkerId,
                            double timestamp,
                            double latitude,
                            char latitudeDirection,
                            double longitude,
                            char longitudeDirection,
                            unsigned int gpsQuality,
                            unsigned int satellites,
                            double hdop,
                            double altitude,
                            char altitudeUnits,
                            double geoidalSeparation,
                            char geoidalSeparationUnits,
                            double dgpsAge,
                            const std::string& dgpsReferenceStationId) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "GGA,";
    payloadStream << std::fixed << std::setprecision(2) << std::setw(9) << std::setfill('0') << timestamp << ",";

    double latitudeDegrees = std::floor(latitude);
    double latitudeMinutes = (latitude - latitudeDegrees) * 60.0;
    double latitudeNmea = latitudeDegrees * 100.0 + latitudeMinutes; // ddmm.mmmm

    double longitudeDegrees = std::floor(longitude);
    double longitudeMinutes = (longitude - longitudeDegrees) * 60.0;
    double longitudeNmea = longitudeDegrees * 100.0 + longitudeMinutes; // dddmm.mmmm

    payloadStream << std::fixed << std::setprecision(6) << std::setw(10) << std::setfill('0') << latitudeNmea << ",";
    payloadStream << latitudeDirection << ",";
    payloadStream << std::fixed << std::setprecision(6) << std::setw(11) << std::setfill('0') << longitudeNmea << ",";
    payloadStream << longitudeDirection << ",";
    payloadStream << gpsQuality << ",";
    payloadStream << std::setw(2) << std::setfill('0') << satellites << ",";
    payloadStream << std::fixed << std::setprecision(1) << hdop << ",";
    payloadStream << std::fixed << std::setprecision(1) << altitude << ",";
    payloadStream << altitudeUnits << ",";

    if (geoidalSeparation == 0.0) {
        payloadStream << ",";
    } else {
        payloadStream << std::fixed << std::setprecision(1) << geoidalSeparation << ",";
    }

    payloadStream << geoidalSeparationUnits << ",";

    if (dgpsAge == 0.0) {
        payloadStream << ",";
    } else {
        payloadStream << std::fixed << std::setprecision(1) << dgpsAge << ",";
    }

    payloadStream << dgpsReferenceStationId;

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

double GGA::getUtcTime() const noexcept {
    return utcTime_;
}

double GGA::getLatitude() const noexcept {
    return latitude_;
}

char GGA::getLatitudeDirection() const noexcept {
    return latitudeDirection_;
}

double GGA::getLongitude() const noexcept {
    return longitude_;
}

char GGA::getLongitudeDirection() const noexcept {
    return longitudeDirection_;
}

unsigned int GGA::getGpsQuality() const noexcept {
    return gpsQuality_;
}

unsigned int GGA::getSatellites() const noexcept {
    return satellites_;
}

double GGA::getHdop() const noexcept {
    return hdop_;
}

double GGA::getAltitude() const noexcept {
    return altitude_;
}

char GGA::getAltitudeUnits() const noexcept {
    return altitudeUnits_;
}

double GGA::getGeoidalSeparation() const noexcept {
    return geoidalSeparation_;
}

char GGA::getGeoidalSeparationUnits() const noexcept {
    return geoidalSeparationUnits_;
}

double GGA::getDgpsAge() const noexcept {
    return dgpsAge_;
}

std::string GGA::getDgpsReferenceStationId() const noexcept {
    return dgpsReferenceStationId_;
}

bool GGA::operator==(const GGA& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
