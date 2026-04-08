#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <array>
#include <optional>
#include <string>
#include <vector>

#include <nmealib.h>

namespace py = pybind11;

namespace {

using nmealib::Message;
using nmealib::NmeaException;
using nmealib::nmea0183::APB;
using nmealib::nmea0183::DBT;
using nmealib::nmea0183::DPT;
using nmealib::nmea0183::GGA;
using nmealib::nmea0183::GLL;
using nmealib::nmea0183::GSA;
using nmealib::nmea0183::GSV;
using nmealib::nmea0183::HDG;
using nmealib::nmea0183::HDM;
using nmealib::nmea0183::InvalidStartCharacterException;
using nmealib::nmea0183::Message0183;
using nmealib::nmea0183::MTW;
using nmealib::nmea0183::MWV;
using nmealib::nmea0183::Nmea0183Factory;
using nmealib::nmea0183::NoChecksumException;
using nmealib::nmea0183::NotEnoughFieldsException;
using nmealib::nmea0183::NotAPBException;
using nmealib::nmea0183::NotDBTException;
using nmealib::nmea0183::NotDPTException;
using nmealib::nmea0183::NotGGAException;
using nmealib::nmea0183::NotGLLException;
using nmealib::nmea0183::NotGSAException;
using nmealib::nmea0183::NotGSVException;
using nmealib::nmea0183::NotHDGException;
using nmealib::nmea0183::NotHDMException;
using nmealib::nmea0183::NotMTWException;
using nmealib::nmea0183::NotMWVException;
using nmealib::nmea0183::NotRMAException;
using nmealib::nmea0183::NotRMBException;
using nmealib::nmea0183::NotRMCException;
using nmealib::nmea0183::NotVTGException;
using nmealib::nmea0183::NotVHWException;
using nmealib::nmea0183::NotVLWException;
using nmealib::nmea0183::NotVWRException;
using nmealib::nmea0183::NotXDRException;
using nmealib::nmea0183::NotXTEException;
using nmealib::nmea0183::NotZDAException;
using nmealib::nmea0183::NoEndlineException;
using nmealib::nmea0183::RMA;
using nmealib::nmea0183::RMB;
using nmealib::nmea0183::RMC;
using nmealib::nmea0183::TooLongSentenceException;
using nmealib::nmea0183::VTG;
using nmealib::nmea0183::VHW;
using nmealib::nmea0183::VLW;
using nmealib::nmea0183::VWR;
using nmealib::nmea0183::XDR;
using nmealib::nmea0183::XTE;
using nmealib::nmea0183::ZDA;
using nmealib::nmea2000::Angle;
using nmealib::nmea2000::Byte;
using nmealib::nmea2000::FrameTooLongException;
using nmealib::nmea2000::HalfByte;
using nmealib::nmea2000::InvalidCanFrameException;
using nmealib::nmea2000::InvalidPgnException;
using nmealib::nmea2000::Latitude;
using nmealib::nmea2000::Longitude;
using nmealib::nmea2000::Message2000;
using nmealib::nmea2000::Nmea2000Factory;
using nmealib::nmea2000::PGN127245;
using nmealib::nmea2000::PGN127250;
using nmealib::nmea2000::PGN127257;
using nmealib::nmea2000::PGN128259;
using nmealib::nmea2000::PGN129025;
using nmealib::nmea2000::PGN129026;
using nmealib::nmea2000::PGN130306;
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
    py::register_exception<NotAPBException>(m, "NotAPBException", nmeaException.ptr());
    py::register_exception<NotDBTException>(m, "NotDBTException", nmeaException.ptr());
    py::register_exception<NotDPTException>(m, "NotDPTException", nmeaException.ptr());
    py::register_exception<NotRMCException>(m, "NotRMCException", nmeaException.ptr());
    py::register_exception<NotGGAException>(m, "NotGGAException", nmeaException.ptr());
    py::register_exception<NotGLLException>(m, "NotGLLException", nmeaException.ptr());
    py::register_exception<NotGSAException>(m, "NotGSAException", nmeaException.ptr());
    py::register_exception<NotGSVException>(m, "NotGSVException", nmeaException.ptr());
    py::register_exception<NotHDGException>(m, "NotHDGException", nmeaException.ptr());
    py::register_exception<NotHDMException>(m, "NotHDMException", nmeaException.ptr());
    py::register_exception<NotMTWException>(m, "NotMTWException", nmeaException.ptr());
    py::register_exception<NotMWVException>(m, "NotMWVException", nmeaException.ptr());
    py::register_exception<NotRMAException>(m, "NotRMAException", nmeaException.ptr());
    py::register_exception<NotRMBException>(m, "NotRMBException", nmeaException.ptr());
    py::register_exception<NotVTGException>(m, "NotVTGException", nmeaException.ptr());
    py::register_exception<NotVHWException>(m, "NotVHWException", nmeaException.ptr());
    py::register_exception<NotVLWException>(m, "NotVLWException", nmeaException.ptr());
    py::register_exception<NotVWRException>(m, "NotVWRException", nmeaException.ptr());
    py::register_exception<NotXDRException>(m, "NotXDRException", nmeaException.ptr());
    py::register_exception<NotXTEException>(m, "NotXTEException", nmeaException.ptr());
    py::register_exception<NotZDAException>(m, "NotZDAException", nmeaException.ptr());
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

