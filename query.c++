#include "query.hpp"
#include <boost/algorithm/string.hpp>
#include <functional>
#include <iterator>
#include <algorithm>
#include <utility>


using namespace boost::asio;
using ip::tcp;

void query::on_connect(const boost::system::error_code &ec, 
        tcp::resolver::iterator it){
        if (!ec) {
            auto buf = buffer(ip.to_string() + "\r\n");
            async_write(*socket, buf, 
                std::bind(&query::on_write, 
                    this, std::placeholders::_1, std::placeholders::_2, 
                    buffer_size(buf)));
        }else {
            //...
        }
}

void query::parse_info(std::istream &is) {
    is.unsetf(std::ios::skipws);
    std::copy(std::istream_iterator<char>(is), std::istream_iterator<char>() ,
        std::back_inserter(info));
}

void query::on_write(const boost::system::error_code &ec,
        std::size_t bytes_sent, const std::size_t message_size) {
    if (!ec && bytes_sent == message_size) {
        async_read(*socket, *response_stream, transfer_all(), 
            std::bind(&query::on_read, this, std::placeholders::_1,
                std::placeholders::_2));
    }
}

void query::on_read(const boost::system::error_code &ec,
        std::size_t bytes_received){
    if (ec == error::eof) {
        std::istream is(response_stream.get());
        parse_info(is);
        static const std::string REFER_KEY("refer");
        auto refer_pos = info.find(REFER_KEY);
        if (refer_pos != std::string::npos) {
            // follow referrals
            auto refer_value_start = std::find(std::begin(info) + refer_pos, 
                std::end(info), ':');
            auto refer_value_end = std::find(refer_value_start, 
                std::end(info), '\n');

            if (refer_value_start != std::end(info) && 
                refer_value_end != std::end(info) ) {
                std::string refer_value(refer_value_start + 1, refer_value_end);
                boost::trim(refer_value);
                std::cout << "[Referred to " << refer_value << " for " 
                    << ip << " ]\n";
                socket.reset(new ip::tcp::socket(socket->get_io_service()));
                tcp::resolver resolver(socket->get_io_service());
                auto endpoint_itr = resolver.resolve({refer_value, "43"});
                info.clear();
                async_connect(*socket, endpoint_itr,
                    std::bind(&query::on_connect, this, 
                        std::placeholders::_1, std::placeholders::_2 ));
            }else {
                std::cerr << "[could not parse refer value]" << std::endl;
            }
            
        }
    }
}


query::query(const std::string &ip_str, io_service &service) : 
    ip(ip::address::from_string(ip_str)), socket( new tcp::socket(service)),
    response_stream(new streambuf()){}

void query::start(io_service &service) {
    if (!socket->is_open()) {
        tcp::resolver resolver(service);
        auto endpoint_itr = resolver.resolve({"whois.iana.org", "43"});
        async_connect(*socket, endpoint_itr, 
                std::bind(&query::on_connect, this, 
                    std::placeholders::_1, std::placeholders::_2));
    }
}