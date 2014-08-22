#include "query.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <thread>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <functional>
#include <list>



int main(int argc, char const *argv[])
{
    if (argc < 2 || (strcmp(argv[1], "-h") == 0 )){ 
        std::cout << "Usage : find <ip>\n"
            << "Where ip is one or more ip addresses\n"; 
    }

    try {
        boost::asio::io_service service;
        std::list<query> queries;
        std::transform(argv + 1, argv + argc,
            std::back_inserter(queries),             
            [&service] (const char *ip) {return query(ip, service); });
        std::for_each(std::begin(queries), std::end(queries), 
            [&service](query &q) {q.start(service);});
        std::thread helper(boost::bind(&boost::asio::io_service::run, &service));
        service.run();
        helper.join();
        std::for_each(std::begin(queries), std::end(queries), 
            [](const query &q) {std::cout << q; });
    }catch (boost::system::system_error &ex){
        std::cout << ex.what() << std::endl;
    }
    
    return 0;
}