    py::class_<GSV::SatelliteInfo>(m0183, "GSVSatelliteInfo")
        .def(py::init<>())
        .def_readwrite("satellite_id", &GSV::SatelliteInfo::satelliteId)
        .def_readwrite("elevation", &GSV::SatelliteInfo::elevation)
        .def_readwrite("azimuth", &GSV::SatelliteInfo::azimuth)
        .def_readwrite("snr", &GSV::SatelliteInfo::snr);

    py::class_<XDR::TransducerMeasurement>(m0183, "XDRTransducerMeasurement")
        .def(py::init<>())
        .def_readwrite("transducer_type", &XDR::TransducerMeasurement::transducerType)
        .def_readwrite("measurement", &XDR::TransducerMeasurement::measurement)
        .def_readwrite("units", &XDR::TransducerMeasurement::units)
        .def_readwrite("name", &XDR::TransducerMeasurement::name);

    py::class_<APB, Message0183>(m0183, "APB")
        .def(py::init<std::string, char, char, double, char, char, char, char, double, char, std::string, double, char, double, char>(),
             py::arg("talker_id"), py::arg("status1"), py::arg("status2"), py::arg("cross_track_error_magnitude"),
             py::arg("direction_to_steer"), py::arg("cross_track_units"), py::arg("arrival_circle_status"),
             py::arg("perpendicular_passed_at_waypoint"), py::arg("bearing_origin_to_destination"),
             py::arg("bearing_origin_to_destination_type"), py::arg("destination_waypoint_id"),
             py::arg("bearing_present_to_destination"), py::arg("bearing_present_to_destination_type"),
             py::arg("heading_to_steer_to_destination_waypoint"), py::arg("heading_to_steer_to_destination_waypoint_type"))
        .def("get_status1", &APB::getStatus1)
        .def("get_status2", &APB::getStatus2)
        .def("get_cross_track_error_magnitude", &APB::getCrossTrackErrorMagnitude)
        .def("get_direction_to_steer", &APB::getDirectionToSteer)
        .def("get_cross_track_units", &APB::getCrossTrackUnits)
        .def("get_arrival_circle_status", &APB::getArrivalCircleStatus)
        .def("get_perpendicular_passed_at_waypoint", &APB::getPerpendicularPassedAtWaypoint)
        .def("get_bearing_origin_to_destination", &APB::getBearingOriginToDestination)
        .def("get_bearing_origin_to_destination_type", &APB::getBearingOriginToDestinationType)
        .def("get_destination_waypoint_id", &APB::getDestinationWaypointId)
        .def("get_bearing_present_to_destination", &APB::getBearingPresentToDestination)
        .def("get_bearing_present_to_destination_type", &APB::getBearingPresentToDestinationType)
        .def("get_heading_to_steer_to_destination_waypoint", &APB::getHeadingToSteerToDestinationWaypoint)
        .def("get_heading_to_steer_to_destination_waypoint_type", &APB::getHeadingToSteerToDestinationWaypointType)
        .def("get_string_content", &APB::getStringContent, py::arg("verbose") = false);

