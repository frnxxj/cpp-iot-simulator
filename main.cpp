#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <random>
#include <fstream>
#include <ctime>
#include <vector>
#include <protocol.hh>
#include <ring_buffer.hh>

c_ring_buffer<std::vector<uint8_t>> msg_queue(10);

int random_int(const int min, const int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

void sensor(const int id, const int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));

    time_t _time;
    _time = time(nullptr);
    const struct tm *now = localtime(&_time);

    const int temp = random_int(15, 75);
    std::string str;
    str = std::format("[{:02}:{:02}:{:02}] Sensor ID {} : {}C",
                      now->tm_hour, now->tm_min, now->tm_sec,
                      id, temp);
    const std::vector<uint8_t> payload(str.begin(), str.end());

    const auto packet = c_protocol::serialize(packet_type::TEMPERATURE, payload);

    msg_queue.push_back(packet);

    printf("%s\n", str.c_str());
}

void processing() {
    std::ofstream log_file("logs.txt", std::ios::app);

    if (!log_file.is_open()) {
        std::cerr << "Failed to open log file" << std::endl;
        return;
    }

    while (true) {
        if (auto opt = msg_queue.pop()) {
            const std::vector<uint8_t>& pkt = *opt;

            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            uint8_t received = pkt.back();

            uint8_t calculated = 0;
            for (int i = 0; i < pkt.size() - 1; i++)
                calculated ^= pkt[i];

            if (received != calculated) {
                std::cerr << "mismatch" << std::endl;
                continue;
            }

            std::string msg;
            if (pkt.size() > 5)
                msg = std::string(pkt.begin() + 4, pkt.end() - 1);

            log_file << msg << std::endl;
            log_file.flush();
        }
    }
}

int main() {
    std::thread t1(sensor, 1, 600);
    std::thread t2(sensor, 2, 1300);
    std::thread cpu(processing);

    t1.join();
    t2.join();
    cpu.join();
}