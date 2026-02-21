#include "controller.h"
#include "database/database_context.h"
#include "resources.h"
#include "scheduler/wanikani_scheduler.h"
#include "system/platform_info.h"
#include "system/resource.h"
#include "wallpaper/wallpaper_service.h"
#include <crow.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <string>

int main()
{
	auto scheduler = std::make_unique<kanji::scheduler::WaniKaniScheduler>();
	kanji::database::DatabaseContext context{kanji::system::PlatformInfo::GetDatabaseLocation()};
	kanji::wallpaper::WallpaperService wallpaper_service{context.GetReviewStateRepository()};
	kanji::Controller controller{context, wallpaper_service, std::move(scheduler)};

	crow::SimpleApp app;

	std::mutex controller_mutex;

	CROW_ROUTE(app, "/api/kanjis").methods("GET"_method)([&]() {
		std::lock_guard lock(controller_mutex);
		nlohmann::json j = controller.GetReviewKanjis();
		return crow::response(j.dump());
	});

	CROW_ROUTE(app, "/api/answers").methods("POST"_method)([&](const crow::request& req) {
		std::lock_guard lock(controller_mutex);
		auto j = nlohmann::json::parse(req.body);
		controller.SetAnswers(j["answers"]);
		return crow::response(200);
	});

	CROW_ROUTE(app, "/api/learn-more").methods("POST"_method)([&]() {
		std::lock_guard lock(controller_mutex);
		controller.LearnMoreKanjis();
		return crow::response(200);
	});

	app.port(8080).run();

	return 0;
}