    py::class_<DBT, Message0183>(m0183, "DBT")
        .def(py::init<std::string, double, char, double, char, double, char>(),
             py::arg("talker_id"), py::arg("depth_feet"), py::arg("feet_unit"),
             py::arg("depth_meters"), py::arg("meters_unit"), py::arg("depth_fathoms"), py::arg("fathoms_unit"))
        .def("get_depth_feet", &DBT::getDepthFeet)
        .def("get_feet_unit", &DBT::getFeetUnit)
        .def("get_depth_meters", &DBT::getDepthMeters)
        .def("get_meters_unit", &DBT::getMetersUnit)
        .def("get_depth_fathoms", &DBT::getDepthFathoms)
        .def("get_fathoms_unit", &DBT::getFathomsUnit)
        .def("get_string_content", &DBT::getStringContent, py::arg("verbose") = false);

    py::class_<DPT, Message0183>(m0183, "DPT")
        .def(py::init<std::string, double, double, double>(),
             py::arg("talker_id"), py::arg("water_depth_meters"), py::arg("transducer_offset"), py::arg("max_range_scale"))
        .def("get_water_depth_meters", &DPT::getWaterDepthMeters)
        .def("get_transducer_offset", &DPT::getTransducerOffset)
        .def("get_max_range_scale", &DPT::getMaxRangeScale)
        .def("get_string_content", &DPT::getStringContent, py::arg("verbose") = false);

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

    py::class_<GSA, Message0183>(m0183, "GSA")
        .def(py::init<std::string, char, unsigned int, std::array<unsigned int, 12>, double, double, double, std::optional<unsigned int>>(),
             py::arg("talker_id"), py::arg("selection_mode"), py::arg("mode"), py::arg("satellite_ids"),
             py::arg("pdop"), py::arg("hdop"), py::arg("vdop"), py::arg("system_id") = std::nullopt)
        .def("get_selection_mode", &GSA::getSelectionMode)
        .def("get_mode", &GSA::getMode)
        .def("get_satellite_ids", &GSA::getSatelliteIds)
        .def("get_satellite_id", &GSA::getSatelliteId, py::arg("index"))
        .def("get_pdop", &GSA::getPdop)
        .def("get_hdop", &GSA::getHdop)
        .def("get_vdop", &GSA::getVdop)
        .def("has_system_id", &GSA::hasSystemId)
        .def("get_system_id", &GSA::getSystemId)
        .def("get_string_content", &GSA::getStringContent, py::arg("verbose") = false);

    py::class_<GSV, Message0183>(m0183, "GSV")
        .def(py::init<std::string, unsigned int, unsigned int, unsigned int, std::vector<GSV::SatelliteInfo>, std::optional<unsigned int>>(),
             py::arg("talker_id"), py::arg("total_sentences"), py::arg("sentence_number"),
             py::arg("satellites_in_view"), py::arg("satellites"), py::arg("signal_id") = std::nullopt)
        .def("get_total_sentences", &GSV::getTotalSentences)
        .def("get_sentence_number", &GSV::getSentenceNumber)
        .def("get_satellites_in_view", &GSV::getSatellitesInView)
        .def("get_satellites", &GSV::getSatellites)
        .def("get_satellite_count", &GSV::getSatelliteCount)
        .def("has_signal_id", &GSV::hasSignalId)
        .def("get_signal_id", &GSV::getSignalId)
        .def("get_string_content", &GSV::getStringContent, py::arg("verbose") = false);

    py::class_<HDG, Message0183>(m0183, "HDG")
        .def(py::init<std::string, double, double, char, double, char>(),
             py::arg("talker_id"), py::arg("magnetic_heading"), py::arg("magnetic_deviation"),
             py::arg("deviation_direction"), py::arg("magnetic_variation"), py::arg("variation_direction"))
        .def("get_magnetic_heading", &HDG::getMagneticHeading)
        .def("get_magnetic_deviation", &HDG::getMagneticDeviation)
        .def("get_deviation_direction", &HDG::getDeviationDirection)
        .def("get_magnetic_variation", &HDG::getMagneticVariation)
        .def("get_variation_direction", &HDG::getVariationDirection)
        .def("get_string_content", &HDG::getStringContent, py::arg("verbose") = false);

    py::class_<HDM, Message0183>(m0183, "HDM")
        .def(py::init<std::string, double, char>(),
             py::arg("talker_id"), py::arg("heading"), py::arg("indicator"))
        .def("get_heading", &HDM::getHeading)
        .def("get_indicator", &HDM::getIndicator)
        .def("get_string_content", &HDM::getStringContent, py::arg("verbose") = false);

