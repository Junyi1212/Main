#include "BoostAsio.h"

//Case1:Timer
void handler1(const boost::system::error_code &ec) 
{ 
  std::cout << "3 s." << std::endl; 
} 

void handler2(const boost::system::error_code &ec) 
{ 
  std::cout << "5 s." << std::endl; 
}

void BoostAsioCase1(void)
{
    std::cout << __FUNCTION__ << std::endl;

    boost::asio::io_service io_service; 
    boost::asio::deadline_timer timer1(io_service, boost::posix_time::seconds(3)); 
    timer1.async_wait(handler1); 
    boost::asio::deadline_timer timer2(io_service, boost::posix_time::seconds(5)); 
    timer2.async_wait(handler2); 
    io_service.run();
}

//Case2:网络编程
//Server

#define MY_SERVER_PORT 12280

boost::asio::io_service io_service; 
boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), MY_SERVER_PORT); 
boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint); 
boost::asio::ip::tcp::socket sock(io_service); 
std::string write_data = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!"; 

void write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) 
{

} 

void accept_handler(const boost::system::error_code &ec) 
{ 
  if (!ec) 
  {
    boost::asio::async_write(sock, boost::asio::buffer(write_data), write_handler); 
  } 
} 
void BoostAsioServer(void)
{
  acceptor.listen(); 
  acceptor.async_accept(sock, accept_handler); 
  std::cout << "Server Start! port:" << MY_SERVER_PORT << std::endl;
  io_service.run(); 
  std::cout << "Server End!"<< std::endl;
}

//Client
//Todo:用boost实现
void BoostAsioClient(void)
{
    std::cout << "Client Start!!"<< std::endl;
    system("python tcp_client.py");
    std::cout << "Client End!"<< std::endl;
}

void BoostAsioCase2(void)
{
    std::cout << __FUNCTION__ << std::endl;

    std::thread thd1(BoostAsioServer);
    std::thread thd2(BoostAsioClient);
    thd1.join(); 
    thd2.join(); 
}

