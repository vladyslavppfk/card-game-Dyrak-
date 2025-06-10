#pragma once

#include <vcclr.h>  // если нужно GC-контейнеры, но обычно не нужен

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace Dyrakgame {

    // Форвард: чтобы видеть Gameform
    ref class Gameform;

    public ref class SettingsForm : public Form {
    public:
        SettingsForm(Form^ parent);

        // Перед открытием диалога можно вызвать это,
        // чтобы синхронизировать состояние CheckBox
        void SetInitialFullscreenState(bool isFullscreen);

    private:
        Form^ parentForm;
        CheckBox^ cbFullscreen;
        Button^ btnOK;

        void InitializeComponent();
        void btnOK_Click(Object^ sender, EventArgs^ e);
    };
}