    py::class_<MTW, Message0183>(m0183, "MTW")
        .def(py::init<std::string, double, char>(),
             py::arg("talker_id"), py::arg("temperature"), py::arg("unit"))
        .def("get_temperature", &MTW::getTemperature)
        .def("get_unit", &MTW::getUnit)
        .def("get_string_content", &MTW::getStringContent, py::arg("verbose") = false);

    py::class_<MWV, Message0183>(m0183, "MWV")
        .def(py::init<std::string, double, char, double, char, char>(),
             py::arg("talker_id"), py::arg("wind_angle"), py::arg("reference"),
             py::arg("wind_speed"), py::arg("wind_speed_units"), py::arg("status"))
        .def("get_wind_angle", &MWV::getWindAngle)
        .def("get_reference", &MWV::getReference)
        .def("get_wind_speed", &MWV::getWindSpeed)
        .def("get_wind_speed_units", &MWV::getWindSpeedUnits)
        .def("get_status", &MWV::getStatus)
        .def("get_string_content", &MWV::getStringContent, py::arg("verbose") = false);

    py::class_<RMA, Message0183>(m0183, "RMA")
        .def(py::init<std::string, char, double, char, double, char, double, double, double, double, double, char>(),
             py::arg("talker_id"), py::arg("status"), py::arg("latitude"), py::arg("latitude_direction"),
             py::arg("longitude"), py::arg("longitude_direction"), py::arg("time_difference_a"),
             py::arg("time_difference_b"), py::arg("speed_over_ground"), py::arg("track_made_good"),
             py::arg("magnetic_variation"), py::arg("variation_direction"))
        .def("get_status", &RMA::getStatus)
        .def("get_latitude", &RMA::getLatitude)
        .def("get_latitude_direction", &RMA::getLatitudeDirection)
        .def("get_longitude", &RMA::getLongitude)
        .def("get_longitude_direction", &RMA::getLongitudeDirection)
        .def("get_time_difference_a", &RMA::getTimeDifferenceA)
        .def("get_time_difference_b", &RMA::getTimeDifferenceB)
        .def("get_speed_over_ground", &RMA::getSpeedOverGround)
        .def("get_track_made_good", &RMA::getTrackMadeGood)
        .def("get_magnetic_variation", &RMA::getMagneticVariation)
        .def("get_variation_direction", &RMA::getVariationDirection)
        .def("get_string_content", &RMA::getStringContent, py::arg("verbose") = false);

    py::class_<RMB, Message0183>(m0183, "RMB")
        .def(py::init<std::string, char, double, char, std::string, std::string, double, char, double, char, double, double, double, char, std::optional<char>>(),
             py::arg("talker_id"), py::arg("status"), py::arg("cross_track_error_nm"), py::arg("direction_to_steer"),
             py::arg("origin_waypoint_id"), py::arg("destination_waypoint_id"), py::arg("destination_latitude"),
             py::arg("destination_latitude_hemisphere"), py::arg("destination_longitude"), py::arg("destination_longitude_hemisphere"),
             py::arg("range_to_destination_nm"), py::arg("bearing_to_destination_true"),
             py::arg("destination_closing_velocity_knots"), py::arg("arrival_status"), py::arg("faa_mode_indicator") = std::nullopt)
        .def("get_status", &RMB::getStatus)
        .def("get_cross_track_error_nm", &RMB::getCrossTrackErrorNm)
        .def("get_direction_to_steer", &RMB::getDirectionToSteer)
        .def("get_origin_waypoint_id", &RMB::getOriginWaypointId)
        .def("get_destination_waypoint_id", &RMB::getDestinationWaypointId)
        .def("get_destination_latitude", &RMB::getDestinationLatitude)
        .def("get_destination_latitude_hemisphere", &RMB::getDestinationLatitudeHemisphere)
        .def("get_destination_longitude", &RMB::getDestinationLongitude)
        .def("get_destination_longitude_hemisphere", &RMB::getDestinationLongitudeHemisphere)
        .def("get_range_to_destination_nm", &RMB::getRangeToDestinationNm)
        .def("get_bearing_to_destination_true", &RMB::getBearingToDestinationTrue)
        .def("get_destination_closing_velocity_knots", &RMB::getDestinationClosingVelocityKnots)
        .def("get_arrival_status", &RMB::getArrivalStatus)
        .def("has_faa_mode_indicator", &RMB::hasFaaModeIndicator)
        .def("get_faa_mode_indicator", &RMB::getFaaModeIndicator)
        .def("get_string_content", &RMB::getStringContent, py::arg("verbose") = false);

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

