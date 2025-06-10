#pragma once

#include <vcclr.h>  // ���� ����� GC-����������, �� ������ �� �����

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace Dyrakgame {

    // �������: ����� ������ Gameform
    ref class Gameform;

    public ref class SettingsForm : public Form {
    public:
        SettingsForm(Form^ parent);

        // ����� ��������� ������� ����� ������� ���,
        // ����� ���������������� ��������� CheckBox
        void SetInitialFullscreenState(bool isFullscreen);

    private:
        Form^ parentForm;
        CheckBox^ cbFullscreen;
        Button^ btnOK;

        void InitializeComponent();
        void btnOK_Click(Object^ sender, EventArgs^ e);
    };
}
