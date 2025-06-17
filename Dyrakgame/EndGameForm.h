#pragma once

namespace Dyrakgame {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class EndGameForm : public Form {
    private:
        Label^ lblMessage;
        Button^ btnMainMenu;
        Button^ btnPlayAgain;

    public:
        EndGameForm(String^ message) {
            this->StartPosition = FormStartPosition::CenterParent;
            this->Size = Drawing::Size(400, 200);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;

            lblMessage = gcnew Label();
            lblMessage->Text = message;
            lblMessage->Font = gcnew System::Drawing::Font("Arial", 14, FontStyle::Bold);
            lblMessage->AutoSize = false;
            lblMessage->TextAlign = ContentAlignment::MiddleCenter;
            lblMessage->Dock = DockStyle::Top;
            lblMessage->Height = 80;
            this->Controls->Add(lblMessage);

            btnPlayAgain = gcnew Button();
            btnPlayAgain->Text = "Грати знову";
            btnPlayAgain->Size = Drawing::Size(120, 30);
            btnPlayAgain->Location = Point(60, 110);
            btnPlayAgain->Click += gcnew EventHandler(this, &EndGameForm::OnPlayAgain);
            this->Controls->Add(btnPlayAgain);

            btnMainMenu = gcnew Button();
            btnMainMenu->Text = "Головне меню";
            btnMainMenu->Size = Drawing::Size(120, 30);
            btnMainMenu->Location = Point(220, 110);
            btnMainMenu->Click += gcnew EventHandler(this, &EndGameForm::OnMainMenu);
            this->Controls->Add(btnMainMenu);
        }

    private:
        void OnPlayAgain(Object^ sender, EventArgs^ e) {
            this->DialogResult = System::Windows::Forms::DialogResult::Retry;
            this->Close();
        }

        void OnMainMenu(Object^ sender, EventArgs^ e) {
            this->DialogResult = System::Windows::Forms::DialogResult::Abort;
            this->Close();
        }
    };
}
