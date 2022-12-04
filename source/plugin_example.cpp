#include <hex/plugin.hpp>

#include <hex/api/content_registry.hpp>
#include <hex/ui/view.hpp>

class ViewExample : public hex::View {
public:
    ViewExample() : hex::View("Example") {}
    ~ViewExample() override = default;

    void drawContent() override {
        if (ImGui::Begin("Example")) {
            ImGui::Text("Custom plugin window");
        }
        ImGui::End();
    }
};

IMHEX_PLUGIN_SETUP("C++ Template Plugin", "Plugin Author", "Plugin Description") {

    hex::ContentRegistry::Views::add<ViewExample>();

}


