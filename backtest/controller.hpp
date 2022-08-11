#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

namespace controller {

using Action = std::function<void(std::string)>;

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(tcp::socket socket, Action action)
      : socket_(std::move(socket)), action(std::move(action)) {
  }

  void start() {
    do_read();
  }

 private:
  void do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(
        boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length) {
          if (!ec) {
            std::cout << data_ << std::endl;
            action(std::string(data_));
            do_write(length, data_);
          }
        });
  }

  void do_write(std::size_t length, const std::string& response = "") {
    auto self(shared_from_this());

    std::string resultPath = response.substr(0, response.size() - 5);
    resultPath += "_metafile.json";
    auto resultResponse = resultPath.c_str();
    std::cout << resultPath.c_str() << std::endl;

    boost::asio::async_write(
        socket_, boost::asio::buffer(resultResponse, strnlen_s(resultResponse, max_length)),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec) {
            do_read();
          }
        });
  }

  tcp::socket socket_;
  enum { max_length = 2048 };
  char data_[max_length]{};
  std::function<void(std::string)> action;
};

class Server {
 public:
  Server(boost::asio::io_context& io_context, short port,
         Action action)
      : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
        action(std::move(action)) {
    do_accept();
  }

 private:
  void do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
          if (!ec) {
            std::make_shared<Session>(std::move(socket), action)->start();
          }
          do_accept();
        });
  }

  tcp::acceptor acceptor_;
  std::function<void(std::string)> action;
};

int run(Action action) {
  try {
    boost::asio::io_context io_context;

    Server s(io_context, 8000, std::move(action));

    io_context.run();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

}  // namespace controller
