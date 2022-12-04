#include <hex/plugin.hpp>

#include <romfs/romfs.hpp>

#include <hex/api/content_registry.hpp>
#include <hex/api/task.hpp>
#include <hex/helpers/net.hpp>
#include <string>
#include <hex/ui/view.hpp>
#include <hex/providers/buffered_reader.hpp>
#include <hex/helpers/logger.hpp>

using namespace hex;
using namespace std::literals::string_literals;

std::string createHexView(u64 offset, size_t size, prv::Provider *provider) {
    constexpr static auto HeaderLine = "Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F\\n";
    std::string result;
    result.reserve(std::string(HeaderLine).size() * size / 0x10);

    result += HeaderLine;

    auto reader = prv::BufferedReader(provider);
    reader.seek(offset);
    reader.setEndAddress((offset + size) - 1);

    u64 address = offset & ~u64(0x0F);
    std::string asciiRow;
    for (u8 byte : reader) {
        if ((address % 0x10) == 0) {
            result += hex::format(" {}", asciiRow);
            result += hex::format("\\n{0:08X}  ", address);

            asciiRow.clear();

            if (address == (offset & ~u64(0x0F))) {
                for (u64 i = 0; i < (offset - address); i++) {
                    result += "   ";
                    asciiRow += " ";
                }
                address = offset;
            }
        }

        result += hex::format("{0:02X} ", byte);

        if (byte == '"')
            asciiRow += "\\\"";
        else if (byte == '\\')
            asciiRow += "\\\\";
        else if (byte >= 0x20 && byte <= 0x7E)
            asciiRow += char(byte);
        else
            asciiRow += ".";

        if ((address % 0x10) == 0x07)
            result += " ";

        address++;
    }

    if ((address % 0x10) != 0x00)
        for (u32 i = 0; i < (0x10 - (address % 0x10)); i++)
            result += "   ";

    result += hex::format(" {}", asciiRow);

    return result;
}

IMHEX_PLUGIN_SETUP("Oracle", "WerWolv", "Plugin to consult the Oracle about file types") {

    for (auto &path : romfs::list("lang"))
        hex::ContentRegistry::Language::addLocalization(nlohmann::json::parse(romfs::get(path).string()));

    ContentRegistry::Settings::add("hex.builtin.setting.general", "hex.oracle.setting.general.openai_token", "", [](auto name, nlohmann::json &setting) {
        static auto token = std::string(setting);

        if (ImGui::InputText(name.data(), token, ImGuiInputTextFlags_CharsNoBlank)) {
            setting = std::string(token.c_str());
            return true;
        }

        return false;
    });

    hex::ContentRegistry::Interface::addMenuItem("hex.builtin.menu.help", 4000, [] {

        if (ImGui::MenuItem("hex.oracle.consult"_lang, nullptr, false, ImHexApi::Provider::isValid())) {
            TaskManager::createTask("hex.oracle.consulting"_lang, 0, [](const auto &){
                Net net;
                auto provider = ImHexApi::Provider::get();

                constexpr static auto ApiUrl = "https://api.openai.com/v1/completions";
                auto apiKey = ContentRegistry::Settings::read("hex.builtin.setting.general", "hex.oracle.setting.general.openai_token", "");

                auto response = net.postJson(ApiUrl, hex::Net::DefaultTimeout,
                                                 {
                                                    { "Authorization", "Bearer "s + apiKey },
                                                    { "Content-Type", "application/json" }
                                                 },
                                                 fmt::format(
                                                             R"({{ "model": "text-davinci-002", "prompt": "```{}``` \n\n The previous hex dump are the first 0x100 bytes of a binary file. Please tell me what kind of format it is plus a short explanation about this format using at least 50 words. If you aren't at least 95% confident in your answer, please answer \"I'm not sure.\" instead of making up something.", "max_tokens": 3000 }})",
                                                             createHexView(0, std::min<size_t>(0x100, provider->getSize()), provider)
                                                             )
                                            ).get();

                if (response.code != 200) {
                    log::error("{} {}", response.code, response.body.dump());
                    hex::View::showErrorPopup("hex.oracle.no_response"_lang);
                } else {
                    try {
                        hex::View::showInfoPopup("hex.oracle.success"_lang + response.body["choices"][0]["text"].get<std::string>());
                    } catch (...) {
                        hex::View::showErrorPopup("hex.oracle.error"_lang);
                    }
                }
            });
        }
    });

}


