#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <map>

class query {
    boost::asio::ip::address ip;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket;
    std::unique_ptr<boost::asio::streambuf> response_stream;
    std::string info;
    void parse_info(std::istream &is);

    void on_connect(const boost::system::error_code &ec, 
        boost::asio::ip::tcp::resolver::iterator it);
    void on_write(const boost::system::error_code &ec,
        std::size_t bytes_sent, std::size_t message_size);
    void on_read(const boost::system::error_code &ec,
        std::size_t bytes_received);

public:
    query(const std::string &ip_str, boost::asio::io_service &service);
    query(const query&) = delete;
    query(query &&) = default;
    void start(boost::asio::io_service &service);

    friend std::ostream & operator<<(std::ostream & out, const query & q) {

        return out << "Info on " << q.ip << '\n' << q.info;       
    }
};