    py::class_<VHW, Message0183>(m0183, "VHW")
        .def(py::init<std::string, double, double, double, double>(),
             py::arg("talker_id"), py::arg("heading_true"), py::arg("heading_magnetic"), py::arg("speed_knots"), py::arg("speed_kph"))
        .def("get_heading_true", &VHW::getHeadingTrue)
        .def("get_heading_true_type", &VHW::getHeadingTrueType)
        .def("get_heading_magnetic", &VHW::getHeadingMagnetic)
        .def("get_heading_magnetic_type", &VHW::getHeadingMagneticType)
        .def("get_speed_knots", &VHW::getSpeedKnots)
        .def("get_speed_knots_type", &VHW::getSpeedKnotsType)
        .def("get_speed_kph", &VHW::getSpeedKph)
        .def("get_speed_kph_type", &VHW::getSpeedKphType)
        .def("get_string_content", &VHW::getStringContent, py::arg("verbose") = false);

    py::class_<VLW, Message0183>(m0183, "VLW")
        .def(py::init<std::string, double, double, std::optional<double>, std::optional<double>>(),
             py::arg("talker_id"), py::arg("total_water_distance_nm"), py::arg("water_distance_since_reset_nm"),
             py::arg("total_ground_distance_nm") = std::nullopt, py::arg("ground_distance_since_reset_nm") = std::nullopt)
        .def("get_total_water_distance_nm", &VLW::getTotalWaterDistanceNm)
        .def("get_total_water_distance_unit", &VLW::getTotalWaterDistanceUnit)
        .def("get_water_distance_since_reset_nm", &VLW::getWaterDistanceSinceResetNm)
        .def("get_water_distance_since_reset_unit", &VLW::getWaterDistanceSinceResetUnit)
        .def("has_ground_distance_data", &VLW::hasGroundDistanceData)
        .def("get_total_ground_distance_nm", &VLW::getTotalGroundDistanceNm)
        .def("get_total_ground_distance_unit", &VLW::getTotalGroundDistanceUnit)
        .def("get_ground_distance_since_reset_nm", &VLW::getGroundDistanceSinceResetNm)
        .def("get_ground_distance_since_reset_unit", &VLW::getGroundDistanceSinceResetUnit)
        .def("get_string_content", &VLW::getStringContent, py::arg("verbose") = false);

    py::class_<VWR, Message0183>(m0183, "VWR")
        .def(py::init<std::string, double, char, double, double, double>(),
             py::arg("talker_id"), py::arg("wind_angle"), py::arg("wind_side"),
             py::arg("speed_knots"), py::arg("speed_mps"), py::arg("speed_kph"))
        .def("get_wind_angle", &VWR::getWindAngle)
        .def("get_wind_side", &VWR::getWindSide)
        .def("get_speed_knots", &VWR::getSpeedKnots)
        .def("get_speed_knots_unit", &VWR::getSpeedKnotsUnit)
        .def("get_speed_mps", &VWR::getSpeedMps)
        .def("get_speed_mps_unit", &VWR::getSpeedMpsUnit)
        .def("get_speed_kph", &VWR::getSpeedKph)
        .def("get_speed_kph_unit", &VWR::getSpeedKphUnit)
        .def("get_string_content", &VWR::getStringContent, py::arg("verbose") = false);

    py::class_<XDR, Message0183>(m0183, "XDR")
        .def(py::init<std::string, std::vector<XDR::TransducerMeasurement>>(),
             py::arg("talker_id"), py::arg("transducers"))
        .def("get_transducers", &XDR::getTransducers)
        .def("get_transducer_count", &XDR::getTransducerCount)
        .def("get_string_content", &XDR::getStringContent, py::arg("verbose") = false);

    py::class_<XTE, Message0183>(m0183, "XTE")
        .def(py::init<std::string, char, char, double, char, char, std::optional<char>>(),
             py::arg("talker_id"), py::arg("status1"), py::arg("status2"), py::arg("cross_track_error"),
             py::arg("steer_direction"), py::arg("cross_track_units"), py::arg("faa_mode_indicator") = std::nullopt)
        .def("get_status1", &XTE::getStatus1)
        .def("get_status2", &XTE::getStatus2)
        .def("get_cross_track_error", &XTE::getCrossTrackError)
        .def("get_steer_direction", &XTE::getSteerDirection)
        .def("get_cross_track_units", &XTE::getCrossTrackUnits)
        .def("has_faa_mode_indicator", &XTE::hasFaaModeIndicator)
        .def("get_faa_mode_indicator", &XTE::getFaaModeIndicator)
        .def("get_string_content", &XTE::getStringContent, py::arg("verbose") = false);

