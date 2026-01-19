#ifndef QWE_PROTOCOL_HH
#define QWE_PROTOCOL_HH

#include <cstdint>
#include <vector>

enum class packet_type : uint8_t {
    HEARTBEAT   = 0x1,
    TEMPERATURE = 0x2,
    VELOCITY    = 0x3
};

struct packet_header_t {
    uint8_t start_byte = 0xAA;
    uint8_t type{};
    uint16_t length{};
};

class c_protocol {
public:
    static std::vector<uint8_t> serialize(packet_type type, std::vector<uint8_t> data) {
        std::vector<uint8_t> buffer;

        packet_header_t header;
        header.type = static_cast<uint8_t>(type);
        header.length = static_cast<uint16_t>(data.size());

        const auto* ptr = reinterpret_cast<const uint8_t*>(&header);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(packet_header_t));

        buffer.insert(buffer.end(), data.begin(), data.end());

        uint8_t checksum = 0;
        for (const uint8_t byte : buffer) {
            checksum ^= byte;
        }
        buffer.push_back(checksum);

        return buffer;
    }
};

#endif //QWE_PROTOCOL_HH