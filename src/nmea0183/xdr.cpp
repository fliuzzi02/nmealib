#include "nmealib/nmea0183/xdr.h"

#include "nmealib/detail/errorSupport.h"
#include "nmealib/detail/parse.h"

#include <iomanip>
#include <sstream>
#include <vector>

namespace nmealib {
namespace nmea0183 {

std::unique_ptr<XDR> XDR::create(std::unique_ptr<Message0183> baseMessage) {
    std::string context = "XDR::create";
    if (baseMessage->getSentenceType() != "XDR") {
        NMEALIB_RETURN_ERROR(NotXDRException(context, "Expected sentence type 'XDR', got " + baseMessage->getSentenceType()));
    }

    std::string payload = baseMessage->getPayload();
    std::istringstream ss(payload);
    std::string token;
    std::vector<std::string> fields;

    while (std::getline(ss, token, ',')) {
        fields.push_back(token);
    }

    if (!payload.empty() && payload.back() == ',') {
        fields.push_back("");
    }

    if (!fields.empty()) {
        fields.erase(fields.begin());
    }

    if (fields.empty() || (fields.size() % 4) != 0) {
        NMEALIB_RETURN_ERROR(NotXDRException(context, "Invalid fields in XDR payload: expected one or more quadruplets, got " + std::to_string(fields.size()) + " fields. Payload: " + payload));
    }

    std::vector<TransducerMeasurement> transducers;
    transducers.reserve(fields.size() / 4);

    for (std::size_t index = 0; index < fields.size(); index += 4) {
        TransducerMeasurement measurement;
        measurement.transducerType = fields[index].empty() ? '\0' : fields[index][0];
        if (!detail::parseOptionalDouble(fields[index + 1], measurement.measurement)) {
            NMEALIB_RETURN_ERROR(NmeaException(context, "Error parsing XDR fields"));
        }
        measurement.units = fields[index + 2].empty() ? '\0' : fields[index + 2][0];
        measurement.name = fields[index + 3];
        transducers.push_back(std::move(measurement));
    }

    return std::unique_ptr<XDR>(new XDR(std::move(*baseMessage), std::move(transducers)));
}

XDR::XDR(Message0183 baseMessage, std::vector<TransducerMeasurement> transducers) noexcept
    : Message0183(std::move(baseMessage)),
      transducers_(std::move(transducers)) {}

XDR::XDR(std::string talkerId, std::vector<TransducerMeasurement> transducers)
    : Message0183(*Message0183::create(composeRaw(talkerId, transducers))),
      transducers_(std::move(transducers)) {}

std::unique_ptr<nmealib::Message> XDR::clone() const {
    return std::unique_ptr<XDR>(new XDR(*this));
}

std::string XDR::getStringContent(bool verbose) const noexcept {
    std::ostringstream ss;
    ss << this->toString(verbose);

    if (verbose) {
        ss << "\tTransducers: " << transducers_.size() << "\n";
        for (std::size_t index = 0; index < transducers_.size(); ++index) {
            const auto& transducer = transducers_[index];
            ss << "\t[" << index << "] Type=" << transducer.transducerType
               << ", Value=" << transducer.measurement
               << ", Units=" << transducer.units
               << ", Name=" << transducer.name << "\n";
        }
    } else {
        ss << "Transducers=" << transducers_.size();
    }

    return ss.str();
}

std::string XDR::composeRaw(const std::string& talkerId,
                            const std::vector<TransducerMeasurement>& transducers) {
    std::ostringstream payloadStream;
    payloadStream << talkerId << "XDR";

    payloadStream << std::setprecision(15);
    for (const auto& transducer : transducers) {
        payloadStream << ",";
        if (transducer.transducerType != '\0') {
            payloadStream << transducer.transducerType;
        }
        payloadStream << ",";
        payloadStream << transducer.measurement;
        payloadStream << ",";
        if (transducer.units != '\0') {
            payloadStream << transducer.units;
        }
        payloadStream << ",";
        payloadStream << transducer.name;
    }

    std::string payload = payloadStream.str();
    return "$" + payload + "\r\n";
}

const std::vector<XDR::TransducerMeasurement>& XDR::getTransducers() const noexcept {
    return transducers_;
}

std::size_t XDR::getTransducerCount() const noexcept {
    return transducers_.size();
}

const XDR::TransducerMeasurement* XDR::getTransducer(std::size_t index) const noexcept {
    if (index >= transducers_.size()) {
        return nullptr;
    }
    return &transducers_[index];
}

bool XDR::operator==(const XDR& other) const noexcept {
    return Message0183::operator==(other);
}

} // namespace nmea0183
} // namespace nmealib
