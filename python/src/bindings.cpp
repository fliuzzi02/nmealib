#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <optional>
#include <string>

#include <nmealib.h>

namespace py = pybind11;

namespace {

using nmealib::Message;
using nmealib::NmeaException;
using nmealib::nmea0183::GGA;
using nmealib::nmea0183::GLL;
using nmealib::nmea0183::InvalidStartCharacterException;
using nmealib::nmea0183::Message0183;
using nmealib::nmea0183::Nmea0183Factory;
using nmealib::nmea0183::NoChecksumException;
using nmealib::nmea0183::NotEnoughFieldsException;
using nmealib::nmea0183::NotGGAException;
using nmealib::nmea0183::NotGLLException;
using nmealib::nmea0183::NotRMCException;
using nmealib::nmea0183::NotVTGException;
using nmealib::nmea0183::NoEndlineException;
using nmealib::nmea0183::RMC;
using nmealib::nmea0183::TooLongSentenceException;
using nmealib::nmea0183::VTG;
using nmealib::nmea2000::Angle;
using nmealib::nmea2000::Byte;
using nmealib::nmea2000::FrameTooLongException;
using nmealib::nmea2000::HalfByte;
using nmealib::nmea2000::InvalidCanFrameException;
using nmealib::nmea2000::InvalidPgnException;
using nmealib::nmea2000::Message2000;
using nmealib::nmea2000::Nmea2000Factory;
using nmealib::nmea2000::PGN127250;
using nmealib::nmea2000::PGN129026;
using nmealib::nmea2000::SignedAngle;
using nmealib::nmea2000::Speed;

template <typename DataType>
void bindDataType(py::module_& m, const char* name) {
    py::class_<DataType>(m, name)
        .def_static("from_value", &DataType::fromValue, py::arg("value"))
        .def_static("from_raw", &DataType::fromRaw, py::arg("raw"))
        .def("is_valid", &DataType::isValid)
        .def("get_value", &DataType::getValue)
        .def("get_raw", &DataType::getRaw)
        .def("to_string", &DataType::toString)
        .def("__repr__", [name](const DataType& self) {
            return std::string("<") + name + " value=" + self.toString() + ">";
        });
}

}  // namespace

