#include "Gameform.h"
#include "GameWindow.h"
namespace Dyrakgame {
    void Gameform::buttonstart_Click(Object^ sender, EventArgs^ e) {
        this->Hide();
        while (true) {
            GameWindow^ gw = gcnew GameWindow();
            auto res = gw->ShowDialog();
            if (res == System::Windows::Forms::DialogResult::Retry) {
                continue;
            }
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
        settingsDialog->SetInitialFullscreenState(FullscreenMode);
        settingsDialog->ShowDialog(this);
    }

}