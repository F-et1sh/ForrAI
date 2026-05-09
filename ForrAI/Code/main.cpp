#include "GUI.hpp"
#include "BusinessLogic.hpp"

int main() {
    fa::ApplicationContext context{};

    fa::GUI                gui{ context };
    fa::BusinessLogic      business_logic{ context };

    while (gui.IsWindowOpen()) {
        business_logic.Update();
        gui.Update();
    }

    return 0;
}