PYBIND11_MODULE(_core, m) {
    m.doc() = "Python bindings for nmealib";
    m.attr("__version__") = NMEALIB_VERSION;

    auto nmeaException = py::register_exception<NmeaException>(m, "NmeaException");
    py::register_exception<TooLongSentenceException>(m, "TooLongSentenceException", nmeaException.ptr());
    py::register_exception<InvalidStartCharacterException>(m, "InvalidStartCharacterException", nmeaException.ptr());
    py::register_exception<NoChecksumException>(m, "NoChecksumException", nmeaException.ptr());
    py::register_exception<NoEndlineException>(m, "NoEndlineException", nmeaException.ptr());
    py::register_exception<NotEnoughFieldsException>(m, "NotEnoughFieldsException", nmeaException.ptr());
    py::register_exception<NotRMCException>(m, "NotRMCException", nmeaException.ptr());
    py::register_exception<NotGGAException>(m, "NotGGAException", nmeaException.ptr());
    py::register_exception<NotGLLException>(m, "NotGLLException", nmeaException.ptr());
    py::register_exception<NotVTGException>(m, "NotVTGException", nmeaException.ptr());
    py::register_exception<InvalidPgnException>(m, "InvalidPgnException", nmeaException.ptr());
    py::register_exception<InvalidCanFrameException>(m, "InvalidCanFrameException", nmeaException.ptr());
    py::register_exception<FrameTooLongException>(m, "FrameTooLongException", nmeaException.ptr());

    py::enum_<Message::Type>(m, "MessageType")
        .value("Unknown", Message::Type::Unknown)
        .value("NMEA0183", Message::Type::NMEA0183)
        .value("NMEA2000", Message::Type::NMEA2000)
        .export_values();

    py::class_<Message>(m, "Message")
        .def("get_type", &Message::getType)
        .def("get_raw_data", &Message::getRawData)
        .def("get_timestamp", &Message::getTimestamp)
        .def("clone", &Message::clone)
        .def("serialize", &Message::serialize)
        .def("validate", &Message::validate)
        .def_static("type_to_string", &Message::typeToString, py::arg("message_type"));

    py::module_ m0183 = m.def_submodule("nmea0183", "NMEA 0183 API");

    py::class_<Message0183, Message>(m0183, "Message0183")
        .def("get_start_char", &Message0183::getStartChar)
        .def("get_talker", &Message0183::getTalker)
        .def("get_sentence_type", &Message0183::getSentenceType)
        .def("get_payload", &Message0183::getPayload)
        .def("get_checksum_str", &Message0183::getChecksumStr)
        .def("get_calculated_checksum_str", &Message0183::getCalculatedChecksumStr)
        .def("get_string_content", &Message0183::getStringContent, py::arg("verbose") = false)
        .def_static("convert_nmea_coordinate_to_decimal_degrees", &Message0183::convertNmeaCoordinateToDecimalDegrees, py::arg("nmea_coordinate"));

    py::class_<Nmea0183Factory>(m0183, "Nmea0183Factory")
        .def_static("create", &Nmea0183Factory::create, py::arg("raw"), py::arg("timestamp") = std::chrono::system_clock::now());

    py::class_<RMC, Message0183>(m0183, "RMC")
        .def(py::init<std::string, unsigned int, char, double, char, double, char, double, double, unsigned int, double, char, char, char>(),
             py::arg("talker_id"), py::arg("utc_fix"), py::arg("status"),
             py::arg("latitude"), py::arg("latitude_direction"),
             py::arg("longitude"), py::arg("longitude_direction"),
             py::arg("speed_over_ground"), py::arg("course_over_ground"),
             py::arg("date"), py::arg("magnetic_variation"),
             py::arg("magnetic_variation_direction"), py::arg("mode_indicator"),
             py::arg("navigation_status"))
        .def("get_utc_fix", &RMC::getUtcFix)
        .def("get_status", &RMC::getStatus)
        .def("get_latitude", &RMC::getLatitude)
        .def("get_latitude_direction", &RMC::getLatitudeDirection)
        .def("get_longitude", &RMC::getLongitude)
        .def("get_longitude_direction", &RMC::getLongitudeDirection)
        .def("get_speed_over_ground", &RMC::getSpeedOverGround)
        .def("get_course_over_ground", &RMC::getCourseOverGround)
        .def("get_date", &RMC::getDate)
        .def("get_magnetic_variation", &RMC::getMagneticVariation)
        .def("get_magnetic_variation_direction", &RMC::getMagneticVariationDirection)
        .def("get_mode_indicator", &RMC::getModeIndicator)
        .def("get_navigation_status", &RMC::getNavigationStatus)
        .def("get_string_content", &RMC::getStringContent, py::arg("verbose") = false);

    py::class_<GGA, Message0183>(m0183, "GGA")
        .def(py::init<std::string, double, double, char, double, char, unsigned int, unsigned int, double, double, char, double, char, double, std::string>(),
             py::arg("talker_id"), py::arg("utc_time"), py::arg("latitude"), py::arg("latitude_direction"),
             py::arg("longitude"), py::arg("longitude_direction"), py::arg("gps_quality"), py::arg("satellites"),
             py::arg("hdop"), py::arg("altitude"), py::arg("altitude_units"), py::arg("geoidal_separation"),
             py::arg("geoidal_separation_units"), py::arg("dgps_age"), py::arg("dgps_reference_station_id"))
        .def("get_utc_time", &GGA::getUtcTime)
        .def("get_latitude", &GGA::getLatitude)
        .def("get_latitude_direction", &GGA::getLatitudeDirection)
        .def("get_longitude", &GGA::getLongitude)
        .def("get_longitude_direction", &GGA::getLongitudeDirection)
        .def("get_gps_quality", &GGA::getGpsQuality)
        .def("get_satellites", &GGA::getSatellites)
        .def("get_hdop", &GGA::getHdop)
        .def("get_altitude", &GGA::getAltitude)
        .def("get_altitude_units", &GGA::getAltitudeUnits)
        .def("get_geoidal_separation", &GGA::getGeoidalSeparation)
        .def("get_geoidal_separation_units", &GGA::getGeoidalSeparationUnits)
        .def("get_dgps_age", &GGA::getDgpsAge)
        .def("get_dgps_reference_station_id", &GGA::getDgpsReferenceStationId)
        .def("get_string_content", &GGA::getStringContent, py::arg("verbose") = false);

    py::class_<GLL, Message0183>(m0183, "GLL")
        .def(py::init<std::string, double, char, double, char, double, char, char>(),
             py::arg("talker_id"), py::arg("latitude"), py::arg("latitude_direction"),
             py::arg("longitude"), py::arg("longitude_direction"), py::arg("utc_time"),
             py::arg("status"), py::arg("mode_indicator"))
        .def("get_latitude", &GLL::getLatitude)
        .def("get_latitude_direction", &GLL::getLatitudeDirection)
        .def("get_longitude", &GLL::getLongitude)
        .def("get_longitude_direction", &GLL::getLongitudeDirection)
        .def("get_utc_time", &GLL::getUtcTime)
        .def("get_status", &GLL::getStatus)
        .def("get_mode_indicator", &GLL::getModeIndicator)
        .def("get_string_content", &GLL::getStringContent, py::arg("verbose") = false);

    py::class_<VTG, Message0183>(m0183, "VTG")
        .def(py::init<std::string, double, double, double, double, std::optional<char>, bool>(),
             py::arg("talker_id"), py::arg("course_over_ground_true"),
             py::arg("course_over_ground_magnetic"), py::arg("speed_over_ground_knots"),
             py::arg("speed_over_ground_kph"), py::arg("faa_mode_indicator") = std::nullopt,
             py::arg("legacy_format") = false)
        .def("get_course_over_ground_true", &VTG::getCourseOverGroundTrue)
        .def("get_course_over_ground_true_type", &VTG::getCourseOverGroundTrueType)
        .def("get_course_over_ground_magnetic", &VTG::getCourseOverGroundMagnetic)
        .def("get_course_over_ground_magnetic_type", &VTG::getCourseOverGroundMagneticType)
        .def("get_speed_over_ground_knots", &VTG::getSpeedOverGroundKnots)
        .def("get_speed_over_ground_knots_type", &VTG::getSpeedOverGroundKnotsType)
        .def("get_speed_over_ground_kph", &VTG::getSpeedOverGroundKph)
        .def("get_speed_over_ground_kph_type", &VTG::getSpeedOverGroundKphType)
        .def("has_faa_mode_indicator", &VTG::hasFaaModeIndicator)
        .def("get_faa_mode_indicator", &VTG::getFaaModeIndicator)
        .def("is_legacy_format", &VTG::isLegacyFormat)
        .def("get_string_content", &VTG::getStringContent, py::arg("verbose") = false);

    py::module_ m2000 = m.def_submodule("nmea2000", "NMEA 2000 API");

    bindDataType<Angle>(m2000, "Angle");
    bindDataType<SignedAngle>(m2000, "SignedAngle");
    bindDataType<Speed>(m2000, "Speed");
    bindDataType<HalfByte>(m2000, "HalfByte");
    bindDataType<Byte>(m2000, "Byte");

    py::class_<Message2000, Message>(m2000, "Message2000")
        .def("get_can_id", &Message2000::getCanId)
        .def("get_can_frame", &Message2000::getCanFrame)
        .def("get_can_frame_length", &Message2000::getCanFrameLength)
        .def("get_priority", &Message2000::getPriority)
        .def("get_reserved", &Message2000::getReserved)
        .def("get_data_page", &Message2000::getDataPage)
        .def("get_pdu_format", &Message2000::getPDUFormat)
        .def("get_pdu_specific", &Message2000::getPDUSpecific)
        .def("get_source_address", &Message2000::getSourceAddress)
        .def("get_destination_address", &Message2000::getDestinationAddress)
        .def("get_pgn", &Message2000::getPgn)
        .def("get_string_content", &Message2000::getStringContent, py::arg("verbose") = false);

    py::class_<Nmea2000Factory>(m2000, "Nmea2000Factory")
        .def_static("create", &Nmea2000Factory::create, py::arg("raw"), py::arg("timestamp") = std::chrono::system_clock::now());

    py::class_<PGN127250, Message2000>(m2000, "PGN127250")
        .def(py::init<uint8_t, Angle, SignedAngle, SignedAngle, HalfByte, Byte>(),
             py::arg("sequence_id"), py::arg("heading"), py::arg("deviation"),
             py::arg("variation"), py::arg("heading_reference"), py::arg("reserved"))
        .def(py::init([](uint8_t sequenceId,
                         float heading,
                         float deviation,
                         float variation,
                         uint8_t headingReference,
                         uint8_t reserved) {
            return PGN127250(
                sequenceId,
                Angle::fromValue(heading),
                SignedAngle::fromValue(deviation),
                SignedAngle::fromValue(variation),
                HalfByte::fromValue(headingReference),
                Byte::fromValue(reserved));
        }),
             py::arg("sequence_id"), py::arg("heading_radians"), py::arg("deviation_radians"),
             py::arg("variation_radians"), py::arg("heading_reference"), py::arg("reserved") = 0)
        .def("get_sequence_id", &PGN127250::getSequenceId)
        .def("get_heading", &PGN127250::getHeading)
        .def("get_deviation", &PGN127250::getDeviation)
        .def("get_variation", &PGN127250::getVariation)
        .def("get_heading_reference", &PGN127250::getHeadingReference)
        .def("get_heading_degrees", &PGN127250::getHeadingDegrees)
        .def("get_deviation_degrees", &PGN127250::getDeviationDegrees)
        .def("get_variation_degrees", &PGN127250::getVariationDegrees)
        .def("get_string_content", &PGN127250::getStringContent, py::arg("verbose") = false);

    py::class_<PGN129026, Message2000>(m2000, "PGN129026")
        .def(py::init<uint8_t, HalfByte, Angle, Speed>(),
             py::arg("sequence_id"), py::arg("cog_reference"), py::arg("cog"), py::arg("sog"))
        .def(py::init<uint8_t, HalfByte, Byte, Angle, Speed, Byte, Byte>(),
             py::arg("sequence_id"), py::arg("cog_reference"), py::arg("reserved1"),
             py::arg("cog"), py::arg("sog"), py::arg("reserved2"), py::arg("reserved3"))
        .def(py::init([](uint8_t sequenceId,
                         uint8_t cogReference,
                         float cog,
                         float sog) {
            return PGN129026(
                sequenceId,
                HalfByte::fromValue(cogReference),
                Angle::fromValue(cog),
                Speed::fromValue(sog));
        }),
             py::arg("sequence_id"), py::arg("cog_reference"), py::arg("cog_radians"), py::arg("sog_mps"))
        .def("get_sequence_id", &PGN129026::getSequenceId)
        .def("get_cog_reference", &PGN129026::getCogReference)
        .def("get_cog", &PGN129026::getCog)
        .def("get_sog", &PGN129026::getSog)
        .def("get_reserved1", &PGN129026::getReserved1)
        .def("get_reserved2", &PGN129026::getReserved2)
        .def("get_reserved3", &PGN129026::getReserved3)
        .def("get_cog_degrees", &PGN129026::getCogDegrees)
        .def("get_sog_knots", &PGN129026::getSogKnots)
        .def("get_string_content", &PGN129026::getStringContent, py::arg("verbose") = false);
}
