#pragma once
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace Dyrakgame {

    ref class Gameform;

    public ref class SettingsForm : public Form {
    public:
        SettingsForm(Form^ parent);
        void SetInitialFullscreenState(bool isFullscreen);

    private:
        Form^ parentForm;
        CheckBox^ cbFullscreen;
        Button^ btnOK;

        void InitializeComponent();
        void btnOK_Click(Object^ sender, EventArgs^ e);
    };
}
