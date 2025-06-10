#include "SettingsForm.h"
#include "Gameform.h"   // чтобы видеть полный тип Gameform
#include "GameWindow.h"
namespace Dyrakgame {

    SettingsForm::SettingsForm(Form^ parent)
        : parentForm(parent)
    {
        InitializeComponent();
    }

    void SettingsForm::SetInitialFullscreenState(bool isFullscreen) {
        cbFullscreen->Checked = isFullscreen;
    }

    void SettingsForm::InitializeComponent() {
        this->Text = L"Настройки";
        this->Size = Drawing::Size(300, 200);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;
        this->StartPosition = FormStartPosition::CenterParent;

        // CheckBox
        cbFullscreen = gcnew CheckBox();
        cbFullscreen->Text = L"Полноэкранный режим";
        cbFullscreen->Location = Point(30, 30);
        this->Controls->Add(cbFullscreen);

        // Кнопка OK
        btnOK = gcnew Button();
        btnOK->Text = L"OK";
        btnOK->Size = Drawing::Size(80, 30);
        btnOK->Location = Point(100, 100);
        btnOK->Click += gcnew EventHandler(this, &SettingsForm::btnOK_Click);
        this->Controls->Add(btnOK);
    }

    void SettingsForm::btnOK_Click(Object^ sender, EventArgs^ e) {
        Gameform^ mf = dynamic_cast<Gameform^>(parentForm);
        if (mf) {
            mf->FullscreenModeStatic = cbFullscreen->Checked; // Accessing through the instance
            mf->ApplyFullscreen();
        }
    }

}