#include "Server.hpp"

#include <spdlog/cfg/argv.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
	spdlog::cfg::load_env_levels();
	spdlog::cfg::load_argv_levels(argc, argv);
	spdlog::set_default_logger(spdlog::stdout_color_mt("server", spdlog::color_mode::always));
	Server server;
	server.run();
}