    py::class_<ZDA, Message0183>(m0183, "ZDA")
        .def(py::init<std::string, double, unsigned int, unsigned int, unsigned int, int, int>(),
             py::arg("talker_id"), py::arg("utc_time"), py::arg("day"), py::arg("month"),
             py::arg("year"), py::arg("local_zone_hours"), py::arg("local_zone_minutes"))
        .def("get_utc_time", &ZDA::getUtcTime)
        .def("get_day", &ZDA::getDay)
        .def("get_month", &ZDA::getMonth)
        .def("get_year", &ZDA::getYear)
        .def("get_local_zone_hours", &ZDA::getLocalZoneHours)
        .def("get_local_zone_minutes", &ZDA::getLocalZoneMinutes)
        .def("get_string_content", &ZDA::getStringContent, py::arg("verbose") = false);

    py::module_ m2000 = m.def_submodule("nmea2000", "NMEA 2000 API");

    bindDataType<Angle>(m2000, "Angle");
    bindDataType<SignedAngle>(m2000, "SignedAngle");
    bindDataType<Speed>(m2000, "Speed");
    bindDataType<HalfByte>(m2000, "HalfByte");
    bindDataType<Byte>(m2000, "Byte");
    bindDataType<Latitude>(m2000, "Latitude");
    bindDataType<Longitude>(m2000, "Longitude");

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

    py::class_<PGN127245, Message2000>(m2000, "PGN127245")
        .def(py::init<uint8_t, HalfByte, SignedAngle, SignedAngle>(),
             py::arg("rudder_id"), py::arg("direction"), py::arg("angle_order"), py::arg("position"))
        .def(py::init([](uint8_t rudderId,
                         uint8_t direction,
                         float angleOrder,
                         float position) {
            return PGN127245(
                rudderId,
                HalfByte::fromValue(direction),
                SignedAngle::fromValue(angleOrder),
                SignedAngle::fromValue(position));
        }),
             py::arg("rudder_id"), py::arg("direction"), py::arg("angle_order_radians"), py::arg("position_radians"))
        .def("get_rudder_id", &PGN127245::getRudderId)
        .def("get_direction", &PGN127245::getDirection)
        .def("get_angle_order", &PGN127245::getAngleOrder)
        .def("get_position", &PGN127245::getPosition)
        .def("get_angle_order_degrees", &PGN127245::getAngleOrderDegrees)
        .def("get_position_degrees", &PGN127245::getPositionDegrees)
        .def("get_direction_string", &PGN127245::getDirectionString)
        .def("get_string_content", &PGN127245::getStringContent, py::arg("verbose") = false);

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

        py::class_<PGN127257, Message2000>(m2000, "PGN127257")
           .def(py::init<uint8_t, SignedAngle, SignedAngle, SignedAngle, Byte>(),
               py::arg("sequence_id"), py::arg("yaw"), py::arg("pitch"), py::arg("roll"), py::arg("reserved"))
           .def(py::init([](uint8_t sequenceId,
                        float yaw,
                        float pitch,
                        float roll,
                        uint8_t reserved) {
              return PGN127257(
                 sequenceId,
                 SignedAngle::fromValue(yaw),
                 SignedAngle::fromValue(pitch),
                 SignedAngle::fromValue(roll),
                 Byte::fromValue(reserved));
           }),
               py::arg("sequence_id"), py::arg("yaw_radians"), py::arg("pitch_radians"),
               py::arg("roll_radians"), py::arg("reserved") = 0)
           .def("get_sequence_id", &PGN127257::getSequenceId)
           .def("get_yaw", &PGN127257::getYaw)
           .def("get_pitch", &PGN127257::getPitch)
           .def("get_roll", &PGN127257::getRoll)
           .def("get_reserved", &PGN127257::getReserved)
           .def("get_yaw_degrees", &PGN127257::getYawDegrees)
           .def("get_pitch_degrees", &PGN127257::getPitchDegrees)
           .def("get_roll_degrees", &PGN127257::getRollDegrees)
           .def("get_string_content", &PGN127257::getStringContent, py::arg("verbose") = false);

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

