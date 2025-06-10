#include "Gameform.h"
#include "GameWindow.h"

namespace Dyrakgame {
    void Gameform::buttonstart_Click(Object^ sender, EventArgs^ e) {
        this->Hide();
        while (true) {
            GameWindow^ gw = gcnew GameWindow();
            auto res = gw->ShowDialog();  // ждЄм, пока игрок не закроет GameWindow
            if (res == System::Windows::Forms::DialogResult::Retry) {
                // »грать снова Ч цикл повтор€етс€ и создаЄтс€ новое GameWindow
                continue;
            }
            // Abort (√лавное меню) или люба€ друга€ Ђвыходна€ї причина:
            break;
        }
        this->Show();
    }
    void Gameform::GameWindowClosed(Object^ sender, FormClosedEventArgs^ e) {
        this->Show();
    }
    void Gameform::buttonexit_Click(Object^ sender, EventArgs^ e) {
        this->Close();
    }
    void Gameform::buttonsettings_Click(Object^ sender, EventArgs^ e) {
        SettingsForm^ settingsDialog = gcnew SettingsForm(this);
        settingsDialog->SetInitialFullscreenState(fullscreenMode);
        settingsDialog->ShowDialog(this);
    }

}
