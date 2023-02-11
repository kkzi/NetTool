#pragma once

#include <boost/asio/detail/socket_option.hpp>

using send_timeout_option = boost::asio::detail::socket_option::integer<SOL_SOCKET, SO_SNDTIMEO>;
using recv_timeout_option = boost::asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>;