    py::class_<PGN128259, Message2000>(m2000, "PGN128259")
        .def(py::init<uint8_t, Speed, Speed, Byte, HalfByte, Byte, HalfByte>(),
             py::arg("sequence_id"), py::arg("speed_water_referenced"), py::arg("speed_ground_referenced"),
             py::arg("speed_water_referenced_type"), py::arg("speed_direction"),
             py::arg("reserved1"), py::arg("reserved2"))
        .def(py::init([](uint8_t sequenceId,
                         float speedWaterReferenced,
                         float speedGroundReferenced,
                         uint8_t speedWaterReferencedType,
                         uint8_t speedDirection,
                         uint8_t reserved1,
                         uint8_t reserved2) {
            return PGN128259(
                sequenceId,
                Speed::fromValue(speedWaterReferenced),
                Speed::fromValue(speedGroundReferenced),
                Byte::fromValue(speedWaterReferencedType),
                HalfByte::fromValue(speedDirection),
                Byte::fromValue(reserved1),
                HalfByte::fromValue(reserved2));
        }),
             py::arg("sequence_id"), py::arg("speed_water_referenced_mps"), py::arg("speed_ground_referenced_mps"),
             py::arg("speed_water_referenced_type"), py::arg("speed_direction"), py::arg("reserved1") = 0, py::arg("reserved2") = 0)
        .def("get_sequence_id", &PGN128259::getSequenceId)
        .def("get_speed_water_referenced", &PGN128259::getSpeedWaterReferenced)
        .def("get_speed_ground_referenced", &PGN128259::getSpeedGroundReferenced)
        .def("get_speed_water_referenced_type", &PGN128259::getSpeedWaterReferencedType)
        .def("get_speed_direction", &PGN128259::getSpeedDirection)
        .def("get_reserved1", &PGN128259::getReserved1)
        .def("get_reserved2", &PGN128259::getReserved2)
        .def("get_string_content", &PGN128259::getStringContent, py::arg("verbose") = false);

    py::class_<PGN129025, Message2000>(m2000, "PGN129025")
        .def(py::init<Latitude, Longitude>(), py::arg("latitude"), py::arg("longitude"))
        .def(py::init([](float latitude, float longitude) {
            return PGN129025(Latitude::fromValue(latitude), Longitude::fromValue(longitude));
        }), py::arg("latitude_degrees"), py::arg("longitude_degrees"))
        .def("get_latitude", &PGN129025::getLatitude)
        .def("get_longitude", &PGN129025::getLongitude)
        .def("get_string_content", &PGN129025::getStringContent, py::arg("verbose") = false);

    py::class_<PGN130306, Message2000>(m2000, "PGN130306")
        .def(py::init<uint8_t, Speed, Angle, HalfByte>(),
             py::arg("sequence_id"), py::arg("wind_speed"), py::arg("wind_direction"), py::arg("wind_reference"))
        .def(py::init<uint8_t, Speed, Angle, HalfByte, HalfByte, Byte, Byte>(),
             py::arg("sequence_id"), py::arg("wind_speed"), py::arg("wind_direction"), py::arg("wind_reference"),
             py::arg("reserved1"), py::arg("reserved2"), py::arg("reserved3"))
        .def(py::init([](uint8_t sequenceId,
                         float windSpeed,
                         float windDirection,
                         uint8_t windReference) {
            return PGN130306(
                sequenceId,
                Speed::fromValue(windSpeed),
                Angle::fromValue(windDirection),
                HalfByte::fromValue(windReference));
        }), py::arg("sequence_id"), py::arg("wind_speed_mps"), py::arg("wind_direction_radians"), py::arg("wind_reference"))
        .def("get_sequence_id", &PGN130306::getSequenceId)
        .def("get_wind_speed", &PGN130306::getWindSpeed)
        .def("get_wind_direction", &PGN130306::getWindDirection)
        .def("get_wind_reference", &PGN130306::getWindReference)
        .def("get_reserved1", &PGN130306::getReserved1)
        .def("get_reserved2", &PGN130306::getReserved2)
        .def("get_reserved3", &PGN130306::getReserved3)
        .def("get_wind_direction_degrees", &PGN130306::getWindDirectionDegrees)
        .def("get_wind_speed_knots", &PGN130306::getWindSpeedKnots)
        .def("get_string_content", &PGN130306::getStringContent, py::arg("verbose") = false);
}
