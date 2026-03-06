#define DUCKDB_EXTENSION_MAIN

#include "ui_dark_mode_extension.hpp"
#include "dark_mode_css.hpp"

#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/table_function.hpp"

#include "httplib.hpp"

#include <memory>
#include <mutex>
#include <string>
#include <thread>

#ifdef _WIN32
#define OPEN_COMMAND "start"
#elif __linux__
#define OPEN_COMMAND "xdg-open"
#else
#define OPEN_COMMAND "open"
#endif

namespace httplib = CPPHTTPLIB_NAMESPACE;

namespace duckdb {

// ---------------------------------------------------------------------------
// Dark mode proxy server – forwards requests to the duckdb-ui server
// (localhost:<ui_port>) and injects dark mode CSS into HTML responses.
// ---------------------------------------------------------------------------

static constexpr uint16_t DEFAULT_DARK_PORT = 4214;
static constexpr uint16_t DEFAULT_UI_PORT = 4213;
static constexpr const char *SETTING_DARK_PORT = "ui_dark_port";
static constexpr const char *SETTING_DARK_MODE = "ui_dark_mode";

struct DarkModeProxy {
	httplib::Server server;
	duckdb::unique_ptr<std::thread> thread;
	uint16_t dark_port = 0;
	uint16_t ui_port = 0;
	std::string style_block;
	std::string local_url;

	static duckdb::unique_ptr<DarkModeProxy> instance;
	static std::mutex mtx;

	static bool Started() {
		std::lock_guard<std::mutex> lock(mtx);
		return instance && instance->thread;
	}

	static void Stop() {
		std::lock_guard<std::mutex> lock(mtx);
		if (instance) {
			instance->server.stop();
			if (instance->thread && instance->thread->joinable()) {
				instance->thread->join();
			}
			instance.reset();
		}
	}

