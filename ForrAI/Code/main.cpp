#include "GUI.hpp"
#include "BusinessLogic.hpp"

#ifdef NDEBUG
#include <Windows.h>
#define main() int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#else
#define main() int main()
#endif

main() {
    fa::ApplicationContext context{};

    fa::GUI           gui{ context };
    fa::BusinessLogic business_logic{ context };

    while (gui.IsWindowOpen()) {
        business_logic.Update();
        gui.Update();
    }

    return 0;
}
