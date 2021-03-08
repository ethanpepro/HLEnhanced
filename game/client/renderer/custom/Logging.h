#pragma once

#include <spdlog/logger.h>

namespace renderer {
	extern std::shared_ptr<spdlog::logger> log;
}

namespace material {
	extern std::shared_ptr<spdlog::logger> log;
}