	static std::string Start(uint16_t dark_port, uint16_t ui_port, const std::string &mode) {
		{
			std::lock_guard<std::mutex> lock(mtx);
			if (instance && instance->thread) {
				return instance->local_url;
			}
		}

		auto style_block = BuildDarkModeStyleBlock(mode);
		auto proxy = duckdb::make_uniq<DarkModeProxy>();
		proxy->dark_port = dark_port;
		proxy->ui_port = ui_port;
		proxy->style_block = std::move(style_block);
		proxy->local_url = StringUtil::Format("http://localhost:%d/", dark_port);

		auto upstream = StringUtil::Format("http://localhost:%d", ui_port);

		// Catch-all: proxy every request to the upstream duckdb-ui server.
		auto *p = proxy.get();
		proxy->server.Get("/.*", [p, upstream](const httplib::Request &req, httplib::Response &res) {
			httplib::Client client(upstream);
			client.set_read_timeout(30);
			client.set_connection_timeout(5);

			httplib::Headers headers;
			for (auto &h : req.headers) {
				if (h.first != "Host") {
					headers.emplace(h.first, h.second);
				}
			}

			auto result = client.Get(req.path, req.params, headers);
			if (!result) {
				res.status = 502;
				res.set_content("Dark mode proxy: upstream duckdb-ui "
				                "server not reachable. Make sure "
				                "'call start_ui_server();' has been run.",
				                "text/plain");
				return;
			}

			res = result.value();

			// Inject dark mode CSS into HTML responses
			if (!p->style_block.empty()) {
				auto ct = res.get_header_value("Content-Type");
				if (ct.find("text/html") != std::string::npos) {
					auto pos = res.body.find("</head>");
					if (pos != std::string::npos) {
						res.body.insert(pos, p->style_block);
					}
				}
			}

			res.headers.erase("Content-Length");
		});

		// Forward POST requests (SQL execution, tokenize, interrupt)
		proxy->server.Post("/.*", [upstream](const httplib::Request &req, httplib::Response &res,
		                                     const httplib::ContentReader &content_reader) {
			std::string body;
			content_reader([&body](const char *data, size_t len) {
				body.append(data, len);
				return true;
			});

			httplib::Client client(upstream);
			client.set_read_timeout(300);
			client.set_connection_timeout(5);

			httplib::Headers headers;
			for (auto &h : req.headers) {
				if (h.first != "Host" && h.first != "Content-Length") {
					headers.emplace(h.first, h.second);
				}
			}
			// Rewrite Origin to match upstream
			headers.erase("Origin");
			headers.emplace("Origin", StringUtil::Format("http://localhost:%d", DEFAULT_UI_PORT));

			auto ct = req.get_header_value("Content-Type");
			auto result = client.Post(req.path, headers, body, ct);
			if (!result) {
				res.status = 502;
				res.set_content("Dark mode proxy: upstream not reachable", "text/plain");
				return;
			}
			res = result.value();
			res.headers.erase("Content-Length");
		});

		proxy->thread = duckdb::make_uniq<std::thread>([p]() { p->server.listen("localhost", p->dark_port); });

		auto url = proxy->local_url;
		{
			std::lock_guard<std::mutex> lock(mtx);
			instance = std::move(proxy);
		}
		std::atexit(DarkModeProxy::Stop);
		return url;
	}
};

duckdb::unique_ptr<DarkModeProxy> DarkModeProxy::instance;
std::mutex DarkModeProxy::mtx;

// ---------------------------------------------------------------------------
// Table functions – single-row result helpers
// ---------------------------------------------------------------------------

struct RunOnceState : public GlobalTableFunctionState {
	bool done = false;
};

static unique_ptr<GlobalTableFunctionState> RunOnceInit(ClientContext &, TableFunctionInitInput &) {
	return make_uniq<RunOnceState>();
}

static unique_ptr<FunctionData> SingleStringBind(ClientContext &, TableFunctionBindInput &input,
                                                 vector<LogicalType> &return_types, vector<string> &names) {
	return_types.push_back(LogicalType::VARCHAR);
	names.push_back("result");
	return nullptr;
}

// ---------------------------------------------------------------------------
// start_dark_ui()  – starts the dark-mode proxy and opens the browser
// ---------------------------------------------------------------------------
static void StartDarkUIFunc(ClientContext &context, TableFunctionInput &input, DataChunk &output) {
	auto &state = input.global_state->Cast<RunOnceState>();
	if (state.done) {
		output.SetCardinality(0);
		return;
	}
	state.done = true;

	// Read settings
	Value v;
	uint16_t dark_port = DEFAULT_DARK_PORT;
	if (context.TryGetCurrentSetting(SETTING_DARK_PORT, v)) {
		dark_port = v.GetValue<uint16_t>();
	}
	uint16_t ui_port = DEFAULT_UI_PORT;
	if (context.TryGetCurrentSetting("ui_local_port", v)) {
		ui_port = v.GetValue<uint16_t>();
	}
	std::string mode = "auto";
	if (context.TryGetCurrentSetting(SETTING_DARK_MODE, v)) {
		mode = v.GetValue<std::string>();
	}

	auto url = DarkModeProxy::Start(dark_port, ui_port, mode);

	auto cmd = StringUtil::Format("%s %s", OPEN_COMMAND, url);
	auto msg = system(cmd.c_str()) ? StringUtil::Format("Dark UI started at %s", url)
	                               : StringUtil::Format("Dark UI opened at %s", url);

	output.SetCardinality(1);
	output.SetValue(0, 0, Value(msg));
}

// ---------------------------------------------------------------------------
// stop_dark_ui()
// ---------------------------------------------------------------------------
static void StopDarkUIFunc(ClientContext &, TableFunctionInput &input, DataChunk &output) {
	auto &state = input.global_state->Cast<RunOnceState>();
	if (state.done) {
		output.SetCardinality(0);
		return;
	}
	state.done = true;

	bool was_running = DarkModeProxy::Started();
	DarkModeProxy::Stop();

	output.SetCardinality(1);
	output.SetValue(0, 0, Value(was_running ? "Dark UI proxy stopped" : "Dark UI proxy was not running"));
}

// ---------------------------------------------------------------------------
// Extension loading
// ---------------------------------------------------------------------------

static void LoadInternal(ExtensionLoader &loader) {
	auto &instance = loader.GetDatabaseInstance();
	auto &config = DBConfig::GetConfig(instance);

	// Settings
	config.AddExtensionOption(SETTING_DARK_PORT, "Port for the dark mode proxy server", LogicalType::USMALLINT,
	                          Value::USMALLINT(DEFAULT_DARK_PORT));

	config.AddExtensionOption(SETTING_DARK_MODE, "Dark mode: 'auto' (follow OS), 'on' (always dark), 'off' (disabled)",
	                          LogicalType::VARCHAR, Value("auto"));

	// Table functions
	{
		TableFunction tf("start_dark_ui", {}, StartDarkUIFunc, SingleStringBind, RunOnceInit);
		loader.RegisterFunction(tf);
	}
	{
		TableFunction tf("stop_dark_ui", {}, StopDarkUIFunc, SingleStringBind, RunOnceInit);
		loader.RegisterFunction(tf);
	}
}

void UiDarkModeExtension::Load(ExtensionLoader &loader) {
	LoadInternal(loader);
}

std::string UiDarkModeExtension::Name() {
	return "ui_dark_mode";
}

std::string UiDarkModeExtension::Version() const {
#ifdef EXT_VERSION_UI_DARK_MODE
	return EXT_VERSION_UI_DARK_MODE;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_CPP_EXTENSION_ENTRY(ui_dark_mode, loader) {
	duckdb::LoadInternal(loader);
}
}
